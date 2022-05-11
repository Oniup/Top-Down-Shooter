#ifndef __TDH_SCENE_ARENA_H__
#define __TDH_SCENE_ARENA_H__

#include <safire/safire.h>




#define TDS_ARENA_TILES_X            40 // non walkable space but still visable at the edge of the world
#define TDS_ARENA_TILES_Y            40 




SAFIRE_API void scene_arena_function_pointer(SFRscene_t* scene);

#endif // __TDH_SCENE_ARENA_H__