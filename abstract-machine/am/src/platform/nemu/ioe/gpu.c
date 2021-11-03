#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  int w = inw(VGACTL_ADDR + 2);  // TODO: get the correct width
  int h = inw(VGACTL_ADDR);  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = inw(VGACTL_ADDR + 2), .height = inw(VGACTL_ADDR),
    .vmemsz = inw(VGACTL_ADDR + 2) * inw(VGACTL_ADDR) * sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int w = inw(VGACTL_ADDR + 2);
  uint32_t* pixels = ctl->pixels;
  for (int j = 0; j < ctl->h; j++)
    for (int i = 0; i < ctl->w; i++) {
      int x = ctl->x + i;
      int y = ctl->y + j;
      outl(FB_ADDR + (y*w + x) * sizeof(uint32_t), pixels[j * ctl->w + i]);
    }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
