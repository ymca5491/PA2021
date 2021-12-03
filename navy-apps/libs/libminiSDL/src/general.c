#include <NDL.h>

int SDL_Init(uint32_t flags) {
  return NDL_Init(flags);
}

void SDL_Quit() {
  NDL_Quit();
}

char *SDL_GetError() {
  return "Navy does not support SDL_GetError()";
}

int SDL_SetError(const char* fmt, ...) {
  panic("Not implemented\n");
  return -1;
}

int SDL_ShowCursor(int toggle) {
  panic("Not implemented\n");
  return 0;
}

void SDL_WM_SetCaption(const char *title, const char *icon) {
  panic("Not implemented\n");
}
