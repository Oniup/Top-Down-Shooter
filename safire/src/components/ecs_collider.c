#include "../include/safire/components.h"

#include "../include/safire/ecs.h"




void                                    _sfr_collider2d_late_update(SFR_Component* component, float late_delta_time);

SFR_Component**                         _sfr_colliders_get(SFR_Component* component, uint32_t* target_count);
SFR_Component*                          _sfr_collider2d_get_correct_trigger(SFR_Component* component);

void                                    _sfr_collider2d_collide(SFR_Component* component, SFR_Component* target);
void                                    _sfr_collider2d_calc_circle(SFR_Component* component, SFR_Component* target, bool same_weight);
void                                    _sfr_collider2d_calc_square(SFR_Component* component, SFR_Component* target, bool same_weight);
void                                    _sfr_collider2d_calc_custom(SFR_Component* component, SFR_Component* target, bool same_weight);

bool                                    _sfr_collider2d_trigger(SFR_Component* component, SFR_Component* target);
bool                                    _sfr_collider2d_calc_trigger_circle(SFR_Component* component, SFR_Component* target);
bool                                    _sfr_collider2d_calc_trigger_square(SFR_Component* component, SFR_Component* target);
bool                                    _sfr_collider2d_calc_trigger_custom(SFR_Component* component, SFR_Component* target);

SFR_Component**                         _sfr_collider2d_trigger_check(SFR_Component* component, uint32_t* target_count);




SFR_Component* sfr_collider2d(SFR_Component* transform) 
{
  SFR_Component* component = sfr_ecs_component(
    SFR_COLLIDER2D, NULL, _sfr_collider2d_late_update, NULL
  );

  component->type = SFR_COMPONENT_TYPE_PHYSICS;

  component->data = (SFR_Collider2D*)malloc(sizeof(SFR_Collider2D));
  SAFIRE_ASSERT(component->data, "[SAFIRE::COMPONENT_COLLIDER_2D]: failed to assign memory SFRcollider2d to custom data for some reason ...");

  SFR_Transform* trans = SFR_COMPONENT_CONVERT(SFR_Transform, transform);

  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
  collider->type = SFR_COLLIDER2D_TYPE_CIRCLE;
  collider->trigger = false;
  glm_vec2_copy(trans->scale, collider->size);
  glm_vec2_copy((vec2){ 0.0f, 0.0f }, collider->offset);
  glm_vec2_scale(collider->size, 0.5f, collider->size);
  collider->weight = 1.0f;

  return component;
}

bool sfr_collider2d_trigger_enter_tag(SFR_Component* component, const char* target_tag, SFR_Component** get) 
{
  component = _sfr_collider2d_get_correct_trigger(component);
  
  uint32_t target_count = 0;
  SFR_Component** targets  = _sfr_collider2d_trigger_check(component, &target_count);

  if (targets != NULL) {
    // checking the triggers
    uint32_t length = sfr_str_length(target_tag);
    for (uint32_t i = 0; i < target_count; i++) 
    {
      if (sfr_str_cmp_length(target_tag, targets[i]->owner->tag, length)) 
      {
        bool result = _sfr_collider2d_trigger(component, targets[i]);
        if (result) 
        {
          if (get != NULL)
            *get = targets[i];
          

          return true;
        }
      }
    }
  }

  return false;
}

bool sfr_collider2d_trigger_enter_name(SFR_Component* component, const char* name, SFR_Component** get) 
{
  component = _sfr_collider2d_get_correct_trigger(component);
  
  uint32_t target_count = 0;
  SFR_Component** targets  = _sfr_collider2d_trigger_check(component, &target_count);

  if (targets != NULL) 
  {
    // checking the triggerss
    uint32_t length = sfr_str_length(name);
    for (uint32_t i = 0; i < target_count; i++) 
    {
      if (sfr_str_cmp_length(name, targets[i]->owner->name, length)) 
      {
        bool result = _sfr_collider2d_trigger(component, targets[i]);
        if (result) 
        {
          if (get != NULL)
            *get = targets[i];

          return true;
        }
      }
    }
  }

  return false;
}

