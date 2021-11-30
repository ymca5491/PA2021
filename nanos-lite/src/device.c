#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  const char* s = buf;
  for (int i = 0; i < len; i++) {
    putch(*s);
    s++;
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) {
    return 0;
  }
  else {
    int n = snprintf(buf, len, "%s %s\n", ev.keydown?"kd":"ku", keyname[ev.keycode]);
    return n;
  }
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T sz = io_read(AM_GPU_CONFIG);
  int ret = snprintf(buf, len, "WIDTH: %d HEIGHT: %d", sz.width, sz.height);
  return ret;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T sz = io_read(AM_GPU_CONFIG);
  int y = offset / sz.width;
  int x = offset % sz.width;
  io_write(AM_GPU_FBDRAW, .x = x, .y = y, .pixels = buf, .w = len , .h = 1, .sync = 1);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
