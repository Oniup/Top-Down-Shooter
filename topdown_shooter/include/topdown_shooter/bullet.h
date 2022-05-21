#ifndef __TDS_BULLET_H__
#define __TDS_BULLET_H__

#ifdef __cplusplus 
extern "C" {
#endif 

#include <safire/safire.h>

#define TDS_BULLET                      "Bullet"

typedef struct TDS_Bullet               TDS_Bullet;




struct TDS_Bullet 
{
  float                                 move_speed;
  int                                   damage;
  char*                                 target_tag;
  vec2                                  direction;
  SFR_Timer                             lifetime;
  bool                                  does_damage;
  SFR_Component*                        sprite_animator;
};

SAFIRE_API SFR_Component*               tds_bullet_create_instance(vec2 move_direction, const char* target_tag, float move_speed, int damage);

SFR_Component*                          tds_bullet(vec2 move_direction, const char* target_tag, float move_speed, int damage);




#ifdef __cplusplus
}
#endif 

#endif // __TDS_BULLET_H__