#include "../include/safire/components.h"

#include "../include/safire/ecs.h"




void                        _sfr_collider2d_late_update(SFRcomponent_t* component, float late_delta_time);

SFRcomponent_t**            _sfr_colliders_get(SFRcomponent_t* component, uint32_t* target_count);
SFRcomponent_t*             _sfr_collider2d_get_correct_trigger(SFRcomponent_t* component);

void                        _sfr_collider2d_collide(SFRcomponent_t* component, SFRcomponent_t* target);
void                        _sfr_collider2d_calc_circle(SFRcomponent_t* component, SFRcomponent_t* target, bool same_weight);
void                        _sfr_collider2d_calc_square(SFRcomponent_t* component, SFRcomponent_t* target, bool same_weight);
void                        _sfr_collider2d_calc_custom(SFRcomponent_t* component, SFRcomponent_t* target, bool same_weight);

bool                        _sfr_collider2d_trigger(SFRcomponent_t* component, SFRcomponent_t* target);
bool                        _sfr_collider2d_calc_trigger_circle(SFRcomponent_t* component, SFRcomponent_t* target);
bool                        _sfr_collider2d_calc_trigger_square(SFRcomponent_t* component, SFRcomponent_t* target);
bool                        _sfr_collider2d_calc_trigger_custom(SFRcomponent_t* component, SFRcomponent_t* target);




SFRcomponent_t* sfr_collider2d() {
  SFRcomponent_t* component = sfr_ecs_component(
    SFR_COLLIDER2D, NULL, _sfr_collider2d_late_update, NULL
  );

  component->type = SFR_COMPONENT_TYPE_PHYSICS;

  component->data = (SFRcollider2d_t*)malloc(sizeof(SFRcollider2d_t));
  SAFIRE_ASSERT(component->data, "[SAFIRE::COMPONENT_COLLIDER_2D]: failed to assign memory SFRcollider2d to custom data for some reason ...");

  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);
  collider->type = SFR_COLLIDER2D_TYPE_CIRCLE;
  collider->trigger = false;
  glm_vec2_copy((vec2){ 1.0f, 1.0f }, collider->size);
  glm_vec2_copy((vec2){ 0.0f, 0.0f }, collider->offset);
  collider->weight = 1.0f;

  return component;
}

bool sfr_collider2d_trigger_enter_tag(SFRcomponent_t* component, const char* target_tag) {
  component = _sfr_collider2d_get_correct_trigger(component);

  if (component != NULL) {
    SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);

    if (collider->trigger) {
      uint32_t target_count = 0;
      SFRcomponent_t** targets = _sfr_colliders_get(component, &target_count);

      if (targets == NULL) {
        return NULL;
      }

      // checking the triggerss
      uint32_t length = sfr_str_length(target_tag);
      for (uint32_t i = 0; i < target_count; i++) {
        if (sfr_str_cmp_length(target_tag, targets[i]->owner->tag, length)) {
          bool result = _sfr_collider2d_trigger(component, targets[i]);
          if (result) {
            return true;
          }
        }
      }
    }
  }

  return false;
}

bool sfr_collider2d_trigger_enter_name(SFRcomponent_t* component, const char* name) {
  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);
  return false;
}

bool sfr_collider2d_trigger_enter_uuid(SFRcomponent_t* component, SFRuuid_t target_uuid) {
  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);
  return false;
}

bool sfr_collider2d_trigger_exit_tag(SFRcomponent_t* component, const char* target_tag) {
  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);
  return false;
}

bool sfr_collider2d_trigger_exit_name(SFRcomponent_t* component, const char* name) {
  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);
  return false;
}

bool sfr_collider2d_trigger_exit_uuid(SFRcomponent_t* component, SFRuuid_t target_uuid) {
  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);
  return false;
}




