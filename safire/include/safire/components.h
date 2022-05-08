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




#define sfr_component_convert(Ty, component) ({\
  SAFIRE_ASSERT(component != NULL, "[SAFIRE::COMPONENT_CONVERT]: failed to convert component custom data as the component given doesn't exist");\
  Ty* result = ((Ty*)component->data);\
  SAFIRE_ASSERT(result != NULL, "[SAFIRE::COMPONENT_CONVERT]: failed to convert component custom data as the component's custom data isn't the target type");\
  result;\
})\


SAFIRE_API void sfr_attach_default_comps(SFRentity_t* entity);

struct SFRtransform {
  vec3                      position;
  vec3                      scale;
  vec4                      rotation;
};

SAFIRE_API SFRcomponent_t*  sfr_transform();

enum SFRcollider2d_type {
  COLLIDER2D_TYPE_CIRCLE              = 0, 
  COLLIDER2D_TYPE_SQUARE                 , // TODO: out of scope for this project
  COLLIDER2D_TYPE_CUSTOM                   // TODO: out of scope for this project
};

struct SFRcollider2d {
  SFRcollider2d_type_t type;
  vec2 size;
};

SAFIRE_API SFRcomponent_t*  sfr_collider2d(); // TODO: implement circle collision detection

struct SFRsprite_renderer {
  uint32_t                  shader;
  SFRsprite_animator_t*     sprite_animator;
  SFRvertex_t*              vertices;
};

SAFIRE_API SFRcomponent_t*  sfr_sprite_renderer(); // TODO: implement rendering sprites 
SAFIRE_API void             sfr_sprite_renderer_set_texture(SFRcomponent_t* component, const char* name);
SAFIRE_API void             sfr_sprite_renderer_set_shader(SFRcomponent_t* component, const char* name);
SAFIRE_API SFRtexture_t*    sfr_sprite_renderer_get_texture(SFRcomponent_t* component);
SAFIRE_API SFRshader_t*     sfr_sprite_renderer_get_shader(SFRcomponent_t* component);

struct SFRsprite_animation {
  uint32_t* frames;
  float* time_between_frames;
};

struct SFRsprite_animator {
  SFRsprite_renderer_t* sprite_renderer;
  ivec2 frame_size;
  SFRsprite_animation_t* animations;
  uint32_t animation_count;
};

SAFIRE_API SFRcomponent_t*  sfr_sprite_animator(SFRentity_t* entity); // TODO: implement rendering sprite animations

#ifdef __cplusplus
}
#endif

#endif // __SAFIRE_COMPONENTS_H__
