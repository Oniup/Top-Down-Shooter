#ifndef __SAFIRE_ECS_H__
#define __SAFIRE_ECS_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "utils.h"

typedef struct SFRecs                   SFRecs_t;
typedef struct SFRentity                SFRentity_t;
typedef struct SFRcomponent             SFRcomponent_t;
typedef struct SFRscene                 SFRscene_t;

typedef uint64_t                        SFRuuid_t;

SAFIRE_API void             sfr_ecs_init(SFRscene_t** scenes, uint32_t scenes_count);
SAFIRE_API SFRecs_t*        sfr_ecs_instance();
SAFIRE_API void             sfr_ecs_free();
SAFIRE_API void             sfr_ecs_clear_stack();                                        // frees all entities & components from the stack

SAFIRE_API void             sfr_ecs_update(float delta_time);                             // calls all the components update loops that have it
SAFIRE_API void             sfr_ecs_late_update(float late_delta_time);                   // calls al the components late update loops that have it

SAFIRE_API void             sfr_ecs_erase_entity(uint32_t index);                         // removes the target entity from the stack
SAFIRE_API void             sfr_ecs_erase_component(uint32_t index);                      // removes the target component from the stack

SAFIRE_API void             sfr_ecs_load_scene(uint32_t id);




typedef void (*sfr_scene_init)          (SFRscene_t*);
typedef void (*sfr_scene_start)         (SFRscene_t*);
typedef void (*sfr_scene_update)        (SFRscene_t*, float);
typedef void (*sfr_scene_late_update)   (SFRscene_t*, float);
typedef void (*sfr_scene_free_data)     (SFRscene_t*);

struct SFRscene {
  char*                     name;
  void*                     data;

  sfr_scene_start           start;
  sfr_scene_update          update;
  sfr_scene_late_update     late_update;
  sfr_scene_free_data       free;
};

SAFIRE_API SFRscene_t*      sfr_scene(const char* name, sfr_scene_init start);            // creates a scene instance
SAFIRE_API uint32_t         sfr_find_scene(const char* name);                             // finds a scene instance so you can change the current scene




struct SFRentity {
  SFRuuid_t                 uuid;
  char*                     name;
  char*                     tag;

  SFRuuid_t*                components;
  uint32_t                  components_count;
};

typedef void (*component_update)        (SFRcomponent_t*, float);
typedef void (*component_late_update)   (SFRcomponent_t*, float);
typedef void (*component_free)          (SFRcomponent_t*);

struct SFRcomponent {
  SFRuuid_t                 uuid;
  char*                     name;
  SFRentity_t*              owner;
  void*                     data;

  component_update update;
  component_late_update late_update;
  component_free free;
};

SAFIRE_API SFRentity_t*     sfr_ecs_push_entity(const char* name, const char* tag);
SAFIRE_API SFRcomponent_t*  sfr_ecs_push_component(SFRentity_t* entity, SFRcomponent_t* component);

SAFIRE_API SFRcomponent_t*  sfr_ecs_component(const char* name, component_update update, 
                                              component_late_update late_update, component_free free);

SAFIRE_API void             sfr_ecs_entity_free(SFRentity_t* entity);
SAFIRE_API void             sfr_ecs_component_free(SFRcomponent_t* component);

SAFIRE_API void             sfr_ecs_entity_target_free(uint32_t id);
SAFIRE_API void             sfr_ecs_component_target_free(uint32_t id);

SAFIRE_API SFRentity_t*     sfr_ecs_find_entity_name(const char* name);
SAFIRE_API SFRentity_t**    sfr_ecs_find_list_entities(const char* tag, uint32_t* entity_count);
SAFIRE_API SFRentity_t*     sfr_ecs_find_entity_uuid(SFRuuid_t uuid);
SAFIRE_API SFRcomponent_t*  sfr_ecs_find_component_uuid(SFRuuid_t uuid);
SAFIRE_API SFRcomponent_t** sfr_ecs_find_list_components(const char* name, uint32_t* component_count);

SAFIRE_API uint32_t         sfr_ecs_entity_find_index_name(uint32_t offset, const char* name);
SAFIRE_API uint32_t         sfr_ecs_entity_find_index_uuid(uint32_t offset, SFRuuid_t uuid);
SAFIRE_API uint32_t         sfr_ecs_component_find_index_uuid(uint32_t offset, SFRuuid_t uuid);

SAFIRE_API void             sfr_ecs_debug_print_entities();
SAFIRE_API void             sfr_ecs_debug_print_components();




#ifdef __cplusplus
}
#endif 

#endif // __SAFIRE_ECS_H__