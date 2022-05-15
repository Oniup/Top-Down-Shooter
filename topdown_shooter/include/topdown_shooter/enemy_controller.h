#ifndef __TDS_ENEMY_CONTROLLER_H__
#define __TDS_ENEMY_CONTROLLER_H__

#include <safire/safire.h>

#define TDS_ENEMY_CONTROLLER            "enemy controller"

typedef struct TDS_EnemyController      TDS_EnemyController;
typedef enum TDS_EnemyType              TDS_EnemyType;




enum TDS_EnemyType 
{
  TDS_ENEMY_TYPE_BLOB                   = 0,
};

struct TDS_EnemyController 
{
  TDS_EnemyType                         type;

  int                                   health;
  float                                 damage;
  float                                 move_speed;

  SFR_Entity*                           player;
};

SAFIRE_API SFR_Component*               tds_enemy_controller(TDS_EnemyType type, SFR_Entity* player);

SAFIRE_API SFR_Entity*                  tds_instantiate_enemy(vec2 spawn_pos, TDS_EnemyType type, SFR_Entity* player);
SAFIRE_API void                         tds_enemy_damage(SFR_Component* component, uint32_t damage);

#endif // __TDS_ENEMY_CONTROLLER_H__