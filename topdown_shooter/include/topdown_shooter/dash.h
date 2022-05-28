#ifndef __TDS_DASH_H__
#define __TDS_DASH_H__


#include <safire/safire.h>

#define TDS_DASH                        "dash controller"

typedef struct TDS_Dash                 TDS_Dash;
typedef void (*tds_trail_instantiate)   (SFR_Texture*, float);


struct TDS_Dash
{
  float                                 duration;
  float                                 speed; 
  vec2                                  direction;
  bool                                  is_dashing;

  SFR_Timer                             timer;

  SFR_Texture*                          trail_texture;
  float                                 trail_life_time;
  tds_trail_instantiate                 trail_instantiate;
};




SFR_Component*                          tds_dash(float duration, float speed);
void                                    tds_set_trail(SFR_Component* component, tds_trail_instantiate trail_instantiate, SFR_Texture* trail_texture, float trail_life_time);
void                                    tds_enable_dash(SFR_Component* component, vec2 direction);


#endif // __TDS_DASH_H__