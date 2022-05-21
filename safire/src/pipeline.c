/* 
  VERTEX DATA EXAMPLES:

  example vertex data in float buffer: 
    float vertices[] = {
      (vertex)                 (uv)             (overlay colour)             (texture id)
      1.0f , 1.0f, 0.0f,       1.0f, 1.0f,      0.0f, 0.0f, 0.0f, 0.0f,      0.0f,            (top right)
     -1.0f,  1.0f, 0.0f,       0.0f, 1.0f,      0.0f, 0.0f, 0.0f, 0.0f,      0.0f,            (top left)
     -1.0f, -1.0f, 0.0f,       0.0f, 0.0f,      0.0f, 0.0f, 0.0f, 0.0f,      0.0f,            (bottom left)
      1.0f, -1.0f, 0.0f,       1.0f, 0.0f,      0.0f, 0.0f, 0.0f, 0.0f,      0.0f,            (bottom right)
    };

  example of indices data for the current vertex buffer:
    uint32_t indices[] = { 
      0, 1, 2,      (first triangle)
      0, 3, 2       (second triangle)
    };
*/

#include "../include/safire/pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// just to be safe, idk how to check how many textures the hardware will allow, 8 will work for everyone in the modern times
#define SFR_MAX_TEXTURES_COUNT 8 




struct SFR_Pipeline 
{
  SFR_Shader**                          shaders;
  uint32_t                              shaders_count;

  SFR_Texture**                         textures;
  uint32_t                              textures_count;

  SFR_Window*                           window;

  mat4                                  projection;
  int                                   projection_mode;
  mat4                                  transform2D;
  mat4                                  view;

  SFR_Renderer*                         renderer;

  vec3                                  culling_position;
  vec3                                  culling_size;
  vec3                                  culling_left;
  vec3                                  culling_right;
};

static SFR_Pipeline*                    _pipeline = NULL;




struct SFR_Renderer 
{
  float*                                vertices;

  uint32_t                              vertices_count;

  uint32_t                              active_shader;

  uint32_t                              active_textures[SFR_MAX_TEXTURES_COUNT];
  uint32_t                              active_textures_count;

  uint32_t                              vao;
  uint32_t                              vbo;
  uint32_t                              ebo;
};

static SFR_Renderer*                    _renderer = NULL;                     // reference to _pipeline->renderer just that its called a lot

void                                    _sfr_renderer_init();
void                                    _sfr_renderer_flush();

void                                    _sfr_renderer_increase_vertices_buffer(SFR_Vertex* vertices, uint32_t quad_count);

void                                    _sfr_renderer_push_data(SFR_Vertex* vertices, uint32_t count, uint32_t shader);
void                                    _sfr_renderer_clear_buffers();

void                                    _sfr_renderer_free();




void sfr_pipeline_init(const char* window_title, int window_width, int window_height, bool fullscreen) 
{
  _pipeline = (SFR_Pipeline*)malloc(sizeof(SFR_Pipeline));

  SAFIRE_ASSERT(glfwInit(), "[SAFIRE::PIPELINE] failed to initialise glfw for some reason ...");

  _pipeline->window = sfr_window(window_title, window_width, window_height, fullscreen, false);
  _pipeline->shaders = NULL;
  _pipeline->textures = NULL;
  _pipeline->shaders_count = 0;
  _pipeline->textures_count = 0;


  sfr_pipeline_push_shader(
    sfr_shader(
      "SAFIRE::default", 
      "./safire/shaders/sfr_default_shader.vert", "./safire/shaders/sfr_default_shader.frag"
    )
  );

  sfr_pipeline_push_texture(
    sfr_texture(
      "SAFIRE::default", "./safire/textures/default_texture.png", false, 4, false
    )
  );

  _sfr_renderer_init();

  // creating the global transform matrix for 2D games
  sfr_pipeline_set_transform_matrix((vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 1.0f, 1.0f, 1.0f }, (vec4){ 0.0f, 0.0f, 1.0f, 0.0f });
  
  // creating the default projection matrix
  sfr_pipeline_set_projection_matrix(SFR_PIPELINE_PROJECTION_ORTHOGRAPHIC);
  glm_mat4_identity(_pipeline->view);

sfr_pipeline_set_culling_centre((vec3){ 0.0f, 0.0f, 0.0f });
  sfr_pipeline_set_culling_size((vec3){ 16.0f * 0.7f, 9.0f * 0.7f, 5.0f });
}

