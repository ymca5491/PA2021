#include <NDL.h>
#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};
static uint8_t keystate[sizeof(keyname) / sizeof(char*)];

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *event) {
  char buf[64];
  char kn[16];
  if (NDL_PollEvent(buf, sizeof(buf)) == 0) return 0;
  if (buf[0] == 'k') {
    if (buf[1] == 'd') {
      event->type = SDL_KEYDOWN;
    }
    else {
      event->type = SDL_KEYUP;
    }
    sscanf(&(buf[3]), "%s\n", kn);
    for (int i = 0; i < sizeof(keyname); i++) {
      if (strcmp(kn , keyname[i]) == 0) {
        event->key.keysym.sym = i;
        keystate[i] = buf[1] == 'd' ? 1 : 0;
        return 1;
      }
    }
  }
  else {
    assert(0);
  }
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  char kn[16];
  while(NDL_PollEvent(buf, sizeof(buf)) == 0);
  if (buf[0] == 'k') {
    if (buf[1] == 'd') {
      event->type = SDL_KEYDOWN;
    }
    else {
      event->type = SDL_KEYUP;
    }
    sscanf(&(buf[3]), "%s\n", kn);
    for (int i = 0; i < sizeof(keyname) / sizeof(char*); i++) {
      if (strcmp(kn , keyname[i]) == 0) {
        event->key.keysym.sym = i;
        keystate[i] = buf[1] == 'd' ? 1 : 0;
        return 1;
      }
    }
  }
  else {
    assert(0);
  }
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  if (numkeys != NULL) *numkeys = sizeof(keyname) / sizeof(char*);
  return keystate;
}
