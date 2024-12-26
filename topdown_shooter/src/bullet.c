#include <topdown_shooter/bullet.h>

#include <topdown_shooter/enemy_controller.h>



void                                    _tds_bullet_update(SFR_Component* component, float delta_time);




SFR_Component* tds_bullet_create_instance(vec2 move_direction, const char* target_tag, float move_speed, int damage) 
{
  SFR_Entity* entity = sfr_ecs_push_entity(TDS_BULLET, "bullet");
  SFR_Component* controller = sfr_ecs_push_component(entity, tds_bullet(move_direction, target_tag, move_speed, damage));
  TDS_Bullet* bullet = SFR_COMPONENT_CONVERT(TDS_Bullet, controller);

  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, entity->components[0]);
  glm_vec3_copy((vec3) { 0.5f, 0.5f, 0.5f }, transform->scale);
  transform->rotation[W] =  sfr_look_at_target((vec2) { 0.0f, 0.0f }, move_direction);

  entity->layer = 100;
  SFR_Component* sprite_renderer = sfr_ecs_push_component(entity, sfr_sprite_renderer());
  sfr_sprite_renderer_set_texture(sprite_renderer, "bullet001");

  bullet->sprite_animator = sfr_ecs_push_component(entity, sfr_sprite_animator(entity));

  ivec2 shooting[] = { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 } };
  float shooting_time[] = { 0.05f, 0.05f, 0.05f, 0.05f, 0.05f };

  sfr_sprite_animator_slice(bullet->sprite_animator, (ivec2) { 16, 16 });
  sfr_sprite_animator_load_animation(bullet->sprite_animator, "shooting", shooting, shooting_time, 5);

  SFR_Component* collider2d = sfr_ecs_push_component(entity, sfr_collider2d(entity->components[0]));
  SFR_Collider2D* collider = SFR_COMPONENT_CONVERT(SFR_Collider2D, collider2d);
  collider->trigger = true;

  return controller;
}

SFR_Component* tds_bullet(vec2 move_direction, const char* target_tag, float move_speed, int damage) 
{
  SFR_Component* component = sfr_ecs_component(
    TDS_BULLET, _tds_bullet_update, NULL, NULL, NULL
  );

  component->data = (TDS_Bullet*)malloc(sizeof(TDS_Bullet));
  TDS_Bullet* bullet = SFR_COMPONENT_CONVERT(TDS_Bullet, component);

  bullet->move_speed = move_speed;
  bullet->damage = damage;
  bullet->target_tag = sfr_str(target_tag);
  bullet->lifetime = 0.0f;
  glm_vec2_copy(move_direction, bullet->direction);
  bullet->sprite_animator = NULL;
  bullet->does_damage = true;

  return component;
}

void _tds_bullet_update(SFR_Component* component, float delta_time)
{
  TDS_Bullet* bullet = SFR_COMPONENT_CONVERT(TDS_Bullet, component);  
  if (!sfr_timer_finished(&bullet->lifetime)) 
  {
    SFR_Component* target = NULL;
    bool colliding = sfr_collider2d_trigger_enter_tag(component, bullet->target_tag, &target);
    if (!colliding) 
    {
      vec2 direction;
      glm_vec2_copy(bullet->direction, direction);
      glm_vec2_scale(direction, bullet->move_speed * delta_time, direction);
      SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);
      glm_vec2_add(transform->position, direction, transform->position);

      return;
    }
    else 
    {
      tds_enemy_damage(target->owner, bullet->damage);
    }
  }

  // destroy entity because its it its target or life time ran out
  uint32_t index = sfr_ecs_entity_find_index_uuid(0, component->owner->uuid);
  if (index != UINT32_MAX) 
    sfr_ecs_erase_entity(index);
}