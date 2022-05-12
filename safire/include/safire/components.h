#ifndef __SAFIRE_COMPONENTS_H__
#define __SAFIRE_COMPONENTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct SFRentity                SFRentity_t;
typedef struct SFRcomponent             SFRcomponent_t;
typedef struct SFRvertex                SFRvertex_t;

// don't change these especially the transform and sprite renderer names as that will break the ecs 
#define SFR_TRANSFORM                   "SFRtransform"
#define SFR_COLLIDER2D                  "SFRcollider2d"
#define SFR_SPRITE_RENDERER             "SFRsprite_renderer"
#define SFR_SPRITE_ANIMATOR             "SFRsprite_animator"

typedef struct SFRtransform             SFRtransform_t;
typedef struct SFRcollider2d            SFRcollider2d_t;
typedef struct SFRsprite_renderer       SFRsprite_renderer_t;
typedef struct SFRsprite_animator       SFRsprite_animator_t;
typedef struct SFRsprite_animation      SFRsprite_animation_t;

typedef enum   SFRcollider2d_type       SFRcollider2d_type_t;

typedef struct SFRtexture               SFRtexture_t;
typedef struct SFRshader                SFRshader_t;
typedef uint64_t                        SFRuuid_t;




SAFIRE_API SFRcomponent_t*  sfr_get_component(SFRcomponent_t* component, const char* name);




struct SFRtransform {
  vec3                      position;
  vec3                      scale;
  vec4                      rotation;
};

SAFIRE_API SFRcomponent_t*  sfr_transform();




enum SFRcollider2d_type {
  SFR_COLLIDER2D_TYPE_CIRCLE              = 0, 
  SFR_COLLIDER2D_TYPE_SQUARE                 , // TODO: out of scope for this project (maybe)
  SFR_COLLIDER2D_TYPE_CUSTOM                   // TODO: out of scope for this project
};

struct SFRcollider2d {
  SFRcollider2d_type_t type;
  bool trigger;
  vec2 size;
  vec2 offset;
  float weight;
};

SAFIRE_API SFRcomponent_t*  sfr_collider2d(); 

SAFIRE_API bool sfr_collider2d_trigger_enter_tag(SFRcomponent_t* component, const char* target_tag);
SAFIRE_API bool sfr_collider2d_trigger_enter_name(SFRcomponent_t* component, const char* name);
SAFIRE_API bool sfr_collider2d_trigger_enter_uuid(SFRcomponent_t* component, SFRuuid_t target_uuid);
SAFIRE_API bool sfr_collider2d_trigger_exit_tag(SFRcomponent_t* component, const char* target_tag);
SAFIRE_API bool sfr_collider2d_trigger_exit_name(SFRcomponent_t* component, const char* name);
SAFIRE_API bool sfr_collider2d_trigger_exit_uuid(SFRcomponent_t* component, SFRuuid_t target_uuid);




struct SFRsprite_renderer {
  uint32_t                  shader;
  SFRsprite_animator_t*     sprite_animator;
  SFRvertex_t*              vertices;
};

SAFIRE_API SFRcomponent_t*  sfr_sprite_renderer(); 

SAFIRE_API void             sfr_sprite_renderer_set_uv(SFRcomponent_t* component, vec2 uv_bottom_left, vec2 uv_top_right);

SAFIRE_API SFRvertex_t*     sfr_sprite_renderer_get_vertices(SFRcomponent_t* component, uint32_t* count);

SAFIRE_API void             sfr_sprite_renderer_set_texture(SFRcomponent_t* component, const char* name);
SAFIRE_API SFRtexture_t*    sfr_sprite_renderer_get_texture(SFRcomponent_t* component);

SAFIRE_API void             sfr_sprite_renderer_set_shader(SFRcomponent_t* component, const char* name);
SAFIRE_API SFRshader_t*     sfr_sprite_renderer_get_shader(SFRcomponent_t* component);




struct SFRsprite_animation {
  char* name;
  ivec2* frames;
  uint32_t frame_count;
  float* time_btw_frames;
};

struct SFRsprite_animator {
  SFRcomponent_t* sprite_renderer;

  ivec2 slices_size;
  ivec2 slices_count;

  uint32_t active_animation;
  uint32_t current_active_frame;
  SFRtimer_t frame_timer;

  SFRsprite_animation_t* animations;
  uint32_t animations_count;
};

SAFIRE_API SFRcomponent_t*        sfr_sprite_animator(SFRentity_t* entity); 

SAFIRE_API void                   sfr_sprite_animator_load_animation(SFRcomponent_t* component, const char* name, ivec2 frames[], float* time_btw_frames, uint32_t frame_count);

SAFIRE_API void                   sfr_sprite_animator_start_animation(SFRcomponent_t* component, const char* name);
SAFIRE_API void                   sfr_sprite_animator_start_animation_index(SFRcomponent_t* component, uint32_t anim_index);

SAFIRE_API void                   sfr_sprite_animator_slice(SFRcomponent_t* component, ivec2 slice_size);




#ifdef __cplusplus
}
#endif

#endif // __SAFIRE_COMPONENTS_H__
