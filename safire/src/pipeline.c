#include "../include/safire/pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct SFRpipeline {
  SFRshader_t* shaders;
  uint32_t shaders_count;
  SFRtexture_t* textures;
  uint32_t textures_count;
};


void sfr_pipeline_init() {
  // TODO: ...
  printf("init pipeline ...\n");
}
void sfr_pipeline_free() {
  printf("free pipeline ...\n");
}

void sfr_pipeline_push_shader(const char* name, const char* vertex_path, const char* fragment_path) {
  // TODO: ...
}

void sfr_pipeline_push_texture(const char* name, const char* path, bool flip, uint32_t channels) {
  // TODO: ...
}
