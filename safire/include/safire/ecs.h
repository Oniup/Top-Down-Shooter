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

SAFIRE_API void sfr_ecs_init(SFRscene_t* opening_scene);
SAFIRE_API SFRecs_t* sfr_ecs_instance();
SAFIRE_API void sfr_ecs_free();

typedef void (*sfr_scene_start)(SFRscene_t*);
typedef void (*sfr_scene_update)(SFRscene_t*);
typedef void (*sfr_scene_late_update)(SFRscene_t*);

struct SFRscene {
  const char* scene_name;

  sfr_scene_start start;
  sfr_scene_update update;
  sfr_scene_late_update late_update;

  SFRentity_t* entities;
  SFRcomponent_t* components;
  uint32_t entities_count;
  uint32_t components_count;  
};

SAFIRE_API SFRscene_t* sfr_scene(sfr_scene_start start);
SAFIRE_API void sfr_scene_free(SFRscene_t* scene);

struct SFRentity {
  uint64_t uuid;
  char* name;
};

struct SFRcomponent {
  uint64_t uuid;
};

SAFIRE_API SFRentity_t* sfr_ecs_push_entity(const char* name);
SAFIRE_API SFRcomponent_t* sfr_ecs_push_compnoent(SFRentity_t* entity, SFRcomponent_t* component);

SAFIRE_API SFRentity_t* sfr_ecs_find_entity_name(const char* name);
SAFIRE_API SFRentity_t* sfr_ecs_find_entity_uuid(uint64_t uuid);
SAFIRE_API SFRcomponent_t** sfr_ecs_find_list_components_name(const char* name);

#ifdef __cplusplus
}
#endif 

#endif // __SAFIRE_ECS_H__