#include "../include/safire/pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

SFRwindow_t* sfr_window_create(const char* window_title, int window_width, int window_height, bool fullscreen, bool transparent) {
  // tells glfw what version of glsl we are using
  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 5);

#ifdef __APPLE__
  // I have no clue what this is doing, I just know you have to add this if on Mac 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

  SFRwindow_t* window = (SFRwindow_t*)malloc(sizeof(SFRwindow_t));
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] something went wrong ...");

  size_t title_length = strlen(window_title);
  window->title = (char*)malloc(sizeof(char*) * title_length);
  window->title[title_length] = '\0';
  strncpy(window->title, window_title, title_length);

  window->fullscreen = fullscreen;
  window->transparent = transparent;

  if (window->transparent) {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  } else {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
  }

  // important for fullscreen and putting the window in the centre of the monitor on windows cause for some reason windows likes to not do that by default
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  if (window->fullscreen) {
    window->width = vidmode->width;
    window->height = vidmode->height;
    window->window = glfwCreateWindow(window->width, window->height, window->title, monitor, NULL);
    SAFIRE_ASSERT(window->window, "[SAFIRE::PIPELINE_WINDOW] something went wrong ...");
  } else {
    window->width = window_width;
    window->height = window_height;
    window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);

    // centring the window
    ivec2 position;
    glfwGetMonitorPos(monitor, &position[X], &position[Y]);
    glfwSetWindowPos(
      window->window,
      position[X] + (vidmode->width - window->width) / 2,
      position[Y] + (vidmode->height - window->height) / 2
    );
  }

  glfwMakeContextCurrent(window->window);
  SAFIRE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "[SAFIRE::PIPELINE_WINDOW] something when wrong when initialising glad ...");
  glfwSwapInterval(0); // by setting it to 0, it should disable vsync, this might cause screen tearing, but most likely it won't

  glViewport(0, 0, window->width, window->height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // this is some blend function, idk too much about this

  glm_vec4_copy((vec4){ 0.0f, 0.0f, 0.0f, 1.0f }, window->clear_colour);

  return window;
}

void sfr_window_free(SFRwindow_t* window) {
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to free window");
  if (window->title != NULL) {
    free(window->title);
  }
  glfwDestroyWindow(window->window);
  free(window);
}

void sfr_window_set_clear(SFRwindow_t* window, vec4 clear_colour) {
  glm_vec4_copy(clear_colour, window->clear_colour);
}

void sfr_window_set_title(SFRwindow_t* window, const char* title) {
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to set title");
  SAFIRE_ASSERT(title, "[SAFIRE::PIPELINE_WINDOW] failed to set title");
  glfwSetWindowTitle(window->window, title);
  if (window->title != NULL) {
    free(window->title);
  }
  size_t length = strlen(title);
  window->title = (char*)malloc(sizeof(char*) * length);
  window->title[length] = '\0';
  strncpy(window->title, title, length);
}

void sfr_window_clear(SFRwindow_t* window) {
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to clear window");
  glClearColor(window->clear_colour[R], window->clear_colour[G], window->clear_colour[B], window->clear_colour[A]);
  glClear(GL_COLOR_BUFFER_BIT); // don't need to include the GL_DEPTH_BUFFER_BIT as it is a 2D renderer not a 3D renderer
  glfwGetFramebufferSize(window->window, &window->width, &window->height);
  glViewport(0, 0, window->width, window->height);
}

void sfr_window_swap_buffers(SFRwindow_t* window) {
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to swap buffers");
  glfwSwapBuffers(window->window);
}

bool sfr_window_closing(SFRwindow_t* window) {
  return (bool)glfwWindowShouldClose(window->window);
}

void sfr_window_quit(SFRwindow_t* window) {
  glfwSetWindowShouldClose(window->window, 0);
}
