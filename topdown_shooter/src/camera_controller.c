#include "../include/topdown_shooter/camera_controller.h"



void                        _tds_camera_controller_late_update(SFRcomponent_t* component, float late_delta_time);




SFRcomponent_t* tds_camera_controller() {
  SFRcomponent_t* component = sfr_ecs_component(
    TDS_CAMERA_CONTROLLER, NULL, _tds_camera_controller_late_update, NULL
  );

  component->data = (TDScamera_controller_t*)malloc(sizeof(TDScamera_controller_t));
  TDScamera_controller_t* controller = SFR_COMPONENT_CONVERT(TDScamera_controller_t, component);

  SFRentity_t* player = sfr_ecs_find_entity_name("Player1");
  SAFIRE_ASSERT(player, "[TDS::CAMERA] failed to find the player entity");

  controller->target = SFR_COMPONENT_CONVERT(SFRtransform_t, player->components[0]);

  return component;
}




void _tds_camera_controller_late_update(SFRcomponent_t* component, float late_delta_time) {
  TDScamera_controller_t* controller = SFR_COMPONENT_CONVERT(TDScamera_controller_t, component);
  SFRtransform_t* transform = SFR_COMPONENT_CONVERT(SFRtransform_t, component->owner->components[0]);

  vec3 target;
  vec3 position;
  glm_vec3_copy(transform->position, position);
  glm_vec3_lerp(position, controller->target->position, 0.002f, position);
  glm_vec3_copy(position, transform->position);

  glm_vec3_copy(
    (vec3) { -(position[X] * 0.5f), -(position[Y] * 0.5f), 0.0f },
    position
  );

  sfr_pipeline_set_transform_matrix(
    position, 
    (vec3) { 0.5f, 0.5f, 1.0f },
    (vec4) { 0.0f, 0.0f, 1.0f, 0.0f }
  );

}