#ifndef EGL_INTERFACE_H
#define EGL_INTERFACE_H

#include "stdint.h"
#include "stdlib.h"

typedef struct
{
  egl_result_t     (*init)          (void);
  egl_result_t     (*open)          (void);
  size_t           (*write)         (void* data, size_t len);
  size_t           (*read)          (void* data, size_t len);
  egl_result_t     (*ioctl)         (uint8_t opcode, void* data, size_t len);
  egl_result_t     (*close)         (void);
  egl_result_t     (*deinit)        (void);
}egl_interface_t;

egl_result_t egl_itf_init  (egl_interface_t *itf);
egl_result_t egl_itf_open  (egl_interface_t *itf);
egl_result_t egl_itf_write (egl_interface_t *itf, void *buff, size_t *len);
egl_result_t egl_itf_ioctl (egl_interface_t *itf, uint8_t opcode, void *data, size_t len);
egl_result_t egl_itf_read  (egl_interface_t *itf, void *buff, size_t *len);
egl_result_t egl_itf_close (egl_interface_t *itf);
egl_result_t egl_itg_deinit(egl_interface_t *itf);

#endif /* EGL_INTERFACE_H */
