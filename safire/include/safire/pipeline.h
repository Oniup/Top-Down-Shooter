#ifndef __SAFIRE_PIPELINE_H__
#define __SAFIRE_PIPELINE_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "utils.h"

typedef struct SFRpipeline SFRpipeline_t;
typedef struct SFRwindow SFRwindow_t;
typedef struct SFRshader SFRshader_t;
typedef struct SFRtexture SFRtexture_t;
typedef struct SFRvertex SFRvertex_t;

SAFIRE_API void             sfr_pipeline_init(const char* window_title, int window_width, int window_height, bool fullscreen);
SAFIRE_API SFRpipeline_t*   sfr_pipeline_instance();
SAFIRE_API void             sfr_pipeline_render();
SAFIRE_API bool             sfr_pipeline_window_closing();
SAFIRE_API void             sfr_pipeline_free();

SAFIRE_API SFRshader_t**    sfr_pipeline_get_shaders();
SAFIRE_API SFRtexture_t**   sfr_pipeline_get_textures();
SAFIRE_API SFRwindow_t*     sfr_pipeline_get_window();

SAFIRE_API void             sfr_pipeline_push_shader(const char* name, const char* vertex_path, const char* fragment_path);
SAFIRE_API void             sfr_pipeline_push_texture(const char* name, const char* path, bool flip, uint32_t channels);




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

SAFIRE_API SFRwindow_t*     sfr_window_create(const char* window_title, int window_width, int window_height, bool fullscreen, bool transparent);
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

SAFIRE_API SFRshader_t*     sfr_shader_create(const char* name, const char* vertex_path, const char* fragment_path);
SAFIRE_API void             sfr_shader_free(SFRshader_t* shader);




struct SFRtexture {
  uint32_t id;
  char* name;
};

SAFIRE_API SFRtexture_t*    sfr_texture_create(const char* name, const char* path, bool flip, uint32_t channels);
SAFIRE_API void             sfr_texture_free(SFRtexture_t* texture);




struct SFRvertex {
  vec3                      vertex;
  vec2                      uv;
  uint32_t                  texture_id;
};




#ifdef __cplusplus 
}
#endif

#endif // __SAFIRE_PIPELINE_H__