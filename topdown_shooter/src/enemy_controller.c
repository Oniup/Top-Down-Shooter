#include "../include/topdown_shooter/enemy_controller.h"

#include "../include/topdown_shooter/player_controller.h"
#include "../include/topdown_shooter/enemy_handler.h"
#include "../include/topdown_shooter/destroy_target.h"
#include "../scenes/scene_arena.h"

#include <topdown_shooter/utils.h>


void                                    _tds_enemy_controller_update(SFR_Component* component, float delta_time);

void                                    _tds_enemy_controller_behaviour_state(SFR_Component* component, float delta_time);

void                                    _tds_enemy_controller_idle_demon(SFR_Component* component);
void                                    _tds_enemy_controller_movement_demon(SFR_Component* component, float delta_time);

void                                    _tds_enemy_controller_idle_gigachad(SFR_Component* component);
void                                    _tds_enemy_controller_movement_gigachad(SFR_Component* component, float delta_time);





SFR_Entity* tds_instantiate_enemy(vec2 spawn_pos, SFR_Component* enemy_handler, TDS_EnemyType type, SFR_Entity* player) 
{
  char* name = NULL;
  char* texture_name = NULL;
  float weight = 0.0f;
  vec2 size;

  switch (type) 
  {
  case TDS_ENEMY_TYPE_DEMON:
  {
    name = sfr_str("enemy:demon");
    texture_name = sfr_str("demon");
    weight = 0.5f;
    glm_vec2_copy((vec2){ 0.6f, 0.6f }, size);
  }
    break;
  
  case TDS_ENEMY_TYPE_GIGACHAD:
  {
    name = sfr_str("enemy:gigachad");
    texture_name = sfr_str("gigachad");
    weight = 5.0f;
    glm_vec2_copy((vec2){ 1.2f, 1.2f }, size);
  }
    break;
  }  

  SFR_Entity* enemy = sfr_ecs_push_entity(name, "enemy");
  sfr_ecs_push_component(enemy, tds_enemy_controller(type, enemy_handler, player));

  enemy->layer = 10;

  SFR_Component* renderer = sfr_ecs_push_component(enemy, sfr_sprite_renderer());
  sfr_sprite_renderer_set_texture(renderer, texture_name);

  SFR_Component* collider = sfr_ecs_push_component(enemy, sfr_collider2d(enemy->components[0]));
  
  SFR_Collider2D* collider_data = SFR_COMPONENT_CONVERT(SFR_Collider2D, collider);
  collider_data->weight = weight;

  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, enemy->components[0]);
  glm_vec2_add(transform->position, spawn_pos, transform->position);
  glm_vec2_copy(size, transform->scale);

  sfr_str_free(&name);
  sfr_str_free(&texture_name);

  sfr_ecs_push_component(enemy, tds_destroy_target(60.0f)); // 3 min of having the dead body on the ground

  // setting the animations
  SFR_Component* animator = sfr_ecs_push_component(enemy, sfr_sprite_animator(enemy));

  switch (type)
  {
  case TDS_ENEMY_TYPE_DEMON:
  {
    sfr_sprite_animator_slice(animator, (ivec2){ 16, 16 });

    ivec2 running[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f }, { 3.0f, 0.0f }, { 4.0f, 0.0f }, { 5.0f, 0.0f }, { 6.0f, 0.0f }};
    float running_time[] = { 0.1f,           0.1f,           0.1f,           0.1f,           0.1f,           0.1f,           0.1f };
    ivec2 idle[] = { { 7.0f, 0.0f }, { 8.0f, 0.0f }, { 9.0f, 0.0f }, { 10.0f, 0.0f }};
    float idle_time[] = { 0.3f,           0.3f,           0.3f,           0.3f };
    ivec2 death[] = { { 11.0f, 0.0f }, { 12.0f, 0.0f }, { 13.0f, 0.0f }};
    float death_time[] = { 0.1f,           0.1f,     SFR_SPRITE_ANIMATOR_STOP };

    sfr_sprite_animator_load_animation(animator, "running", running, running_time, 7);
    sfr_sprite_animator_load_animation(animator, "idle", idle, idle_time, 4);
    sfr_sprite_animator_load_animation(animator, "death", death, death_time, 3);
  } 
    break;
  case TDS_ENEMY_TYPE_GIGACHAD:
  {
    sfr_sprite_animator_slice(animator, (ivec2){ 32, 32 });

    ivec2 running[] = { { 6, 0 }, { 7, 0 }, { 8, 0 }, { 9, 0 }, { 10, 0 }, { 11, 0 } };
    float running_time[] = { 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f };
    ivec2 idle[] = { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 } };
    float idle_time[] = { 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f };
    ivec2 death[] = { { 23, 0 }, { 24, 0 }, { 25, 0 }, { 26, 0 } };
    float death_time[] = { 0.1f,  0.1f, 0.1f, SFR_SPRITE_ANIMATOR_STOP };
    ivec2 attack[] = { 
      { 12, 0 }, { 13, 0 }, { 14, 0 }, { 15, 0 }, { 16, 0 }, { 17, 0 },
      { 18, 0 }, { 19, 0 }, { 20, 0 }, { 21, 0 }, { 22, 0 }
    };
    float attack_time[] = { 
      0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 
      0.1f, 0.1f, 0.1f, 0.1f, 0.1f,  
    };

    sfr_sprite_animator_load_animation(animator, "running", running, running_time, 6);
    sfr_sprite_animator_load_animation(animator, "idle", idle, idle_time, 6);
    sfr_sprite_animator_load_animation(animator, "death", death, death_time, 4);
    sfr_sprite_animator_load_animation(animator, "attack", attack, attack_time, 11);
  }
    break;
  }

  return enemy;
}

