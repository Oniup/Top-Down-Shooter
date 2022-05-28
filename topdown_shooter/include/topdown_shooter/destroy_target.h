#ifndef __TDS_DESTROY_TARGET_H__
#define __TDS_DESTROY_TARGET_H__


#include <safire/safire.h>


#define TDS_DESTROY_TARGET              "destroy target"

typedef struct TDS_DestroyTarget        TDS_DestroyTarget;




struct TDS_DestroyTarget 
{
  bool                                  timer_started;
  float                                 timer;
};

SAFIRE_API SFR_Component*               tds_destroy_target(float timer_length);
SAFIRE_API void                         tds_destroy_target_start(SFR_Component* component);

#endif // __TDS_DESTROY_TARGET_H__