SFR_Pipeline* sfr_pipeline_instance() 
{
  return _pipeline;
}

void sfr_pipeline_render() 
{
  _sfr_renderer_flush();
  
  sfr_window_swap_buffers(_pipeline->window);
  sfr_window_clear(_pipeline->window);  
}

bool sfr_pipeline_window_closing() 
{
  return sfr_window_closing(_pipeline->window);
}

void sfr_pipeline_free() 
{
  _sfr_renderer_free();
  _pipeline->renderer = NULL;

  sfr_window_free(_pipeline->window);
  
  free(_pipeline);
  _pipeline = NULL;
  glfwTerminate();
}

void sfr_pipeline_set_projection_matrix(int mode) 
{
  if (mode == SFR_PIPELINE_PROJECTION_PERSPECTIVE) 
  {
    // TODO: add perspective mode for 3D games
    // pipeline->projection = glm_perspective();
    // pipeline->projection_mode = mode;
  } 
  else if (mode == SFR_PIPELINE_PROJECTION_ORTHOGRAPHIC) 
  {
    glm_ortho(
      -9.0f, 9.0f, -4.5f, 4.5f,
      -1.0f, 1.0f, _pipeline->projection
    );
    _pipeline->projection_mode = mode;
  } 
  else
    printf("[SAFIRE_WARNING::PIPELINE] projection matrix can either be orthographic or perspective\n");
}

void sfr_pipeline_set_transform_matrix(vec3 position, vec3 scale, vec4 rotation) 
{
  glm_mat4_identity(_pipeline->transform2D);
  glm_translate(_pipeline->transform2D, position);
  glm_scale(_pipeline->transform2D, scale);
  glm_rotate(_pipeline->transform2D, rotation[W], rotation);
}

void sfr_pipeline_set_view_matrix(vec3 position) 
{
  vec3 up = { 0.0f, 1.0f, 0.0f };
  vec3 eye = { 0.0f, 0.0f, 1.0f };

  glm_vec3_add(position, eye, eye);
  glm_lookat(eye, position, up, _pipeline->view);
}

void sfr_pipeline_get_projection_matrix(mat4 projection) 
{
  glm_mat4_copy(_pipeline->projection, projection);
}

void sfr_pipeline_get_transform_matrix(mat4 transform) 
{
  glm_mat4_copy(_pipeline->transform2D, transform);
}

void sfr_pipeline_get_view_matrix(mat4 view) 
{
  glm_mat4_copy(_pipeline->view, view);
}

void sfr_pipeline_set_culling_centre(vec3 position)
{
  glm_vec3_copy(position, _pipeline->culling_position);
  glm_vec3_sub(_pipeline->culling_position, _pipeline->culling_size, _pipeline->culling_left);
  glm_vec3_add(_pipeline->culling_position, _pipeline->culling_size, _pipeline->culling_right);  
}

void sfr_pipeline_set_culling_size(vec3 size)
{
  glm_vec3_copy(size, _pipeline->culling_size);
}

SFR_Shader** sfr_pipeline_get_shaders(uint32_t* shader_count) 
{
  *shader_count = _pipeline->shaders_count;
  return _pipeline->shaders;
}

SFR_Texture** sfr_pipeline_get_textures(uint32_t* texture_count) 
{
  *texture_count = _pipeline->textures_count;
  return _pipeline->textures;
}

SFR_Window* sfr_pipeline_get_window() 
{
  return _pipeline->window;
}

SFR_Shader* sfr_pipeline_get_target_shader(const char* name) 
{
  if (name != NULL) 
  {
    uint32_t length = sfr_str_length(name);
    if (length > 0) 
    {
      for (uint32_t i = 0; i < _pipeline->shaders_count; i++) 
      {
        if (sfr_str_cmp_length(name, _pipeline->shaders[i]->name, length)) 
          return _pipeline->shaders[i];
      }
    }
  }

  printf("[SAFIRE::FIND_TARGET_SHADER] failed to find shader with the name '%s' as it doesn't exist\n", name); 
  return NULL;
}

