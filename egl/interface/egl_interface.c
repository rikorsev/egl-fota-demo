#include "egl_result.h"
#include "egl_interface.h"

egl_result_t egl_itf_init(egl_interface_t *itf)
{
    EGL_ASSERT_CHECK(itf);
    EGL_ASSERT_CHECK(itf->init);

    return itf->init();
}

egl_result_t egl_itf_open(egl_interface_t* itf)
{
    EGL_ASSERT_CHECK(itf);
    EGL_ASSERT_CHECK(itf->open);

    return itf->open();
}

egl_result_t egl_itf_write(egl_interface_t *itf, void *buff, size_t *len)
{
    EGL_ASSERT_CHECK(itf);
    EGL_ASSERT_CHECK(buff);
    EGL_ASSERT_CHECK(len);
    EGL_ASSERT_CHECK(itf->write);

    *len = itf->write(buff, *len);

    return EGL_SUCCESS;
}

egl_result_t egl_itf_ioctl(egl_interface_t *itf, uint8_t opcode, void *data, size_t len)
{
    EGL_ASSERT_CHECK(itf);
    EGL_ASSERT_CHECK(itf->ioctl);

    return itf->ioctl(opcode, data, len);
}

egl_result_t egl_itf_read(egl_interface_t *itf, void *buff, size_t *len)
{
    EGL_ASSERT_CHECK(itf);
    EGL_ASSERT_CHECK(buff);
    EGL_ASSERT_CHECK(len);
    EGL_ASSERT_CHECK(itf->read);

    *len = itf->read(buff, *len);

    return EGL_SUCCESS;
}

egl_result_t egl_itf_close(egl_interface_t *itf)
{
    EGL_ASSERT_CHECK(itf);
    EGL_ASSERT_CHECK(itf->close);

    return itf->close();
}

egl_result_t egl_itf_deinit(egl_interface_t *itf)
{
    EGL_ASSERT_CHECK(itf);
    EGL_ASSERT_CHECK(itf->deinit);

    return itf->deinit();
}