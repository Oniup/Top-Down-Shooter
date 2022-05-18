#include "../include/topdown_shooter/camera_controller.h"



void                                    _tds_camera_controller_late_update(SFR_Component* component, float late_delta_time);




SFR_Component* tds_camera_controller() 
{
  SFR_Component* component = sfr_ecs_component(
    TDS_CAMERA_CONTROLLER, NULL, _tds_camera_controller_late_update, NULL
  );

  component->data = (TDS_CameraController*)malloc(sizeof(TDS_CameraController));
  TDS_CameraController* controller = SFR_COMPONENT_CONVERT(TDS_CameraController, component);

  SFR_Entity* player = sfr_ecs_find_entity_name("Player");
  SAFIRE_ASSERT(player, "[TDS::CAMERA] failed to find the player entity");

  controller->target = SFR_COMPONENT_CONVERT(SFR_Transform, player->components[0]);

  return component;
}




void _tds_camera_controller_late_update(SFR_Component* component, float late_delta_time) 
{
  TDS_CameraController* controller = SFR_COMPONENT_CONVERT(TDS_CameraController, component);
  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);

  vec3 target;
  vec3 position;
  glm_vec3_copy(transform->position, position);
  glm_vec3_lerp(position, controller->target->position, 0.02f, position);
  glm_vec3_copy(position, transform->position);

  sfr_pipeline_set_culling_centre(transform->position);

  sfr_pipeline_set_view_matrix(position);
}