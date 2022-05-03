#include "../include/safire/ecs.h"

#include "../include/safire/components.h"

typedef struct _SFRecs_names_used       _SFRecs_names_used_t;
typedef struct _SFRcomponent_indices    _SFRcomponent_indices_t;

struct SFRecs {
  uint32_t                  current_scene;
  SFRscene_t**              scenes;
  uint32_t                  scenes_count;

  SFRentity_t**             entities;
  SFRcomponent_t**          components;
  uint32_t                  entities_count;
  uint32_t                  components_count;  

  _SFRcomponent_indices_t*  component_indices;

  uint32_t*                 removing_entities;
  uint32_t                  removing_entities_count;

  _SFRecs_names_used_t*     names_used;
  uint32_t                  names_used_count;
};

struct _SFRecs_names_used {  
  char*                     name;
  uint32_t                  count;
};

// based on the SFRcomponent_type_t enum
struct _SFRcomponent_indices {
  uint32_t                  non_function_start_index;                                     // these can be skipped in run loop
  uint32_t                  functional_start_index;
  uint32_t                  physics_start_index;
  uint32_t                  graphics_start_index;
};


static SFRecs_t* _ecs = NULL;


#define SFR_CURRENT_SCENE _ecs->scenes[_ecs->current_scene]


void                      _sfr_ecs_move_component(uint32_t target, uint32_t dest);
void                      _sfr_ecs_erase_entity(uint32_t index);


void sfr_ecs_init(SFRscene_t** scenes, uint32_t scenes_count) {
  _ecs = (SFRecs_t*)malloc(sizeof(SFRecs_t));
  SAFIRE_ASSERT(_ecs, "[SAFIRE::ECS] failed to create ecs for some reason ...");

  _ecs->current_scene = 0;
  _ecs->scenes = scenes;
  _ecs->scenes_count = scenes_count;

  _ecs->entities = NULL;
  _ecs->components = NULL;
  _ecs->entities_count = 0;
  _ecs->components_count = 0;

  // so that components are placed in the correct location in the buffer
  _ecs->component_indices = (_SFRcomponent_indices_t*)malloc(sizeof(_SFRcomponent_indices_t));
  _ecs->component_indices->non_function_start_index = 0;
  _ecs->component_indices->functional_start_index = 0;
  _ecs->component_indices->physics_start_index = 0;
  _ecs->component_indices->graphics_start_index = 0;

  _ecs->removing_entities = NULL;
  _ecs->removing_entities_count = 0;

  _ecs->names_used = NULL;
  _ecs->names_used_count = 0;

  SFR_CURRENT_SCENE->start(SFR_CURRENT_SCENE);

  for (uint32_t i = 0; i < _ecs->scenes_count; i++) {
    printf("scene name: %s\n", scenes[i]->name);
  }
}

SFRecs_t* sfr_ecs_instance() {
  return _ecs;
}

void sfr_ecs_free() {
  sfr_ecs_clear_stack();

  for (uint32_t i = 0; i < _ecs->scenes_count; i++) {
    if (_ecs->scenes[i]->free != NULL) {
      _ecs->scenes[i]->free(_ecs->scenes[i]);
    }
    sfr_str_free(&_ecs->scenes[i]->name);
    free(_ecs->scenes[i]);
  }
  free(_ecs->scenes);

  free(_ecs->component_indices);
  free(_ecs->names_used);

  free(_ecs);
  _ecs = NULL;
}

void sfr_ecs_clear_stack() {
  if (_ecs->entities != NULL) {
    if (_ecs->entities_count > 0) {
      for (uint32_t i = 0; i < _ecs->entities_count; i++) {
        sfr_ecs_entity_free(_ecs->entities[i]);
      }
    }
    free(_ecs->entities);
    _ecs->entities = NULL;
  }
  _ecs->entities_count = 0;

  _ecs->component_indices->non_function_start_index = 0;
  _ecs->component_indices->functional_start_index = 0;
  _ecs->component_indices->physics_start_index = 0;
  _ecs->component_indices->graphics_start_index = 0;
}

