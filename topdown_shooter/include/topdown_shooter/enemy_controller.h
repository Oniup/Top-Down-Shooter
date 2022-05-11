#ifndef __TDS_ENEMY_CONTROLLER_H__
#define __TDS_ENEMY_CONTROLLER_H__

#include <safire/safire.h>

#define TDS_ENEMY_CONTROLLER "enemy controller"

typedef struct TDSenemy_controller     TDSenemy_controller_t;
typedef enum TDSenemy_type             TDSenemy_type_t;




enum TDSenemy_type {
  TDS_ENEMY_TYPE_BLOB                  = 0,
};

struct TDSenemy_controller {
  TDSenemy_type_t                   type;

  int                               health;
  float                             damage;
  float                             move_speed;

  SFRentity_t*                      player;
};

SAFIRE_API SFRcomponent_t*          tds_enemy_controller(TDSenemy_type_t type, SFRentity_t* player);

SAFIRE_API SFRentity_t*             tds_instantiate_enemy(vec2 spawn_pos, TDSenemy_type_t type, SFRentity_t* player);
SAFIRE_API void                     tds_enemy_damage(SFRcomponent_t* component, uint32_t damage);

#endif // __TDS_ENEMY_CONTROLLER_H__