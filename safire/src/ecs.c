#include "../include/safire/ecs.h"

void sfr_ecs_init() {
  // TODO: ...
  printf("init ecs ...\n");
}

void sfr_ecs_free() {
  // TODO: ...
  printf("free ecs ...\n");
}

SFRentity_t* sfr_ecs_push_entity(const char* name) {
  // TODO: ...
  return NULL;
}

SFRentity_t* sfr_ecs_find_entity_name(const char* name) {
  // TODO: ...
  return NULL;
}

SFRentity_t* sfr_ecs_find_entity_uuid(uint64_t uuid) {
  // TODO: ...
  return NULL;
}

SFRcomponent_t* sfr_ecs_push_components(SFRentity_t* entity, SFRcomponent_t* component) {
  // TODO: ...
  return NULL;
}

SFRcomponent_t** sfr_ecs_find_list_components_name(const char* name) {
  // TODO: ...
  return NULL;
}

