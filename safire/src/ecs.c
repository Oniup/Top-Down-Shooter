#include "../include/safire/ecs.h"

struct SFRecs {
  uint32_t                  current_scene;
  SFRscene_t**              scenes;
  uint32_t                  scenes_count;

  SFRentity_t**             entities;
  SFRcomponent_t**          components;
  uint32_t                  entities_count;
  uint32_t                  components_count;  

  SFRentity_t**             adding_entities;
  SFRcomponent_t**          adding_components;
  uint32_t                  adding_entities_count;
  uint32_t                  adding_components_count;

  uint32_t*                 terminate_entities;
  uint32_t                  terminate_entities_count;
};

#define SFR_CURRENT_SCENE ecs->scenes[ecs->current_scene]

static SFRecs_t* ecs = NULL;

void sfr_ecs_init(SFRscene_t** scenes, uint32_t scenes_count) {
  ecs = (SFRecs_t*)malloc(sizeof(SFRecs_t));
  SAFIRE_ASSERT(ecs, "[SAFIRE::ECS] failed to create ecs for some reason ...");

  ecs->current_scene = 0;
  ecs->scenes = scenes;
  ecs->scenes_count = scenes_count;

  ecs->entities = NULL;
  ecs->components = NULL;
  ecs->entities_count = 0;
  ecs->components_count = 0;

  ecs->adding_entities = NULL;
  ecs->adding_components = NULL;
  ecs->adding_entities_count = 0;
  ecs->adding_components_count = 0;

  ecs->terminate_entities = NULL;
  ecs->terminate_entities_count = 0;

  SFR_CURRENT_SCENE->start(SFR_CURRENT_SCENE);

  for (uint32_t i = 0; i < ecs->scenes_count; i++) {
    printf("scene name: %s\n", scenes[i]->name);
  }

  // TODO: create scene instance
}

SFRecs_t* sfr_ecs_instance() {
  return ecs;
}

void sfr_ecs_free() {
  sfr_ecs_clear_stack();

  for (uint32_t i = 0; i < ecs->scenes_count; i++) {
    if (ecs->scenes[i]->free != NULL) {
      ecs->scenes[i]->free(ecs->scenes[i]);
    }
    sfr_str_free(&ecs->scenes[i]->name);
    free(ecs->scenes[i]);
  }
  free(ecs->scenes);

  free(ecs);
  ecs = NULL;
}

void sfr_ecs_clear_stack() {
  if (ecs->entities != NULL) {
    if (ecs->entities_count > 0) {
      for (uint32_t i = 0; i < ecs->entities_count; i++) {
        sfr_ecs_entity_free(ecs->entities[i]);
      }
    }
    free(ecs->entities);
    ecs->entities = NULL;
  }
  ecs->entities_count = 0;

  if (ecs->components != NULL) {
    if (ecs->components_count > 0) {
      for (uint32_t i = 0; i < ecs->components_count; i++) {
        sfr_ecs_component_free(ecs->components[i]);
      }
    } 
    free(ecs->components);
    ecs->components = NULL;
  }
  ecs->components_count = 0;
}

void sfr_ecs_update(float delta_time) {
  // TODO:: entities update loop implementation ...
  if (SFR_CURRENT_SCENE->update != NULL) {
    SFR_CURRENT_SCENE->update(SFR_CURRENT_SCENE, delta_time);
  }
}

void sfr_ecs_late_update(float late_delta_time) {
  // TODO:: entities late update loop implementation ...
  if (SFR_CURRENT_SCENE->late_update != NULL) {
    SFR_CURRENT_SCENE->late_update(SFR_CURRENT_SCENE, late_delta_time);
  }
}

void sfr_ecs_load_scene(uint32_t id) {
  sfr_ecs_clear_stack();

  if (SFR_CURRENT_SCENE->free != NULL) {
    SFR_CURRENT_SCENE->free(SFR_CURRENT_SCENE);
  }

  ecs->current_scene = id;
  SFR_CURRENT_SCENE->start(SFR_CURRENT_SCENE);
}
 
SFRscene_t* sfr_scene(const char* name, sfr_scene_init init) {
  SAFIRE_ASSERT(init, "[SAFIRE::ECS] failed to create scene as the start function pointer doesn't exist");

  SFRscene_t* scene = (SFRscene_t*)malloc(sizeof(SFRscene_t));
  scene->name = sfr_str(name);
  init(scene);

  return scene;
}

uint32_t sfr_find_scene(const char* name) {
  return 0;
}

SFRentity_t* sfr_ecs_push_entity(const char* name) {
  return NULL;
}

SFRcomponent_t* sfr_ecs_push_component(SFRentity_t* entity, SFRcomponent_t* component) {
  return NULL;
}

void sfr_ecs_entity_free(SFRentity_t* entity) {
  if (entity != NULL) {
    sfr_str_free(&entity->name);
    free(entity);
  }
}

void sfr_ecs_component_free(SFRcomponent_t* component) {

}

void sfr_ecs_entity_target_free() {

}

void sfr_ecs_component_target_free() {

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
