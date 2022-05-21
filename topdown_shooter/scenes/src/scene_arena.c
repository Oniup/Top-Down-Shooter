#include "../scene_arena.h"

// entities
#include <topdown_shooter/player_controller.h>
#include <topdown_shooter/enemy_handler.h>
#include <topdown_shooter/enemy_controller.h>
#include <topdown_shooter/camera_controller.h>




typedef struct TDS_Arena                TDS_Arena;

struct TDS_Arena 
{
  SFR_Component*                        player;
  SFR_Component*                        player_health;

  SFR_Entity*                           cursor;
  SFR_Component*                        camera;
  
  SFR_Component*                        enemy_handler;
};

void                                    _scene_arena_start(SFR_Scene* scene);
void                                    _scene_arena_update(SFR_Scene* scene, float delta_time);

void                                    _scene_arena_create_background(SFR_Scene* scene);
void                                    _scene_arena_world_inits(SFR_Scene* scene);
void                                    _scene_arena_load_assets(SFR_Scene* scene);




void scene_arena_function_pointer(SFR_Scene* scene) 
{
  scene->data = NULL;
  scene->start = _scene_arena_start;
  scene->update = _scene_arena_update;
  scene->late_update = NULL;
  scene->free = NULL;
}

void _scene_arena_start(SFR_Scene* scene) 
{
  scene->data = (TDS_Arena*)malloc(sizeof(TDS_Arena));
  TDS_Arena* arena = SFR_COMPONENT_CONVERT(TDS_Arena, scene);
  
  sfr_window_set_clear(sfr_pipeline_get_window(), (vec4){ 0.14f, 0.19f, 0.17f, 1.0f });

  _scene_arena_load_assets(scene);
  _scene_arena_create_background(scene);
  _scene_arena_world_inits(scene);

  sfr_input_cursor_mode(SFR_CURSOR_HIDDEN);


  SFR_Entity* camera = sfr_ecs_push_entity("camera", "camera");
  arena->camera = sfr_ecs_push_component(camera, tds_camera_controller());
}

void _scene_arena_update(SFR_Scene* scene, float delta_time) 
{
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_R))
  {
    sfr_ecs_load_scene(sfr_ecs_get_scene("Arena"));
  }

  TDS_Arena* arena = SFR_COMPONENT_CONVERT(TDS_Arena, scene);

  SFR_Transform* player_health_transform = SFR_COMPONENT_CONVERT(SFR_Transform, arena->player_health->owner->components[0]);
  SFR_Transform* player_transform = SFR_COMPONENT_CONVERT(SFR_Transform, arena->player->owner->components[0]);
  static vec2 offset = { -0.0f, 1.0f  };
  vec2 position;
  glm_vec2_add(player_transform->position, offset, position);
  glm_vec2_lerp(player_health_transform->position, position, 15.0f * delta_time, player_health_transform->position);
  // glm_vec2_add(player_transform->position, offset, player_health_transform->position);

  TDS_PlayerController* player_controller = SFR_COMPONENT_CONVERT(TDS_PlayerController, arena->player);
  if (player_controller->health < 1)
  {
    static bool print_message_once = false;

    if (!print_message_once)
    {
      TDS_EnemyHandler* enemy_handler = SFR_COMPONENT_CONVERT(TDS_EnemyHandler, arena->enemy_handler);
      printf("\n\nwave: %u\nkills: %u\nscore: %u\n\n", enemy_handler->wave, enemy_handler->kills, enemy_handler->score);

      print_message_once = true;
    }
  }

  SFR_Transform* cursor_transform = SFR_COMPONENT_CONVERT(SFR_Transform, arena->cursor->components[0]);
  sfr_input_get_mouse_position(cursor_transform->position, false);
}

