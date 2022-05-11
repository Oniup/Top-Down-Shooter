#ifndef __TDS_PLAYER_CONTROLLER_H__
#define __TDS_PLAYER_CONTROLLER_H__

#include <safire/safire.h>

#define TDS_PLAYER_CONTROLLER "player controller"

typedef struct TDSplayer_controller TDSplayer_controller_t;




struct TDSplayer_controller {
  float move_speed;
  int damage;
};

SAFIRE_API TDSplayer_controller_t*  tds_player_controller_create_instance();
SAFIRE_API void                     tds_player_controller_load_assets();

SAFIRE_API SFRcomponent_t*          tds_player_controller();
SAFIRE_API void                     tds_player_damage(SFRcomponent_t* player_controller_comp, uint32_t damage);




#endif // __TDS_PLAYER_CONTROLLER_H__