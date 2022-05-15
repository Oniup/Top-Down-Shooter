#ifndef __TDS_ENEMY_HANDLER_H__
#define __TDS_ENEMY_HANDLER_H__

#include <safire/safire.h>




#define TDS_ENEMY_HANDLER "enemy handler"




typedef struct TDS_EnemyHandler         TDS_EnemyHandler;
typedef struct TDS_EnemySpawner         TDS_EnemySpawner;




struct TDS_EnemyHandler 
{
  uint32_t                              wave;
  uint32_t                              spawn_rate;
  float                                 time_btw_waves;

  vec2*                                 spawn_locations;

  SFR_Entity*                           player;
};

SAFIRE_API TDS_EnemyHandler*            tds_enemy_handler_create_instance(SFR_Entity* player);
SAFIRE_API void                         tds_enemy_handler_load_assets();

SAFIRE_API SFR_Component*               tds_enemy_handler(SFR_Entity* player);




#endif // __TDS_PLAYER_CONTROLLER_H__