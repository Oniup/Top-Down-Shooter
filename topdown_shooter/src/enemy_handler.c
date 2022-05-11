#include "../include/topdown_shooter/enemy_handler.h"
#include "../include/topdown_shooter/enemy_controller.h"




void _tds_enemy_handler_late_update(SFRcomponent_t* component, float late_delta_time);




TDSenemy_handler_t* tds_enemy_handler_create_instance(SFRentity_t* player) {
  SFRentity_t* entity = sfr_ecs_push_entity(TDS_ENEMY_HANDLER, TDS_ENEMY_HANDLER);

  SFRcomponent_t* component = sfr_ecs_push_component(entity, tds_enemy_handler(player));
  TDSenemy_handler_t* handler = SFR_COMPONENT_CONVERT(TDSenemy_handler_t, component);

  return handler;
}

void tds_enemy_handler_load_assets() {
  // loading all the enemy textures
  sfr_pipeline_push_texture(sfr_texture(
    "enemy-blob", "./art/sadge.png", true, 4, false
  ));
}

SFRcomponent_t* tds_enemy_handler(SFRentity_t* player) {
  SFRcomponent_t* component =  sfr_ecs_component(
    TDS_ENEMY_HANDLER, NULL, _tds_enemy_handler_late_update, NULL
  );

  component->data = (TDSenemy_handler_t*)malloc(sizeof(TDSenemy_handler_t));
  TDSenemy_handler_t* handler = SFR_COMPONENT_CONVERT(TDSenemy_handler_t, component);

  handler->spawn_rate = 1;
  handler->wave = 1;
  handler->time_btw_waves = 2.0f;
  handler->player = player;

  return component;
}




void _tds_enemy_handler_late_update(SFRcomponent_t* component, float late_delta_time) {
  TDSenemy_handler_t* handler = SFR_COMPONENT_CONVERT(TDSenemy_handler_t, component);

  static SFRtimer_t timer = 0.0f;
  if (sfr_timer_finished(&timer)) {
    timer = sfr_timer_start(handler->time_btw_waves);
    static vec2 pos = { 0.0f, 0.0f };
    tds_instantiate_enemy(pos, TDS_ENEMY_TYPE_BLOB, handler->player);
  }
}
