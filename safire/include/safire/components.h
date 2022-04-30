#ifndef __SAFIRE_COMPONENTS_H__
#define __SAFIRE_COMPONENTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct SFRentity                SFRentity_t;
typedef struct SFRcomponent             SFRcomponent_t;

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
  uint32_t                  texture;
  uint32_t                  shader;
  vec4                      over_lay_colour;
  SFRsprite_animator_t*     sprite_animator;
};

SAFIRE_API SFRcomponent_t*  sfr_sprite_renderer(); // TODO: implement rendering sprites 

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
