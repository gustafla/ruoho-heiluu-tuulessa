#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "GL/glew.h"
#include "music_player.h"
#include "sync.h" // Rocket
#include "demo.h"
#include "util.h"

#if !(SYNC_PLAYER)
static void pause(void *d, int flag) {
  static_cast<MusicPlayer*>(d)->pause(flag);
}

static void setRow(void *d, int row) {
  static_cast<MusicPlayer*>(d)->setPosition(row/ROW_RATE);
}

static int isPlaying(void *d) {
  return static_cast<MusicPlayer*>(d)->isPlaying();
}

static struct sync_cb playerControls = {
  pause, setRow, isPlaying
};
#endif

void toggle_fullscreen(SDL_Window *w) {
  bool isFullscreen = SDL_GetWindowFlags(w) & SDL_WINDOW_FULLSCREEN;
  SDL_SetWindowFullscreen(w, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
  SDL_ShowCursor(isFullscreen); // Based on old value
}

int main(int argc, char *argv[]) {
  int err;

  int width = 1280;
  int height = 720;
  bool fullscreen = false;

  // Parse conf file
  std::ifstream conf("demo.conf", std::ifstream::in);
  std::string token, op;
  int val;
  while (conf >> token >> op >> val) {
    if (token == "width") {
      width = val;
    } else if (token == "height") {
      height = val;
    } else if (token == "fullscreen") {
      fullscreen = val;
    }
  }

  // Parse cli args
  for (int i=1; i<argc-1; i++) { // Args with pairs here
    if (strcmp(argv[i], "-w") == 0) {
      width = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-h") == 0) {
      height = atoi(argv[i+1]);
    }
  }
  for (int i=1; i<argc; i++) { // Args without pair
    if (strcmp(argv[i], "--fullscreen") == 0) {
      fullscreen = true;
    }
  }

  // Init SDL2
  Uint32 const FLAGS = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
  if ((err = SDL_Init(FLAGS)) < 0) {
    std::cerr << "Failed to initialize SDL:"
      << std::endl << SDL_GetError() << std::endl;
    return err;
  }

  // Set OpenGL attributes
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

  // Init a window
  SDL_Window *w = SDL_CreateWindow("instanssidemo",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      width, height, SDL_WINDOW_OPENGL);

  if (w == nullptr) {
    std::cerr << "Failed to get a window with sRGB:"
      << std::endl << SDL_GetError() << std::endl;

    // Try again without sRGB
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 0);
    w = SDL_CreateWindow("instanssidemo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL);

    if (w == nullptr) { // Still no window?
      std::cerr << "Failed to get a window without sRGB (exiting):"
        << std::endl << SDL_GetError() << std::endl;
      die(EXIT_FAILURE);
    }
  }

  // Enter fullscreen if needed
  if (fullscreen) {
    toggle_fullscreen(w);
  }

  // Init a context
  SDL_GLContext c = SDL_GL_CreateContext(w);
  if (c == nullptr) {
    std::cerr << "Failed to get an OpenGL context:"
      << std::endl << SDL_GetError() << std::endl;
    die(EXIT_FAILURE);
  }

  // Load OpenGL api/calls/whatever
  if ((err = glewInit()) != GLEW_OK) {
    std::cerr << "GLEW failed to initialize:"
      << std::endl << glewGetErrorString(err) << std::endl;
    die(err);
  }

  // Not required as CreateContext does this already
  // SDL_GL_MakeCurrent(w, c);

  // Set regular vsync, late swap tearing not needed for this demo
  if (SDL_GL_SetSwapInterval(1)) {
    std::cerr << "Failed to get vsync:"
      << std::endl << SDL_GetError() << std::endl;
  }

  // Check if OpenGL framebuffer got SRGB capability and enable it
  int framebuffer_srgb;
  SDL_GL_GetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, &framebuffer_srgb);
  if (framebuffer_srgb) {
    glEnable(GL_FRAMEBUFFER_SRGB);
  }

  // Set OpenGL up further
  glViewport(0, 0, width, height);
  glClearColor(1, 0, 0, 1); // Red for visibility
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up rocket
  sync_device *rocket = sync_create_device("sync");

  // Set up music player
  MusicPlayer player = MusicPlayer("music.ogg");

  // Demo loop
  Demo demo(rocket, player);
  SDL_Event e;
  bool running = true;
  while (running) {

#if !(SYNC_PLAYER)
    // Rocket update
    if (sync_update(rocket, (int)(player.getTime()*ROW_RATE),
          &playerControls, (void*)&player)) {
      sync_tcp_connect(rocket, "localhost", SYNC_DEFAULT_PORT);
    }
#endif

    demo.render();

    // Swap window framebuffers
    SDL_GL_SwapWindow(w);

    // Handle SDL events suh as kill signals and keyboard
    SDL_PollEvent(&e);
    switch(e.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
          case SDLK_q:
          case SDLK_ESCAPE:
            running = false;
            break;
          case SDLK_f:
            toggle_fullscreen(w);
            break;
        }
        break;
    }
  }

#if !(SYNC_PLAYER)
  sync_save_tracks(rocket);
#endif
  sync_destroy_device(rocket);
  SDL_Quit();
  return 0;
}
