#include "../include/safire/components.h"

#include "../include/safire/ecs.h"
#include "../include/safire/pipeline.h"

void _sfr_sprite_renderer_update(SFRcomponent_t* component, float delta_time);
void _sfr_sprite_renderer_free(SFRcomponent_t* component);

void _sfr_sprite_animator_update(SFRcomponent_t* component, float delta_time);
void _sfr_sprite_animator_free(SFRcomponent_t* component);

SFRcomponent_t*  sfr_sprite_renderer() {
  SFRcomponent_t* component = sfr_ecs_component(
    SFR_SPRITE_RENDERER, _sfr_sprite_renderer_update, NULL, _sfr_sprite_renderer_free
  );

  component->type = SFR_COMPONENT_TYPE_GRAPHICS;

  component->data = (SFRsprite_renderer_t*)malloc(sizeof(SFRsprite_renderer_t));
  SAFIRE_ASSERT(component->data, "[SAFIRE::COMPONENT_SPRITE_RENDERER] failed to assign memory to component for some reason");

  SFRsprite_renderer_t* renderer = ((SFRsprite_renderer_t*)component->data);
  glm_vec4_copy((vec4){ 1.0f, 1.0f, 1.0f, 0.0f }, renderer->over_lay_colour);
  renderer->shader = 0;
  renderer->texture = 0;
  renderer->sprite_animator = NULL;

  renderer->vertices = (SFRvertex_t*)malloc(sizeof(SFRvertex_t) * 2);

  // bottom left
  glm_vec3_copy((vec3){ 0.0f, 0.0f , 0.0f }, renderer->vertices[0].vertex);
  renderer->vertices[0].texture_id = 0;
  glm_vec2_copy((vec2){ 0.0f, 0.0f }, renderer->vertices[0].uv);

  // top right
  glm_vec3_copy((vec3){ 1.0f, 1.0f , 0.0f }, renderer->vertices[1].vertex);
  renderer->vertices[1].texture_id = 0;
  glm_vec2_copy((vec2){ 1.0f, 1.0f }, renderer->vertices[1].uv);

  return component;
}

SFRcomponent_t*  sfr_sprite_animator(SFRentity_t* entity) {
  // checks if there is a sprite renderer already attached to component
  SFRsprite_renderer_t* renderer = NULL;
  for (uint32_t i = 0; i < entity->components_count; i++) {
    if (sfr_str_cmp(entity->components[i]->name, SFR_SPRITE_RENDERER)) {
      renderer = ((SFRsprite_renderer_t*)entity->components[i]->data);
      break;
    }
  }

  // creating the sprite animator component
  if (renderer != NULL) {
    SFRcomponent_t* component = sfr_ecs_component(
      SFR_SPRITE_ANIMATOR, _sfr_sprite_animator_update, NULL, _sfr_sprite_animator_free
    );

    // sprite animator creation
    component->data = (SFRsprite_animator_t*)malloc(sizeof(SFRsprite_animator_t));
    SFRsprite_animator_t* animator = ((SFRsprite_animator_t*)component->data);
    animator->sprite_renderer = renderer;
    animator->animations = NULL;
    animator->animation_count = 0;
    animator->frame_size[X] = 0;
    animator->frame_size[Y] = 0;

    return component;
  } else {
    printf("[SAFIRE_WARNING::SPRITE_RENDERER] cannot add sprite animator if there is no sprite renderer attached to entity");
    return NULL;
  }
}

void _sfr_sprite_renderer_update(SFRcomponent_t* component, float delta_time) {
  SFRsprite_renderer_t* renderer = ((SFRsprite_renderer_t*)component->data);
  SAFIRE_ASSERT(renderer, "[SAFIRE::COMPONENT_SPRITE_UPDATE] for some reason the component data cannot be converted to sprite renderer");
}

void _sfr_sprite_renderer_free(SFRcomponent_t* component) {
  SFRsprite_renderer_t* renderer = ((SFRsprite_renderer_t*)component->data);
  SAFIRE_ASSERT(renderer, "[SAFIRE::COMPONENT_SPRITE_FREE] for some reason the component data cannot be converted to sprite renderer");

  free(renderer->vertices);
}

void _sfr_sprite_animator_update(SFRcomponent_t* component, float delta_time) {

}

void _sfr_sprite_animator_free(SFRcomponent_t* component) {

}