void sfr_ecs_remove_erased_entities() {
  if (_ecs->removing_entities != NULL) {
    // sorting the buffer as it will be easier to remove entities from the stack
    // and not the expensive as most of the time in this buffer, there will only be
    // a couple of indices
    sfr_qsort_uint32(_ecs->removing_entities, _ecs->removing_entities_count);

    // removing the entities from the stack from the largest to smallest
    for (int i = (int)_ecs->removing_entities_count - 1; i > -1; i--) {
      _sfr_ecs_erase_entity(_ecs->removing_entities[i]);
    }

    // resetting the buffer
    free(_ecs->removing_entities);
    _ecs->removing_entities = NULL;
    _ecs->removing_entities_count = 0;
  }
}

void _sfr_ecs_erase_entity(uint32_t index) {
  _ecs->entities_count--;
  uint32_t temp1_size = index;
  uint32_t temp2_size = _ecs->entities_count - index;
  SFRentity_t** temp1 = (SFRentity_t**)malloc(sizeof(SFRentity_t*) * temp1_size);
  SFRentity_t** temp2 = (SFRentity_t**)malloc(sizeof(SFRentity_t*) * temp2_size);
  memcpy(temp1, _ecs->entities, sizeof(SFRentity_t*) * temp1_size);
  memcpy(temp2, _ecs->entities + (index + 1), sizeof(SFRentity_t*) * temp2_size);

  sfr_ecs_entity_free(_ecs->entities[index]);
    
  free(_ecs->entities);
  _ecs->entities = (SFRentity_t**)malloc(sizeof(SFRentity_t*) * _ecs->entities_count);
  memcpy(_ecs->entities, temp1, sizeof(SFRentity_t*) * temp1_size);
  memcpy(_ecs->entities + temp1_size, temp2, sizeof(SFRentity_t*) * _ecs->entities_count);

  free(temp1);
  free(temp2); 
}

void sfr_ecs_update(float delta_time) {
  static uint32_t start_index = 0;
  static uint32_t end_index = 0;

  start_index = _ecs->component_indices->functional_start_index;
  end_index = _ecs->component_indices->graphics_start_index;

  for (uint32_t i = start_index; i < end_index; i++) {
    if (_ecs->components[i]->update != NULL) {
      _ecs->components[i]->update(_ecs->components[i], delta_time);
    }
  }  

  if (SFR_CURRENT_SCENE->update != NULL) {
    SFR_CURRENT_SCENE->update(SFR_CURRENT_SCENE, delta_time);
  }
}

void sfr_ecs_late_update(float late_delta_time) {
  static uint32_t start_index = 0;
  static uint32_t end_index = 0;

  start_index = _ecs->component_indices->functional_start_index;
  end_index = _ecs->component_indices->graphics_start_index;

  for (uint32_t i = start_index; i < end_index; i++) {
    if (_ecs->components[i]->late_update != NULL) {
      _ecs->components[i]->late_update(_ecs->components[i], late_delta_time);
    }
  }  

  if (SFR_CURRENT_SCENE->late_update != NULL) {
    SFR_CURRENT_SCENE->late_update(SFR_CURRENT_SCENE, late_delta_time);
  }
}

void sfr_ecs_render_update() {
  static uint32_t start_index = 0;
  static uint32_t end_index = 0;

  start_index = _ecs->component_indices->graphics_start_index;
  end_index = _ecs->components_count;

  for (uint32_t i = start_index; i < end_index; i++) {
    if (_ecs->components[i]->update != NULL) {
      _ecs->components[i]->update(_ecs->components[i], 0.0f);
    }
  }  
}

