#ifndef __TDS_ENEMY_HANDLER_H__
#define __TDS_ENEMY_HANDLER_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include <safire/safire.h>




#define TDS_ENEMY_HANDLER "enemy handler"




typedef struct TDS_EnemyHandler         TDS_EnemyHandler;
typedef struct TDS_EnemySpawner         TDS_EnemySpawner;
typedef struct TDS_PlayerController     TDS_PlayerController;




struct TDS_EnemyHandler 
{
  uint32_t                              wave;
  uint32_t                              kills;
  uint32_t                              score;

  uint32_t                              spawn_rate;
  float                                 time_btw_waves;

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