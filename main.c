#include <safire/safire.h>
#include <scenes/scene_start_manager.h>
int main(int argc, char** argv) {  
  uint32_t scenes_count = 0;
  SFRscene_t** scenes = sfr_get_scenes(&scenes_count);
  safire("Top-Down Shooter game", 1920 / 2, 1080 / 2, false, scenes, scenes_count);
  return 0;
}