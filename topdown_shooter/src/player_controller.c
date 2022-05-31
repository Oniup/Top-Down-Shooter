#include "../include/topdown_shooter/player_controller.h"

#include "../include/topdown_shooter/bullet.h"
#include "../include/topdown_shooter/enemy_controller.h"
#include "../include/topdown_shooter/dash.h"

#include "../scenes/scene_arena.h"

#include "../include/topdown_shooter/utils.h"




void                                    _tds_player_controller_update(SFR_Component* component, float delta_time);

void                                    _tds_player_controller_change_gun_sprite(SFR_Component* component, bool* has_shot);

void                                    _tds_player_death_update(SFR_Component* component);
void                                    _tds_player_alive_update(SFR_Component* component, float delta_time);




SFR_Component* tds_player_controller_create_instance() 
{
  SFR_Entity* player = sfr_ecs_push_entity("Player", "player");

  player->layer = 20;

  SFR_Component* player_controller = sfr_ecs_push_component(player, tds_player_controller());

  SFR_Component* renderer = sfr_ecs_push_component(player, sfr_sprite_renderer());
  SFR_Component* animator = sfr_ecs_push_component(player, sfr_sprite_animator(player));

  sfr_sprite_renderer_set_texture(renderer, "player");

  ivec2 idle_animation[] = { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 0 } };
  float idle_time[]      = {   0.5f,     1.0f,     0.5f,     1.0f };
  ivec2 run_animation[] = { { 3, 0 }, { 4, 0 }, { 5, 0 }, { 6, 0 }, { 7, 0 } };
  float run_time[]      = {   0.1f,     0.1f,     0.1f,     0.1f,     0.1f   };
  ivec2 death_animation[] = { { 8, 0 }, { 9, 0 }, { 10, 0 },     { 11, 0 } };
  float death_time[]      = {   0.1f,     0.1f,      0.1f, SFR_SPRITE_ANIMATOR_STOP };

  sfr_sprite_animator_slice(animator, (ivec2) { 16, 16 });
  sfr_sprite_animator_load_animation(animator, "idle", idle_animation, idle_time, 4);
  sfr_sprite_animator_load_animation(animator, "run", run_animation, run_time, 5);
  sfr_sprite_animator_load_animation(animator, "death", death_animation, death_time, 4);

  SFR_Component* player_collider = sfr_ecs_push_component(player, sfr_collider2d(player->components[0]));  
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, player_collider);
  collider->trigger = true;

  TDS_PlayerController* controller = SFR_COMPONENT_CONVERT(TDS_PlayerController, player_controller);

  controller->dash = sfr_ecs_push_component(player, tds_dash(1.0f, 8.0f));


  
  SFR_Entity* player_gun = sfr_ecs_push_entity("Player Gun", "gun");

  player_gun->layer = 30;

  // setting up the gun
  SFR_Component* gun_sprite_renderer = sfr_ecs_push_component(player_gun, sfr_sprite_renderer());

  sfr_sprite_renderer_set_texture(gun_sprite_renderer, "player_gun");
  vec2 uv_size = { 0.5f, 1.0f };
  sfr_sprite_renderer_set_uv(
    gun_sprite_renderer, 
    (vec2) {
      uv_size[X] * 0.0f,
      uv_size[Y] * 0.0f,
    },
    (vec2) {
      uv_size[X] * 1.0f,
      uv_size[Y] * 1.0f,
    }
  );

  controller->gun = player_gun;

  SFR_Transform* player_transform = SFR_COMPONENT_CONVERT(SFR_Transform, player->components[0]);
  SFR_Transform* gun_transform = SFR_COMPONENT_CONVERT(SFR_Transform, player_gun->components[0]);
  glm_vec2_copy((vec2) { 0.6f, 0.6f }, player_transform->scale);
  glm_vec2_copy((vec2) { 0.6f, 0.6f }, gun_transform->scale);

  return player_controller;
}

