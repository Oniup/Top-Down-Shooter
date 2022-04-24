#include "../include/safire/ecs.h"

struct SFRecs {
  SFRentity_t** entities;
  uint32_t entities_count;
  SFRcomponent_t** components;
  uint32_t components_count;
  char* current_scene;
};

static SFRecs_t* ecs = NULL;

void sfr_ecs_init(SFRscene_t* opening_scene) {
  ecs = (SFRecs_t*)malloc(sizeof(SFRecs_t));
  SAFIRE_ASSERT(ecs, "[SAFIRE::ECS] failed to create ecs for some reason ...");

  ecs->entities = NULL;
  ecs->components = NULL;
  ecs->entities_count = 0;
  ecs->components_count = 0;

  // TODO: create scene instance
}

SFRecs_t* sfr_ecs_instance() {
  return ecs;
}

void sfr_ecs_free() {
  // TODO: ...
  printf("free ecs ...\n");
}

SFRscene_t* sfr_scene(sfr_scene_start start) {

}

void sfr_scene_free(SFRscene_t* scene) {

}

SFRentity_t* sfr_ecs_push_entity(const char* name) {
  return NULL;
}

SFRcomponent_t* sfr_ecs_push_compnoent(SFRentity_t* entity, SFRcomponent_t* component) {
  return NULL;
}

SFRentity_t* sfr_ecs_find_entity_name(const char* name) {
  return NULL;
}

SFRentity_t* sfr_ecs_find_entity_uuid(uint64_t uuid) {
  return NULL;
}

SFRcomponent_t** sfr_ecs_find_list_components_name(const char* name) {
  return NULL;
}
