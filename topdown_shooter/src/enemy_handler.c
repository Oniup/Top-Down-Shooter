#include "../include/topdown_shooter/enemy_handler.h"

#include "../include/topdown_shooter/enemy_controller.h"
#include "../include/topdown_shooter/player_controller.h"
#include "../scenes/scene_arena.h"


void                                    _tds_enemy_handler_late_update(SFR_Component* component, float late_delta_time);
void                                    _tds_enemy_handler_free(SFR_Component* component);




SFR_Component* tds_enemy_handler_create_instance(SFR_Entity* player) 
{
  SFR_Entity* entity = sfr_ecs_push_entity(TDS_ENEMY_HANDLER, TDS_ENEMY_HANDLER);

  SFR_Component* component = sfr_ecs_push_component(entity, tds_enemy_handler(player));
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  return component;
}

void tds_enemy_handler_load_assets() 
{
  // loading all the enemy textures
  sfr_pipeline_push_texture(sfr_texture(
    "demon", "./art/enemy001-sprite-sheet.png", true, 4, false
  ));  

  sfr_pipeline_push_texture(sfr_texture(
    "gigachad", "./art/enemy002-sprite-sheet.png", true, 4, false
  ));
}

void tds_enemy_handler_add_kill(SFR_Component* component, uint32_t kill_points)
{
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  if (handler->player_controller->health > 0)
  {
    handler->kills++;
    handler->score += kill_points;
    
    handler->kills_until_next_wave -= 1;
  }
}

SFR_Component* tds_enemy_handler(SFR_Entity* player) 
{
  SFR_Component* component =  sfr_ecs_component(
    TDS_ENEMY_HANDLER, NULL, _tds_enemy_handler_late_update, NULL, NULL
  );

  component->data = (TDS_EnemyHandler*)malloc(sizeof(TDS_EnemyHandler));
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  handler->wave = 1;
  handler->spawn_rate = 8;
  handler->kills_until_next_wave = 8;

  handler->kills = 0;
  handler->score = 0;

  handler->player = player;

  SFR_Component* player_controller = sfr_get_component(player->components[0], TDS_PLAYER_CONTROLLER);
  handler->player_controller = SFR_COMPONENT_CONVERT(TDS_PlayerController, player_controller);

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
  
  if (handler->player_controller->health > 0)
  {
    // rotating the 4 spawn locations around the map 
    static float angle = 0.0f;
    angle += 200.0f * late_delta_time;
        

    static uint32_t spawn_cost = 0;
    static bool spawning = true;
    static SFR_Timer btw_spawn = 0.0f; 
    static SFR_Timer btw_wave = 0.0f;

    SFR_Transform* player_transform = SFR_COMPONENT_CONVERT(SFR_Transform, handler->player->components[0]);

    if (spawning)
    {
      if (sfr_timer_finished(&btw_spawn))
      {
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

        for (uint32_t i = 0; i < TDS_ENEMY_HANDLER_SPAWNER_COUNT; i++)
        {
          vec2 location;
          glm_vec2_add(handler->spawn_locations[i], player_transform->position, location);

          // spawning enemies
          if (spawn_cost % TDS_ENEMY_CAN_SPAWN_GIGACHAD == 0 && handler->wave > TDS_ENEMY_CAN_WAVE_GIGACHAD)
          {
            printf("spawn gigachad!!!\n");
            tds_instantiate_enemy(location, component, TDS_ENEMY_TYPE_GIGACHAD, handler->player);
            spawn_cost += TDS_ENEMY_HANDLER_COST_GIGACHAD;
          }
          // else if (spawn_cost % TDS_ENEMY_CAN_SPAWN_SLENDER == 0 && handler->wave > TDS_ENEMY_CAN_WAVE_SLENDER)
          // {
          //   // TODO: add slender enemy type ...
          //   printf("spawn slender!!!\n");
            
          // }
          else // just spawn the generic demon enemy
          {
            // tds_instantiate_enemy(location, component, TDS_ENEMY_TYPE_DEMON, handler->player);
            tds_instantiate_enemy(location, component, TDS_ENEMY_TYPE_GIGACHAD, handler->player);
            
            spawn_cost += TDS_ENEMY_HANDLER_COST_DEMON;
          }

          // printf("spawn count: %u\n", spawn_cost);

          if (spawn_cost >= handler->spawn_rate)
          {
            spawning = false;
            btw_wave = sfr_timer_start(TDS_ENEMY_HANDLER_T_BTW_WAVE);
            break;
          }
        }

        btw_spawn = sfr_timer_start(TDS_ENEMY_HANDLER_T_BTW_SPAWN);
      }
    }
    else
    {
      if (sfr_timer_finished(&btw_wave) || handler->kills_until_next_wave == 0)
      {
        spawning = true;
        spawn_cost = 0;
        handler->spawn_rate += roundf(handler->spawn_rate * 0.15);
        handler->kills_until_next_wave = handler->spawn_rate;
        handler->wave++;

        printf("wave: %u, kills: %u, score: %u\n", handler->wave, handler->kills, handler->score);

        if (handler->player_controller->health < 3)
        {
          handler->player_controller->health++;
          scene_arena_set_player_health(sfr_ecs_get_active_scene(), handler->player_controller->health);
        }
      }
    }
  }
}

void _tds_enemy_handler_free(SFR_Component* component) 
{
  TDS_EnemyHandler* handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, component);

  if (handler->spawn_locations != NULL) 
    free(handler->spawn_locations);
}
