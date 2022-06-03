#include "../include/safire/components.h"

#include "../include/safire/ecs.h"

#include "../include/safire/safire.h"




void                                    _sfr_collider2d_fixed_update(SFR_Component* component, float fixed_delta_time);
void                                    _sfr_collider2d_free(SFR_Component* component);

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




// references to the collider
static SFR_Component**                  _collider_comps = NULL;
static uint32_t                         _collider_comps_count = 0;




SFR_Component* sfr_collider2d(SFR_Component* transform) 
{
  SFR_Component* component = sfr_ecs_component(
    SFR_COLLIDER2D, NULL, NULL, _sfr_collider2d_fixed_update, _sfr_collider2d_free
  );

  component->type = SFR_COMPONENT_TYPE_PHYSICS;

  component->data = (SFR_Collider2D*)malloc(sizeof(SFR_Collider2D));
  SAFIRE_ASSERT(component->data, "[SAFIRE::COMPONENT_COLLIDER_2D]: failed to assign memory SFRcollider2d to custom data for some reason ...");

  SFR_Transform* trans = SFR_COMPONENT_CONVERT(SFR_Transform, transform);

  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);
  collider->type = SFR_COLLIDER2D_TYPE_CIRCLE;
  collider->trigger = false;
  glm_vec2_copy((vec2){ 1.0f, 1.0f }, collider->size);
  glm_vec2_copy((vec2){ 0.0f, 0.0f }, collider->offset);
  collider->weight = 1.0f;
  collider->id = _collider_comps_count;

  // private reference
  _collider_comps_count++;
  if (_collider_comps != NULL)
  {
    _collider_comps = (SFR_Component**)realloc(_collider_comps, sizeof(SFR_Component*) * _collider_comps_count);
    SAFIRE_ASSERT(_collider_comps, "[SAFIRE::COMPONENT_COLLIDER_2D]: failed to resize private collider buffer");
  }
  else
  {
    _collider_comps = (SFR_Component**)malloc(sizeof(SFR_Component*) * _collider_comps_count);
    SAFIRE_ASSERT(_collider_comps, "[SAFIRE::COMPONENT_COLLIDER_2D]: failed to assign memory to private collider buffer");
  }
  _collider_comps[collider->id] = component;

  return component;
}

bool sfr_collider2d_trigger_enter_tag(SFR_Component* component, const char* target_tag, SFR_Component** get) 
{
  component = _sfr_collider2d_get_correct_trigger(component);
  
  uint32_t target_count = 0;
  SFR_Component** targets  = _sfr_collider2d_trigger_check(component, &target_count);

  if (targets != NULL) 
  {
    // checking the triggers
    uint32_t length = sfr_str_length(target_tag);
    for (uint32_t i = 0; i < target_count; i++) 
    {
      if (targets[i]->active)
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
    // checking the triggers
    uint32_t length = sfr_str_length(name);
    for (uint32_t i = 0; i < target_count; i++) 
    {
      if (targets[i]->active)
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
      if (targets[i]->active)
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




void _sfr_collider2d_fixed_update(SFR_Component* component, float fixed_delta_time) 
{
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);

  // make sure that the collider is not a trigger collider  
  if (!collider->trigger && component->active) 
  {
    for (uint32_t i = 0; i < _collider_comps_count; i++) 
    {
      if (i != collider->id)
      {
        SFR_Collider2D* target_collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, _collider_comps[i]);
        if (!target_collider->trigger && _collider_comps[i]->active) 
        {
          _sfr_collider2d_collide(component, _collider_comps[i]);
        }
      }
    }
  }
}

void _sfr_collider2d_free(SFR_Component* component)
{
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, component);

  _collider_comps_count--;
  // erase component from private stack
  uint32_t temp1_size = collider->id;
  uint32_t temp2_size = _collider_comps_count - collider->id;

  SFR_Component** temp1 = (SFR_Component**)malloc(sizeof(SFR_Component*) * temp1_size);
  SFR_Component** temp2 = (SFR_Component**)malloc(sizeof(SFR_Component*) * temp2_size);

  memcpy(temp1, _collider_comps, sizeof(SFR_Component*) * temp1_size);
  memcpy(temp2, _collider_comps + (temp1_size + 1), sizeof(SFR_Component*) * temp2_size);

  for (uint32_t i = 0; i < temp2_size; i++)
  {
    SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, temp2[i]);
    collider->id--;
  }

  free(_collider_comps);
  _collider_comps = (SFR_Component**)malloc(sizeof(SFR_Component*) * _collider_comps_count);
  memcpy(_collider_comps, temp1, sizeof(SFR_Component*) * temp1_size);
  memcpy(_collider_comps + temp1_size, temp2, sizeof(SFR_Component*) * temp2_size);
}

// TODO: improve performance   
SFR_Component** _sfr_colliders_get(SFR_Component* component, uint32_t* target_count) 
{
  // identifying the physics components range, as that is where all the colliders are, therefore, we only have to search those
  uint32_t component_count = sfr_ecs_get_component_count();
  uint32_t start = sfr_ecs_get_starting_index(SFR_COMPONENT_TYPE_PHYSICS);
  uint32_t stop = sfr_ecs_get_starting_index(SFR_COMPONENT_TYPE_GRAPHICS);

  SFR_Component** list = NULL;
  uint32_t count = 0;

  for (uint32_t i = start; i < stop; i++) 
  {
    SFR_Component* target = sfr_ecs_get_target_component(i);
    // skip itself
    if (target->uuid != component->uuid) 
    {
      // checking if it is a collider component
      if (sfr_str_cmp(component->name, target->name)) 
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
  float length = glm_vec3_dot(direction, direction);

  float comp_radius = fabsf(comp_transform->scale[X]) * comp_collider->size[X];
  float targ_radius = fabsf(targ_transform->scale[X]) * targ_collider->size[X];  
  float max_distance = comp_radius + targ_radius;

  // if they are overlapping, else do nothing
  if (length < max_distance * max_distance) 
  {
    length = max_distance - sqrtf(length);
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

  float comp_radius = fabsf(comp_transform->scale[X]) * comp_collider->size[X];
  float targ_radius = fabsf(targ_transform->scale[X]) * targ_collider->size[X];
  float max_distance = comp_radius + targ_radius;

  if (length < max_distance) 
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
