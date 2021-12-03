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
  char kn[16];
  while(NDL_PollEvent(buf, sizeof(buf)) == 0);
  if (buf[0] == 'k') {
    if (buf[0] == 'd') {
      event->type = SDL_KEYDOWN;
      printf("kd ");
    }
    else {
      event->type = SDL_KEYUP;
      printf("ku ");
    }
    sscanf(&(buf[3]), "%s\n", kn);
    printf("%skey\n", kn);
    for (int i = 0; i < sizeof(keyname); i++) {
      if (strcmp(kn , keyname[i]) == 0) {
        printf("success\n");
        event->key.keysym.sym = i;
        return 1;
      }
    }
  }
  else {
    return 0;
  }
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
