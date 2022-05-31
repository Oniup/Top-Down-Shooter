#include "../include/safire/components.h"

#include "../include/safire/ecs.h"




void                                    _sfr_transform_free(SFR_Component* component);




SFR_Component* sfr_transform() 
{
  SFR_Component* component = sfr_ecs_component(
    SFR_TRANSFORM, NULL, NULL, NULL, _sfr_transform_free
  );

  component->type = SFR_COMPONENT_TYPE_NON_FUNCTIONAL;
  
  component->data = (SFR_Transform*)malloc(sizeof(SFR_Transform));
  SAFIRE_ASSERT(component->data, "[SAFIRE::ECS_COMPONENT_TRANSFORM] failed to assign memory to components data for some reason");

  SFR_Transform* transform = ((SFR_Transform*)component->data);
  glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f }, transform->position);
  glm_vec3_copy((vec3){ 1.0f, 1.0f, 1.0f }, transform->scale);
  glm_vec4_copy((vec4){ 0.0f, 0.0f, 1.0f, 0.0f }, transform->rotation);

  return component;
}

void _sfr_transform_free(SFR_Component* component) 
{
  SFR_Transform* transform = ((SFR_Transform*)component->data);
  free(transform);
  component->data = NULL;
}