bool sfr_collider2d_trigger_enter_uuid(SFR_Component* component, SFR_Uuid target_uuid, SFR_Component** get) 
{
  component = _sfr_collider2d_get_correct_trigger(component);
  
  uint32_t target_count = 0;
  SFR_Component** targets  = _sfr_collider2d_trigger_check(component, &target_count);

  if (targets != NULL) 
  {
    // checking the triggerss
    for (uint32_t i = 0; i < target_count; i++) 
    {
      if ((unsigned long long)target_uuid == (unsigned long long)targets[i]->owner->uuid) 
      {
        bool result = _sfr_collider2d_trigger(component, targets[i]);
        if (result) 
        {
          if (get != NULL)
            *get = targets[i];

          return true;
        }
      }
    }
  }

  return false;
}

bool sfr_collider2d_trigger_exit_tag(SFR_Component* component, const char* target_tag, SFR_Component** get) 
{
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
  return false;
}

bool sfr_collider2d_trigger_exit_name(SFR_Component* component, const char* name, SFR_Component** get) 
{
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
  return false;
}

bool sfr_collider2d_trigger_exit_uuid(SFR_Component* component, SFR_Uuid target_uuid, SFR_Component** get) 
{
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
  return false;
}




void _sfr_collider2d_late_update(SFR_Component* component, float delta_time) 
{
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);

  // make sure that the collider is not a trigger collider  
  if (!collider->trigger) 
  {
    // get all the colliders that the current collider is colliding with
    uint32_t target_count = 0;
    SFR_Component** target = _sfr_colliders_get(component, &target_count);
    if (target == NULL) 
    {
      return;
    }

    for (uint32_t i = 0; i < target_count; i++) 
    {
      SFR_Collider2D* target_collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, target[i]);
      if (!target_collider->trigger) 
      {
        _sfr_collider2d_collide(component, target[i]);
      }
    }

    free(target);
  }
}

SFR_Component** _sfr_colliders_get(SFR_Component* component, uint32_t* target_count) 
{
  // identifying the physics components range, as that is where all the colliders are, therefore, we only have to search those
  uint32_t component_count = sfr_ecs_get_component_count();
  uint32_t start = sfr_ecs_get_starting_index(SFR_COMPONENT_TYPE_PHYSICS);
  uint32_t stop = sfr_ecs_get_starting_index(SFR_COMPONENT_TYPE_GRAPHICS);

  uint32_t name_length = sfr_str_length(component->name);

  SFR_Component** list = NULL;
  uint32_t count = 0;

  for (uint32_t i = start; i < stop; i++) {
    SFR_Component* target = sfr_ecs_get_target_component(i);
    // skip itself
    if (target->uuid != component->uuid) 
    {
      // checking if it is a collider component
      if (sfr_str_cmp_length(component->name, target->name, name_length)) 
      {
        count++;
        if (list != NULL) 
        {
          list = (SFR_Component**)realloc(list, sizeof(SFR_Component*) * count);
          SAFIRE_ASSERT(list, "[SAFIRE::COLLIDER_IS_COLLIDING] failed to resize the buffer for some reason ...");
        } 
        else 
        {
          list = (SFR_Component**)malloc(sizeof(SFR_Component*) * count);
          SAFIRE_ASSERT(list, "[SAFIRE::COLLIDER_IS_COLLIDING] failed to assign memory to buffer for some reason ...");
        }

        list[count - 1] = target;
      }
    }
  }

  *target_count = count;
  return list;
}

SFR_Component* _sfr_collider2d_get_correct_trigger(SFR_Component* component) 
{
  SAFIRE_ASSERT(component, "[SAFIRE::COLLIDER_GET_CORRECT_TRIGGER] failed to get the correct trigger collider as the component that was passed doesn't have any memory assigned to it");

  uint32_t length = sfr_str_length(SFR_COLLIDER2D);
  if (sfr_str_cmp_length(component->name, SFR_COLLIDER2D, length)) 
  {
    return component;
  }

  for (uint32_t i = 0; i < component->owner->components_count; i++) 
  {
    SFR_Component* target = component->owner->components[i];
    if (sfr_str_cmp_length(SFR_COLLIDER2D, target->name, length)) 
    {
      SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, target);
      if (collider->trigger) 
      {
        return target;
      }
    }
  }

  printf("[SAFIRE::COLLIDER_GET_CORRECT_TRIGGER]: entity doesn't have a collider component on it");
  return NULL;
}

