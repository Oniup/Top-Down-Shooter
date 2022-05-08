#ifndef __TDS_ENEMY_CONTROLLER_H__
#define __TDS_ENEMY_CONTROLLER_H__

#include <safire/safire.h>

#define TDS_ENEMY_CONTROLLER "enemy controller"

typedef struct TDSenemy_controller     TDSenemy_controller_t;
typedef enum TDSenemy_type             TDSenemy_type_t;




enum TDSenemy_type {
  TDS_ENEMY_TYPE_IMP                    = 0,
};

struct TDSenemy_controller {
  int health;
  float damage;
  float move_speed;
};

SAFIRE_API SFRcomponent_t*  tds_enemy_controller();
SAFIRE_API void             tds_enemy_damage(SFRcomponent_t* component, uint32_t damage);

#endif // __TDS_ENEMY_CONTROLLER_H__