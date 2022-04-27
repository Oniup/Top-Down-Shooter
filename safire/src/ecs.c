#include "../include/safire/ecs.h"

typedef struct SFRecs_names_used        SFRecs_names_used_t;

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

  SFRecs_names_used_t*      names_used;
  uint32_t                  names_used_count;
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

void sfr_ecs_erase_entity(uint32_t index) {
  // TODO: ...
}

void sfr_ecs_erase_component(uint32_t index) {
  // TODO: ...
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

SFRentity_t* sfr_ecs_push_entity(const char* name, const char* tag) {
#ifndef NDEBUG
  if (name != NULL) {
    uint32_t length = sfr_str_length(name);
    if (length > 0) {

#endif

      // incrase entity buffer size
      uint32_t c = ecs->entities_count;
      ecs->entities_count++;
      if (ecs->entities != NULL) {
        SFRentity_t** temp = (SFRentity_t**)realloc(ecs->entities, sizeof(SFRentity_t*) * ecs->entities_count);
        SAFIRE_ASSERT(temp, "[SAFIRE::ECS_ENTITY_PUSH] failed to resize entity buffer for some reason");
        ecs->entities = temp;
      } else {
        ecs->entities = (SFRentity_t**)malloc(sizeof(SFRentity_t*) * ecs->entities_count);
      }

      // setting the entity data 
      ecs->entities[c] = (SFRentity_t*)malloc(sizeof(SFRentity_t));
      SAFIRE_ASSERT(ecs->entities[c], "[SAFIRE::ECS_ENTITY_PUSH] failed to assign memory to entity for some reason");

      ecs->entities[c]->name = sfr_str(name);
      ecs->entities[c]->uuid = sfr_rand_uint64();
      ecs->entities[c]->tag = tag != NULL ? sfr_str(tag) : NULL;
      ecs->entities[c]->components = NULL;
      ecs->entities[c]->components_count = 0;

      return ecs->entities[c];

#ifndef NDEBUG
    }
  }
  return NULL;
#endif
}

SFRcomponent_t* sfr_ecs_push_component(SFRentity_t* entity, SFRcomponent_t* component) {
  #ifndef NDEBUG
  if (component != NULL) {
    uint32_t length = sfr_str_length(component->name);
    if (length > 0) {

#endif

      // incrase entity buffer size
      uint32_t c = ecs->components_count;
      ecs->components_count++;
      if (ecs->components != NULL) {
        SFRcomponent_t** temp = (SFRcomponent_t**)realloc(ecs->components, sizeof(SFRcomponent_t*) * ecs->components_count);
        SAFIRE_ASSERT(temp, "[SAFIRE::ECS_COMPONENT_PUSH=] failed to resize component buffer for some reason");
        ecs->components = temp;
      } else {
        ecs->components = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * ecs->entities_count);
      }

      // setting the entity data 
      ecs->components[c] = component;
      SAFIRE_ASSERT(ecs->components[c], "[SAFIRE::ECS_COMPONENT_PUSH] failed to assign memory to component for some reason");

      ecs->components[c]->uuid = sfr_rand_uint64();
      ecs->components[c]->owner = entity;

      return ecs->components[c];

#ifndef NDEBUG
    }
  }
  return NULL;
#endif
}

SFRcomponent_t*  sfr_ecs_component(const char* name, component_update update, component_late_update late_update, component_free free) {
  SFRcomponent_t* component = (SFRcomponent_t*)malloc(sizeof(SFRcomponent_t*));
  SAFIRE_ASSERT(component, "[SAFIRE::ECS_COMPONENT] failed to assgin memory to component for some reason");

  component->name = sfr_str(name);
  component->update = update;
  component->late_update = late_update;
  component->free = free;

  component->owner = NULL;
  component->uuid = 0;
  component->data = NULL;

  return component;
}

void sfr_ecs_entity_free(SFRentity_t* entity) {
  if (entity != NULL) {
    if (entity->components != NULL) {
      // freeing the component data
      uint32_t target = 0;
      for (uint32_t i = 0; i < entity->components_count; i++) {
        target = sfr_ecs_component_find_index_uuid(target, entity->components[i]);
        sfr_ecs_component_free(ecs->components[target]);
        sfr_ecs_erase_component(target);
      }
      free(entity->components);
    }

    sfr_str_free(&entity->name);

    if (entity->tag != NULL) {
      sfr_str_free(&entity->tag);
    }
    
    free(entity);
  }
}

void sfr_ecs_component_free(SFRcomponent_t* component) {
  if (component != NULL) {
    sfr_str_free(&component->name);

    if (component->free != NULL) {
      component->free(component);
      if (component->data != NULL) {
        free(component->data);
      }
    }

    free(component);
  }
}

SFRentity_t* sfr_ecs_find_entity_name(const char* name) {
  // Im pre sure that there is a better way of doing this, but idk how to improve this
  uint32_t length = sfr_str_length(name);
  for (uint32_t i = 0; i < ecs->entities_count; i++) {
    if (sfr_str_cmp_length(name, ecs->entities[i]->name, length)) {
      return ecs->entities[i];
    }
  }

  return NULL;
}

