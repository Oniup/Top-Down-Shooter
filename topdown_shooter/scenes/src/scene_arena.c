#include "../scene_arena.h"

// entities
#include <topdown_shooter/player_controller.h>
#include <topdown_shooter/enemy_handler.h>
#include <topdown_shooter/enemy_controller.h>
#include <topdown_shooter/camera_controller.h>


typedef struct TDS_Arena                TDS_Arena;

struct TDS_Arena 
{
  TDS_PlayerController*                 player;
  TDS_EnemyHandler*                     enemy_handler;
};

void                                    _scene_arena_start(SFR_Scene* scene);
void                                    _scene_arena_update(SFR_Scene* scene, float delta_time);
void                                    _scene_arena_free(SFR_Scene* scene);

void                                    _scene_arena_create_background(SFR_Scene* scene);
void                                    _scene_arena_world_inits(SFR_Scene* scene);
void                                    _scene_arena_load_assets(SFR_Scene* scene);




void scene_arena_function_pointer(SFR_Scene* scene) 
{
  scene->data = NULL;
  scene->start = &_scene_arena_start;
  scene->update = &_scene_arena_update;
  scene->late_update = NULL;
  scene->free = &_scene_arena_free;
}

void _scene_arena_start(SFR_Scene* scene) 
{
  scene->data = (TDS_Arena*)malloc(sizeof(TDS_Arena));
  TDS_Arena* data = ((TDS_Arena*)scene->data);

  sfr_window_set_clear(sfr_pipeline_get_window(), (vec4){ 0.14f, 0.19f, 0.17f, 1.0f });

  _scene_arena_load_assets(scene);
  _scene_arena_create_background(scene);
  _scene_arena_world_inits(scene);


  SFR_Entity* camera = sfr_ecs_push_entity("camera", "camera");
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

void _scene_arena_update(SFR_Scene* scene, float delta_time) 
{

}

void _scene_arena_free(SFR_Scene* scene) 
{
  TDS_Arena* arena = ((TDS_Arena*)scene->data);

  free(scene->data);
}

void _scene_arena_create_background(SFR_Scene* scene) 
{
  vec2 centre = { TDS_ARENA_TILES_X, TDS_ARENA_TILES_Y };
  glm_vec2_scale(centre, 0.5f, centre);

  vec2 uv_size = {
    8.0f / 64.0f,
    1.0f
  };

  for (uint32_t y = 0; y < TDS_ARENA_TILES_Y; y++) 
  {
    for (uint32_t x = 0; x < TDS_ARENA_TILES_X; x++) 
    {
      char name[10];
      sprintf(name, "tile:%u:%u", x, y);
      name[9] = '\0';

      SFR_Entity* tile = sfr_ecs_push_entity(name, "tile");

      SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, tile->components[0]);
      vec3 offset = { x - centre[X], y - centre[Y], 0.0f };
      glm_vec3_scale(offset, 0.5f, offset);
      glm_vec3_add(transform->position, offset, transform->position);
      glm_vec2_copy((vec2) { 0.3f, 0.3f }, transform->scale);
      
      SFR_Component* sprite_renderer = sfr_ecs_push_component(tile, sfr_sprite_renderer());
      sfr_sprite_renderer_set_texture(sprite_renderer, "tile");

      // walkable tile texture
      if (x > TDS_ARENA_TILES_WALLS_X && x < TDS_ARENA_TILES_X - TDS_ARENA_TILES_WALLS_X) 
      {
        if (y > TDS_ARENA_TILES_WALLS_X && y < TDS_ARENA_TILES_Y - TDS_ARENA_TILES_WALLS_X) 
        {
          sfr_sprite_renderer_set_uv(
            sprite_renderer, 
            (vec2) {
              uv_size[X] * 1.0f,
              uv_size[Y] * 1.0f,
            },
            (vec2) {
              uv_size[X] * 2.0f,
              uv_size[Y] * 2.0f,
            }
          );

          continue;
        }
      }

      // non walkable tile texture
      sfr_sprite_renderer_set_uv(
        sprite_renderer, 
        (vec2) {
          uv_size[X] * 0.0f,
          uv_size[Y] * 0.0f,
        },
        (vec2) {
          uv_size[X] * 1.0f,
          uv_size[Y] * 1.0f,
        }
      );
    }
  }
}

void _scene_arena_world_inits(SFR_Scene* scene) 
{
  TDS_Arena* arena = ((TDS_Arena*)scene->data);

  
  arena->player        = tds_player_controller_create_instance();
  SFR_Entity* player  = sfr_ecs_get_target_entity(sfr_ecs_get_entities_count() - 2);
  arena->enemy_handler = tds_enemy_handler_create_instance(player);

}

void _scene_arena_load_assets(SFR_Scene* scene) 
{
  // loading the tile assest
  sfr_pipeline_push_texture(sfr_texture(
    "tile", "./art/tile-sprite-sheet.png", false, 4, false
  ));

  tds_player_controller_load_assets();
  tds_enemy_handler_load_assets();
}
