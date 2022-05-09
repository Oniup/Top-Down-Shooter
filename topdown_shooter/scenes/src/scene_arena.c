#include "../scene_arena.h"

// entities
#include <topdown_shooter/player_controller.h>
#include <topdown_shooter/enemy_controller.h>

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

void scene_arena_start(SFRscene_t* scene) {
  scene->data = (TDSarena_t*)malloc(sizeof(TDSarena_t));
  TDSarena_t* data = ((TDSarena_t*)scene->data);
  data->can_print_debug = true;

  sfr_pipeline_push_texture(sfr_texture(
    "sadge", "./art/sadge.png", true, 4, false
  ));

  sfr_pipeline_push_texture(sfr_texture(
    "popcat", "./art/popcat.png", true, 4, false
  ));

  sfr_window_set_clear(sfr_pipeline_get_window(), (vec4){ 0.14f, 0.19f, 0.17f, 1.0f });

  // initialise starting entities ...

  SFRentity_t* player = sfr_ecs_push_entity("Player1", "player");

  player->layer = 20;

  SFRcomponent_t* player_controller = sfr_ecs_push_component(player, tds_player_controller());
  SFRcomponent_t* player_renderer = sfr_ecs_push_component(player, sfr_sprite_renderer());
  // sfr_sprite_renderer_set_texture(player_renderer, "popcat");

  SFRcomponent_t* player_collider = sfr_ecs_push_component(player, sfr_collider2d());  
  {
    SFRcollider2d_t* collider = sfr_component_convert(SFRcollider2d_t, player_collider);
    collider->trigger = true;
  }


  for (uint32_t i = 0; i < 5; i++) {
    SFRentity_t* entity = sfr_ecs_push_entity("enemy1", "enemy");

    entity->layer = 10;

    static vec2 offset = { -7.0f, 0.0f };
    glm_vec2_add(offset, (vec2){ 3.0f, 0.0f }, offset);
    SFRtransform_t* transform = sfr_component_convert(SFRtransform_t, entity->components[0]);
    glm_vec2_add(offset, transform->position, transform->position);

    SFRcomponent_t* controller = sfr_ecs_push_component(entity, tds_enemy_controller());
    SFRcomponent_t* renderer = sfr_ecs_push_component(entity, sfr_sprite_renderer());
    SFRcomponent_t* collider = sfr_ecs_push_component(entity, sfr_collider2d());

    SFRcollider2d_t* c = sfr_component_convert(SFRcollider2d_t, collider);

    // sfr_sprite_renderer_set_texture(renderer, "sadge");
  }


  /**
   * TODO: scene setup
   * spawn entity into the scene
    * entity
    * enemy spawners 
    * scoring system
    * entity health system
  */
}

void scene_arena_update(SFRscene_t* scene, float delta_time) {
  // TODO: manage entity health and score ...
  TDSarena_t* arena = ((TDSarena_t*)scene->data);

  if (arena->can_print_debug) {
    if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_P)) {
      sfr_ecs_debug_print_entities();
      sfr_ecs_debug_print_components();
      printf("\n\n");
      sfr_pipeline_print_vertices_debug();
      
      arena->can_print_debug = false;
    } else if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_K) && sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_LEFT_SHIFT)) {
      if (sfr_ecs_get_entities_count() > 0) {
        sfr_ecs_erase_entity(0);

        arena->can_print_debug = false;
      }
    }
    if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_F)) {
      SFRentity_t* entity = sfr_ecs_push_entity("enemy1", "enemy");

      entity->layer = 10;

      static vec2 offset = { -7.0f, 3.0f };
      glm_vec2_add(offset, (vec2){ 3.0f, 0.0f }, offset);
      SFRtransform_t* transform = sfr_component_convert(SFRtransform_t, entity->components[0]);
      glm_vec2_add(offset, transform->position, transform->position);

      SFRcomponent_t* controller = sfr_ecs_push_component(entity, tds_enemy_controller());
      SFRcomponent_t* renderer = sfr_ecs_push_component(entity, sfr_sprite_renderer());

      sfr_sprite_renderer_set_texture(renderer, "sadge");

      SFRcomponent_t* animator = sfr_ecs_push_component(entity, sfr_sprite_animator(entity));

      arena->can_print_debug = false;
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
