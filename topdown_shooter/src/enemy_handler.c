#include "../include/topdown_shooter/enemy_handler.h"
#include "../include/topdown_shooter/enemy_controller.h"




void                                    _tds_enemy_handler_late_update(SFR_Component* component, float late_delta_time);
void                                    _tds_enemy_hander_free(SFR_Component* component);




TDS_EnemyHandler* tds_enemy_handler_create_instance(SFR_Entity* player) 
{
  SFR_Entity* entity = sfr_ecs_push_entity(TDS_ENEMY_HANDLER, TDS_ENEMY_HANDLER);

  SFR_Component* component = sfr_ecs_push_component(entity, tds_enemy_handler(player));
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  return handler;
}

void tds_enemy_handler_load_assets() 
{
  // loading all the enemy textures
  sfr_pipeline_push_texture(sfr_texture(
    "enemy-blob", "./art/sadge.png", true, 4, false
  ));
}

SFR_Component* tds_enemy_handler(SFR_Entity* player) 
{
  SFR_Component* component =  sfr_ecs_component(
    TDS_ENEMY_HANDLER, NULL, _tds_enemy_handler_late_update, NULL
  );

  component->data = (TDS_EnemyHandler*)malloc(sizeof(TDS_EnemyHandler));
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  handler->spawn_rate = 1;
  handler->wave = 1;
  handler->time_btw_waves = 2.0f;
  handler->player = player;

  handler->spawn_locations = (vec2*)malloc(sizeof(vec2) * 2);
  glm_vec2_copy((vec2) { 5.0f , 0.0f }, handler->spawn_locations[0]);
  glm_vec2_copy((vec2) { -5.0f , 0.0f }, handler->spawn_locations[1]);

  return component;
}




void _tds_enemy_handler_late_update(SFR_Component* component, float late_delta_time) 
{
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  // rotating the 2 spawn locations around the map 

  static float angle = 0.0f;
  angle += 200.0f * late_delta_time;
  if (angle >= 3.1416f) 
    angle = 0.0f;

  uint32_t i = 0;
  for (float theta = angle; theta < 3.1416f; theta += 1.5708f) 
  {
    float radius = sqrtf(glm_vec2_dot(handler->spawn_locations[i], handler->spawn_locations[i]));

    vec2 position = { 
      cosf(theta) * radius,
      sinf(theta) * radius
    };

    glm_vec2_copy(position, handler->spawn_locations[i]);
    i++;
  }

  static SFR_Timer timer = 0.0f;
  if (sfr_timer_finished(&timer)) 
  {
    timer = sfr_timer_start(handler->time_btw_waves);
    tds_instantiate_enemy(handler->spawn_locations[0], TDS_ENEMY_TYPE_BLOB, handler->player);
    tds_instantiate_enemy(handler->spawn_locations[1], TDS_ENEMY_TYPE_BLOB, handler->player);
  }
}

void _tds_enemy_hander_free(SFR_Component* component) 
{
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  if (handler->spawn_locations != NULL) 
    free(handler->spawn_locations);
}