void _sfr_collider2d_late_update(SFRcomponent_t* component, float delta_time) {
  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);

  // make sure that the collider is not a trigger collider  
  if (!collider->trigger) {
    // get all the colliders that the current collider is colliding with
    uint32_t target_count = 0;
    SFRcomponent_t** target = _sfr_colliders_get(component, &target_count);
    if (target == NULL) {
      return;
    }

    for (uint32_t i = 0; i < target_count; i++) {
      SFRcollider2d_t* target_collider = sfr_component_convert(SFRcollider2d_t, target[i]);
      if (!target_collider->trigger) {
        _sfr_collider2d_collide(component, target[i]);
      }
    }

    free(target);
  }
}

SFRcomponent_t** _sfr_colliders_get(SFRcomponent_t* component, uint32_t* target_count) {
  // identifying the physics components range, as that is where all the colliders are, therefore, we only have to search those
  uint32_t component_count = sfr_ecs_get_component_count();
  uint32_t start = sfr_ecs_get_starting_index(SFR_COMPONENT_TYPE_PHYSICS);
  uint32_t stop = sfr_ecs_get_starting_index(SFR_COMPONENT_TYPE_GRAPHICS);

  uint32_t name_length = sfr_str_length(component->name);

  SFRcomponent_t** list = NULL;
  uint32_t count = 0;

  for (uint32_t i = start; i < stop; i++) {
    SFRcomponent_t* target = sfr_ecs_get_target_component(i);
    // skip itself
    if (target->uuid != component->uuid) {
      // checking if it is a collider component
      if (sfr_str_cmp_length(component->name, target->name, name_length)) {
        count++;
        if (list != NULL) {
          list = (SFRcomponent_t**)realloc(list, sizeof(SFRcomponent_t*) * count);
          SAFIRE_ASSERT(list, "[SAFIRE::COLLIDER_IS_COLLIDING] failed to resize the buffer for some reason ...");
        } else {
          list = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * count);
          SAFIRE_ASSERT(list, "[SAFIRE::COLLIDER_IS_COLLIDING] failed to assign memory to buffer for some reason ...");
        }
        list[count - 1] = target;
      }
    }
  }

  *target_count = count;
  return list;
}

SFRcomponent_t* _sfr_collider2d_get_correct_trigger(SFRcomponent_t* component) {
  SAFIRE_ASSERT(component, "[SAFIRE::COLLIDER_GET_CORRECT_TRIGGER] failed to get the correct trigger collider as the component that was passed doesn't have any memory assigned to it");

  uint32_t length = sfr_str_length(SFR_COLLIDER2D);
  if (sfr_str_cmp_length(component->name, SFR_COLLIDER2D, length)) {
    return component;
  }

  for (uint32_t i = 0; i < component->owner->components_count; i++) {
    SFRcomponent_t* target = component->owner->components[i];

    if (sfr_str_cmp_length(SFR_COLLIDER2D, target->name, length)) {
      SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, target);

      if (collider->trigger) {
        return target;
      }
    }
  }

  printf("[SAFIRE::COLLIDER_GET_CORRECT_TRIGGER]: entity doesn't have a collider component on it");
  return NULL;
}

void _sfr_collider2d_collide(SFRcomponent_t* component, SFRcomponent_t* target) {
  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, component);
  SFRcollider2d_t* target_collider = sfr_component_convert(SFRcollider2d_t, target);

  bool same_weight = false;
  if (collider->weight < target_collider->weight) {
    SFRcomponent_t* temp = component;
    component = target;
    target = temp;

    collider = sfr_component_convert(SFRcollider2d_t, component);
    target_collider = sfr_component_convert(SFRcollider2d_t, target);
  } else if (collider->weight == target_collider->weight) {
    same_weight = true;
  }

  switch (collider->type) {
  case SFR_COLLIDER2D_TYPE_CIRCLE:
    _sfr_collider2d_calc_circle(component, target, same_weight);
    break;

  case SFR_COLLIDER2D_TYPE_SQUARE:
    _sfr_collider2d_calc_square(component, target, same_weight);
    break;
    
  case SFR_COLLIDER2D_TYPE_CUSTOM:
    _sfr_collider2d_calc_custom(component, target, same_weight);
    break;
  }
}

