#ifndef __SAFIRE_COMPONENTS_H__
#define __SAFIRE_COMPONENTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

typedef struct SFR_Entity                 SFR_Entity;
typedef struct SFR_Component              SFR_Component;
typedef struct SFR_Vertex                 SFR_Vertex;

// don't change these especially the transform and sprite renderer names as that will break the ecs 
#define SFR_TRANSFORM                     "SFRtransform"
#define SFR_COLLIDER2D                    "SFRcollider2d"
#define SFR_SPRITE_RENDERER               "SFRsprite_renderer"
#define SFR_SPRITE_ANIMATOR               "SFRsprite_animator"

typedef struct SFR_Transform              SFR_Transform;
typedef struct SFR_Collider2D             SFR_Collider2D;
typedef struct SFR_SpriteRenderer         SFR_SpriteRenderer;
typedef struct SFR_SpriteAnimator         SFR_SpriteAnimator;
typedef struct SFR_SpriteAnimation        SFR_SpriteAnimation;

typedef struct SFR_TextRenderer           SFR_TextRenderer;
typedef struct SFR_TextRendererChar       SFR_TextRendererChar;
typedef struct SFR_Font                   SFR_Font;

typedef enum   SFR_Collide2DType          SFR_Collide2DType;

typedef struct SFR_Texture                SFR_Texture;
typedef struct SFR_Shader                 SFR_Shader;
typedef uint64_t                          SFR_Uuid;




SAFIRE_API SFR_Component*                 sfr_get_component(SFR_Component* component, const char* name);




struct SFR_Transform 
{
  vec3                                    position;
  vec3                                    scale;
  vec4                                    rotation;
};

SAFIRE_API SFR_Component*                 sfr_transform();




enum SFR_Collide2DType 
{
  SFR_COLLIDER2D_TYPE_CIRCLE              = 0, 
  SFR_COLLIDER2D_TYPE_SQUARE                 , // TODO: out of scope for this project (maybe)
  SFR_COLLIDER2D_TYPE_CUSTOM                   // TODO: out of scope for this project
};

struct SFR_Collider2D {
  SFR_Collide2DType type;
  
  bool trigger;

  vec2 size;
  vec2 offset;

  float weight;

  uint32_t id;
};

SAFIRE_API SFR_Component*                 sfr_collider2d(SFR_Component* transform); 

SAFIRE_API bool                           sfr_collider2d_trigger_enter_tag(SFR_Component* component, const char* target_tag, SFR_Component** get);
SAFIRE_API bool                           sfr_collider2d_trigger_enter_name(SFR_Component* component, const char* name, SFR_Component** get);
SAFIRE_API bool                           sfr_collider2d_trigger_enter_uuid(SFR_Component* component, SFR_Uuid target_uuid, SFR_Component** get);
SAFIRE_API bool                           sfr_collider2d_trigger_exit_tag(SFR_Component* component, const char* target_tag, SFR_Component** get);
SAFIRE_API bool                           sfr_collider2d_trigger_exit_name(SFR_Component* component, const char* name, SFR_Component** get);
SAFIRE_API bool                           sfr_collider2d_trigger_exit_uuid(SFR_Component* component, SFR_Uuid target_uuid, SFR_Component** get);




struct SFR_SpriteRenderer {
  uint32_t                                shader;
  SFR_SpriteAnimator*                     sprite_animator;
  SFR_Vertex*                             vertices;
  bool                                    culled;
};

SAFIRE_API SFR_Component*                 sfr_sprite_renderer(); 
            
SAFIRE_API void                           sfr_sprite_renderer_set_uv(SFR_Component* component, vec2 uv_bottom_left, vec2 uv_top_right);
            
SAFIRE_API SFR_Vertex*                    sfr_sprite_renderer_get_vertices(SFR_Component* component, uint32_t* count);
            
SAFIRE_API void                           sfr_sprite_renderer_set_texture(SFR_Component* component, const char* name);
SAFIRE_API SFR_Texture*                   sfr_sprite_renderer_get_texture(SFR_Component* component);
            
SAFIRE_API void                           sfr_sprite_renderer_set_shader(SFR_Component* component, const char* name);
SAFIRE_API SFR_Shader*                    sfr_sprite_renderer_get_shader(SFR_Component* component);




struct SFR_SpriteAnimation {
  char*                                   name;
  ivec2*                                  frames;
  uint32_t                                frame_count;
  float*                                  time_btw_frames;
};

struct SFR_SpriteAnimator {
  SFR_Component*                          sprite_renderer;

  ivec2                                   slices_size;
  ivec2                                   slices_count;

  uint32_t                                active_animation;
  uint32_t                                current_active_frame;
  SFR_Timer                               frame_timer;

  bool                                    is_looping;

  SFR_SpriteAnimation*                    animations;
  uint32_t                                animations_count;
};

#define SFR_SPRITE_ANIMATOR_STOP          -1.0f

SAFIRE_API SFR_Component*                 sfr_sprite_animator(SFR_Entity* entity); 

SAFIRE_API void                           sfr_sprite_animator_load_animation(SFR_Component* component, const char* name, ivec2 frames[], float* time_btw_frames, uint32_t frame_count);

SAFIRE_API void                           sfr_sprite_animator_start_animation(SFR_Component* component, const char* name);
SAFIRE_API void                           sfr_sprite_animator_start_animation_index(SFR_Component* component, uint32_t anim_index);

SAFIRE_API void                           sfr_sprite_animator_slice(SFR_Component* component, ivec2 slice_size);




struct SFR_TextRenderer
{
  SFR_TextRendererChar*                   characters;
  uint32_t                                characters_count;

  SFR_Font*                               font;
  uint32_t                                size;
};

SAFIRE_API SFR_Component*                 sfr_text_renderer(SFR_Font* font);
SAFIRE_API void                           sfr_text_renderer_set_size(SFR_Component* component, uint32_t size);




#ifdef __cplusplus
}
#endif

#endif // __SAFIRE_COMPONENTS_H__
