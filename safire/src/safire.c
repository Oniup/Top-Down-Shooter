#include "../include/safire/safire.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// void safire(const char* window_title, uint32_t window_width, uint32_t window_height, bool fullscreen, SFRentity_t* game_manager) {
void safire(const char* window_title, int window_width, int window_height, bool fullscreen) {  
  sfr_pipeline_init(window_title, window_width, window_height, fullscreen);
  sfr_ecs_init();
  sfr_run();
  sfr_free();
}

void sfr_free() {
  printf("free safire ...\n");
  sfr_pipeline_free();
  sfr_ecs_free();
}

void sfr_run() {
  SFRwindow_t* window = sfr_pipeline_get_window();
  while (!sfr_window_closing(window)) {
    sfr_window_clear(window);
    sfr_window_swap_buffers(window);
    glfwPollEvents();
  }
}

SFRpipeline_t* sfr_get_pipeline() {
  return NULL;
}

SFRecs_t* sfr_get_ecs() {
  return NULL;
}