void _scene_arena_create_background(SFR_Scene* scene) 
{
  vec2 centre = { TDS_ARENA_TILES_X, TDS_ARENA_TILES_Y };
  glm_vec2_scale(centre, 0.5f, centre);

  vec2 uv_size = {
    0.5f,
    1.0f
  };

  for (uint32_t y = 0; y < TDS_ARENA_TILES_Y; y++) 
  {
    for (uint32_t x = 0; x < TDS_ARENA_TILES_X; x++) 
    {
      SFR_Entity* tile = sfr_ecs_push_entity("tile", "tile");

      SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, tile->components[0]);
      vec3 offset = { x - centre[X], y - centre[Y], 0.0f };
      glm_vec3_scale(offset, 1.2f, offset);
      glm_vec3_add(transform->position, offset, transform->position);
      glm_vec2_copy((vec2) { 0.6f, 0.6f }, transform->scale);
      
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
  
  arena->player = tds_player_controller_create_instance();
  arena->enemy_handler = tds_enemy_handler_create_instance(arena->player->owner);

  SFR_Entity* player_health = sfr_ecs_push_entity("Player Health", "player health");
  player_health->layer = 200;
  arena->player_health = sfr_ecs_push_component(player_health, sfr_sprite_renderer());
  sfr_sprite_renderer_set_texture(arena->player_health, "player-health-bar");

  SFR_Transform* player_health_transform = SFR_COMPONENT_CONVERT(SFR_Transform, arena->player_health->owner->components[0]);
  glm_vec3_copy((vec3){ 0.6f, 0.6f, 0.0f }, player_health_transform->scale);

  arena->cursor = sfr_ecs_push_entity("Cursor", "cursor");
  arena->cursor->layer = 500;
  SFR_Component* sprite_renderer = sfr_ecs_push_component(arena->cursor, sfr_sprite_renderer());
  sfr_sprite_renderer_set_texture(sprite_renderer, "cursor");
  SFR_Transform* cursor_transform = SFR_COMPONENT_CONVERT(SFR_Transform, arena->cursor->components[0]);
  glm_vec3_copy((vec3){ 0.6f, 0.6f, 0.0f }, cursor_transform->scale);

  scene_arena_set_player_health(scene, 3);
}

void _scene_arena_load_assets(SFR_Scene* scene) 
{
  // loading the tile assest
  sfr_pipeline_push_texture(sfr_texture(
    "tile", "./art/tile-sprite-sheet.png", false, 4, false
  ));

  sfr_pipeline_push_texture(sfr_texture(
    "blood-splat", "./art/blood-splat.png", false, 4, false
  ));

  sfr_pipeline_push_texture(sfr_texture(
    "cursor", "./art/cursor.png", false, 4, false
  ));

  tds_player_controller_load_assets();
  tds_enemy_handler_load_assets();
}

void scene_arena_set_player_health(SFR_Scene* scene, uint32_t health)
{
  TDS_Arena* arena = SFR_COMPONENT_CONVERT(TDS_Arena, scene);

  float uv_size = 16.0f / (4.0f * 16.0f);
  vec2 uv_left;
  vec2 uv_right;

  switch (health)
  {
  case 0:
    glm_vec2_copy((vec2) { uv_size * 3.0f, 0.0f }, uv_left);
    glm_vec2_copy((vec2) { uv_size * 4.0f, 1.0f }, uv_right);
    break;

  case 1:
    glm_vec2_copy((vec2) { uv_size * 2.0f, 0.0f }, uv_left);
    glm_vec2_copy((vec2) { uv_size * 3.0f, 1.0f }, uv_right);
    break;
  
  case 2:
    glm_vec2_copy((vec2) { uv_size * 1.0f, 0.0f }, uv_left);
    glm_vec2_copy((vec2) { uv_size * 2.0f, 1.0f }, uv_right);
    break;

  case 3:
    glm_vec2_copy((vec2) { uv_size * 0.0f, 0.0f }, uv_left);
    glm_vec2_copy((vec2) { uv_size * 1.0f, 1.0f }, uv_right);
    break;
  }

  sfr_sprite_renderer_set_uv(arena->player_health, uv_left, uv_right);
}

SFR_Component* scene_arena_get_player(SFR_Scene* scene)
{
  TDS_Arena* arena = SFR_COMPONENT_CONVERT(TDS_Arena, scene);
  return arena->player;
}