SFRentity_t** sfr_ecs_find_list_entities(const char* tag, uint32_t* entity_count) {
  SFRentity_t** buffer = NULL;
  uint32_t count = 0;
  uint32_t length = sfr_str_length(tag);

  for (uint32_t i = 0; i < ecs->entities_count; i++) {
    if (sfr_str_cmp_length(tag, ecs->entities[i]->tag, length)) {
      count += 1;
      if (buffer != NULL) {
        SFRentity_t** temp =  (SFRentity_t**)realloc(buffer, sizeof(SFRentity_t*) * count);
        if (temp != NULL) {
          buffer = temp;
        } else {
          printf("[SAFIRE-WARNING::ECS_ENTITIES] failed to find entities with the tag %s because something went wrong when resizing the buffer\n", tag);

          free(buffer);

          *entity_count = 0;
          return NULL;
        }
      } else {
        buffer = (SFRentity_t**)malloc(sizeof(SFRentity_t*) * count);
        if (buffer == NULL) {
          printf("[SAFIRE-WARNING::ECS_ENTITIES] failed to find entities with the tag %s because something went wrong when creating the buffer\n", tag);

          *entity_count = 0;
          return NULL;
        }
      }

      buffer[i] = ecs->entities[i];
    }
  }

  *entity_count = count;
  return buffer;
}

SFRentity_t* sfr_ecs_find_entity_uuid(uint64_t uuid) {
  // I don't really know how to implement this with uuid other than a linear search, so slow code here too I guess
  for (uint32_t i = 0; i < ecs->entities_count; i++) {
    if (uuid == ecs->entities[i]->uuid) {
      return ecs->entities[i];
    }
  }

  return NULL;
}

SFRcomponent_t* sfr_ecs_find_component_uuid(SFRuuid_t uuid) {
  // I don't really know how to implement this with uuid other than a linear search, so slow code here too I guess
  for (uint32_t i = 0; i < ecs->components_count; i++) {
    if (uuid == ecs->components[i]->uuid) {
      return ecs->components[i];
    }
  }

  return NULL;
}

SFRcomponent_t** sfr_ecs_find_list_components(const char* name, uint32_t* component_count) {
  SFRcomponent_t** buffer = NULL;
  uint32_t count = 0;
  uint32_t length = sfr_str_length(name);

  for (uint32_t i = 0; i < ecs->components_count; i++) {
    if (sfr_str_cmp_length(name, ecs->components[i]->name, length)) {
      count += 1;
      if (buffer != NULL) {
        SFRcomponent_t** temp =  (SFRcomponent_t**)realloc(buffer, sizeof(SFRcomponent_t*) * count);
        if (temp != NULL) {
          buffer = temp;
        } else {
          printf("[SAFIRE-WARNING::ECS_COMPONENT] failed to find components with the tag %s because something went wrong when resizing the buffer\n", name);

          free(buffer);

          *component_count = 0;
          return NULL;
        }
      } else {
        buffer = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * count);
        if (buffer == NULL) {
          printf("[SAFIRE-WARNING::ECS_COMPONENT] failed to find components with the tag %s because something went wrong when creating the buffer\n", name);

          *component_count = 0;
          return NULL;
        }
      }

      buffer[i] = ecs->components[i];
    }
  }

  *component_count = count;
  return buffer;
}


uint32_t sfr_ecs_entity_find_index_name(uint32_t offset, const char* name) {
  // Im pre sure that there is a better way of doing this, but idk how to improve this
  uint32_t length = sfr_str_length(name);
  for (uint32_t i = offset; i < ecs->entities_count; i++) {
    if (sfr_str_cmp_length(name, ecs->entities[i]->name, length)) {
      return i;
    }
  }

  return UINT32_MAX;
}

uint32_t sfr_ecs_entity_find_index_uuid(uint32_t offset, SFRuuid_t uuid) {
  // I don't really know how to implement this with uuid other than a linear search, so slow code here too I guess
  for (uint32_t i = offset; i < ecs->entities_count; i++) {
    if (uuid == ecs->entities[i]->uuid) {
      return i;
    }
  }

  return UINT32_MAX;
}

uint32_t sfr_ecs_component_find_index_uuid(uint32_t offset, SFRuuid_t uuid) {
  // I don't really know how to implement this with uuid other than a linear search, so slow code here too I guess
  for (uint32_t i = offset; i < ecs->components_count; i++) {
    if (uuid == ecs->components[i]->uuid) {
      return i;
    }
  }

  return UINT32_MAX;
}

void sfr_ecs_debug_print_entities() {
  if (ecs->entities_count > 0) {
    for (uint32_t i = 0; i < ecs->entities_count; i++) {
      printf(
        "[%d]: n('%s'), t('%s'), c(%d)\n", 
        i, ecs->entities[i]->name, ecs->entities[i]->tag, ecs->entities[i]->components_count
      );
    }
  } else {
    printf("currently no entities in buffer\n");
  }
}

void sfr_ecs_debug_print_components() {
  printf("TODO: print component list here");
}
