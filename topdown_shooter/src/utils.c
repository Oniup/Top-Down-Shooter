#include <topdown_shooter/utils.h>

SFR_Entity* tds_instantiate_blood(vec3 position, int type)
{
  SFR_Entity* blood_splat = sfr_ecs_push_entity("blood-splat", "blood-splat");
  SFR_Component* sprite_renderer =  sfr_ecs_push_component(blood_splat, sfr_sprite_renderer());
  sfr_sprite_renderer_set_texture(sprite_renderer, "blood-splat");

  float uv_x = 16.0f / 32.0f;

  float uv_left;
  float uv_right;

  switch (type)
  {
  case 0:
    uv_left = uv_x * 0.0f;
    uv_right = uv_x * 1.0f;
    break;

  case 1:
    uv_left = uv_x * 1.0f;
    uv_right = uv_x * 2.0f;
    break;

  case 2:
    {
      int which = rand() % 2;
      if (which == 0)
      {
        uv_left = uv_x * 0.0f;
        uv_right = uv_x * 1.0f;
      }
      else 
      {
        uv_left = uv_x * 1.0f;
        uv_right = uv_x * 2.0f;
      }
    }
    break;
  }  

  sfr_sprite_renderer_set_uv(sprite_renderer, (vec2){ uv_left, 0.0f }, (vec2){ uv_right, 1.0f });

  SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, blood_splat->components[0]);
  glm_vec3_copy(position, transform->position);
  glm_vec3_copy((vec3){ 0.6f, 0.6f, 0.0f }, transform->scale);

  return blood_splat;
}
