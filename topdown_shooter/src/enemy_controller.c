#include "../include/topdown_shooter/enemy_controller.h"

void _tds_enemy_controller_update(SFRcomponent_t* component, float delta_time);

SFRcomponent_t* tds_enemy_controller() {
  SFRcomponent_t* component = sfr_ecs_component(
    TDS_ENEMY_CONTROLLER, _tds_enemy_controller_update, NULL, NULL
  );

  component->data = (TDSenemy_controller_t*)malloc(sizeof(TDSenemy_controller_t));
  TDSenemy_controller_t* controller = sfr_component_convert(TDSenemy_controller_t, component);

  controller->health = 1;
  controller->damage = 1;
  controller->move_speed = 1.0f;

  return component;
}

void _tds_enemy_controller_update(SFRcomponent_t* component, float delta_time) {
  TDSenemy_controller_t* controller = sfr_component_convert(TDSenemy_controller_t, component);
  SFRtransform_t* transform = sfr_component_convert(SFRtransform_t, component->owner->components[0]);

  // transform->rotation[W] -= 2.0f * delta_time;
  // static SFRtimer_t timer = 0.0f;
  // static bool decrease = false;
  // if (sfr_timer_finished(&timer)) {
  //   if (decrease) {
  //     decrease = false;
  //   } else {
  //     decrease = true;
  //   }
  //   timer = sfr_timer_start(1.0f);
  // }

  // if (decrease) {
  //   // glm_vec3_scale(transform->scale, 0.9999f, transform->scale);
  //   transform->scale[Y] *= 0.9999f;
  // } else {
  //   // glm_vec3_scale(transform->scale, 1.0001f, transform->scale);
  //   transform->scale[Y] *= 1.0001f;
  // }
}




void tds_enemy_damage(SFRcomponent_t* component, uint32_t damage) {

}