void tds_player_controller_load_assets() 
{
  sfr_pipeline_push_texture(sfr_texture(
    "player", "./art/player-sprite-sheet.png", true, 4, false
  ));
  
  sfr_pipeline_push_texture(sfr_texture(
    "player_gun", "./art/gun-sprite-sheet.png", true, 4, false
  ));
  
  sfr_pipeline_push_texture(sfr_texture(
    "bullet001", "./art/bullet001-sprite-sheet.png", false, 4, false
  ));

  sfr_pipeline_push_texture(sfr_texture(
    "player-health-bar", "./art/health-bar-sprite-sheet.png", true, 4, false
  ));
}

SFR_Component* tds_player_controller() 
{
  SFR_Component* component = sfr_ecs_component(
    TDS_PLAYER_CONTROLLER, _tds_player_controller_update, NULL, NULL, NULL
  );

  component->data = (TDS_PlayerController*)malloc(sizeof(TDS_PlayerController));
  TDS_PlayerController* player_controller = SFR_COMPONENT_CONVERT(TDS_PlayerController, component);

  player_controller->damage = 1;
  player_controller->health = 3;
  player_controller->move_speed = 4.0f;
  player_controller->gun = NULL;
  player_controller->time_btw_bullets = 0.3f;

  return component;
}

void tds_player_damage(SFR_Component* player_controller_comp, int damage) 
{
  static SFR_Timer timer = 0.0f;
  if (sfr_timer_finished(&timer))
  {    
    TDS_PlayerController* controller = SFR_COMPONENT_CONVERT(TDS_PlayerController, player_controller_comp);
    SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, player_controller_comp->owner->components[0]);

    if (damage > 0.0f)
      tds_instantiate_blood(transform->position, 2);

    if (controller->health > 0)    
    {
      controller->health -= damage;    
      scene_arena_set_player_health(sfr_ecs_get_active_scene(), controller->health);

      if (controller->health < 1)
      {
        SFR_Component* animator = sfr_get_component(player_controller_comp, SFR_SPRITE_ANIMATOR);
        sfr_sprite_animator_start_animation(animator, "death");

        uint32_t index = sfr_ecs_entity_find_index_uuid(0, controller->gun->uuid);
        sfr_ecs_erase_entity(index);
        controller->gun = NULL;

        uint32_t count = 0;
        SFR_Entity** enemies = sfr_ecs_find_list_entities("enemy", &count);
        for (uint32_t i = 0; i < count; i++)
        {
          tds_enemy_damage(enemies[i], 1000);
        }

        free(enemies);
      }

      timer = sfr_timer_start(2.0f);
    }
    else
      timer = sfr_timer_start(FLT_MAX);
  }
}

void _tds_player_controller_update(SFR_Component* component, float delta_time) 
{
  TDS_PlayerController* controller = ((TDS_PlayerController*)component->data);

  if (controller->health < 1)
    _tds_player_death_update(component);
  else
    _tds_player_alive_update(component, delta_time);
}