void _sfr_collider2d_collide(SFR_Component* component, SFR_Component* target) 
{
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
  SFR_Collider2D* target_collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, target);

  bool same_weight = false;
  if (collider->weight < target_collider->weight) 
  {
    SFR_Component* temp = component;
    component = target;
    target = temp;

    collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
    target_collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, target);
  } 
  else if (collider->weight == target_collider->weight) 
  {
    same_weight = true;
  }

  switch (collider->type) 
  {
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

void _sfr_collider2d_calc_circle(SFR_Component* component, SFR_Component* target, bool same_weight) 
{
  SFR_Transform* comp_transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);
  SFR_Transform* targ_transform = SFR_COMPONENT_CONVERT(SFR_Transform, target->owner->components[0]);
  SFR_Collider2D* comp_collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
  SFR_Collider2D* targ_collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, target);

  // calculating the distance between the two colliders
  vec3 direction;
  glm_vec3_sub(targ_transform->position, comp_transform->position, direction);
  float length = sqrtf(glm_vec3_dot(direction, direction));
  float max_distance = comp_collider->size[X] + targ_collider->size[X];

  // if they are overlapping, else do nothing
  if (length < comp_collider->size[X] + targ_collider->size[X]) 
  {
    length = max_distance - length;
    glm_vec3_normalize(direction);
    glm_vec3_scale(direction, length, direction);

    if (same_weight) 
    {
      // push both entities back to the edge of the collider
      glm_vec3_scale(direction, 0.5f, direction);
      vec3 comp_direction;
      glm_vec3_scale(direction, -1.0f, comp_direction);
      glm_vec3_add(comp_transform->position, comp_direction, comp_transform->position);
    } 

    glm_vec3_add(targ_transform->position, direction, targ_transform->position);
  }
}

void _sfr_collider2d_calc_square(SFR_Component* component, SFR_Component* target, bool same_weight) 
{
  // TODO: ...
  if (!same_weight) 
  {
    // push the lighter weighted collider

  } 
  else 
  {
    // push both entities back to the edge of the collider
  }
}

void _sfr_collider2d_calc_custom(SFR_Component* component, SFR_Component* target, bool same_weight) 
{
  // TODO: ...
  if (!same_weight) 
  {
    // push the lighter weighted collider

  } 
  else 
  {
    // push both entities back to the edge of the collider
  }
}


bool _sfr_collider2d_trigger(SFR_Component* component, SFR_Component* target) 
{
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, target);

  bool is_colliding = false;
  switch (collider->type) 
  {
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

SFR_Component** _sfr_collider2d_trigger_check(SFR_Component* component, uint32_t* target_count) 
{
  if (component != NULL) 
  {
    SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D,component);
    if (collider->trigger) 
    {
      SFR_Component** targets = _sfr_colliders_get(component, target_count);
      if (targets == NULL) 
      {
        return NULL;
      }

      return targets;
    }
  }

  return NULL;
}

bool _sfr_collider2d_calc_trigger_circle(SFR_Component* component, SFR_Component* target) 
{
  SFR_Transform* comp_transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);
  SFR_Transform* targ_transform = SFR_COMPONENT_CONVERT(SFR_Transform, target->owner->components[0]);
  SFR_Collider2D* comp_collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
  SFR_Collider2D* targ_collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, target);

  // calculating the distance between the two colliders
  vec3 direction;
  glm_vec3_sub(targ_transform->position, comp_transform->position, direction);
  float length = sqrtf(glm_vec3_dot(direction, direction));
  float max_distance = comp_collider->size[X] + targ_collider->size[X];

  if (length < comp_collider->size[X] + targ_collider->size[X]) 
  {
    return true;
  } 
  
  return false;
}

bool _sfr_collider2d_calc_trigger_square(SFR_Component* component, SFR_Component* target) 
{
  // TODO: ...
  return false;
}

bool _sfr_collider2d_calc_trigger_custom(SFR_Component* component, SFR_Component* target) 
{
  // TODO: ...
  return false;
}