SFR_Texture* sfr_pipeline_get_target_texture(const char* name) 
{
  if (name != NULL) 
  {
    uint32_t length = sfr_str_length(name);
    if (length > 0) 
    {
      for (uint32_t i = 0; i < _pipeline->textures_count; i++) 
      {
        if (sfr_str_cmp_length(name, _pipeline->textures[i]->name, length)) 
          return _pipeline->textures[i];
      }
    }
  }
  
  printf("[SAFIRE::FIND_TARGET_TEXTURE] failed to find texture with the name '%s' as it doesn't exist\n", name);
  return NULL;
}

uint32_t sfr_pipeline_get_target_shader_index(const char* name) 
{
  if (name != NULL) 
  {
    uint32_t length = sfr_str_length(name);
    if (length > 0) 
    {
      for (uint32_t i = 0; i < _pipeline->shaders_count; i++) 
      {
        if (sfr_str_cmp_length(name, _pipeline->shaders[i]->name, length)) 
          return i;
      }
    }
  }

  printf("[SAFIRE::FIND_TARGET_SHADER_INDEX] failed to find shader with the name '%s' as it doesn't exist\n", name); 
  return UINT32_MAX;
}

uint32_t sfr_pipeline_get_target_texture_index(const char* name) 
{
  if (name != NULL) 
  {
    uint32_t length = sfr_str_length(name);
    if (length > 0) 
    {
      for (uint32_t i = 0; i < _pipeline->textures_count; i++) 
      {
        if (sfr_str_cmp_length(name, _pipeline->textures[i]->name, length))
          return i;
      }
    }
  }
  
  printf("[SAFIRE::FIND_TARGET_TEXTURE_INDEX] failed to find texture with the name '%s' as it doesn't exist\n", name);
  return UINT32_MAX;
}

void sfr_pipeline_push_shader(SFR_Shader* shader) 
{
  if (shader != NULL) 
  {
    uint32_t length = sfr_str_length(shader->name);
    for (uint32_t i = 0; i < _pipeline->shaders_count; i++)
    {
      if (sfr_str_cmp_length(shader->name, _pipeline->shaders[i]->name, length))
      {
        sfr_shader_free(shader);
        
        return;
      }
    }


    _pipeline->shaders_count++;

    if (_pipeline->shaders != NULL) 
    {
      _pipeline->shaders = (SFR_Shader**)realloc(_pipeline->shaders, sizeof(SFR_Shader*) * _pipeline->shaders_count);
      SAFIRE_ASSERT(_pipeline->shaders, "[SAFIRE::PIPELINE_SHADER_PUSH] failed to resize shader buffer stack");
    } 
    else 
    {
      _pipeline->shaders = (SFR_Shader**)malloc(sizeof(SFR_Shader*) * _pipeline->shaders_count);
      SAFIRE_ASSERT(_pipeline->shaders, "[SAFIRE::PIPELINE_SHADER_PUSH] failed to assign memory to shader buffer stack");
    }

    _pipeline->shaders[_pipeline->shaders_count - 1] = shader;
  } 
  else 
    printf("[SAFIRE_WARNING::PIPELINE_SHADER_PUSH] cannot push shader, as the shader doesn't exist");
}

void sfr_pipeline_push_texture(SFR_Texture* texture) 
{
  if (texture != NULL) 
  {  
    uint32_t length = sfr_str_length(texture->name);
    for (uint32_t i = 0; i < _pipeline->textures_count; i++)
    {
      if (sfr_str_cmp_length(texture->name, _pipeline->textures[i]->name, length))
      {
        sfr_texture_free(texture);
        
        return;
      }
    }

    _pipeline->textures_count++;

    if (_pipeline->textures != NULL) 
    {
      _pipeline->textures = (SFR_Texture**)realloc(_pipeline->textures, sizeof(SFR_Texture*) * _pipeline->textures_count);
      SAFIRE_ASSERT(_pipeline->textures, "[SAFIRE::PIPELINE_TEXTURE_PUSH] failed to resize texture buffer stack");
    } 
    else 
    {
      _pipeline->textures = (SFR_Texture**)malloc(sizeof(SFR_Texture*) * _pipeline->textures_count);
      SAFIRE_ASSERT(_pipeline->textures, "[SAFIRE::PIPELINE_TEXTURE_PUSH] failed to assign memory to texture buffer stack");
    }

    _pipeline->textures[_pipeline->textures_count - 1] = texture;
  } 
  else 
    printf("[SAFIRE_WARNING::PIPELINE_TEXTURE_PUSH] cannot push texture, as the texture doesn't exist");
}

