#ifndef __TDH_SCENE_ARENA_H__
#define __TDH_SCENE_ARENA_H__

#include <safire/safire.h>



// world size
#define TDS_ARENA_TILES_X               60
#define TDS_ARENA_TILES_Y               60
// non walkable space in world
#define TDS_ARENA_TILES_WALLS_X         8  // non walkable space but still visable at the edge of the world
#define TDS_ARENA_TILES_WALLS_Y         8 



SAFIRE_API void                         scene_arena_function_pointer(SFR_Scene* scene);

SAFIRE_API void                         scene_arena_set_player_health(SFR_Scene* scene, uint32_t health);
SAFIRE_API SFR_Component*               scene_arena_get_player(SFR_Scene* scene);

#endif // __TDH_SCENE_ARENA_H__