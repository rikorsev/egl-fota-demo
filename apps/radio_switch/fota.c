#include "egl_lib.h"
#include "plat.h"
#include "protocol.h"
#include "fota.h"
#include "radio.h"

#define FOTA_CHUNK_SIZE (4096U)

static void *thread_handle = NULL;
static void *flags_handle = NULL;

enum
{
    FOTA_START_FLAG = 1,
};

typedef struct
{
    uint32_t size;
    uint32_t offset;
}__attribute__((packed)) fota_request_t;

static egl_result_t fota_start_handle(void)
{
    egl_result_t result;
    PROTOCOL_PACKET_DECLARE(packet, sizeof(fota_request_t));
    fota_request_t *fota_request = (fota_request_t *)packet->payload;

    fota_request->size = FOTA_CHUNK_SIZE;
    fota_request->offset = 0;

    EGL_LOG_INFO("[OUTPUT] Request. Size = %u, offset = %u", fota_request->size, fota_request->offset);

    result = protocol_packet_build(packet, PROTOCOL_CMD_FOTA_REQUEST_CHUNK, NULL, sizeof(fota_request_t));
    EGL_RESULT_CHECK(result);

    result = radio_packet_send(packet);
    EGL_RESULT_CHECK(result);

    return result;
}

static void fota_thread_entry(void *param)
{
    egl_result_t result;
    unsigned int flags = 0;

    while(1)
    {
        result = egl_os_flags_wait(SYSOS, flags_handle, FOTA_START_FLAG, &flags,
                                   EGL_OS_FLAGS_OPT_WAIT_ANY, EGL_OS_WAIT_FOREWER);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);

        if(flags & FOTA_START_FLAG)
        {
            result = fota_start_handle();
            EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
        }
    }
}

egl_result_t fota_request_chunk_handle(protocol_packet_t *packet)
{
    egl_result_t result;
    PROTOCOL_PACKET_DECLARE(response, 0);

    result = protocol_packet_validate(packet, PROTOCOL_CMD_FOTA_REQUEST_CHUNK, sizeof(fota_request_t));
    EGL_RESULT_CHECK(result);

    fota_request_t *fota_request = (fota_request_t *)packet->payload;

    EGL_LOG_INFO("[INPUT] Request. Size = %u, offset = %u", fota_request->size, fota_request->offset);

    result = protocol_packet_build(response, PROTOCOL_CMD_FOTA_COMPLETE, NULL, 0);
    EGL_RESULT_CHECK(result);

    result = radio_packet_send(response);
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t fota_complete_handle(protocol_packet_t *packet)
{
    egl_result_t result;

    result = protocol_packet_validate(packet, PROTOCOL_CMD_FOTA_COMPLETE, 0);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("FOTA complete");

    return result;
}

egl_result_t fota_start(void)
{
    egl_result_t result;

    result = egl_os_flags_set(SYSOS, flags_handle, FOTA_START_FLAG);
    EGL_RESULT_CHECK(result);

    return EGL_SUCCESS;
}

egl_result_t fota_init(void)
{
    egl_result_t result;

    static egl_os_flags_ctx flags_ctx;
    static egl_os_thread_ctx thread_ctx;
    static uint8_t stack[1024];

    result = egl_os_flags_create(SYSOS, &flags_handle, "fota_flags", &flags_ctx);
    EGL_RESULT_CHECK(result);

    result = egl_os_thread_create(SYSOS, &thread_handle, "fota",
                                  fota_thread_entry, NULL,
                                  stack, sizeof(stack),
                                  1, &thread_ctx);
    EGL_RESULT_CHECK(result);

    return result;
}