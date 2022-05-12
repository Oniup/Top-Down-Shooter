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

  SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);

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
  SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);

  SFRvertex_t vertices[4] = { {}, {} };
  _sfr_sprite_renderer_translation(component, vertices);
  sfr_pipeline_push_vertices(vertices, 4, renderer->shader);
}




void sfr_sprite_renderer_set_uv(SFRcomponent_t* component, vec2 uv_bottom_left, vec2 uv_top_right) {
  SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);

  // top right
  glm_vec2_copy(
    uv_top_right, 
    renderer->vertices[0].uv
  );
  // top left
  glm_vec2_copy(
    (vec2) { uv_bottom_left[X], uv_top_right[Y] }, 
    renderer->vertices[1].uv
  );
  // bottom left
  glm_vec2_copy(
    uv_bottom_left, 
    renderer->vertices[2].uv
  );
  // bottom right
  glm_vec2_copy(
    (vec2) { uv_top_right[X], uv_bottom_left[Y] }, 
    renderer->vertices[3].uv
  );
}

SFRvertex_t* sfr_sprite_renderer_get_vertices(SFRcomponent_t* component, uint32_t* count) {
  SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);

  if (count != NULL) {
    *count = 4;
  }
  SFRvertex_t* vertices = (SFRvertex_t*)malloc(sizeof(SFRvertex_t) * 4);
  memcpy(vertices, renderer->vertices, sizeof(SFRvertex_t) * 4);
  
  return vertices;
}

void sfr_sprite_renderer_set_texture(SFRcomponent_t* component, const char* name) {
  SFRtexture_t* target = sfr_pipeline_get_target_texture(name);
  if (target != NULL) {
    SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);
    for (uint32_t i = 0; i < 4; i++) {
      renderer->vertices[i].texture_id = (float)target->id;
    }
  }
}

void sfr_sprite_renderer_set_shader(SFRcomponent_t* component, const char* name) {
  SFRshader_t* target = sfr_pipeline_get_target_shader(name);
  if (target != NULL) {
    SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);
    renderer->shader = target->id;
  }
}

SFRtexture_t* sfr_sprite_renderer_get_texture(SFRcomponent_t* component) {
  SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);
  
  uint32_t texture_id = renderer->vertices[0].texture_id;

  uint32_t textures_cout = 0;
  SFRtexture_t** textures = sfr_pipeline_get_textures(&textures_cout);

  for (uint32_t i = 0; i < textures_cout; i++) {
    if (textures[i]->id == texture_id) {
      return textures[i];
    }
  }

  printf("[SAFIRE::SPRITE_RENDERER_GET_TEXTURE] failed to find the texture with GPU id %u\n", texture_id);
  return NULL;
}

SFRshader_t* sfr_sprite_renderer_get_shader(SFRcomponent_t* component) {
  // TODO: ...
  return NULL;
}




void _sfr_sprite_renderer_free(SFRcomponent_t* component) {
  SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);
  SAFIRE_ASSERT(renderer, "[SAFIRE::COMPONENT_SPRITE_FREE] for some reason the component data cannot be converted to sprite renderer");

  free(renderer->vertices);
}