void sfr_pipeline_clear_assets_stack() {

  if (_pipeline->shaders != NULL) 
  {
    for (uint32_t i = 0; i < _pipeline->shaders_count; i++) 
    {
      sfr_shader_free(_pipeline->shaders[i]);
    }

    free(_pipeline->shaders);
    _pipeline->shaders = NULL;
    _pipeline->shaders_count = 0;
  }

  if (_pipeline->textures != NULL) 
  {
    for (uint32_t i = 0; i < _pipeline->textures_count; i++) 
    {
      sfr_texture_free(_pipeline->textures[i]);
    }

    free(_pipeline->textures);
    _pipeline->textures = NULL;
    _pipeline->textures_count = 0;
  }
}

void sfr_pipeline_push_vertices(SFR_Vertex* vertices, uint32_t count, uint32_t shader) {

  _sfr_renderer_push_data(vertices, count, shader);
}

void sfr_pipeline_print_vertices_debug() 
{
  printf("renderer batch buffer debug:\n");
  if (_renderer->vertices != NULL) 
  {
    printf("\nvertices: (%u)\n(vertices:vec3)(uv:vec2)(overlay_colour:vec4)(texture_id:float)\n", _renderer->vertices_count);
    int k = 0;
    for (uint32_t i = 0; i < _renderer->vertices_count; i++) 
    {
      if (i % 40 == 0) 
      {
        k++;
        printf("quad %d\n", k);
      }

      printf("%f ", _renderer->vertices[i]);
      int br = (i + 1) % 10;
      if (br == 0) 
        printf("\n");
    }

    printf("active_shader id: %u\n", _renderer->active_shader);
    printf("active_textures: (%u/10)\n", _renderer->active_textures_count);
    for (uint32_t i = 0; i < _renderer->active_textures_count; i++) 
    {
      printf("* [%u]: %u\n", i, _renderer->active_textures[i]);
    }

  } 
  else 
  {
    printf("buffers are currently empty\n");
  }
}

void sfr_vertex_copy(SFR_Vertex* dest, SFR_Vertex* source) 
{
  glm_vec3_copy(source->vertex, dest->vertex);
  glm_vec2_copy(source->uv, dest->uv);
  glm_vec4_copy(source->overlay_colour, dest->overlay_colour);
  dest->texture_id = source->texture_id;
}

void _sfr_renderer_init() 
{
  _pipeline->renderer = (SFR_Renderer*)malloc(sizeof(SFR_Renderer));
  _renderer = _pipeline->renderer;

  _renderer->vertices = NULL;
  _renderer->vertices_count = 0;  

  _renderer->active_shader = 0; // the index to the shader in the pipeline

  memset(_renderer->active_textures, 0, sizeof(uint32_t) * SFR_MAX_TEXTURES_COUNT);
  _renderer->active_textures_count = 0;

  glGenVertexArrays(1, &_renderer->vao);
  glBindVertexArray(_renderer->vao);
  glGenBuffers(1, &_renderer->vbo);
  glGenBuffers(1, &_renderer->ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderer->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, _renderer->vbo);
  glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);         // vertex         (vec3)
  glEnableVertexAttribArray(1);         // uv coords      (vec2)
  glEnableVertexAttribArray(2);         // overlay colour (vec4)
  glEnableVertexAttribArray(3);         // texture id     (float)

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)(sizeof(float) * 3));
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)(sizeof(float) * 5));
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)(sizeof(float) * 9));

  glBindVertexArray(0);
}

