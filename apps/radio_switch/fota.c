#include "egl_lib.h"
#include "plat.h"
#include "protocol.h"
#include "fota.h"
#include "radio.h"

#define FOTA_REPEAT_COUNT_MAX    (5U)
#define FOTA_STEP_WAIT_TIMEOUT   (1000U)

static void *thread_handle = NULL;
// static void *flags_handle = NULL;
static void *step_sem_handle = NULL;

enum
{
    FOTA_REQUEST_META_FLAG = 1,
    FOTA_REQUEST_CHUNK_FLAG = 2,
};

typedef struct
{
    uint32_t chunk_num;
}__attribute__((packed)) fota_request_chunk_t;

typedef struct
{
    uint32_t chunk_num;
    uint8_t chunk[PLAT_FLASH_PAGE_SIZE];
}__attribute__((packed)) fota_response_chunk_t;

typedef enum
{
    FOTA_STATE_IDLE,
    FOTA_STATE_GET_META,
    FOTA_STATE_GET_CHUNK,
    FOTA_STATE_COMPLETE
}fota_state_t;

static struct fota_meta_s
{
    app_info_t app_info;
    uint32_t current_chunk;
    uint32_t total_chunks;
    uint8_t repeat_count;
    fota_state_t state;
    egl_result_t result;
    uint32_t timeout;
}
fota_meta =
{
    .current_chunk = 0,
    .state = FOTA_STATE_IDLE,
    .timeout = EGL_OS_WAIT_FOREVER
};

