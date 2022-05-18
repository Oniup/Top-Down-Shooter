#include "../include/topdown_shooter/enemy_controller.h"




void                                    _tds_enemy_controller_update(SFR_Component* component, float delta_time);




SFR_Component* tds_enemy_controller(TDS_EnemyType type, SFR_Entity* player) 
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
    controller->health = 3;
    controller->damage = 1;
    controller->move_speed = 3.0f;
    break;
  }

  controller->type = type;
  controller->player = player;
  controller->flip = false;

  return component;
}

void _tds_enemy_controller_update(SFR_Component* component, float delta_time) 
{
  TDS_EnemyController* controller = SFR_COMPONENT_CONVERT(TDS_EnemyController, component);
  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);

  bool colliding =  sfr_collider2d_trigger_enter_uuid(component, controller->player->uuid, NULL);
  if (colliding) 
  {
    // this really isn't a clean and clear way to freeing the current entity or any entity really
    uint32_t index = sfr_ecs_entity_find_index_uuid(0, component->owner->uuid);
    if (index != UINT32_MAX) 
      sfr_ecs_erase_entity(index);

    return;
  }

  // basic enemy move towards the player
  SFR_Transform* player_transform = SFR_COMPONENT_CONVERT(SFR_Transform, controller->player->components[0]);
  vec3 direction = { 0.0f, 0.0f, 0.0f };
  glm_vec3_sub(player_transform->position, transform->position, direction);
  glm_vec3_normalize(direction);
  glm_vec3_scale(direction, controller->move_speed * delta_time, direction);
  glm_vec3_add(transform->position, direction, transform->position);

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




SFR_Entity* tds_instantiate_enemy(vec2 spawn_pos, TDS_EnemyType type, SFR_Entity* player) 
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
  sfr_ecs_push_component(enemy, tds_enemy_controller(type, player));

  SFR_Component* renderer = sfr_ecs_push_component(enemy, sfr_sprite_renderer());
  sfr_sprite_renderer_set_texture(renderer, texture_name);

  SFR_Component* animator = sfr_ecs_push_component(enemy, sfr_sprite_animator(enemy));
  sfr_sprite_animator_slice(animator, (ivec2){ 16, 16 });

  ivec2 running[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f }, { 3.0f, 0.0f }, { 4.0f, 0.0f }, { 5.0f, 0.0f }, { 6.0f, 0.0f }};
  float running_time[] = { 0.1f,           0.1f,           0.1f,           0.1f,           0.1f,           0.1f,           0.1f };
  ivec2 idle[] = { { 7.0f, 0.0f }, { 8.0f, 0.0f }, { 9.0f, 0.0f }, { 10.0f, 0.0f }};
  float idle_time[] = { 0.3f,           0.3f,           0.3f,           0.3f };

  sfr_sprite_animator_load_animation(animator, "running", running, running_time, 7);
  sfr_sprite_animator_load_animation(animator, "idle", idle, idle_time, 4);

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

void tds_enemy_damage(SFR_Component* component, uint32_t damage) 
{

}
