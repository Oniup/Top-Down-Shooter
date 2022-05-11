#include "../include/topdown_shooter/camera_controller.h"



void                        _tds_camera_controller_late_update(SFRcomponent_t* component, float late_delta_time);




SFRcomponent_t* tds_camera_controller() {
  SFRcomponent_t* component = sfr_ecs_component(
    TDS_CAMERA_CONTROLLER, NULL, _tds_camera_controller_late_update, NULL
  );

  component->data = (TDScamera_controller_t*)malloc(sizeof(TDScamera_controller_t));
  TDScamera_controller_t* controller = sfr_component_convert(TDScamera_controller_t, component);

  SFRentity_t* player = sfr_ecs_find_entity_name("Player1");
  SAFIRE_ASSERT(player, "[TDS::CAMERA] failed to find the player entity");

  controller->target = sfr_component_convert(SFRtransform_t, player->components[0]);

  return component;
}




void _tds_camera_controller_late_update(SFRcomponent_t* component, float late_delta_time) {
  TDScamera_controller_t* controller = sfr_component_convert(TDScamera_controller_t, component);

  vec3 position = {
    -(controller->target->position[X]),
    -(controller->target->position[Y]),
    0.0f
  };

  sfr_pipeline_set_transform_matrix(
    position, 
    (vec3) { 1.0f, 1.0f, 1.0f },
    (vec4) { 0.0f, 0.0f, 1.0f, 0.0f }
  );
}