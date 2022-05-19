#include "../include/safire/safire.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

float calculate_delta_time();

void safire(const char* window_title, int window_width, int window_height, bool fullscreen, SFR_Scene** scenes, uint32_t scenes_count) 
{  
  sfr_pipeline_init(window_title, window_width, window_height, fullscreen);
  sfr_input_init();
  sfr_ecs_init(scenes, scenes_count);
  sfr_run();
  sfr_free();
}

void sfr_free() 
{
  sfr_ecs_free();
  sfr_pipeline_free();
}

void sfr_run() 
{
  while (!sfr_pipeline_window_closing()) 
  {
    sfr_ecs_remove_erased_entities();

    // static SFR_Timer onesec = 0.0f;
    // static uint32_t frame_count = 0;
    // if (sfr_timer_finished(&onesec))
    // {
    //   onesec = sfr_timer_start(1.0f);
    //   printf("fps: %u\n", frame_count);
    //   frame_count = 0;
    // }
    // frame_count++;

    float delta_time = calculate_delta_time();
    sfr_ecs_update(delta_time);
    delta_time = calculate_delta_time();
    sfr_ecs_late_update(delta_time);

    sfr_ecs_render_update();

    glfwPollEvents();
    sfr_pipeline_render();

  }
}

float sfr_elapsed_time() 
{
  return (float)glfwGetTime();
}

SFR_Entity* sfr_instantiate(const char* name) 
{
  return NULL;
}

void sfr_destroy(SFR_Entity* entity) 
{
  // TODO: ...
}

float calculate_delta_time() 
{
  float time = sfr_elapsed_time();
  static float last_time = 0;
  float delta_time = time - last_time;
  last_time = time;
  return delta_time;
}