void _tds_player_controller_change_gun_sprite(SFR_Component* component, bool* has_shot)
{
  TDS_PlayerController* controller = SFR_COMPONENT_CONVERT(TDS_PlayerController, component);

  if (*has_shot) 
  {
    SFR_Component* gun_renderer = controller->gun->components[1];    
    
    vec2 uv_size = { 0.5f, 1.0f };
    sfr_sprite_renderer_set_uv(
      gun_renderer, 
      (vec2) {
        uv_size[X] * 1.0f,
        uv_size[Y] * 0.0f,
      },
      (vec2) {
        uv_size[X] * 2.0f,
        uv_size[Y] * 1.0f,
      }
    );

    controller->shooting_animation_time = sfr_timer_start(0.05f);
    *has_shot = false;
  }

  if (sfr_timer_finished(&controller->shooting_animation_time))
  {
    SFR_Component* gun_renderer = controller->gun->components[1];

    vec2 uv_size = { 0.5f, 1.0f };
    sfr_sprite_renderer_set_uv(
      gun_renderer, 
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

void _tds_player_death_update(SFR_Component* component)
{

}

void _tds_player_alive_update(SFR_Component* component, float delta_time)
{
  TDS_PlayerController* controller = SFR_COMPONENT_CONVERT(TDS_PlayerController, component);

  uint32_t animation = 0;
  static uint32_t last_animation = 0;
  // movement controller

  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);
  TDS_Dash* dash = SFR_COMPONENT_CONVERT(TDS_Dash, controller->dash);

  static vec3 offset_gun = {
    0.0f, 0.1f, 0.0f
  };


  vec3 direction = { 0, 0, 0 };
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_A) && transform->position[X] > -11.2f) 
    direction[X] = -1;
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_D) && transform->position[X] < 11.2f) 
    direction[X] = 1;
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_S) && transform->position[Y] > -11.2f) 
    direction[Y] = -1;
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_W) && transform->position[Y] < 11.2f) 
    direction[Y] = 1;

  if (direction[X] != 0 || direction[Y] != 0) 
  {
    static bool flipped_x = false;
    if (direction[X] < -0.0f && !flipped_x) 
    {
      transform->scale[X] = -transform->scale[X];
      offset_gun[X] = -offset_gun[X];
      flipped_x = true;
    } 
    else if (direction[X] > 0.0f && flipped_x) 
    {
      transform->scale[X] = -transform->scale[X];
      offset_gun[X] = -offset_gun[X];
      flipped_x = false;
    }
    
    float mag = sqrtf(glm_vec2_dot(direction, direction));
    glm_vec2_divs(direction, mag, direction);

    if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_LEFT_SHIFT))
    {

    }

    glm_vec2_scale(direction, controller->move_speed * delta_time, direction);
    glm_vec2_add(direction, transform->position, transform->position);

    animation = 1;
  }

  if (animation != last_animation) 
  {
    SFR_Component* animator = sfr_get_component(component, SFR_SPRITE_ANIMATOR);

    sfr_sprite_animator_start_animation_index(animator, animation);
    last_animation = animation;
  }

  // gun controller

  vec2 mouse_pos;
  sfr_input_get_mouse_position(mouse_pos, false);

  SFR_Transform* gun_transform = SFR_COMPONENT_CONVERT(SFR_Transform, controller->gun->components[0]);
  glm_vec3_copy(transform->position, gun_transform->position);
  glm_vec3_sub(gun_transform->position, offset_gun, gun_transform->position);  

  static bool flip = false;
  gun_transform->rotation[W] = sfr_look_at_target_flip(gun_transform, mouse_pos, &flip);

  // shooting the gun
  static SFR_Timer time_btw_bullets = 0.0f;
  static bool has_shot = false;
  if (sfr_timer_finished(&time_btw_bullets) && sfr_input_mouse(SFR_INPUT_PRESS, SFR_MOUSE_BUTTON_LEFT)) 
  {
    vec2 direction;
    glm_vec2_sub(mouse_pos, gun_transform->position, direction);
    glm_vec2_normalize(direction);

    SFR_Component* bullet = tds_bullet_create_instance(direction, "enemy", 20.0f, controller->damage);
    SFR_Transform* bullet_transform = SFR_COMPONENT_CONVERT(SFR_Transform, bullet->owner->components[0]);
    glm_vec3_copy(gun_transform->position, bullet_transform->position);
    
    TDS_Bullet* bullet_controller = SFR_COMPONENT_CONVERT(TDS_Bullet, bullet);
    bullet_controller->lifetime = sfr_timer_start(1.0f);
    
    time_btw_bullets = sfr_timer_start(controller->time_btw_bullets);
    has_shot = true;
  }

  _tds_player_controller_change_gun_sprite(component, &has_shot);



  // checking if any enemies are in range
  SFR_Component* enemy = NULL;
  bool colliding = sfr_collider2d_trigger_enter_tag(component, "enemy", &enemy);
  if (colliding)
    tds_enemy_change_state(enemy, TDS_ENEMY_STATE_IDLE);
}