SFR_Component* tds_enemy_controller(TDS_EnemyType type, SFR_Component* enemy_handler, SFR_Entity* player) 
{
  SFR_Component* component = sfr_ecs_component(
    TDS_ENEMY_CONTROLLER, _tds_enemy_controller_update, NULL, NULL, NULL
  );

  component->data = (TDS_EnemyController*)malloc(sizeof(TDS_EnemyController));
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, component);

  // creating different types of enemies with different stats
  switch (type) 
  {
  case TDS_ENEMY_TYPE_DEMON:
    controller->health = 2;
    controller->damage = 1;
    controller->move_speed = 3.0f;
    controller->kill_points = 1;
    controller->state = TDS_ENEMY_STATE_ATTACKING;
    controller->movement_offset = rand() % 3;
    break;

  case TDS_ENEMY_TYPE_GIGACHAD:
    controller->health = 10;
    controller->damage = 2;
    controller->move_speed = 2.0f;
    controller->kill_points = 5;
    controller->state = TDS_ENEMY_STATE_ATTACKING;
    controller->movement_offset = 0;
    break;
  }

  controller->type = type;
  controller->player = player;
  controller->enemy_handler = enemy_handler;
  controller->flip = false;
  controller->spawn_blood = false;
  controller->after_hit_timer = 0.0f;

  return component;
}

void _tds_enemy_controller_update(SFR_Component* component, float delta_time) 
{
  _tds_enemy_controller_behaviour_state(component, delta_time);
}




void _tds_enemy_controller_behaviour_state(SFR_Component* component, float delta_time)
{
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, component);

  if (controller->spawn_blood)
  {
    SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);
    tds_instantiate_blood(transform->position, 2);

    controller->spawn_blood = false;
  }

  switch (controller->state)
  {
  case TDS_ENEMY_STATE_IDLE:
    switch (controller->type)
    {
    case TDS_ENEMY_TYPE_DEMON:
      _tds_enemy_controller_idle_demon(component);
      break;

    case TDS_ENEMY_TYPE_GIGACHAD:
      // _tds_enemy_controller_idle_gigachad(component);
      _tds_enemy_controller_idle_demon(component);
      break;
    }
    break;

  case TDS_ENEMY_STATE_ATTACKING:
    switch (controller->type)
    {
    case TDS_ENEMY_TYPE_DEMON:
      _tds_enemy_controller_movement_demon(component, delta_time);
      break;

    case TDS_ENEMY_TYPE_GIGACHAD:
      // _tds_enemy_controller_movement_gigachad(component, delta_time);
      _tds_enemy_controller_movement_demon(component, delta_time);

      break;
    }
    break;

  case TDS_ENEMY_STATE_DEATH:
    switch (controller->type)
    {
    case TDS_ENEMY_TYPE_DEMON:
      // TODO: ...
      return;

    case TDS_ENEMY_TYPE_GIGACHAD:
      // TODO: ...
      return;
    }
    break;
  }
}




/* =========================================== DEMON ============================================ */

void _tds_enemy_controller_idle_demon(SFR_Component* component)
{
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, component);

  if (sfr_timer_finished(&controller->after_hit_timer))
    tds_enemy_change_state(component, TDS_ENEMY_STATE_ATTACKING);
}

