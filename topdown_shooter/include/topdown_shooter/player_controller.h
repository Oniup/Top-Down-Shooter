#ifndef __TDS_PLAYER_CONTROLLER_H__
#define __TDS_PLAYER_CONTROLLER_H__

#include <safire/safire.h>

typedef struct TDSplayer_controller TDSplayer_controller_t;

struct TDSplayer_controller {
  float move_speed;
  int damage;
};

#define TDS_PLAYER_CONTROLLER "player controller"

#endif // __TDS_PLAYER_CONTROLLER_H__