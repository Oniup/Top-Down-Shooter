#ifndef __SAFIRE_PIPELINE_H__
#define __SAFIRE_PIPELINE_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "utils.h"



#define SFR_PIPELINE_PROJECTION_PERSPECTIVE     0 
#define SFR_PIPELINE_PROJECTION_ORTHOGRAPHIC    1

#define SFR_VERTEX_SIZE                         10



typedef struct SFR_Pipeline             SFR_Pipeline;
typedef struct SFR_Window               SFR_Window;
typedef struct SFR_Shader               SFR_Shader;
typedef struct SFR_Texture              SFR_Texture;
typedef struct SFR_Renderer             SFR_Renderer;
typedef struct SFR_Vertex               SFR_Vertex;
typedef struct SFR_Font                 SFR_Font;




SAFIRE_API void                         sfr_pipeline_init(const char* window_title, int window_width, int window_height, bool fullscreen);
SAFIRE_API SFR_Pipeline*                sfr_pipeline_instance();
SAFIRE_API void                         sfr_pipeline_render();
SAFIRE_API bool                         sfr_pipeline_window_closing();
SAFIRE_API void                         sfr_pipeline_free();

SAFIRE_API void                         sfr_pipeline_set_projection_matrix(int mode);
SAFIRE_API void                         sfr_pipeline_set_transform_matrix(vec3 position, vec3 scale, vec4 rotation);
SAFIRE_API void                         sfr_pipeline_set_view_matrix(vec3 position);

SAFIRE_API void                         sfr_pipeline_get_projection_matrix(mat4 projection);
SAFIRE_API void                         sfr_pipeline_get_transform_matrix(mat4 transform);
SAFIRE_API void                         sfr_pipeline_get_view_matrix(mat4 view);

SAFIRE_API bool                         sfr_pipeline_cull_vertices(SFR_Vertex* vertices, uint32_t count);
SAFIRE_API void                         sfr_pipeline_set_culling_centre(vec3 position);
SAFIRE_API void                         sfr_pipeline_set_culling_size(vec3 size);

SAFIRE_API SFR_Shader**                 sfr_pipeline_get_shaders(uint32_t* shaders_count);
SAFIRE_API SFR_Texture**                sfr_pipeline_get_textures(uint32_t* textures_count);
SAFIRE_API SFR_Window*                  sfr_pipeline_get_window();

SAFIRE_API SFR_Shader*                  sfr_pipeline_get_target_shader(const char* name);
SAFIRE_API SFR_Texture*                 sfr_pipeline_get_target_texture(const char* name);
SAFIRE_API uint32_t                     sfr_pipeline_get_target_shader_index(const char* name);
SAFIRE_API uint32_t                     sfr_pipeline_get_target_texture_index(const char* name);

SAFIRE_API void                         sfr_pipeline_push_shader(SFR_Shader* shader);
SAFIRE_API void                         sfr_pipeline_push_texture(SFR_Texture* texture);
SAFIRE_API void                         sfr_pipeline_clear_assets_stack();

SAFIRE_API void                         sfr_pipeline_push_vertices(SFR_Vertex* vertices, uint32_t count, uint32_t shader);
SAFIRE_API void                         sfr_pipeline_print_vertices_debug();




struct SFR_Vertex 
{
  vec3                                  vertex;
  vec2                                  uv;
  vec4                                  overlay_colour;
  float                                 texture_id;
};

SAFIRE_API void                         sfr_vertex_copy(SFR_Vertex* dest, SFR_Vertex* source);




typedef struct GLFWwindow               GLFWwindow;

struct SFR_Window 
{
  char*                                 title;
  GLFWwindow*                           window;
  int                                   width;
  int                                   height;
  bool                                  fullscreen;
  bool                                  transparent;
  vec4                                  clear_colour;
  ivec3                                 glsl_version;
};

SAFIRE_API SFR_Window*                  sfr_window(const char* window_title, int window_width, int window_height, bool fullscreen, bool transparent);
SAFIRE_API void                         sfr_window_free(SFR_Window* window);

SAFIRE_API void                         sfr_window_set_clear(SFR_Window* window, vec4 clear_colour);
SAFIRE_API void                         sfr_window_set_title(SFR_Window* window, const char* title);

SAFIRE_API void                         sfr_window_clear(SFR_Window* window);
SAFIRE_API void                         sfr_window_swap_buffers(SFR_Window* window);
SAFIRE_API bool                         sfr_window_closing(SFR_Window* window);

SAFIRE_API void                         sfr_window_quit(SFR_Window* window);




struct SFR_Shader
{
  uint32_t                              id;
  char*                                 name;
};

SAFIRE_API SFR_Shader*                  sfr_shader(const char* name, const char* vertex_path, const char* fragment_path);
SAFIRE_API void                         sfr_shader_bind(SFR_Shader* shader);
SAFIRE_API void                         sfr_shader_unbind();
SAFIRE_API void                         sfr_shader_free(SFR_Shader* shader);




struct SFR_Texture 
{
  uint32_t                              id;
  char*                                 name;

  uint32_t                              width;
  uint32_t                              height;  
};

SAFIRE_API SFR_Texture*                 sfr_texture(const char* name, const char* path, bool flip, uint32_t channels, bool linear);
SAFIRE_API void                         sfr_texture_bind(SFR_Shader* shader, SFR_Texture* texture, uint32_t id);
SAFIRE_API void                         sfr_texture_unbind(uint32_t id);
SAFIRE_API void                         sfr_texture_free(SFR_Texture* texture);




struct SFR_Font
{
  char*                                 name;
  uint32_t                              texture_id;
};

SAFIRE_API SFR_Font*                    sfr_font(const char* name, const char* location);
SAFIRE_API SFR_Font*                    sfr_font_bind(SFR_Font* font, char character);



#ifdef __cplusplus 
}
#endif

#endif // __SAFIRE_PIPELINE_H__