#ifndef __TDS_ENEMY_HANDLER_H__
#define __TDS_ENEMY_HANDLER_H__

#include <safire/safire.h>




#define TDS_ENEMY_HANDLER "enemy handler"




typedef struct TDSenemy_handler     TDSenemy_handler_t;
typedef struct TDSenemy_spawner     TDSenemy_spawner_t;




struct TDSenemy_handler {
  uint32_t                          wave;
  uint32_t                          spawn_rate;
  float                             time_btw_waves;

  SFRentity_t*                      player;
};

SAFIRE_API TDSenemy_handler_t*      tds_enemy_handler_create_instance(SFRentity_t* player);
SAFIRE_API void                     tds_enemy_handler_load_assets();

SAFIRE_API SFRcomponent_t*          tds_enemy_handler(SFRentity_t* player);




#endif // __TDS_PLAYER_CONTROLLER_H__