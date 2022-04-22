#include "../include/safire/safire.h"

struct SFRcontext {

  SFRpipeline_t* pipeline;
  SFRecs_t* ecs;
};

void sfr_init(const char* window_title, uint32_t window_width, uint32_t window_height, bool fullscreen) {
  printf("init safire ...\n");
  printf("window title: %s, window size: [%u, %u]\n", window_title, window_width, window_height);
  sfr_pipeline_init();
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
  printf("run safire ...\n");
}

SFRpipeline_t* sfr_get_pipeline() {
  return NULL;
}

SFRecs_t* sfr_get_ecs() {
  return NULL;
}