void sfr_ecs_erase_entity(uint32_t index) {
  if (index < _ecs->entities_count) {
    _ecs->removing_entities_count++;

    if (_ecs->removing_entities != NULL) {
      _ecs->removing_entities = (uint32_t*)realloc(_ecs->removing_entities, sizeof(uint32_t) * _ecs->removing_entities_count);
      SAFIRE_ASSERT(_ecs->removing_entities, "[SAFIRE::ERASE_ENTITY] failed to resize the buffer");
    } else {
      _ecs->removing_entities = (uint32_t*)malloc(sizeof(uint32_t) * _ecs->removing_entities_count);
      SAFIRE_ASSERT(_ecs->removing_entities, "[SAFIRE::ERASE_ENTITY] failed to assgin memory to buffer");
    }

    _ecs->removing_entities[_ecs->removing_entities_count - 1] = index;
  } else {
    printf("[SAFIRE_WARNING::ERASE_ENTITY] cannot erase entity when the index is larger than the entity buffer");
  }
}

void sfr_ecs_erase_component(uint32_t index) {
  if (index < _ecs->components_count) {
    sfr_ecs_component_free(_ecs->components[index]);

    switch (_ecs->components[index]->type) {
      case SFR_COMPONENT_TYPE_NON_FUNCTIONAL:
        _ecs->component_indices->functional_start_index--;
        _ecs->component_indices->physics_start_index--;
        _ecs->component_indices->graphics_start_index--;
        break;

      case SFR_COMPONENT_TYPE_FUNCTIONAL:
      _ecs->component_indices->physics_start_index--;
      _ecs->component_indices->graphics_start_index--;
        break;

      case SFR_COMPONENT_TYPE_PHYSICS:
        _ecs->component_indices->graphics_start_index--;
        break;
      
      case SFR_COMPONENT_TYPE_GRAPHICS:
        break;

    }

    if (index - 1 == _ecs->components_count) {
      // erasing the last component in the stack
      uint32_t old = _ecs->components_count;
      _ecs->components_count--;
      
      SFRcomponent_t** temp = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * _ecs->components_count);
      memcpy(temp, _ecs->components, _ecs->components_count);
      free(_ecs->components);

      _ecs->components = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * _ecs->components_count);
      memcpy(_ecs->components, temp, _ecs->components_count);
      free(temp);
    } else {
      // erasing component anywhere in the stack besides the last one
      _ecs->components_count--;
      uint32_t temp1_size = index;
      uint32_t temp2_size = _ecs->components_count - index;

      SFRcomponent_t** temp1 = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * temp1_size);
      SFRcomponent_t** temp2 = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * temp2_size);

      memcpy(temp1, _ecs->components, sizeof(SFRcomponent_t*) * temp1_size);
      memcpy(temp2, _ecs->components + (index + 1), sizeof(SFRcomponent_t*) * temp2_size);

      free(_ecs->components);
      _ecs->components = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * _ecs->components_count);

      memcpy(_ecs->components, temp1, sizeof(SFRcomponent_t*) * temp1_size);
      memcpy(_ecs->components + temp1_size, temp2, sizeof(SFRcomponent_t*) * temp2_size);

      free(temp1);
      free(temp2);
    }
  } else {
    printf("[SAFIRE_WARNING::ERASE_COMPONENT] cannot erase componenent when the index is larger than the component buffer");
  }
}

uint32_t sfr_ecs_get_entities_count() {
  return _ecs->entities_count;
}

uint32_t sfr_ecs_get_component_count() {
  return _ecs->components_count;
}