void _sfr_renderer_flush() 
{
  if (_renderer->vertices != NULL) 
  {
    uint32_t shader = _renderer->active_shader;    

    // creating the index data
    uint32_t quad_count = _renderer->vertices_count / 40;
    uint32_t indices_count = quad_count * 6;
    uint32_t* indices = (uint32_t*)malloc(sizeof(uint32_t) * indices_count);
    int k = 0;
    for (uint32_t i = 0; i < indices_count; i += 6) 
    {
      indices[i + 0] = k + 0;
      indices[i + 1] = k + 1;
      indices[i + 2] = k + 2;
      indices[i + 3] = k + 0;
      indices[i + 4] = k + 3;
      indices[i + 5] = k + 2;
      k += 4;
    }

    // pushing the data to the GPU
    glBindVertexArray(_renderer->vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderer->ebo);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, 
      sizeof(float) * indices_count, indices, 
      GL_DYNAMIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, _renderer->vbo);
    glBufferData(
      GL_ARRAY_BUFFER, 
      sizeof(float) * _renderer->vertices_count, _renderer->vertices, 
      GL_DYNAMIC_DRAW
    );

    // TODO: add the view matrix
    uint32_t u_texture_count  = glGetUniformLocation(shader, "u_textures_count");
    uint32_t u_projection     = glGetUniformLocation(shader, "u_projection");
    uint32_t u_transform      = glGetUniformLocation(shader, "u_transform");
    uint32_t u_view           = glGetUniformLocation(shader, "u_view");

    // glUniform1i(u_texture_count, _renderer->active_textures_count);
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &_pipeline->projection[0][0]);
    glUniformMatrix4fv(u_transform,  1, GL_FALSE, &_pipeline->transform2D[0][0]);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &_pipeline->view[0][0]);

    // draw call
    glBindVertexArray(_renderer->vao);
    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, (const void*)0);

    // freeing the data
    _sfr_renderer_clear_buffers();
    free(indices);
  }
}

bool sfr_pipeline_cull_vertices(SFR_Vertex* vertices, uint32_t count)
{
  for (uint32_t i = 0; i < count; i++)
  {
    vec3 position;
    glm_vec3_copy(vertices[i].vertex, position);
    if (position[X] < _pipeline->culling_left[X] || position[X] > _pipeline->culling_right[X])
    {
      return true;
    }
    if (position[Y] < _pipeline->culling_left[Y] || position[Y] > _pipeline->culling_right[Y])
    {
      return true;
    }
  }

  return false;
}

