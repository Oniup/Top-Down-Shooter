#include "../include/topdown_shooter/enemy_controller.h"

#include "../include/topdown_shooter/player_controller.h"
#include "../include/topdown_shooter/enemy_handler.h"
#include "../scenes/scene_arena.h"

#include <topdown_shooter/utils.h>


void                                    _tds_enemy_controller_update(SFR_Component* component, float delta_time);

void                                    _tds_enemy_controller_behaviour_state(SFR_Component* component, float delta_time);

void                                    _tds_enemy_controller_movement_demon(SFR_Component* component, float delta_time);





SFR_Entity* tds_instantiate_enemy(vec2 spawn_pos, SFR_Component* enemy_handler, TDS_EnemyType type, SFR_Entity* player) 
{
  char* name = NULL;
  char* texture_name = NULL;

  switch (type) 
  {
  case TDS_ENEMY_TYPE_DEMON:
    name = sfr_str("enemy:demon");
    texture_name = sfr_str("demon");
    break;
  }  

  SFR_Entity* enemy = sfr_ecs_push_entity(name, "enemy");
  sfr_ecs_push_component(enemy, tds_enemy_controller(type, enemy_handler, player));

  enemy->layer = 10;

  SFR_Component* renderer = sfr_ecs_push_component(enemy, sfr_sprite_renderer());
  sfr_sprite_renderer_set_texture(renderer, texture_name);

  SFR_Component* animator = sfr_ecs_push_component(enemy, sfr_sprite_animator(enemy));
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

  SFR_Component* collider =  sfr_ecs_push_component(enemy, sfr_collider2d(enemy->components[0]));
  SFR_Collider2D* collider_data = SFR_COMPONENT_CONVERT(SFR_Collider2D, collider);
  collider_data->trigger = true;
  glm_vec2_add(collider_data->size, (vec2) { 0.1f, 0.1f, }, collider_data->size);

  collider = sfr_ecs_push_component(enemy, sfr_collider2d(enemy->components[0]));
  collider_data = SFR_COMPONENT_CONVERT(SFR_Collider2D, collider);
  collider_data->weight = 5.0f;

  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, enemy->components[0]);
  glm_vec2_add(transform->position, spawn_pos, transform->position);
  glm_vec2_copy((vec2){ 0.6f, 0.6f }, transform->scale);

  sfr_str_free(&name);
  sfr_str_free(&texture_name);

  return enemy;
}

SFR_Component* tds_enemy_controller(TDS_EnemyType type, SFR_Component* enemy_handler, SFR_Entity* player) 
{
  SFR_Component* component = sfr_ecs_component(
    TDS_ENEMY_CONTROLLER, _tds_enemy_controller_update, NULL, NULL
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
    controller->after_hit_timer = 0.0f;
    controller->movement_offset = rand() % 3;
    break;
  }

  controller->type = type;
  controller->player = player;
  controller->enemy_handler = enemy_handler;
  controller->flip = false;
  controller->spawn_blood = false;

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
      if (sfr_timer_finished(&controller->after_hit_timer))    
      {
        SFR_Component* animator = sfr_get_component(component, SFR_SPRITE_ANIMATOR);
        sfr_sprite_animator_start_animation(animator, "running");
        controller->state = TDS_ENEMY_STATE_ATTACKING;
      }
      break;
    }
    break;

  case TDS_ENEMY_STATE_ATTACKING:
    switch (controller->type)
    {
    case TDS_ENEMY_TYPE_DEMON:
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
      break;
    }
    break;
  }
}




void _tds_enemy_controller_movement_demon(SFR_Component* component, float delta_time)
{
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, component);
  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);
  bool colliding =  sfr_collider2d_trigger_enter_uuid(component, controller->player->uuid, NULL);
  if (colliding) 
  {
    SFR_Scene* scene = sfr_ecs_get_active_scene();
    tds_player_damage(scene_arena_get_player(scene), controller->damage);

    SFR_Component* animator = sfr_get_component(component, SFR_SPRITE_ANIMATOR);
    sfr_sprite_animator_start_animation(animator, "idle");

    controller->state = TDS_ENEMY_STATE_IDLE;
    controller->after_hit_timer = sfr_timer_start(1.0f);

    return;
  }

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
  }
  else if (direction[X] >= 0.0f && controller->flip)
  {
    transform->scale[X] = -transform->scale[X];
    controller->flip = false;
  }
}




void tds_enemy_damage(SFR_Entity* enemy, uint32_t damage) 
{
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, sfr_get_component(enemy->components[0], TDS_ENEMY_CONTROLLER));
  controller->health -= damage;

  if (controller->health < 1)
  {
    SFR_Component* animator = sfr_get_component(enemy->components[0], SFR_SPRITE_ANIMATOR);
    sfr_sprite_animator_start_animation(animator, "death");
    controller->state = TDS_ENEMY_STATE_DEATH;

    uint32_t length = sfr_str_length(SFR_COLLIDER2D);
    for (uint32_t i = 0; i < enemy->components_count; i++)
    {
      if (sfr_str_cmp_length(SFR_COLLIDER2D, enemy->components[i]->name, length))
      {
        tds_enemy_handler_add_kill(controller->enemy_handler, controller->kill_points);
        enemy->components[i]->active = false;
      }
    }

    return;
  }

  controller->spawn_blood = true;
}
