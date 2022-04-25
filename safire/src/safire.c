#include "../include/safire/safire.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// void safire(const char* window_title, uint32_t window_width, uint32_t window_height, bool fullscreen, SFRentity_t* game_manager) {
void safire(const char* window_title, int window_width, int window_height, bool fullscreen, SFRscene_t** scenes) {  
  sfr_pipeline_init(window_title, window_width, window_height, fullscreen);
  sfr_input_init();
  sfr_ecs_init(scenes);
  sfr_run();
  sfr_free();
}

void sfr_free() {
  printf("free safire ...\n");
  sfr_pipeline_free();
  sfr_ecs_free();
}

void sfr_run() {
  while (!sfr_pipeline_window_closing()) {

    sfr_pipeline_render();
    glfwPollEvents();
  }
}

float sfr_elapsed_time() {
  return (float)glfwGetTime();
}

SFRentity_t* sfr_instantiate(const char* name) {
  return sfr_ecs_push_entity(name);
}

void sfr_destroy(SFRentity_t* entity) {
  // TODO: ...
}
