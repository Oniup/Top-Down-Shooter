#include "../include/topdown_shooter/enemy_handler.h"
#include "../include/topdown_shooter/enemy_controller.h"


#define TDS_ENEMY_HANDLER_SPAWNER_COUNT 4


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
    "demon", "./art/enemy001-sprite-sheet.png", true, 4, false
  ));  
}

void tds_enemy_handler_add_kill(SFR_Component* component, uint32_t kill_points)
{
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);
  handler->kills++;
  handler->score += kill_points;
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
  handler->kills = 0;
  handler->score = 0;
  handler->time_btw_waves = 2.0f;
  handler->player = player;

  handler->spawn_locations = (vec2*)malloc(sizeof(vec2) * TDS_ENEMY_HANDLER_SPAWNER_COUNT);
  glm_vec2_copy((vec2) { 1.0f , 0.0f }, handler->spawn_locations[0]);
  glm_vec2_copy((vec2) { -1.0f , 0.0f }, handler->spawn_locations[1]);
  glm_vec2_copy((vec2) { 0.0f , 1.0f }, handler->spawn_locations[2]);
  glm_vec2_copy((vec2) { 0.0f , -1.0f }, handler->spawn_locations[3]);

  for (uint32_t i = 0; i < 4; i++)
  {
    if (i < 2)
      glm_vec2_scale(handler->spawn_locations[i], 15.0f, handler->spawn_locations[i]);
    else
      glm_vec2_scale(handler->spawn_locations[i], 10.0f, handler->spawn_locations[i]);
  }

  return component;
}




void _tds_enemy_handler_late_update(SFR_Component* component, float late_delta_time) 
{
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  // rotating the 2 spawn locations around the map 

  static float angle = 0.0f;
  angle += 200.0f * late_delta_time;

  float theta = angle;
  for (uint32_t i = 0; i < TDS_ENEMY_HANDLER_SPAWNER_COUNT; i++) 
  {
    float radius = sqrtf(glm_vec2_dot(handler->spawn_locations[i], handler->spawn_locations[i]));

    vec2 position = { 
      cosf(theta) * radius,
      sinf(theta) * radius
    };

    glm_vec2_copy(position, handler->spawn_locations[i]);
    theta += 1.5708f;
  }

  static SFR_Timer timer = 0.0f;
  if (sfr_timer_finished(&timer)) 
  {
    timer = sfr_timer_start(handler->time_btw_waves);

    SFR_Transform* player_transform = SFR_COMPONENT_CONVERT(SFR_Transform, handler->player->components[0]);

    for (uint32_t i = 0; i < TDS_ENEMY_HANDLER_SPAWNER_COUNT; i++)
    {
      vec2 location;  
      glm_vec2_add(handler->spawn_locations[i], player_transform->position, location);
      tds_instantiate_enemy(location, TDS_ENEMY_TYPE_DEMON, handler->player);
    }
  }
}

void _tds_enemy_hander_free(SFR_Component* component) 
{
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  if (handler->spawn_locations != NULL) 
    free(handler->spawn_locations);
}
