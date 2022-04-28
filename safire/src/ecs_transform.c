#include "../include/safire/components.h"

#include "../include/safire/ecs.h"

void _sfr_transform_free(SFRcomponent_t* component);

SFRcomponent_t* sfr_transform() {
  SFRcomponent_t* component = sfr_ecs_component(
    "SFRtransform", NULL, NULL, _sfr_transform_free
  );

  component->type = SFR_COMPONENT_TYPE_NON_FUNCTIONAL;
  
  component->data = (SFRtransform_t*)malloc(sizeof(SFRtransform_t));
  SAFIRE_ASSERT(component->data, "[SAFIRE::ECS_COMPONENT_TRANSFORM] failed to assign memory to components data for some reason");

  SFRtransform_t* transform = ((SFRtransform_t*)component->data);
  glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f }, transform->position);
  glm_vec3_copy((vec3){ 1.0f, 1.0f, 1.0f }, transform->scale);
  glm_vec3_copy((vec4){ 0.0f, 0.0f, 1.0f, 0.0f }, transform->rotation);

  return component;
}

void _sfr_transform_free(SFRcomponent_t* component) {
  SFRtransform_t* transform = ((SFRtransform_t*)component->data);
  free(transform);
  component->data = NULL;
}

