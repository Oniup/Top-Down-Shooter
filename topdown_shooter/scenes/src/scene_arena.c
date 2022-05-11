#include "../scene_arena.h"

// entities
#include <topdown_shooter/player_controller.h>
#include <topdown_shooter/enemy_handler.h>
#include <topdown_shooter/enemy_controller.h>
#include <topdown_shooter/camera_controller.h>




typedef struct TDSarena {
  TDSplayer_controller_t* player;
  TDSenemy_handler_t* enemy_handler;
} TDSarena_t;

void _scene_arena_start(SFRscene_t* scene);
void _scene_arena_update(SFRscene_t* scene, float delta_time);
void _scene_arena_free(SFRscene_t* scene);

void _scene_arena_create_background(SFRscene_t* scene);
void _scene_arena_world_inits(SFRscene_t* scene);
void _scene_arena_load_assets(SFRscene_t* scene);




void scene_arena_function_pointer(SFRscene_t* scene) {
  scene->data = NULL;
  scene->start = &_scene_arena_start;
  scene->update = &_scene_arena_update;
  scene->late_update = NULL;
  scene->free = &_scene_arena_free;
}

void _scene_arena_start(SFRscene_t* scene) {
  scene->data = (TDSarena_t*)malloc(sizeof(TDSarena_t));
  TDSarena_t* data = ((TDSarena_t*)scene->data);

  sfr_window_set_clear(sfr_pipeline_get_window(), (vec4){ 0.14f, 0.19f, 0.17f, 1.0f });

  _scene_arena_load_assets(scene);
  _scene_arena_create_background(scene);
  _scene_arena_world_inits(scene);


  SFRentity_t* camera = sfr_ecs_push_entity("camera", "camera");
  sfr_ecs_push_component(camera, tds_camera_controller());

  /**
   * TODO: scene setup
   * spawn entity into the scene
    * entity
    * enemy spawners 
    * scoring system
    * entity health system
  */
}

void _scene_arena_update(SFRscene_t* scene, float delta_time) {

}

void _scene_arena_free(SFRscene_t* scene) {
  TDSarena_t* arena = ((TDSarena_t*)scene->data);

  free(scene->data);
}

void _scene_arena_create_background(SFRscene_t* scene) {
  vec2 centre = { TDS_ARENA_TILES_X, TDS_ARENA_TILES_Y };
  glm_vec2_scale(centre, 0.5f, centre);

  for (uint32_t y = 0; y < TDS_ARENA_TILES_Y; y++) {
    for (uint32_t x = 0; x < TDS_ARENA_TILES_X; x++) {
      char name[10];
      sprintf(name, "tile:%u:%u", x, y);
      name[9] = '\0';

      SFRentity_t* tile = sfr_ecs_push_entity(name, "tile:walkbale");

      SFRtransform_t* transform = SFR_COMPONENT_CONVERT(SFRtransform_t, tile->components[0]);
      vec3 offset = { x - centre[X], y - centre[Y], 0.0f };
      glm_vec3_scale(offset, 2.0f, offset);
      glm_vec3_add(transform->position, offset, transform->position);
      
      SFRcomponent_t* sprite_renderer = sfr_ecs_push_component(tile, sfr_sprite_renderer());
      if (x > 5 && x < TDS_ARENA_TILES_X - 5) {
        if (y > 5 && y < TDS_ARENA_TILES_Y - 5) {
          sfr_sprite_renderer_set_texture(sprite_renderer, "tile:floor");
          continue;
        }
      }
            
      sfr_sprite_renderer_set_texture(sprite_renderer, "tile:wall");
    }
  }
}

void _scene_arena_world_inits(SFRscene_t* scene) {
  TDSarena_t* arena = ((TDSarena_t*)scene->data);

  
  arena->player        = tds_player_controller_create_instance();
  SFRentity_t* player = sfr_ecs_get_target_entity(sfr_ecs_get_entities_count() - 1);
  arena->enemy_handler = tds_enemy_handler_create_instance(player);

}

void _scene_arena_load_assets(SFRscene_t* scene) {
  // loading the tile assest
  sfr_pipeline_push_texture(sfr_texture(
    "tile:floor", "./art/sand.png", false, 4, false
  ));
  sfr_pipeline_push_texture(sfr_texture(
    "tile:wall", "./art/wall.png", false, 4, false
  ));

  tds_player_controller_load_assets();
  tds_enemy_handler_load_assets();
}
