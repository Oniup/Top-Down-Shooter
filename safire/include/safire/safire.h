#ifndef __SAFIRE_H__
#define __SAFIRE_H__

#ifdef __cplusplus 
extern "C" {
#endif

// main includes
#include "pipeline.h"
#include "ecs.h"
#include "components.h"
#include "input.h"
#include "utils.h"




SAFIRE_API void                         safire(const char* window_title, int window_width, int window_height, 
                                        bool fullscreen, SFR_Scene** scenes, uint32_t scenes_count);
SAFIRE_API void                         sfr_free();
SAFIRE_API void                         sfr_run();

SAFIRE_API float                        sfr_elapsed_time();

SAFIRE_API SFR_Entity*                  sfr_instantiate(const char* name);
SAFIRE_API void                         sfr_destroy(SFR_Entity* entity);




#ifdef __cplusplus
}
#endif

#endif // __SAFIRE_H__