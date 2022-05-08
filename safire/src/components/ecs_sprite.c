#include "../include/safire/components.h"

#include "../include/safire/ecs.h"
#include "../include/safire/pipeline.h"




void _sfr_sprite_renderer_update(SFRcomponent_t* component, float delta_time);
void _sfr_sprite_renderer_free(SFRcomponent_t* component);

void _sfr_sprite_renderer_translation(SFRcomponent_t* component, SFRvertex_t* vertices);




void _sfr_sprite_animator_update(SFRcomponent_t* component, float delta_time);
void _sfr_sprite_animator_free(SFRcomponent_t* component);




SFRcomponent_t*  sfr_sprite_renderer() {
  SFRcomponent_t* component = sfr_ecs_component(
    SFR_SPRITE_RENDERER, _sfr_sprite_renderer_update, NULL, _sfr_sprite_renderer_free
  );

  component->type = SFR_COMPONENT_TYPE_GRAPHICS;

  component->data = (SFRsprite_renderer_t*)malloc(sizeof(SFRsprite_renderer_t));
  SAFIRE_ASSERT(component->data, "[SAFIRE::COMPONENT_SPRITE_RENDERER] failed to assign memory to component for some reason");

  SFRsprite_renderer_t* renderer = sfr_component_convert(SFRsprite_renderer_t, component);

  SFRshader_t* target_shader = sfr_pipeline_get_target_shader("SAFIRE::default");
  SFRtexture_t* target_texture = sfr_pipeline_get_target_texture("SAFIRE::default");
  renderer->shader = target_shader->id;
  renderer->sprite_animator = NULL;

  // setting the vertices
  renderer->vertices = (SFRvertex_t*)malloc(sizeof(SFRvertex_t) * 4);
  SAFIRE_ASSERT(renderer->vertices, "[SAFIRE::COMPONENT_SPRITE_RENDERER] failed to assign memory to sprite renderere's vertices for some reason");
  // top right
  glm_vec3_copy((vec3){ 1.0f, 1.0f , 0.0f }, renderer->vertices[0].vertex);
  glm_vec2_copy((vec2){ 1.0f, 1.0f }, renderer->vertices[0].uv);
  // top left
  glm_vec3_copy((vec3){ -1.0f, 1.0f , 0.0f }, renderer->vertices[1].vertex);
  glm_vec2_copy((vec2){ 0.0f, 1.0f }, renderer->vertices[1].uv);
  // bottom left
  glm_vec3_copy((vec3){ -1.0f, -1.0f , 0.0f }, renderer->vertices[2].vertex);
  glm_vec2_copy((vec2){ 0.0f, 0.0f }, renderer->vertices[2].uv);
  // bottom right
  glm_vec3_copy((vec3){ 1.0f, -1.0f , 0.0f }, renderer->vertices[3].vertex);
  glm_vec2_copy((vec2){ 1.0f, 0.0f }, renderer->vertices[3].uv);

  for (uint32_t i = 0; i < 4; i++) {
    renderer->vertices[i].texture_id = target_texture->id;
    glm_vec4_copy((vec4){ 1.0f, 1.0f, 1.0f, 0.0f }, renderer->vertices[i].overlay_colour);
  }

  return component;
}

void _sfr_sprite_renderer_update(SFRcomponent_t* component, float delta_time) {
  SFRsprite_renderer_t* renderer = sfr_component_convert(SFRsprite_renderer_t, component);

  SFRvertex_t vertices[4] = { {}, {} };
  _sfr_sprite_renderer_translation(component, vertices);
  sfr_pipeline_push_vertices(vertices, 4, renderer->shader);
}




void sfr_sprite_renderer_set_texture(SFRcomponent_t* component, const char* name) {
  SFRtexture_t* target = sfr_pipeline_get_target_texture(name);
  if (target != NULL) {
    SFRsprite_renderer_t* renderer = sfr_component_convert(SFRsprite_renderer_t, component);
    for (uint32_t i = 0; i < 4; i++) {
      renderer->vertices[i].texture_id = (float)target->id;
    }
  }
}

void sfr_sprite_renderer_set_shader(SFRcomponent_t* component, const char* name) {
  SFRshader_t* target = sfr_pipeline_get_target_shader(name);
  if (target != NULL) {
    SFRsprite_renderer_t* renderer = sfr_component_convert(SFRsprite_renderer_t, component);
    renderer->shader = target->id;
  }
}

SFRtexture_t* sfr_sprite_renderer_get_texture(SFRcomponent_t* component) {
  SFRsprite_renderer_t* renderer = sfr_component_convert(SFRsprite_renderer_t, component);
  // TODO: ...
  return NULL;
}

SFRshader_t* sfr_sprite_renderer_get_shader(SFRcomponent_t* component) {
  // TODO: ...
  return NULL;
}




void _sfr_sprite_renderer_free(SFRcomponent_t* component) {
  SFRsprite_renderer_t* renderer = sfr_component_convert(SFRsprite_renderer_t, component);
  SAFIRE_ASSERT(renderer, "[SAFIRE::COMPONENT_SPRITE_FREE] for some reason the component data cannot be converted to sprite renderer");

  free(renderer->vertices);
}

void _sfr_sprite_renderer_translation(SFRcomponent_t* component, SFRvertex_t* vertices) {
  // getting the components
  SFRsprite_renderer_t* renderer = sfr_component_convert(SFRsprite_renderer_t, component);
  SFRtransform_t* transform = sfr_component_convert(SFRtransform_t, component->owner->components[0]);

  // setting the correct vertex data
  float angle = transform->rotation[W] - 0.78633229259f; // subtracting 45 degrees because every vertex needs to rotate that much to the left
  for (uint32_t i = 0; i < 4; i++) {
    sfr_vertex_copy(&vertices[i], &renderer->vertices[i]);
    
    // rotaing
    float radius = sqrtf(glm_vec3_dot(vertices[i].vertex, vertices[i].vertex));
    angle += 1.5708f;
    glm_vec3_copy(
      (vec3) {
        cosf(angle) * radius,
        sinf(angle) * radius,
        0.0f
      },
      vertices[i].vertex
    );

    // scaling
    glm_vec3_mul(vertices[i].vertex, transform->scale, vertices[i].vertex);
    
    // translating
    glm_vec3_add(vertices[i].vertex, transform->position, vertices[i].vertex);

    // TODO: other transform related calculations (scaling and rotating) ...
  }
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
    SFRsprite_animator_t* animator = sfr_component_convert(SFRsprite_animator_t, component);
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

void _sfr_sprite_animator_update(SFRcomponent_t* component, float delta_time) {

}

void _sfr_sprite_animator_free(SFRcomponent_t* component) {

}
