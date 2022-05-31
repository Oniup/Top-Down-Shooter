#ifndef __TDS_ENEMY_HANDLER_H__
#define __TDS_ENEMY_HANDLER_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include <safire/safire.h>




#define TDS_ENEMY_HANDLER "enemy handler"




typedef struct TDS_EnemyHandler         TDS_EnemyHandler;
typedef struct TDS_PlayerController     TDS_PlayerController;


#define TDS_ENEMY_HANDLER_T_BTW_WAVE    40.0f
#define TDS_ENEMY_HANDLER_T_BTW_SPAWN   1.0f
#define TDS_ENEMY_HANDLER_SPAWNER_COUNT 4

#define TDS_ENEMY_CAN_WAVE_DEMON        0
#define TDS_ENEMY_CAN_WAVE_SLENDER_MAN  15
#define TDS_ENEMY_CAN_WAVE_GIGACHAD     10

#define TDS_ENEMY_CAN_SPAWN_DEMON       0
#define TDS_ENEMY_CAN_SPAWN_SLENDER_MAN 80
#define TDS_ENEMY_CAN_SPAWN_GIGACHAD    40




struct TDS_EnemyHandler 
{
  uint32_t                              wave;
  float                                 spawn_rate;
  SFR_Timer                             spawn_timer;
  uint32_t                              kills_until_next_wave;

  uint32_t                              kills;
  uint32_t                              score;

  vec2*                                 spawn_locations;

  SFR_Entity*                           player;
  TDS_PlayerController*                 player_controller;
};

SAFIRE_API SFR_Component*               tds_enemy_handler_create_instance(SFR_Entity* player);
SAFIRE_API void                         tds_enemy_handler_load_assets();
SAFIRE_API void                         tds_enemy_handler_add_kill(SFR_Component* component, uint32_t kill_points);

SAFIRE_API SFR_Component*               tds_enemy_handler(SFR_Entity* player);




#ifdef __cplusplus
}
#endif 

#endif // __TDS_PLAYER_CONTROLLER_H__