#ifndef __TDS_PLAYER_CONTROLLER_H__
#define __TDS_PLAYER_CONTROLLER_H__

#include <safire/safire.h>

#define TDS_PLAYER_CONTROLLER           "player controller"

typedef struct TDS_PlayerController     TDS_PlayerController;




struct TDS_PlayerController 
{
  float                                 move_speed;
  int                                   damage;
  SFR_Timer                             shooting_animation_time;
  SFR_Entity*                           gun;
};

SAFIRE_API TDS_PlayerController*        tds_player_controller_create_instance();
SAFIRE_API void                         tds_player_controller_load_assets();

SAFIRE_API SFR_Component*               tds_player_controller();
SAFIRE_API void                         tds_player_damage(SFR_Component* player_controller_comp, uint32_t damage);




#endif // __TDS_PLAYER_CONTROLLER_H__