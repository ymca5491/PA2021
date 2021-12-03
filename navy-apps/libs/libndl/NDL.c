#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>    
#include <fcntl.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval t;
  assert(gettimeofday(&t, NULL) == 0);
  uint32_t ms = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ms;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events", 0);
  int ret = read(fd, buf, len);
  close(fd);
  if (ret >= 4 && buf[strlen(buf)-1] == '\n')
    return 1;
  else
    return 0;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
  /* Not nwm_app */
  else {
    char buf[128];

    int fd = open("/proc/dispinfo", 0);
    int ret = read(fd, buf, sizeof(buf));
    if (ret == sizeof(buf) && buf[sizeof(buf) - 1] != '\0') assert(0); // warning for incorrect result
    sscanf(buf, "WIDTH : %d HEIGHT : %d", &screen_w, &screen_h);
    assert(screen_w >= *w && screen_h >= *h);
    if (*w == 0 && *h == 0) {
      *w = screen_w;
      *h = screen_h;
    }
    canvas_h = *h;
    canvas_w = *w;
    close(fd);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int fd = open("/dev/fb", 0);
  x += (screen_w - canvas_w) / 2;
  y += (screen_h - canvas_h) / 2;
  for (int j = y; j < y + h; j++) {
    lseek(fd, 4 * (j * screen_w + x) , SEEK_SET);
    write(fd, pixels,  4 * w);
    pixels += w;
  }
  close(fd);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
