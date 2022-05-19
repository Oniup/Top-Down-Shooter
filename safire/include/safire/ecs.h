#ifndef __SAFIRE_ECS_H__
#define __SAFIRE_ECS_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "utils.h"

typedef struct SFR_Ecs                  SFR_Ecs;
typedef struct SFR_Entity               SFR_Entity;
typedef struct SFR_Component            SFR_Component;
typedef struct SFR_Scene                SFR_Scene;

typedef enum SFR_ComponentType          SFR_ComponentType;

typedef uint64_t                        SFR_Uuid;

SAFIRE_API void                         sfr_ecs_init(SFR_Scene** scenes, uint32_t scenes_count);
SAFIRE_API SFR_Ecs*                     sfr_ecs_instance();
SAFIRE_API void                         sfr_ecs_free();
SAFIRE_API void                         sfr_ecs_clear_stack();                                        // frees all entities & components from the stack

SAFIRE_API void                         sfr_ecs_remove_erased_entities();

SAFIRE_API void                         sfr_ecs_update(float delta_time);                             // calls all the components update loops that have it
SAFIRE_API void                         sfr_ecs_late_update(float late_delta_time);                   // calls al the components late update loops that have it
SAFIRE_API void                         sfr_ecs_render_update();                                      // calls all the graphics components loops that have it

SAFIRE_API void                         sfr_ecs_erase_entity(uint32_t index);                         // removes the target entity from the stack
SAFIRE_API void                         sfr_ecs_erase_component(uint32_t index);                      // removes the target component from the stack

SAFIRE_API SFR_Entity*                  sfr_ecs_get_target_entity(uint32_t index);
SAFIRE_API SFR_Component*               sfr_ecs_get_target_component(uint32_t index);

SAFIRE_API uint32_t                     sfr_ecs_get_entities_count();
SAFIRE_API uint32_t                     sfr_ecs_get_component_count();

SAFIRE_API uint32_t                     sfr_ecs_get_starting_index(SFR_ComponentType type);

SAFIRE_API void                         sfr_ecs_load_scene(uint32_t id);
SAFIRE_API SFR_Scene*                   sfr_ecs_get_active_scene();

SAFIRE_API void                         sfr_ecs_attach_default_comps(SFR_Entity* entity);




typedef void (*SFR_PtrSceneInit)       (SFR_Scene*);
typedef void (*SFR_PtrSceneStart)      (SFR_Scene*);
typedef void (*SFR_PtrSceneUpdate)     (SFR_Scene*, float);
typedef void (*SFR_PtrSceneLateUpdate) (SFR_Scene*, float);
typedef void (*SFR_PtrSceneFreeData)   (SFR_Scene*);

struct SFR_Scene 
{
  char*                                 name;
  void*                                 data;

  SFR_PtrSceneStart                     start;
  SFR_PtrSceneUpdate                    update;
  SFR_PtrSceneLateUpdate                late_update;
  SFR_PtrSceneFreeData                  free;
};

SAFIRE_API SFR_Scene*                   sfr_scene(const char* name, SFR_PtrSceneInit start);          // creates a scene instance
SAFIRE_API uint32_t                     sfr_find_scene(const char* name);                             // finds a scene instance so you can change the current scene




struct SFR_Entity 
{
  SFR_Uuid                              uuid;
  char*                                 name;
  char*                                 tag;
  uint32_t                              layer;

  SFR_Component**                       components;
  uint32_t                              components_count;
};

enum SFR_ComponentType 
{
  SFR_COMPONENT_TYPE_NON_FUNCTIONAL     = 0,
  SFR_COMPONENT_TYPE_FUNCTIONAL,
  SFR_COMPONENT_TYPE_PHYSICS,
  SFR_COMPONENT_TYPE_GRAPHICS
};

typedef void (*component_update)        (SFR_Component*, float);
typedef void (*component_late_update)   (SFR_Component*, float);
typedef void (*component_free)          (SFR_Component*);

struct SFR_Component 
{
  SFR_Uuid                              uuid;
  char*                                 name;
  SFR_Entity*                           owner;
  void*                                 data;
  SFR_ComponentType                     type;

  component_update                      update;
  component_late_update                 late_update;
  component_free                        free;
};

SAFIRE_API SFR_Entity*                  sfr_ecs_push_entity(const char* name, const char* tag);
SAFIRE_API SFR_Component*               sfr_ecs_push_component(SFR_Entity* entity, SFR_Component* component);

SAFIRE_API SFR_Component*               sfr_ecs_component(const char* name, component_update update, 
                                              component_late_update late_update, component_free free);

SAFIRE_API void                         sfr_ecs_entity_free(SFR_Entity* entity);
SAFIRE_API void                         sfr_ecs_component_free(SFR_Component* component);

SAFIRE_API void                         sfr_ecs_entity_set_layer(SFR_Entity* entity, uint32_t layer);

SAFIRE_API SFR_Entity*                  sfr_ecs_find_entity_name(const char* name);
SAFIRE_API SFR_Entity**                 sfr_ecs_find_list_entities(const char* tag, uint32_t* entity_count);
SAFIRE_API SFR_Entity*                  sfr_ecs_find_entity_uuid(SFR_Uuid uuid);
SAFIRE_API SFR_Component*               sfr_ecs_find_component_uuid(SFR_Uuid uuid);
SAFIRE_API SFR_Component**              sfr_ecs_find_list_components(const char* name, uint32_t* component_count);

SAFIRE_API uint32_t                     sfr_ecs_entity_find_index_name(uint32_t offset, const char* name);
SAFIRE_API uint32_t                     sfr_ecs_entity_find_index_uuid(uint32_t offset, SFR_Uuid uuid);
SAFIRE_API uint32_t                     sfr_ecs_component_find_index_uuid(uint32_t offset, SFR_Uuid uuid);

SAFIRE_API void                         sfr_ecs_debug_print_entities();
SAFIRE_API void                         sfr_ecs_debug_print_components();
SAFIRE_API void                         sfr_ecs_debug_print_entities_names();




#ifdef __cplusplus
}
#endif 

#endif // __SAFIRE_ECS_H__