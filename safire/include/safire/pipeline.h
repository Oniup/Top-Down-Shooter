#ifndef __SAFIRE_PIPELINE_H__
#define __SAFIRE_PIPELINE_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "utils.h"



#define SFR_PIPELINE_PROJECTION_PERSPECTIVE     0 
#define SFR_PIPELINE_PROJECTION_ORTHOGRAPHIC    1

#define SFR_VERTEX_SIZE                         10



typedef struct SFRpipeline              SFRpipeline_t;
typedef struct SFRwindow                SFRwindow_t;
typedef struct SFRshader                SFRshader_t;
typedef struct SFRtexture               SFRtexture_t;
typedef struct SFRrenderer              SFRrenderer_t;
typedef struct SFRvertex                SFRvertex_t;




SAFIRE_API void             sfr_pipeline_init(const char* window_title, int window_width, int window_height, bool fullscreen);
SAFIRE_API SFRpipeline_t*   sfr_pipeline_instance();
SAFIRE_API void             sfr_pipeline_render();
SAFIRE_API bool             sfr_pipeline_window_closing();
SAFIRE_API void             sfr_pipeline_free();

SAFIRE_API void             sfr_pipeline_set_projection_matrix(int mode);
SAFIRE_API void             sfr_pipeline_get_projection_matrix(mat4 projection);
SAFIRE_API void             sfr_pipeline_set_transform_matrix(vec3 position, vec3 scale, vec4 rotation);

SAFIRE_API SFRshader_t**    sfr_pipeline_get_shaders(uint32_t* shaders_count);
SAFIRE_API SFRtexture_t**   sfr_pipeline_get_textures(uint32_t* textures_count);
SAFIRE_API SFRwindow_t*     sfr_pipeline_get_window();

SAFIRE_API SFRshader_t*     sfr_pipeline_get_target_shader(const char* name);
SAFIRE_API SFRtexture_t*    sfr_pipeline_get_target_texture(const char* name);
SAFIRE_API uint32_t         sfr_pipeline_get_target_shader_index(const char* name);
SAFIRE_API uint32_t         sfr_pipeline_get_target_texture_index(const char* name);

SAFIRE_API void             sfr_pipeline_push_shader(SFRshader_t* shader);
SAFIRE_API void             sfr_pipeline_push_texture(SFRtexture_t* texture);
SAFIRE_API void             sfr_pipeline_clear_assets_stack();

SAFIRE_API void             sfr_pipeline_push_vertices(SFRvertex_t* vertices, uint32_t count, uint32_t shader);
SAFIRE_API void             sfr_pipeline_print_vertices_debug();




struct SFRvertex {
  vec3                      vertex;
  vec2                      uv;
  vec4                      overlay_colour;
  float                     texture_id;
};

SAFIRE_API void             sfr_vertex_copy(SFRvertex_t* dest, SFRvertex_t* source);




typedef struct GLFWwindow GLFWwindow;

struct SFRwindow {
  char*                     title;
  GLFWwindow*               window;
  int                       width;
  int                       height;
  bool                      fullscreen;
  bool                      transparent;
  vec4                      clear_colour;
  ivec3                     glsl_version;
};

SAFIRE_API SFRwindow_t*     sfr_window(const char* window_title, int window_width, int window_height, bool fullscreen, bool transparent);
SAFIRE_API void             sfr_window_free(SFRwindow_t* window);

SAFIRE_API void             sfr_window_set_clear(SFRwindow_t* window, vec4 clear_colour);
SAFIRE_API void             sfr_window_set_title(SFRwindow_t* window, const char* title);

SAFIRE_API void             sfr_window_clear(SFRwindow_t* window);
SAFIRE_API void             sfr_window_swap_buffers(SFRwindow_t* window);
SAFIRE_API bool             sfr_window_closing(SFRwindow_t* window);

SAFIRE_API void             sfr_window_quit(SFRwindow_t* window);




struct SFRshader {
  uint32_t                  id;
  char*                     name;
};

SAFIRE_API SFRshader_t*     sfr_shader(const char* name, const char* vertex_path, const char* fragment_path);
SAFIRE_API void             sfr_shader_bind(SFRshader_t* shader);
SAFIRE_API void             sfr_shader_unbind();
SAFIRE_API void             sfr_shader_free(SFRshader_t* shader);




struct SFRtexture {
  uint32_t                  id;
  char*                     name;

  uint32_t                  width;
  uint32_t                  height;  
};

SAFIRE_API SFRtexture_t*    sfr_texture(const char* name, const char* path, bool flip, uint32_t channels, bool linear);
SAFIRE_API void             sfr_texture_bind(SFRshader_t* shader, SFRtexture_t* texture, uint32_t id);
SAFIRE_API void             sfr_texture_unbind(uint32_t id);
SAFIRE_API void             sfr_texture_free(SFRtexture_t* texture);




#ifdef __cplusplus 
}
#endif

#endif // __SAFIRE_PIPELINE_H__