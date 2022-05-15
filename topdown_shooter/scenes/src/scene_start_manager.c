#include "../scene_start_manager.h"

#include "../scene_arena.h"

/**
 * This is where you define all the scenes in the game
 * all the scenes here will be pushed to safire
*/
SFR_Scene** sfr_get_scenes(uint32_t* count) 
{
  *count = 1;
  SFR_Scene** scenes = (SFR_Scene**)malloc(sizeof(SFR_Scene*));

  scenes[0] = sfr_scene("Arena", scene_arena_function_pointer);

  return scenes;
}