void _sfr_collider2d_calc_circle(SFRcomponent_t* component, SFRcomponent_t* target, bool same_weight) {
  SFRtransform_t* comp_transform = sfr_component_convert(SFRtransform_t, component->owner->components[0]);
  SFRtransform_t* targ_transform = sfr_component_convert(SFRtransform_t, target->owner->components[0]);
  SFRcollider2d_t* comp_collider = sfr_component_convert(SFRcollider2d_t, component);
  SFRcollider2d_t* targ_collider = sfr_component_convert(SFRcollider2d_t, target);

  // calculating the distance between the two colliders
  vec3 direction;
  glm_vec3_sub(targ_transform->position, comp_transform->position, direction);
  float length = sqrtf(glm_vec3_dot(direction, direction));
  float max_distance = comp_collider->size[X] + targ_collider->size[X];

  // if they are overlapping, else do nothing
  if (length < comp_collider->size[X] + targ_collider->size[X]) {
    length = max_distance - length;
    glm_vec3_normalize(direction);
    glm_vec3_scale(direction, length, direction);

    if (same_weight) {
      // push both entities back to the edge of the collider
      glm_vec3_scale(direction, 0.5f, direction);
      vec3 comp_direction;
      glm_vec3_scale(direction, -1.0f, comp_direction);
      glm_vec3_add(comp_transform->position, comp_direction, comp_transform->position);
    } 

    glm_vec3_add(targ_transform->position, direction, targ_transform->position);
  }
}

void _sfr_collider2d_calc_square(SFRcomponent_t* component, SFRcomponent_t* target, bool same_weight) {
  // TODO: ...
  if (!same_weight) {
    // push the lighter weighted collider

  } else {
    // push both entities back to the edge of the collider
  }
}

void _sfr_collider2d_calc_custom(SFRcomponent_t* component, SFRcomponent_t* target, bool same_weight) {
  // TODO: ...
  if (!same_weight) {
    // push the lighter weighted collider

  } else {
    // push both entities back to the edge of the collider
  }
}


bool _sfr_collider2d_trigger(SFRcomponent_t* component, SFRcomponent_t* target) {
  SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, target);

  bool is_colliding = false;
  switch (collider->type) {
  case SFR_COLLIDER2D_TYPE_CIRCLE:
    return _sfr_collider2d_calc_trigger_circle(component, target);  
    break;

  case SFR_COLLIDER2D_TYPE_SQUARE:
    return _sfr_collider2d_calc_trigger_square(component, target);
    break;
    
  case SFR_COLLIDER2D_TYPE_CUSTOM:
    return _sfr_collider2d_calc_trigger_custom(component, target);  
    break;
  }

  return false;
}

bool _sfr_collider2d_calc_trigger_circle(SFRcomponent_t* component, SFRcomponent_t* target) {
  SFRtransform_t* comp_transform = sfr_component_convert(SFRtransform_t, component->owner->components[0]);
  SFRtransform_t* targ_transform = sfr_component_convert(SFRtransform_t, target->owner->components[0]);
  SFRcollider2d_t* comp_collider = sfr_component_convert(SFRcollider2d_t, component);
  SFRcollider2d_t* targ_collider = sfr_component_convert(SFRcollider2d_t, target);

  // calculating the distance between the two colliders
  vec3 direction;
  glm_vec3_sub(targ_transform->position, comp_transform->position, direction);
  float length = sqrtf(glm_vec3_dot(direction, direction));
  float max_distance = comp_collider->size[X] + targ_collider->size[X];

  if (length < comp_collider->size[X] + targ_collider->size[X]) {
    return true;
  } 
  
  return false;
}

bool _sfr_collider2d_calc_trigger_square(SFRcomponent_t* component, SFRcomponent_t* target) {
  // TODO: ...
  return false;
}

bool _sfr_collider2d_calc_trigger_custom(SFRcomponent_t* component, SFRcomponent_t* target) {
  // TODO: ...
  return false;
}