void _sfr_renderer_increase_vertices_buffer(SFR_Vertex* vertices, uint32_t quad_count) 
{
  SAFIRE_ASSERT(vertices, "[SAFIRE::RENDERER_INCREASE_VERTICES] failed to push vertices as the vertices doesn't have any memory assigned to it");

  // increase buffer size
  if (_renderer->vertices != NULL) 
  {
    _renderer->vertices = (float*)realloc(
      _renderer->vertices, 
      sizeof(float) * (_renderer->vertices_count + (quad_count * SFR_VERTEX_SIZE * 4))
    );
    SAFIRE_ASSERT(_renderer->vertices, "[SAFIRE::RENDERER_INCREASE_VERTICES] failed to resize buffer for some reason ...");
  } 
  else 
  {
    _renderer->vertices = (float*)malloc(
      sizeof(float) * (_renderer->vertices_count + (quad_count * SFR_VERTEX_SIZE * 4))
    );
    SAFIRE_ASSERT(_renderer->vertices, "[SAFIRE::RENDERER_INCREASE_VERTICES] failed to assign memory to the buffer for some reason ...");
  }

  // setting the data
  uint32_t tr = 0;
  SFR_Vertex* v = vertices;
  for (uint32_t i = 0; i < quad_count; i++) 
  {
    uint32_t texture = 0; // defines the index of the texture in the texture sampler2D array in the shader
    // seeing what texture is being used
    bool found = false;
    for (uint32_t j = 0; j < _renderer->active_textures_count; j++) 
    {
      if (v[tr].texture_id == _renderer->active_textures[j]) 
      {
        found = true;
        texture = j;
        break;
      }
    }

    if (!found) 
    {
      // flush because we can't fit anymore textures in the buffer
      if (_renderer->active_textures_count == SFR_MAX_TEXTURES_COUNT) 
      {
        // flush and clear the buffers
        _sfr_renderer_flush();
        _sfr_renderer_clear_buffers();

        // create new buffer and continue adding the new stuff to that buffer now
        _renderer->vertices = (float*)malloc(
          sizeof(SFR_Vertex) * ((quad_count - i) * SFR_VERTEX_SIZE * 4)
        );
        SAFIRE_ASSERT(_renderer->vertices, "[SAFIRE::RENDERER_INCREASE_VERTICES] failed to assign memory to the buffer for some reason ...");

        // still want to push the texture id
        found = false;

        _renderer->active_textures_count = 0;
      }

      _renderer->active_textures[_renderer->active_textures_count] = (uint32_t)v[tr].texture_id;

      // should never be larger than 14 characters, largets: u_textures[10]
      char variable[14];
      sprintf(variable, "u_textures[%u]", _renderer->active_textures_count);

      glActiveTexture(GL_TEXTURE0 + _renderer->active_textures_count);
      glBindTexture(GL_TEXTURE_2D, (uint32_t)v[tr].texture_id);
      texture = _renderer->active_textures_count;

      uint32_t location = glGetUniformLocation(_renderer->active_shader, variable);
      glUniform1i(location, _renderer->active_textures_count);

      _renderer->active_textures_count++;
    }

    // converting the vertex data
    uint32_t tl = tr + 1;
    uint32_t bl = tr + 2;
    uint32_t br = tr + 3;
    float t = (float)texture;
    float quad[] = {
      // top right 
      v[tr].vertex[X], v[tr].vertex[Y], v[tr].vertex[Z], 
      v[tr].uv[X], v[tr].uv[Y], 
      v[tr].overlay_colour[R], v[tr].overlay_colour[G], v[tr].overlay_colour[B], v[tr].overlay_colour[A], 
      t,

      // top left
      v[tl].vertex[X], v[tl].vertex[Y], v[tl].vertex[Z], 
      v[tl].uv[X], v[tl].uv[Y], 
      v[tl].overlay_colour[R], v[tl].overlay_colour[G], v[tl].overlay_colour[B], v[tl].overlay_colour[A], 
      t,

      // bottom left
      v[bl].vertex[X], v[bl].vertex[Y], v[bl].vertex[Z], 
      v[bl].uv[X], v[bl].uv[Y], 
      v[bl].overlay_colour[R], v[bl].overlay_colour[G], v[bl].overlay_colour[B], v[bl].overlay_colour[A], 
      t,

      // bottom right
      v[br].vertex[X], v[br].vertex[Y], v[br].vertex[Z], 
      v[br].uv[X], v[br].uv[Y], 
      v[br].overlay_colour[R], v[br].overlay_colour[G], v[br].overlay_colour[B], v[br].overlay_colour[A], 
      t,
    };
    
    // pushing the quad vertex data to the vertices batch renderer buffer
    // TODO: probs will be better to change the for loop into memcpy 
    _renderer->vertices_count += 40;
    uint32_t k = 0;
    for (uint32_t j = _renderer->vertices_count - 40; j < _renderer->vertices_count; j++) 
    {
      _renderer->vertices[j] = quad[k];
      k++;
    }

    tr += 4;
  }
}

void _sfr_renderer_push_data(SFR_Vertex* vertices, uint32_t count, uint32_t shader) 
{
  SAFIRE_ASSERT(count != 0, "[SAFIRE::PIPELINE_RENDERER_PUSH_DATA] failed to push data to the buffer as the vertices count is set to 0");

  // everything in the buffer must be using the same shader
  if (shader != _renderer->active_shader) 
  {
    if (_renderer->vertices != NULL) 
    {
      _sfr_renderer_flush();
      _sfr_renderer_clear_buffers();
    }
    
    glUseProgram(shader);

    _renderer->active_shader = shader;
  }

  // increase and push the data to the batch renderer's buffers
  uint32_t quad_count = count * 0.25;
  _sfr_renderer_increase_vertices_buffer(vertices, quad_count);
}

void _sfr_renderer_clear_buffers() 
{
  if (_renderer->vertices != NULL) 
  {
    free(_renderer->vertices);

    _renderer->vertices = NULL;    
    _renderer->vertices_count = 0;
    
    _renderer->active_textures_count = 0;
    _renderer->active_shader = 0;
    
    glUseProgram(0);
  }
}

