#include "../include/topdown_shooter/enemy_controller.h"

void _tds_enemy_controller_update(SFRcomponent_t* component, float delta_time);

SFRcomponent_t* tds_enemy_controller(TDSenemy_type_t type, SFRentity_t* player) {
  SFRcomponent_t* component = sfr_ecs_component(
    TDS_ENEMY_CONTROLLER, _tds_enemy_controller_update, NULL, NULL
  );

  component->data = (TDSenemy_controller_t*)malloc(sizeof(TDSenemy_controller_t));
  TDSenemy_controller_t* controller = SFR_COMPONENT_CONVERT(TDSenemy_controller_t, component);

  // creating different types of enemies with different stats
  switch (type) {
  case TDS_ENEMY_TYPE_BLOB:
    controller->health = 1;
    controller->damage = 1;
    controller->move_speed = 1.0f;
    break;
  }

  controller->type = type;
  controller->player = player;

  return component;
}

void _tds_enemy_controller_update(SFRcomponent_t* component, float delta_time) {
  TDSenemy_controller_t* controller = SFR_COMPONENT_CONVERT(TDSenemy_controller_t, component);
  SFRtransform_t* transform = SFR_COMPONENT_CONVERT(SFRtransform_t, component->owner->components[0]);

  bool colliding =  sfr_collider2d_trigger_enter_uuid(component, controller->player->uuid);
  if (colliding) {
    // this really isn't a clean and clear way to freeing the current entity or any entity really
    uint32_t index = sfr_ecs_entity_find_index_uuid(0, component->owner->uuid);
    static uint32_t count = 0;
    count++;
    if (index != UINT32_MAX) {
      sfr_ecs_erase_entity(index);
    }

    return;
  }

  // basic enemy move towards the player
  SFRtransform_t* player_transform = SFR_COMPONENT_CONVERT(SFRtransform_t, controller->player->components[0]);
  vec3 direction = { 0.0f, 0.0f, 0.0f };
  glm_vec3_sub(player_transform->position, transform->position, direction);
  glm_vec3_normalize(direction);
  glm_vec3_scale(direction, controller->move_speed * delta_time, direction);
  glm_vec3_add(transform->position, direction, transform->position);
}




SFRentity_t* tds_instantiate_enemy(vec2 spawn_pos, TDSenemy_type_t type, SFRentity_t* player) {
  char* name = NULL;

  switch (type) {
  case TDS_ENEMY_TYPE_BLOB:
    name = sfr_str("enemy:blob");
    break;
  }  

  SFRentity_t* enemy = sfr_ecs_push_entity(name, "enemy");
  sfr_ecs_push_component(enemy, tds_enemy_controller(type, player));

  SFRcomponent_t* renderer = sfr_ecs_push_component(enemy, sfr_sprite_renderer());
  sfr_sprite_renderer_set_texture(renderer, "enemy-blob");

  SFRcomponent_t* collider =  sfr_ecs_push_component(enemy, sfr_collider2d());
  SFRcollider2d_t* collider_data = SFR_COMPONENT_CONVERT(SFRcollider2d_t, collider);
  collider_data->trigger = true;
  glm_vec2_copy((vec2) { 1.1f, 1.1f }, collider_data->size);
  collider = sfr_ecs_push_component(enemy, sfr_collider2d());
  collider_data = SFR_COMPONENT_CONVERT(SFRcollider2d_t, collider);
  collider_data->weight = 5.0f;

  SFRtransform_t* transform = SFR_COMPONENT_CONVERT(SFRtransform_t, enemy->components[0]);
  glm_vec2_add(transform->position, spawn_pos, transform->position);

  return enemy;
}

void tds_enemy_damage(SFRcomponent_t* component, uint32_t damage) {

}
