#include "dnload.h"

#if defined(USE_LD)
int main(int argc, char *argv[]) {
#else
void _start() {
#endif

  dnload();

  dnload_SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *w = dnload_SDL_CreateWindow(nullptr,
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      1280, 720, SDL_WINDOW_SHOWN);

  /*SDL_Event e;
  while (e.type != SDL_QUIT) {
    dnload_SDL_PollEvent(&e);
  }*/

#if defined(USE_LD)
  SDL_Quit();
  return 0;
#else
  asm_exit();
#endif
}
