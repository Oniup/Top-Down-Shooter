#include "../scene_arena.h"

// entities
#include <topdown_shooter/player_controller.h>

typedef struct TDSarena {
  char* message;
} TDSarena_t;

void scene_arena_start(SFRscene_t* scene);
void scene_arena_update(SFRscene_t* scene, float delta_time);
void scene_arena_free(SFRscene_t* scene);

void scene_arena_function_pointer(SFRscene_t* scene) {
  scene->data = NULL;
  scene->start = &scene_arena_start;
  scene->update = &scene_arena_update;
  scene->late_update = NULL;
  scene->free = &scene_arena_free;
}

// TODO: make it so that the scene can have its own custom data added to it
// typedef struct TDSarena_scene {
  
// } TDSarena_scene_t;

void scene_arena_start(SFRscene_t* scene) {
  scene->data = (TDSarena_t*)malloc(sizeof(TDSarena_t));
  TDSarena_t* data = ((TDSarena_t*)scene->data);
  data->message = sfr_str("this is a test from the arena");

  sfr_instantiate("Player");
  

  /**
   * TODO: scene setup
   * spawn entity into the scene
    * player
    * enemy spawners 
    * scoring system
    * player health system
  */
}

void scene_arena_update(SFRscene_t* scene, float delta_time) {
  // TODO: manage player health and score ...
}

void scene_arena_free(SFRscene_t* scene) {
  TDSarena_t* arena = ((TDSarena_t*)scene->data);

  sfr_str_free(&arena->message);
  free(scene->data);
}
