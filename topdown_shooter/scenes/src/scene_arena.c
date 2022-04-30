#include "../scene_arena.h"

// entities
#include <topdown_shooter/player_controller.h>

typedef struct TDSarena {
  bool can_print_debug;
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
  data->can_print_debug = true;

  for (uint32_t i = 0; i < 2; i++) {
    SFRentity_t* player = NULL;
    if (i == 0) {
      player = sfr_ecs_push_entity("Player1", "player");
    } else {
      player = sfr_ecs_push_entity("Player2", "player");
    }
    sfr_ecs_push_component(player, tds_player_controller());
    sfr_ecs_push_component(player, sfr_sprite_renderer());
    sfr_ecs_push_component(player, sfr_sprite_animator(player));
  }

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
  TDSarena_t* arena = ((TDSarena_t*)scene->data);

  if (arena->can_print_debug) {
    if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_P)) {
      sfr_ecs_debug_print_entities();
      sfr_ecs_debug_print_components();
      arena->can_print_debug = false;
    } else if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_K) && sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_LEFT_SHIFT)) {
      if (sfr_ecs_get_entities_count() > 0) {
        sfr_ecs_erase_entity(0);
        arena->can_print_debug = false;
      }
    }
  } else {
    if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_H)) {
      arena->can_print_debug = true;
      sfr_clear_terminal();
    }
  }
}

void scene_arena_free(SFRscene_t* scene) {
  TDSarena_t* arena = ((TDSarena_t*)scene->data);

  free(scene->data);
}
