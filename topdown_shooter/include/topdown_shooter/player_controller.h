#ifndef __TDS_PLAYER_CONTROLLER_H__
#define __TDS_PLAYER_CONTROLLER_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include <safire/safire.h>

#define TDS_PLAYER_CONTROLLER           "player controller"

typedef struct TDS_PlayerController     TDS_PlayerController;




struct TDS_PlayerController 
{
  float                                 move_speed;
  int                                   health;
  int                                   damage;
  
  SFR_Timer                             shooting_animation_time;
  float                                 time_btw_bullets;
  SFR_Entity*                           gun;

  SFR_Component*                        dash;
};

SAFIRE_API SFR_Component*               tds_player_controller_create_instance();
SAFIRE_API void                         tds_player_controller_load_assets();

SAFIRE_API SFR_Component*               tds_player_controller();
SAFIRE_API void                         tds_player_damage(SFR_Component* player_controller_comp, int damage);




#ifdef __cplusplus
}
#endif 

#endif // __TDS_PLAYER_CONTROLLER_H__