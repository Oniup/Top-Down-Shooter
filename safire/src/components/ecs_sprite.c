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
  
  SFRshader_t* target_shader = sfr_pipeline_get_target_shader("SAFIRE::default");
  SFRtexture_t* target_texture = sfr_pipeline_get_target_texture("SAFIRE::default");

  renderer->shader = target_shader->id;
  renderer->texture = target_texture->id;
  renderer->sprite_animator = NULL;

  renderer->vertices = (SFRvertex_t*)malloc(sizeof(SFRvertex_t) * 2);

  // bottom left
  glm_vec3_copy((vec3){ -1.0f, -1.0f , 0.0f }, renderer->vertices[0].vertex);
  renderer->vertices[0].texture_id = 0;
  glm_vec2_copy((vec2){ 0.0f, 0.0f }, renderer->vertices[0].uv);
  glm_vec4_copy((vec4){ 1.0f, 1.0f, 1.0f, 0.0f }, renderer->vertices[0].overlay_colour);

  // top right
  glm_vec3_copy((vec3){ 1.0f, 1.0f , 0.0f }, renderer->vertices[1].vertex);
  renderer->vertices[1].texture_id = 0;
  glm_vec2_copy((vec2){ 1.0f, 1.0f }, renderer->vertices[1].uv);
  glm_vec4_copy((vec4){ 1.0f, 1.0f, 1.0f, 0.0f }, renderer->vertices[1].overlay_colour);

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

  // the first component in an entity static will always be the transform, unless you change it which you shouldn't do
  SFRtransform_t* transform = ((SFRtransform_t*)component->owner->components[0]->data);
  SFRvertex_t vertices[2];
  for (uint32_t i = 0; i < 2; i++) {
    sfr_vertex_copy(&vertices[i], &renderer->vertices[i]);

    // so the centre of the quad is corrected to be the entities world coordinates
    glm_vec3_scale(vertices[i].vertex, 0.5f, vertices[i].vertex); 
    glm_vec3_add(transform->position, vertices[i].vertex, vertices[i].vertex);

    // printf("vertex %d: [%f, %f]\n", i + 1, vertices[i].vertex[X], vertices[i].vertex[Y]);

    // TODO: other transform related calculations (scaling and rotating) ...
  }

  sfr_pipeline_push_vertices(vertices, 2, renderer->shader);
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
