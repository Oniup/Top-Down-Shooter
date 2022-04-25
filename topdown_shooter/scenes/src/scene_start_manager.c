#include "../scene_start_manager.h"

#include "../scene_arena.h"

/**
 * This is where you define all the scenes in the game
 * all the scenes here will be pushed to safire
*/
SFRscene_t** sfr_get_scenes(uint32_t* count) {
  *count = 1;
  SFRscene_t** scenes = (SFRscene_t**)malloc(sizeof(SFRscene_t*));

  scenes[0] = sfr_scene("Arena", scene_arena_function_pointer);

  return scenes;
}
