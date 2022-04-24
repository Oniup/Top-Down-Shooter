#ifndef __SAFIRE_H__
#define __SAFIRE_H__

// main includes
#include "pipeline.h"
#include "ecs.h"
#include "input.h"
#include "utils.h"

typedef struct SFRcontext SFRcontext_t;
typedef struct SFRcreate_info SFRcreate_info_t;

struct SFRcreate_info {
  const char* window_title;
  uint32_t window_size[2];
};

// TODO: game manager entity needed in the parameters here later
extern void safire(const char* window_title, int window_width, int window_height, bool fullscreen);
// extern void safire(const char* window_title, uint32_t window_width, uint32_t window_height, bool fullscreen, SFRentity_t* game_manager);

extern void sfr_free();

extern void sfr_run();
extern SFRpipeline_t* sfr_get_pipeline();
extern SFRecs_t* sfr_get_ecs();



#endif // __SAFIRE_H__