#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  char kstate[2];
  char kname[32];
  while(1) {
    if (event->type != SDL_USEREVENT) {
      NDL_PollEvent(buf, sizeof(buf));
      sscanf(buf, "%s %s\n", kstate, kname);
      if (strlen(kstate) != 0) printf("%s: %s\n", kstate, kname);
      if (strcmp(kstate, "kd") == 0 && event->type == SDL_KEYDOWN) {
        if (strcmp(kname, keyname[event->key.keysym.sym]) == 0 ) return 1;
      }
      else if (strcmp(kstate, "ku") == 0 && event->type == SDL_KEYUP) {
        if (strcmp(kname, keyname[event->key.keysym.sym]) == 0 ) return 1;
      }
    }
  }
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
