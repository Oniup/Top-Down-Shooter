#include "../include/safire/safire.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

float calculate_delta_time();

void safire(const char* window_title, int window_width, int window_height, bool fullscreen, SFRscene_t** scenes, uint32_t scenes_count) {  
  sfr_pipeline_init(window_title, window_width, window_height, fullscreen);
  sfr_input_init();
  sfr_ecs_init(scenes, scenes_count);
  sfr_run();
  sfr_free();
}

void sfr_free() {
  sfr_ecs_free();
  sfr_pipeline_free();
}

void sfr_run() {

 while (!sfr_pipeline_window_closing()) {
    sfr_ecs_remove_erased_entities();

    float delta_time = calculate_delta_time();
    sfr_ecs_update(delta_time);
    delta_time = calculate_delta_time();
    sfr_ecs_late_update(delta_time);

    sfr_ecs_render_update();

    glfwPollEvents();
    sfr_pipeline_render();
  }

}

float sfr_elapsed_time() {
  return (float)glfwGetTime();
}

SFRentity_t* sfr_instantiate(const char* name) {
  return NULL;
}

void sfr_destroy(SFRentity_t* entity) {
  // TODO: ...
}

float calculate_delta_time() {
  float time = sfr_elapsed_time();
  static float last_time = 0;
  float delta_time = time - last_time;
  last_time = time;
  return delta_time;
}