void _sfr_renderer_free() 
{
  glDeleteVertexArrays(1, &_renderer->vao);
  glDeleteBuffers(1, &_renderer->vbo);
  glDeleteBuffers(1, &_renderer->ebo);

  _sfr_renderer_clear_buffers();

  free(_renderer);
  _renderer = NULL;
}

SFR_Window* sfr_window(const char* window_title, int window_width, int window_height, bool fullscreen, bool transparent) 
{
  // tells glfw what version of glsl we are using
  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 5);

#ifdef __APPLE__
  // I have no clue what this is doing, I just know you have to add this if on Mac 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

  SFR_Window* window = (SFR_Window*)malloc(sizeof(SFR_Window));
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] something went wrong ...");

  window->title = window_title != NULL ? sfr_str(window_title) : NULL;
  window->fullscreen = fullscreen;
  window->transparent = transparent;

  if (window->transparent) 
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  else
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);

  // important for fullscreen and putting the window in the centre of the monitor on windows cause for some reason windows likes to not do that by default
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  if (window->fullscreen) 
  {
    window->width = vidmode->width;
    window->height = vidmode->height;
    window->window = glfwCreateWindow(window->width, window->height, window->title, monitor, NULL);
    SAFIRE_ASSERT(window->window, "[SAFIRE::PIPELINE_WINDOW] something went wrong ...");
  } 
  else 
  {
    window->width = window_width;
    window->height = window_height;
    window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);

    // centring the window
    ivec2 position;
    glfwGetMonitorPos(monitor, &position[X], &position[Y]);
    glfwSetWindowPos(
      window->window,
      position[X] + (vidmode->width - window->width) / 2,
      position[Y] + (vidmode->height - window->height) / 2
    );
  }

  glfwSetWindowAspectRatio(window->window, 16, 9);

  glfwMakeContextCurrent(window->window);
  SAFIRE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "[SAFIRE::PIPELINE_WINDOW] something when wrong when initialising glad ...");
  glfwSwapInterval(0); // by setting it to 0, it should disable vsync, this might cause screen tearing, but most likely it won't

  glViewport(0, 0, window->width, window->height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // this is some blend function, idk too much about this

  glm_vec4_copy((vec4){ 0.0f, 0.0f, 0.0f, 1.0f }, window->clear_colour);

  return window;
}

void sfr_window_free(SFR_Window* window) 
{
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to free window");
  sfr_str_free(&window->title);
  glfwDestroyWindow(window->window);
  free(window);
}

void sfr_window_set_clear(SFR_Window* window, vec4 clear_colour) 
{
  glm_vec4_copy(clear_colour, window->clear_colour);
}

void sfr_window_set_title(SFR_Window* window, const char* title) 
{
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to set title");
  SAFIRE_ASSERT(title, "[SAFIRE::PIPELINE_WINDOW] failed to set title");
  glfwSetWindowTitle(window->window, title);
  if (window->title != NULL) 
    sfr_str_free(&window->title);
  window->title = sfr_str(title);
}

void sfr_window_clear(SFR_Window* window) 
{
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to clear window");
  glClearColor(window->clear_colour[R], window->clear_colour[G], window->clear_colour[B], window->clear_colour[A]);
  glClear(GL_COLOR_BUFFER_BIT); // don't need to include the GL_DEPTH_BUFFER_BIT as it is a 2D renderer not a 3D renderer
  glfwGetFramebufferSize(window->window, &window->width, &window->height);
  glViewport(0, 0, window->width, window->height);
}

void sfr_window_swap_buffers(SFR_Window* window) 
{
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to swap buffers");
  glfwSwapBuffers(window->window);
}

bool sfr_window_closing(SFR_Window* window) 
{
  return (bool)glfwWindowShouldClose(window->window);
}

void sfr_window_quit(SFR_Window* window) 
{
  glfwSetWindowShouldClose(window->window, 0);
}

