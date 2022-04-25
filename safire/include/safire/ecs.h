#ifndef __SAFIRE_ECS_H__
#define __SAFIRE_ECS_H__

#ifdef __cplusplus 
SAFIRE_API "C" {
#endif

#include "utils.h"

typedef struct SFRecs SFRecs_t;
typedef struct SFRentity SFRentity_t;
typedef struct SFRcomponent SFRcomponent_t;

typedef struct SFRscene SFRscene_t;

SAFIRE_API void sfr_ecs_init(SFRscene_t** scenes);
SAFIRE_API SFRecs_t* sfr_ecs_instance();
SAFIRE_API void sfr_ecs_free();
SAFIRE_API void sfr_ecs_clear_stack();

SAFIRE_API void sfr_ecs_load_scene(uint32_t id);

typedef void (*sfr_scene_start)(SFRscene_t*);
typedef void (*sfr_scene_update)(SFRscene_t*);
typedef void (*sfr_scene_late_update)(SFRscene_t*);

struct SFRscene {
  const char* name;

  sfr_scene_start start;
  sfr_scene_update update;
  sfr_scene_late_update late_update;
};

SAFIRE_API SFRscene_t* sfr_scene(sfr_scene_start start);
SAFIRE_API uint32_t sfr_find_scene(const char* name);

struct SFRentity {
  uint64_t uuid;
  char* name;
};

struct SFRcomponent {
  uint64_t uuid;
  char* name;
  SFRentity_t* owner;

  // TODO: add pointer functions here ...  
};

SAFIRE_API SFRentity_t* sfr_ecs_push_entity(const char* name);
SAFIRE_API SFRcomponent_t* sfr_ecs_push_component(SFRentity_t* entity, SFRcomponent_t* component);

SAFIRE_API void sfr_ecs_entity_free(SFRentity_t* entity);
SAFIRE_API void sfr_ecs_component_free(SFRcomponent_t* component);

SAFIRE_API void sfr_ecs_entity_target_free();
SAFIRE_API void sfr_ecs_component_target_free();

SAFIRE_API SFRentity_t* sfr_ecs_find_entity_name(const char* name);
SAFIRE_API SFRentity_t* sfr_ecs_find_entity_uuid(uint64_t uuid);
SAFIRE_API SFRcomponent_t** sfr_ecs_find_list_components_name(const char* name);

#ifdef __cplusplus
}
#endif 

#endif // __SAFIRE_ECS_H__