void _tds_enemy_controller_movement_demon(SFR_Component* component, float delta_time)
{
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, component);
  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);

  // basic enemy move towards the player
  SFR_Transform* player_transform = SFR_COMPONENT_CONVERT(SFR_Transform, controller->player->components[0]);
  vec3 direction = { 0.0f, 0.0f, 0.0f };
  glm_vec3_sub(player_transform->position, transform->position, direction);


  vec3 offset = { 0.0f, 0.0f, 0.0f };
  switch (controller->movement_offset)
  {
case 0:
    break;

  case 1:
    glm_vec3_cross(direction, (vec3){ 0.0f, 0.0f, 1.0f }, offset);
    glm_vec3_normalize(offset);
    break;

  case 2:
    glm_vec3_cross(direction, (vec3){ 0.0f, 0.0f, 1.0f }, offset);
    glm_vec3_normalize(offset);
    glm_vec3_scale(offset, -1.0f, offset);
    break;
  }
  offset[Z] = 0.0f;
  
  glm_vec3_normalize(direction);
  glm_vec3_add(direction, offset, direction);
  float mag = sqrtf(glm_vec2_dot(direction, direction));
  glm_vec2_divs(direction, mag, direction);

  glm_vec3_scale(direction, controller->move_speed * delta_time, direction);
  glm_vec2_add(transform->position, direction, transform->position);

  if (direction[X] < 0.0f && !controller->flip)
  {
    transform->scale[X] = -transform->scale[X];
    controller->flip = true;
    printf("flip\n");
  }
  else if (direction[X] >= 0.0f && controller->flip)
  {
    transform->scale[X] = -transform->scale[X];
    controller->flip = false;
  }
}




/* ========================================== GIGACHAD ========================================== */

void _tds_enemy_controller_idle_gigachad(SFR_Component* component)
{
  // TODO: ...
}

void _tds_enemy_controller_movement_gigachad(SFR_Component* component, float delta_time)
{
  // TODO: ...
}




/* ======================================== RELATED TO ALL ====================================== */

void tds_enemy_damage(SFR_Entity* enemy, uint32_t damage) 
{
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, sfr_get_component(enemy->components[0], TDS_ENEMY_CONTROLLER));
  controller->health -= damage;

  if (controller->health < 1)
  {
    tds_enemy_change_state(enemy->components[0], TDS_ENEMY_STATE_DEATH);

    return;
  }

  controller->spawn_blood = true;
}

void tds_enemy_change_state(SFR_Component* component, TDS_EnemyState state)
{
  component = sfr_get_component(component, TDS_ENEMY_CONTROLLER);
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, component);

  switch (state)
  {
  case TDS_ENEMY_STATE_IDLE:
  {
    switch (controller->type)
    {
    case TDS_ENEMY_TYPE_DEMON:
    {
      SFR_Scene* scene = sfr_ecs_get_active_scene();
      tds_player_damage(scene_arena_get_player(scene), controller->damage);

      SFR_Component* animator = sfr_get_component(component, SFR_SPRITE_ANIMATOR);
      sfr_sprite_animator_start_animation(animator, "idle");

      controller->after_hit_timer = sfr_timer_start(1.0f);
    }
      break;
    case TDS_ENEMY_TYPE_GIGACHAD:
    {
      // TODO: ...
      SFR_Scene* scene = sfr_ecs_get_active_scene();
      tds_player_damage(scene_arena_get_player(scene), controller->damage);

      SFR_Component* animator = sfr_get_component(component, SFR_SPRITE_ANIMATOR);
      sfr_sprite_animator_start_animation(animator, "idle");

      controller->after_hit_timer = sfr_timer_start(1.0f);
    }
    break;
    }
  }
  break;

  case TDS_ENEMY_STATE_ATTACKING:
  {
    switch (controller->type)
    {
    case TDS_ENEMY_TYPE_DEMON:
    {
      SFR_Component* animator = sfr_get_component(component, SFR_SPRITE_ANIMATOR);
      sfr_sprite_animator_start_animation(animator, "running");
    }
      break;
    case TDS_ENEMY_TYPE_GIGACHAD:
    {
      // TODO: ...
    }
      break;
    }
    break;
  }
  break;

  case TDS_ENEMY_STATE_DEATH:
  {
    SFR_Component* animator = sfr_get_component(component, SFR_SPRITE_ANIMATOR);
    sfr_sprite_animator_start_animation(animator, "death");
    controller->state = TDS_ENEMY_STATE_DEATH;

    SFR_Component* destroy_target_comp = sfr_get_component(animator, TDS_DESTROY_TARGET);
    tds_destroy_target_start(destroy_target_comp);

    // deactivating the circle colliders attached to the enemy, because there isn't really a good way to remove them and can't be bothered
    uint32_t length = sfr_str_length(SFR_COLLIDER2D);
    for (uint32_t i = 0; i < component->owner->components_count; i++)
    {
      if (sfr_str_cmp_length(SFR_COLLIDER2D, component->owner->components[i]->name, length))
        component->owner->components[i]->active = false;
    }

    tds_enemy_handler_add_kill(controller->enemy_handler, controller->kill_points);
  }
  break;
  }

  controller->state = state;
}
