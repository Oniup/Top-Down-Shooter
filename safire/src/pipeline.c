#include "../include/safire/pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct SFRpipeline {
  SFRshader_t* shaders;
  uint32_t shaders_count;

  SFRtexture_t* textures;
  uint32_t textures_count;

  SFRwindow_t* window;
};

SFRpipeline_t* pipeline = NULL;

void sfr_pipeline_init(const char* window_title, int window_width, int window_height, bool fullscreen) {
  pipeline = (SFRpipeline_t*)malloc(sizeof(SFRpipeline_t));

  SAFIRE_ASSERT(glfwInit(), "[SAFIRE::PIPELINE] failed to initialise glfw for some reason ...");

  pipeline->window = sfr_window_create(window_title, window_width, window_height, fullscreen, false);
  pipeline->shaders = NULL;
  pipeline->textures = NULL;
  pipeline->shaders_count = 0;
  pipeline->textures_count = 0;
}

void sfr_pipeline_render() {
  sfr_window_clear(pipeline->window);
  sfr_window_swap_buffers(pipeline->window);
}

bool sfr_pipeline_window_closing() {
  return sfr_window_closing(pipeline->window);
}

void sfr_pipeline_free() {
  sfr_window_free(pipeline->window);
  
  free(pipeline);
  pipeline = NULL;
  glfwTerminate();
}

SFRshader_t** sfr_pipeline_get_shaders() {
  return NULL;
}

SFRtexture_t** sfr_pipeline_get_textures() {
  return NULL;
}

SFRwindow_t* sfr_pipeline_get_window() {
  return pipeline->window;
}


void sfr_pipeline_push_shader(const char* name, const char* vertex_path, const char* fragment_path) {
  // TODO: ...
}

void sfr_pipeline_push_texture(const char* name, const char* path, bool flip, uint32_t channels) {
  // TODO: ...
}
