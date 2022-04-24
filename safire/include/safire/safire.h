#ifndef __SAFIRE_H__
#define __SAFIRE_H__

#ifdef __cplusplus 
extern "C" {
#endif

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

SAFIRE_API void safire(const char* window_title, int window_width, int window_height, bool fullscreen, SFRscene_t* opening_scene);
SAFIRE_API void sfr_free();
SAFIRE_API void sfr_run();

SAFIRE_API SFRentity_t* sfr_instantiate(const char* name);
SAFIRE_API void sfr_destroy(SFRentity_t* entity);

#ifdef __cplusplus
}
#endif

#endif // __SAFIRE_H__