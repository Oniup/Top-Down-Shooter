#ifndef __SAFIRE_PIPELINE_H__
#define __SAFIRE_PIPELINE_H__

#include "utils.h"

typedef struct SFRpipeline SFRpipeline_t;
typedef struct SFRwindow SFRwindow_t;
typedef struct SFRshader SFRshader_t;
typedef struct SFRtexture SFRtexture_t;
typedef struct SFRvertex SFRvertex_t;

extern void sfr_pipeline_init(const char* window_title, int window_width, int window_height, bool fullscreen);
extern void sfr_pipeline_flush();
extern void sfr_pipeline_free();

extern SFRshader_t** sfr_pipeline_get_shaders();
extern SFRtexture_t** sfr_pipeline_get_textures();
extern SFRwindow_t* sfr_pipeline_get_window();

extern void sfr_pipeline_push_shader(const char* name, const char* vertex_path, const char* fragment_path);
extern void sfr_pipeline_push_texture(const char* name, const char* path, bool flip, uint32_t channels);

typedef struct GLFWwindow GLFWwindow;
struct SFRwindow {
  char* title;
  GLFWwindow* window;
  int width;
  int height;
  bool fullscreen;
  bool transparent;
  vec4 clear_colour;
  ivec3 glsl_version;
};

extern SFRwindow_t* sfr_window_create(const char* window_title, int window_width, int window_height, bool fullscreen, bool transparent);
extern void sfr_window_free(SFRwindow_t* window);

extern void sfr_window_set_clear(SFRwindow_t* window, vec4 clear_colour);
extern void sfr_window_set_title(SFRwindow_t* window, const char* title);

extern void sfr_window_clear(SFRwindow_t* window);
extern void sfr_window_swap_buffers(SFRwindow_t* window);
extern bool sfr_window_closing(SFRwindow_t* window);

extern void sfr_window_quit(SFRwindow_t* window);

struct SFRshader {
  uint32_t id;
  char* name;
};

extern SFRshader_t* sfr_shader_create(const char* name, const char* vertex_path, const char* fragment_path);
extern void sfr_shader_free(SFRshader_t* shader);

struct SFRtexture {
  uint32_t id;
  char* name;
};

extern SFRtexture_t* sfr_texture_create(const char* name, const char* path, bool flip, uint32_t channels);
extern void sfr_texture_free(SFRtexture_t* texture);

struct SFRvertex {
  vec3 vertex;
  vec2 uv;
  uint32_t texture_id;
};


#endif // __SAFIRE_PIPELINE_H__