void sfr_ecs_load_scene(uint32_t id) {
  sfr_ecs_clear_stack();

  if (SFR_CURRENT_SCENE->free != NULL) {
    SFR_CURRENT_SCENE->free(SFR_CURRENT_SCENE);
  }

  _ecs->current_scene = id;
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
      uint32_t c = _ecs->entities_count;
      _ecs->entities_count++;
      if (_ecs->entities != NULL) {
        SFRentity_t** temp = (SFRentity_t**)realloc(_ecs->entities, sizeof(SFRentity_t*) * _ecs->entities_count);
        SAFIRE_ASSERT(temp, "[SAFIRE::ECS_ENTITY_PUSH] failed to resize entity buffer for some reason");
        _ecs->entities = temp;
      } else {
        _ecs->entities = (SFRentity_t**)malloc(sizeof(SFRentity_t*) * _ecs->entities_count);
      }

      // setting the entity data 
      _ecs->entities[c] = (SFRentity_t*)malloc(sizeof(SFRentity_t));
      SAFIRE_ASSERT(_ecs->entities[c], "[SAFIRE::ECS_ENTITY_PUSH] failed to assign memory to entity for some reason");

      _ecs->entities[c]->name = sfr_str(name);
      _ecs->entities[c]->uuid = sfr_rand_uint64();
      _ecs->entities[c]->tag = tag != NULL ? sfr_str(tag) : NULL;
      _ecs->entities[c]->layer = 0;
      _ecs->entities[c]->components = NULL;
      _ecs->entities[c]->components_count = 0;
      
      sfr_attach_default_comps(_ecs->entities[c]);
      

      return _ecs->entities[c];

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

      _ecs->components_count++;
      if (_ecs->components != NULL) {
        _ecs->components = (SFRcomponent_t**)realloc(_ecs->components, sizeof(SFRcomponent_t*) * _ecs->components_count);
        SAFIRE_ASSERT(_ecs->components, "[SAFIRE::ECS_COMPONENT_PUSH] failed to resize component buffer for some reason");
      } else {
        _ecs->components = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * _ecs->components_count);
      }

      // finding where it will be stored and then incert into that location in the component buffer
      uint32_t t_index = 0;
      switch (component->type) {
      case SFR_COMPONENT_TYPE_NON_FUNCTIONAL:
        t_index = _ecs->component_indices->non_function_start_index;
        _ecs->component_indices->functional_start_index++;
        _ecs->component_indices->physics_start_index++;
        _ecs->component_indices->graphics_start_index++;
        break;

      case SFR_COMPONENT_TYPE_FUNCTIONAL:
        t_index = _ecs->component_indices->functional_start_index;
        _ecs->component_indices->physics_start_index++;
        _ecs->component_indices->graphics_start_index++;
        break;

      case SFR_COMPONENT_TYPE_PHYSICS:
        t_index = _ecs->component_indices->physics_start_index;
        _ecs->component_indices->graphics_start_index++;
        break;

      case SFR_COMPONENT_TYPE_GRAPHICS:
        // incerting component into correct location based on the entity layer
        t_index = _ecs->component_indices->graphics_start_index;
        uint32_t old_size = _ecs->components_count - 1;
        for (uint32_t i = _ecs->component_indices->graphics_start_index; i < old_size; i++) {
          if (entity->layer >= _ecs->components[i]->owner->layer) {
            t_index++;
            break;
          }
        }
        break;
      }

      // incert component into correct location
      if (t_index < _ecs->components_count - 1) {
        uint32_t temp_size = _ecs->components_count - t_index - 1;
        SFRcomponent_t** temp = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * temp_size);
        memcpy(temp, _ecs->components + t_index, sizeof(SFRcomponent_t*) * temp_size);

        _ecs->components[t_index] = component;
        int k = 0;
        for (uint32_t i = t_index + 1; i < _ecs->components_count; i++) {
          _ecs->components[i] = temp[k];
          k++;
        }
      } else {
        _ecs->components[t_index] = component;
        SAFIRE_ASSERT(
          _ecs->components[t_index], 
          "[SAFIRE::_ecs_COMPONENT_PUSH] failed to assign memory to component for some reason"
        );
      }
      
      // setting the entity data 

      _ecs->components[t_index]->uuid = sfr_rand_uint64();
      _ecs->components[t_index]->owner = entity;

      // add the component to the buffer of the entity
      entity->components_count++;
      if (entity->components_count > 0) {
        entity->components = (SFRcomponent_t**)realloc(entity->components, sizeof(SFRcomponent_t*) * entity->components_count);
        SAFIRE_ASSERT(entity->components, "[SAFIRE::ECS_COMPONENT_PUSH] failed to resize entity component reference buffer");
      } else {
        entity->components = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * entity->components_count);
        SAFIRE_ASSERT(entity->components, "[SAFIRE::ECS_COMPONENT_PUSH] failed to assgin memory to the entity component reference");
      }
      entity->components[entity->components_count - 1] = _ecs->components[t_index];

      return _ecs->components[t_index];