static egl_result_t fota_meta_request(void)
{
    egl_result_t result;

    PROTOCOL_PACKET_DECLARE(packet, 0);

    EGL_LOG_INFO("[OUTPUT] Request meta");

    result = protocol_packet_build(packet, PROTOCOL_CMD_FOTA_REQUEST_META, NULL, 0);
    EGL_RESULT_CHECK(result);

    result = radio_packet_send(packet);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t fota_chunk_request(void)
{
    egl_result_t result;
    PROTOCOL_PACKET_DECLARE(packet, sizeof(fota_request_chunk_t));
    fota_request_chunk_t *fota_request = (fota_request_chunk_t *)packet->payload;

    fota_request->chunk_num = fota_meta.current_chunk;

    EGL_LOG_INFO("[OUTPUT] Request chunk %u", fota_request->chunk_num);

    result = protocol_packet_build(packet, PROTOCOL_CMD_FOTA_REQUEST_CHUNK, NULL, sizeof(fota_request_chunk_t));
    EGL_RESULT_CHECK(result);

    result = radio_packet_send(packet);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t fota_complete(egl_result_t fota_result)
{
    egl_result_t result;
    PROTOCOL_PACKET_DECLARE(packet, sizeof(result));

    EGL_LOG_INFO("[OUTPUT] Fota complete. Result: %s", EGL_RESULT(fota_result));

    result = protocol_packet_build(packet, PROTOCOL_CMD_FOTA_COMPLETE, &fota_result, sizeof(fota_result));
    EGL_RESULT_CHECK(result);

    result = radio_packet_send(packet);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t fota_state_set(fota_state_t state)
{
    egl_result_t result;

    fota_meta.state = state;
    fota_meta.repeat_count = 0;

    result = egl_os_sem_post(SYSOS, step_sem_handle);
    EGL_RESULT_CHECK(result);

    return result;
}

static inline egl_result_t fota_complete_set(egl_result_t result)
{
    fota_meta.result = result;
    return fota_state_set(FOTA_STATE_COMPLETE);
}

static inline egl_result_t fota_state_idle_step(void)
{
    EGL_LOG_INFO("Step: Idle");

    fota_meta.repeat_count = 0;
    fota_meta.timeout = EGL_OS_WAIT_FOREVER;

    return EGL_SUCCESS;
}

static inline egl_result_t fota_state_get_meta_step(void)
{
    egl_result_t result;

    EGL_LOG_INFO("Step: Request meta. Attempt %u", fota_meta.repeat_count);

    if(fota_meta.repeat_count++ >= FOTA_REPEAT_COUNT_MAX)
    {
        EGL_LOG_WARN("Reach attempts limit");
        return fota_complete_set(EGL_TIMEOUT);
    }

    fota_meta.timeout = FOTA_STEP_WAIT_TIMEOUT;

    result = fota_meta_request();
    EGL_RESULT_CHECK(result);

    return result;
}

static inline egl_result_t fota_state_get_chunk_step(void)
{
    egl_result_t result;

    EGL_LOG_INFO("Step: Get chunk %u/%u. Attempt %u", fota_meta.current_chunk,
                                                      fota_meta.total_chunks,
                                                      fota_meta.repeat_count);

    if(fota_meta.repeat_count++ >= FOTA_REPEAT_COUNT_MAX)
    {
        EGL_LOG_WARN("Reach attempts limit");
        return fota_complete_set(EGL_TIMEOUT);
    }

    result = fota_chunk_request();
    EGL_RESULT_CHECK(result);

    return result;
}

static inline egl_result_t fota_state_complete_step(void)
{
    egl_result_t result;

    EGL_LOG_INFO("Step: Complete");

    result = fota_complete(fota_meta.result);
    EGL_RESULT_CHECK(result);

    result = fota_state_set(FOTA_STATE_IDLE);
    EGL_RESULT_CHECK(result);

    return result;
}

static inline egl_result_t fota_state_step(void)
{
    egl_result_t result;

    switch(fota_meta.state)
    {
        case FOTA_STATE_IDLE:
            result = fota_state_idle_step();
            break;

        case FOTA_STATE_GET_META:
            result = fota_state_get_meta_step();
            break;

        case FOTA_STATE_GET_CHUNK:
            result = fota_state_get_chunk_step();
            break;

        case FOTA_STATE_COMPLETE:
            result = fota_state_complete_step();
            break;

    }
    EGL_RESULT_CHECK(result);

    return result;
}

static void fota_thread_entry(void *param)
{
    egl_result_t result;

    while(1)
    {
        result = egl_os_sem_wait(SYSOS, step_sem_handle, fota_meta.timeout);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS || result == EGL_TIMEOUT, RETURN_VOID);

        result = fota_state_step();
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
    }
}

egl_result_t fota_request_meta_handle(protocol_packet_t *packet)
{
    egl_result_t result;
    PROTOCOL_PACKET_DECLARE(response, sizeof(app_info_t));
    app_info_t *meta;

    result = protocol_packet_validate(packet, PROTOCOL_CMD_FOTA_REQUEST_META, 0);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("[INPUT] Request meta");

    result = egl_value_ptr_get(SLOT_B_INFO, (void **)&meta);
    EGL_RESULT_CHECK(result);
    EGL_ASSERT_CHECK(meta, EGL_NULL_POINTER);

    EGL_LOG_INFO("[OUTPUT] Meta: %s (%u.%u.%u%s)", meta->name,
                                                   meta->version.major,
                                                   meta->version.minor,
                                                   meta->version.revision,
                                                   meta->version.sufix);
    EGL_LOG_INFO("[OUTPUT] Date: %s", meta->buildtime);
    EGL_LOG_INFO("[OUTPUT] Size: %u", meta->size);
    EGL_LOG_INFO("[OUTPUT] Checksum: %08x", meta->checksum);

    result = protocol_packet_build(response, PROTOCOL_CMD_FOTA_RESPONSE_META, (uint8_t *)meta, sizeof(app_info_t));
    EGL_RESULT_CHECK(result);

    result = radio_packet_send(response);
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t fota_response_meta_handle(protocol_packet_t *packet)
{
    egl_result_t result;

    result = protocol_packet_validate(packet, PROTOCOL_CMD_FOTA_RESPONSE_META, sizeof(app_info_t));
    EGL_RESULT_CHECK(result);

    memcpy(&fota_meta.app_info, packet->payload, sizeof(app_info_t));

    EGL_LOG_INFO("[INPUT] Response meta");
    EGL_LOG_INFO("[INPUT] Meta: %s (%u.%u.%u%s)", fota_meta.app_info.name,
                                                  fota_meta.app_info.version.major,
                                                  fota_meta.app_info.version.minor,
                                                  fota_meta.app_info.version.revision,
                                                  fota_meta.app_info.version.sufix);
    EGL_LOG_INFO("[INPUT] Date: %s", fota_meta.app_info.buildtime);
    EGL_LOG_INFO("[INPUT] Size: %u", fota_meta.app_info.size);
    EGL_LOG_INFO("[INPUT] Checksum: %08x", fota_meta.app_info.checksum);

    if(fota_meta.app_info.magic != CONFIG_PLAT_SLOT_INFO_MAGIC_VALUE)
    {
        EGL_LOG_ERROR("Incorrect meta. Magic mismatch");
        return fota_complete_set(EGL_CHECKSUM_MISMATCH);
    }

    if(fota_meta.app_info.size == 0)
    {
        EGL_LOG_ERROR("Meta size is zero");
        return fota_complete_set(EGL_INVALID_PARAM);
    }

    fota_meta.total_chunks = (fota_meta.app_info.size / PLAT_FLASH_PAGE_SIZE) + 1;

    result = fota_state_set(FOTA_STATE_GET_CHUNK);
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t fota_request_chunk_handle(protocol_packet_t *packet)
{
    egl_result_t result;
    static PROTOCOL_PACKET_DECLARE(response, sizeof(fota_response_chunk_t));

    result = protocol_packet_validate(packet, PROTOCOL_CMD_FOTA_REQUEST_CHUNK, sizeof(fota_request_chunk_t));
    EGL_RESULT_CHECK(result);

    fota_request_chunk_t *fota_request = (fota_request_chunk_t *)packet->payload;

    EGL_LOG_INFO("[INPUT] Request chunk: %u", fota_request->chunk_num);

    fota_response_chunk_t *fota_response = (fota_response_chunk_t *)response->payload;
    fota_response->chunk_num = fota_request->chunk_num;

    uint32_t addr;
    result = egl_value_u32_get(SLOT_B_ADDR, &addr);
    EGL_RESULT_CHECK(result);

    result = egl_block_read(PLAT_FLASH, addr + fota_request->chunk_num * PLAT_FLASH_PAGE_SIZE, fota_response->chunk);
    EGL_RESULT_CHECK(result);

    result = protocol_packet_build(response, PROTOCOL_CMD_FOTA_RESPONSE_CHUNK, NULL, sizeof(fota_response_chunk_t));
    EGL_RESULT_CHECK(result);

    result = radio_packet_send(response);
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t fota_response_chunk_handle(protocol_packet_t *packet)
{
    egl_result_t result;

    result = protocol_packet_validate(packet, PROTOCOL_CMD_FOTA_RESPONSE_CHUNK, sizeof(fota_response_chunk_t));
    EGL_RESULT_CHECK(result);

    fota_response_chunk_t *fota_response = (fota_response_chunk_t *)packet->payload;

    EGL_LOG_INFO("[INPUT] Response chunk: %u", fota_response->chunk_num);

    if(fota_response->chunk_num != fota_meta.current_chunk)
    {
        EGL_LOG_WARN("Wrong chunk number: %u. Expected: %u", fota_response->chunk_num, fota_meta.current_chunk);
        return fota_complete_set(EGL_INVALID_PARAM);
    }

    uint32_t addr;
    result = egl_value_u32_get(SLOT_B_ADDR, &addr);
    EGL_RESULT_CHECK(result);

    result = egl_block_write(PLAT_FLASH, addr + fota_response->chunk_num * PLAT_FLASH_PAGE_SIZE, fota_response->chunk);
    EGL_RESULT_CHECK(result);

    if(++fota_meta.current_chunk < fota_meta.total_chunks)
    {
        result = fota_state_set(FOTA_STATE_GET_CHUNK);
        EGL_RESULT_CHECK(result);
    }
    else
    {
        result = fota_complete_set(EGL_SUCCESS);
        EGL_RESULT_CHECK(result);
    }

    return result;
}

egl_result_t fota_complete_handle(protocol_packet_t *packet)
{
    egl_result_t result;
    egl_result_t fota_result;

    result = protocol_packet_validate(packet, PROTOCOL_CMD_FOTA_COMPLETE, sizeof(egl_result_t));
    EGL_RESULT_CHECK(result);

    fota_result = *(egl_result_t *)packet->payload;

    EGL_LOG_INFO("[INPUT] FOTA complete. Result: %s", EGL_RESULT(fota_result));

    result = fota_state_set(FOTA_STATE_IDLE);
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t fota_start(void)
{
    egl_result_t result;

    if(fota_meta.state != FOTA_STATE_IDLE)
    {
        EGL_LOG_WARN("Request rejected. Fota not in idle state");
        return EGL_PROCESS;
    }

    result = fota_state_set(FOTA_STATE_GET_META);
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t fota_init(void)
{
    egl_result_t result;

    static egl_os_sem_ctx step_sem_ctx;
    static egl_os_thread_ctx thread_ctx;
    static uint8_t stack[1024];

    result = egl_os_sem_create(SYSOS, &step_sem_handle, "fota_step_sem",
                                                       1, 0, &step_sem_ctx);
    EGL_RESULT_CHECK(result);

    result = egl_os_thread_create(SYSOS, &thread_handle, "fota",
                                  fota_thread_entry, NULL,
                                  stack, sizeof(stack),
                                  1, &thread_ctx);
    EGL_RESULT_CHECK(result);

    return result;
}