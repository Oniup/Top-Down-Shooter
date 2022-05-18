#ifndef __TDH_SCENE_ARENA_H__
#define __TDH_SCENE_ARENA_H__

#include <safire/safire.h>



// world size
#define TDS_ARENA_TILES_X               100
#define TDS_ARENA_TILES_Y               100
// non walkable space in world
#define TDS_ARENA_TILES_WALLS_X         5  // non walkable space but still visable at the edge of the world
#define TDS_ARENA_TILES_WALLS_Y         5 



SAFIRE_API void                         scene_arena_function_pointer(SFR_Scene* scene);

#endif // __TDH_SCENE_ARENA_H__