SFR_Shader* sfr_shader(const char* name, const char* vertex_path, const char* fragment_path) 
{
  const char* paths[] = { fragment_path, vertex_path };
  uint32_t shaders[2] = { 0, 0 };

  for (uint32_t i = 0; i < 2; i++) 
  {
    FILE* file = fopen(paths[i], "rb");
    if (file == NULL) 
    {
      printf("file at path '%s' doesn't exist\n", paths[i]);
      SAFIRE_ASSERT(file, "failed to open file");
      return NULL;
    }

    fseek(file, 0, SEEK_END);
    uint32_t length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* source = (char*)malloc(sizeof(char*) * length);
    source[length] = '\0';
    fread(source, length, 1, file);
    fclose(file);

    shaders[i] = glCreateShader(GL_FRAGMENT_SHADER + i);
    glShaderSource(shaders[i], 1, (const char* const*)&source, NULL);
    glCompileShader(shaders[i]);

    int result;
    glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &result);
    if (!result) 
    {
      glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &result);
      char* message = (char*)malloc(sizeof(char*) * result);
      glGetShaderInfoLog(shaders[i], result, &result, message);
      printf(
        "[SAFIRE::SHADER] failed to compile shader at path '%s', OpenGL Error:\n%s\n", 
        paths[i], message
      );

      if (i == 1) 
        glDeleteShader(shaders[0]);

      free(message);
      free(source);

#ifndef NDEBUG
      exit(-1);
#else
      return NULL;
#endif
    }

    free(source);
  }

  SFR_Shader* program = (SFR_Shader*)malloc(sizeof(SFR_Shader));
  program->id = glCreateProgram();
  glAttachShader(program->id, shaders[0]); // fragment shader
  glAttachShader(program->id, shaders[1]); // vertex shader
  glLinkProgram(program->id);

  glDeleteShader(shaders[0]);
  glDeleteShader(shaders[1]);

  int result;
  glGetProgramiv(program->id, GL_LINK_STATUS, &result);
  if (!result) 
  {
    glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &result);
    char* message = (char*)malloc(sizeof(char*) * result);
    glGetProgramInfoLog(program->id, result, &result, message);
    printf(
      "[SAFIRE::SHADER] failed to link shader program at path '%s' & '%s', OpenGL Error:\n%s\n", 
      paths[0], paths[1], message
    );

    free(message);

#ifndef NDEBUG
    exit(-1);
#else
    return NULL;
#endif
  }

  program->name = sfr_str(name);

  return program;
}

void sfr_shader_bind(SFR_Shader* shader) 
{
  glUseProgram(shader->id);
}

void sfr_shader_unbind() 
{
  glUseProgram(0);
}

void sfr_shader_free(SFR_Shader* shader) 
{
  if (shader != NULL) 
  {
    if (shader->id != 0) 
    {
      glDeleteProgram(shader->id);

      sfr_str_free(&shader->name);

      free(shader);
    }
  }
}

SFR_Texture* sfr_texture(const char* name, const char* path, bool flip, uint32_t channels, bool linear) 
{
  if (name != NULL) 
  {
    if (sfr_str_length(name) > 0) {

      if (path != NULL) {
        stbi_set_flip_vertically_on_load(flip);

        int width, height, channel;
        uint8_t* buffer = stbi_load(path, &width, &height, &channel, channels);

        if (buffer == NULL)
        {
          printf("[SAFIRE::PIPELINE_TEXTURE] failed to load texture at path '%s', probably because it doesn't exist", path);
          exit(-1);
        }

        uint32_t id = 0;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        if (linear) 
        {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        } 
        else 
        {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        uint32_t format = channels > 3 ? GL_RGBA : GL_RGB;
        glTexImage2D(
          GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        SFR_Texture* texture = (SFR_Texture*)malloc(sizeof(SFR_Texture));
        texture->name = sfr_str(name);
        texture->id = id;
        texture->width = (uint32_t)width;
        texture->height = (uint32_t)height;
        
        return texture;
      }
    }
  }

  return NULL;
}

void sfr_texture_bind(SFR_Shader* shader, SFR_Texture* texture, uint32_t id) 
{
  uint32_t location = glGetUniformLocation(shader->id, "textures");
  glActiveTexture(GL_TEXTURE0 + id);
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

void sfr_texture_unbind(uint32_t id) 
{
  glActiveTexture(GL_TEXTURE0 + id);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void sfr_texture_free(SFR_Texture* texture) 
{
  if (texture != NULL) 
  {
    if (texture->id != 0) 
    {
      glDeleteTextures(1, &texture->id);

      sfr_str_free(&texture->name);

      free(texture);
    }
  }
}
