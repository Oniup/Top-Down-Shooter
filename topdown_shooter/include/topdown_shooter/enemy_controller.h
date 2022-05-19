#ifndef __TDS_ENEMY_CONTROLLER_H__
#define __TDS_ENEMY_CONTROLLER_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include <safire/safire.h>

#define TDS_ENEMY_CONTROLLER            "enemy controller"

typedef struct TDS_EnemyController      TDS_EnemyController;
typedef enum TDS_EnemyType              TDS_EnemyType;
typedef enum TDS_EnemyState             TDS_EnemyState;




enum TDS_EnemyType 
{
  TDS_ENEMY_TYPE_DEMON                   = 0,
};

enum TDS_EnemyState
{
  TDS_ENEMY_STATE_IDLE                  = 0,
  TDS_ENEMY_STATE_ATTACKING,
  TDS_ENEMY_STATE_DEATH
};

struct TDS_EnemyController 
{
  TDS_EnemyType                         type;

  int                                   health;
  uint32_t                              damage;
  float                                 move_speed;
  bool                                  flip;

  TDS_EnemyState                        state;
  SFR_Timer                             after_hit_timer;
  int                                   movement_offset;                                 

  SFR_Entity*                           player;
};

SAFIRE_API SFR_Component*               tds_enemy_controller(TDS_EnemyType type, SFR_Entity* player);

SAFIRE_API SFR_Entity*                  tds_instantiate_enemy(vec2 spawn_pos, TDS_EnemyType type, SFR_Entity* player);
SAFIRE_API void                         tds_enemy_damage(SFR_Entity* enemy, uint32_t damage);




#ifdef __cplusplus
}
#endif 

#endif // __TDS_ENEMY_CONTROLLER_H__