void _sfr_sprite_renderer_translation(SFRcomponent_t* component, SFRvertex_t* vertices) {
  // getting the components
  SFRsprite_renderer_t* renderer = SFR_COMPONENT_CONVERT(SFRsprite_renderer_t, component);
  SFRtransform_t* transform = SFR_COMPONENT_CONVERT(SFRtransform_t, component->owner->components[0]);

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




SFRcomponent_t* sfr_sprite_animator(SFRentity_t* entity) {
  // checks if there is a sprite renderer already attached to component
  SFRcomponent_t* renderer = NULL;
  for (uint32_t i = 0; i < entity->components_count; i++) {
    if (sfr_str_cmp(entity->components[i]->name, SFR_SPRITE_RENDERER)) {
      renderer = entity->components[i];
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
    SFRsprite_animator_t* animator = SFR_COMPONENT_CONVERT(SFRsprite_animator_t, component);
    animator->sprite_renderer = renderer;
    animator->animations = NULL;
    animator->animations_count = 0;
    animator->slices_size[X] = 0;
    animator->slices_size[Y] = 0;
    animator->slices_count[X] = 0;
    animator->slices_count[Y] = 0;
    animator->active_animation = 0;
    animator->current_active_frame = 0;
    animator->frame_timer = 0.0f;

    return component;
  } else {
    printf("[SAFIRE_WARNING::SPRITE_RENDERER] cannot add sprite animator if there is no sprite renderer attached to entity");
    return NULL;
  }
}

void sfr_sprite_animator_slice(SFRcomponent_t* component, ivec2 slice_size) {
  SFRsprite_animator_t* animator = SFR_COMPONENT_CONVERT(SFRsprite_animator_t, component);

  SFRtexture_t* texture =  sfr_sprite_renderer_get_texture(animator->sprite_renderer);

  animator->slices_size[X] = slice_size[X];
  animator->slices_size[Y] = slice_size[Y];
  animator->slices_count[X] = texture->width / slice_size[X];
  animator->slices_count[Y] = texture->height / slice_size[Y];

  if (animator->animations != NULL) {
    free(animator->animations);
    animator->animations = NULL;
    animator->animations_count = 0;
  }
}

void sfr_sprite_animator_load_animation(SFRcomponent_t* component, const char* name, ivec2 frames[], float* time_btw_frames, uint32_t frame_count) {
  SFRsprite_animator_t* animator = SFR_COMPONENT_CONVERT(SFRsprite_animator_t, component);

  // validation
  for (uint32_t i = 0; i < frame_count; i++) {
    if (frames[i][X] < animator->slices_count[X] && frames[i][X] > -1) {
      if (frames[i][Y] < animator->slices_count[Y] && frames[i][Y] > -1) {
        continue;
      }
    }

    printf("[SAFIRE::SPRITE_ANIMATOR_LOAD_ANIMATION] failed to load animation as the frame [%d, %d] in sprite sheet doesn't exist\n", frames[i][X], frames[i][Y]);
    return;
  }

  // increasing animation buffer size
  animator->animations_count++;
  if (animator->animations != NULL) {
    animator->animations = (SFRsprite_animation_t*)realloc(animator->animations, sizeof(SFRsprite_animation_t) * animator->animations_count);
    SAFIRE_ASSERT(animator->animations, "[SAFIRE::SPRITE_ANIMATOR_LOAD_ANIMATION] failed to resize memory to buffer for some reason... ");
  } else {
    animator->animations = (SFRsprite_animation_t*)malloc(sizeof(SFRsprite_animation_t) * animator->animations_count);
    SAFIRE_ASSERT(animator->animations, "[SAFIRE::SPRITE_ANIMATOR_LOAD_ANIMATION] failed to assign memory to buffer for some reason... ");
  }

  // setting the animation data
  SFRsprite_animation_t* animation =  &animator->animations[animator->animations_count - 1];
  animation->name = sfr_str(name);

  animation->frames = (ivec2*)malloc(sizeof(ivec2) * frame_count);
  SAFIRE_ASSERT(animation->frames, "[SAFIRE::SPRITE_ANIMATOR_LOAD_ANIMATION] failed to assign memory to buffer for some reason... ");
  animation->time_btw_frames = (float*)malloc(sizeof(float) * frame_count);
  SAFIRE_ASSERT(animation->time_btw_frames, "[SAFIRE::SPRITE_ANIMATOR_LOAD_ANIMATION] failed to assign memory to buffer for some reason... ");

  memcpy(animation->frames, frames, sizeof(ivec2) * frame_count);
  memcpy(animation->time_btw_frames, time_btw_frames, sizeof(float) * frame_count);

  animation->frame_count = frame_count;
}

void sfr_sprite_animator_start_animation(SFRcomponent_t* component, const char* name) {
  SFRsprite_animator_t* animator = SFR_COMPONENT_CONVERT(SFRsprite_animator_t, component);

  uint32_t length = sfr_str_length(name);
  for (uint32_t i = 0; i < animator->animations_count; i++) {
    if (sfr_str_cmp_length(animator->animations[i].name, name, length)) {
      sfr_sprite_animator_start_animation_index(component, i);
      return;
    }
  }

  printf("[SAFIRE::SPRITE_ANIMATOR_START_ANIMATION] failed to find %s's animation because it doesn't exist doesn't exist\n", name);
}

void sfr_sprite_animator_start_animation_index(SFRcomponent_t* component, uint32_t anim_index) {
  SFRsprite_animator_t* animator = SFR_COMPONENT_CONVERT(SFRsprite_animator_t, component);

  animator->active_animation = anim_index;
  animator->current_active_frame = 0;
  animator->frame_timer = 0.0f;
}




void _sfr_sprite_animator_update(SFRcomponent_t* component, float delta_time) {
  SFRsprite_animator_t* animator = SFR_COMPONENT_CONVERT(SFRsprite_animator_t, component);

  if (sfr_timer_finished(&animator->frame_timer)) {
    SFRsprite_animation_t* animation = &animator->animations[animator->active_animation];

    // setting the timer
    animator->frame_timer = sfr_timer_start(animation->time_btw_frames[animator->current_active_frame]);

    ivec2 frame = {
      animation->frames[animator->current_active_frame][X],
      animation->frames[animator->current_active_frame][Y]
    };

    // setting the uvs of the sprite renderer to be the correct one
    vec2 uv_size = { 
      1.0f / animator->slices_count[X], 
      1.0f / animator->slices_count[Y]
    };
    vec2 bottom_left = {
      uv_size[X] * frame[X],
      uv_size[Y] * frame[Y]
    };
    vec2 top_right;
    glm_vec2_copy(bottom_left, top_right);
    glm_vec2_add(top_right, uv_size, top_right);

    sfr_sprite_renderer_set_uv(animator->sprite_renderer, bottom_left, top_right);

    // getting the frame index (doing this last so it can play the first frame first, not the second one)
    animator->current_active_frame++;
    if (animator->current_active_frame == animation->frame_count) {
      animator->current_active_frame = 0;
    }
  }
}

void _sfr_sprite_animator_free(SFRcomponent_t* component) {
  SFRsprite_animator_t* animator = SFR_COMPONENT_CONVERT(SFRsprite_animator_t, component);
  
  if (animator->animations != NULL) {
    for (uint32_t i = 0; i < animator->animations_count; i++) {
      free(animator->animations[i].frames);
      free(animator->animations[i].time_btw_frames);
    }
    
    free(animator->animations);
  }
}