#ifndef NDEBUG
    }
  }
  return NULL;
#endif
}

SFRcomponent_t* sfr_ecs_component(const char* name, component_update update, component_late_update late_update, component_free free) {
  /*
  the following line of code caused a memory curroption error that took like 2 hours to figure out
  curroption code: SFRcomponent_t* component = (SFRcomponent_t*)malloc(sizeof(SFRcomponent_t*)); 
  */
  SFRcomponent_t* component = (SFRcomponent_t*)malloc(sizeof(SFRcomponent_t)); 
  SAFIRE_ASSERT(component, "[SAFIRE::ECS_COMPONENT] failed to assgin memory to component for some reason");

  component->name = sfr_str(name);
  component->update = update;
  component->late_update = late_update;
  component->free = free;
  component->type = SFR_COMPONENT_TYPE_FUNCTIONAL;

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
        target = sfr_ecs_component_find_index_uuid(0, entity->components[i]->uuid);
        printf("%d\n", target);
        SAFIRE_ASSERT(target != UINT32_MAX, "[SAFIRE::ECS_ENTITY_FREE] failed to find target");
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
    } else if(component->data != NULL) {
      free(component->data);
    }

    free(component);
  }
}

void sfr_ecs_entity_set_layer(SFRentity_t* entity, uint32_t layer) {
  entity->layer = layer;
  for (uint32_t i = 0; i < entity->components_count; i++) {
    if (sfr_str_cmp(entity->components[i]->name, SFR_SPRITE_RENDERER)) {

    }
  }
}

SFRentity_t* sfr_ecs_find_entity_name(const char* name) {
  // Im pre sure that there is a better way of doing this, but idk how to improve this
  uint32_t length = sfr_str_length(name);
  for (uint32_t i = 0; i < _ecs->entities_count; i++) {
    if (sfr_str_cmp_length(name, _ecs->entities[i]->name, length)) {
      return _ecs->entities[i];
    }
  }

  return NULL;
}

SFRentity_t** sfr_ecs_find_list_entities(const char* tag, uint32_t* entity_count) {
  SFRentity_t** buffer = NULL;
  uint32_t count = 0;
  uint32_t length = sfr_str_length(tag);

  for (uint32_t i = 0; i < _ecs->entities_count; i++) {
    if (sfr_str_cmp_length(tag, _ecs->entities[i]->tag, length)) {
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

      buffer[i] = _ecs->entities[i];
    }
  }

  *entity_count = count;
  return buffer;
}

SFRentity_t* sfr_ecs_find_entity_uuid(uint64_t uuid) {
  // I don't really know how to implement this with uuid other than a linear search, so slow code here too I guess
  for (uint32_t i = 0; i < _ecs->entities_count; i++) {
    if (uuid == _ecs->entities[i]->uuid) {
      return _ecs->entities[i];
    }
  }

  return NULL;
}

SFRcomponent_t* sfr_ecs_find_component_uuid(SFRuuid_t uuid) {
  // I don't really know how to implement this with uuid other than a linear search, so slow code here too I guess
  for (uint32_t i = 0; i < _ecs->components_count; i++) {
    if (uuid == _ecs->components[i]->uuid) {
      return _ecs->components[i];
    }
  }

  return NULL;
}

SFRcomponent_t** sfr_ecs_find_list_components(const char* name, uint32_t* component_count) {
  SFRcomponent_t** buffer = NULL;
  uint32_t count = 0;
  uint32_t length = sfr_str_length(name);

  for (uint32_t i = 0; i < _ecs->components_count; i++) {
    if (sfr_str_cmp_length(name, _ecs->components[i]->name, length)) {
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

      buffer[i] = _ecs->components[i];
    }
  }

  *component_count = count;
  return buffer;
}


uint32_t sfr_ecs_entity_find_index_name(uint32_t offset, const char* name) {
  // Im pre sure that there is a better way of doing this, but idk how to improve this
  uint32_t length = sfr_str_length(name);
  for (uint32_t i = offset; i < _ecs->entities_count; i++) {
    if (sfr_str_cmp_length(name, _ecs->entities[i]->name, length)) {
      return i;
    }
  }

  return UINT32_MAX;
}

uint32_t sfr_ecs_entity_find_index_uuid(uint32_t offset, SFRuuid_t uuid) {
  // I don't really know how to implement this with uuid other than a linear search, so slow code here too I guess
  for (uint32_t i = offset; i < _ecs->entities_count; i++) {
    if (uuid == _ecs->entities[i]->uuid) {
      return i;
    }
  }

  return UINT32_MAX;
}

uint32_t sfr_ecs_component_find_index_uuid(uint32_t offset, SFRuuid_t uuid) {
  // I don't really know how to implement this with uuid other than a linear search, so slow code here too I guess
  for (uint32_t i = offset; i < _ecs->components_count; i++) {
    if ((unsigned long long)uuid == (unsigned long long)_ecs->components[i]->uuid) {
      return i;
    }
  }

  return UINT32_MAX;
}

void sfr_ecs_debug_print_entities() {
  if (_ecs->entities_count > 0) {
    printf("entity list (%d):\n", _ecs->entities_count);
    for (uint32_t i = 0; i < _ecs->entities_count; i++) {
      printf(
        "[%d]: n('%s'), t('%s'), c(%d)\n", 
        i, _ecs->entities[i]->name, _ecs->entities[i]->tag, _ecs->entities[i]->components_count
      );
    }
  } else {
    printf("currently no entities in the buffer\n");
  }
}

void sfr_ecs_debug_print_components() {
  if (_ecs->components_count > 0) {
    printf("component list (%d):\n", _ecs->components_count);
    for (uint32_t i = 0; i < _ecs->components_count; i++) {
      printf(
        "[%d]: n('%s'), e('%s'), d(%d), u(%d), l(%d), f(%d), t(", 
        i, _ecs->components[i]->name, _ecs->components[i]->owner->name, _ecs->components[i]->data != NULL,
        _ecs->components[i]->update != NULL, _ecs->components[i]->late_update != NULL,
        _ecs->components[i]->free != NULL
      );
      
      switch (_ecs->components[i]->type) {
      case SFR_COMPONENT_TYPE_NON_FUNCTIONAL:
        printf("non functional");
        break;
      case SFR_COMPONENT_TYPE_FUNCTIONAL:
        printf("functional");
        break;
      case SFR_COMPONENT_TYPE_PHYSICS:
        printf("physics");
        break;
      case SFR_COMPONENT_TYPE_GRAPHICS:
        printf("graphics");
        break;
      }
      printf(")\n");
    }

    printf(
      "\nstart indices:\n* non functional: %d\n* functional: %d\n* physics: %d\n* graphics: %d\n\n",
      _ecs->component_indices->non_function_start_index,
      _ecs->component_indices->functional_start_index,
      _ecs->component_indices->physics_start_index,
      _ecs->component_indices->graphics_start_index
    );
  } else {
    printf("currently no components in the buffer\n");
  }
}

void _sfr_ecs_move_component(uint32_t target, uint32_t dest) {
  // TODO: move the target entity to the correct location and a move everything down/up in both directions
  // for this project it is out of scope
}
