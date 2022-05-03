#include "../include/safire/pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>




struct SFRpipeline {
  SFRshader_t**             shaders;
  uint32_t                  shaders_count;

  SFRtexture_t**            textures;
  uint32_t                  textures_count;

  SFRwindow_t*              window;

  mat4                      projection;
  int                       projection_mode;
  mat4                      transform2D;

  SFRrenderer_t*            renderer;
};

static SFRpipeline_t*       _pipeline = NULL;




struct SFRrenderer {
  float*                    vertices;
  uint32_t*                 indices;

  uint32_t                  vertices_count;
  uint32_t                  indices_count;

  uint32_t                  active_shader;

  uint32_t                  vao;
  uint32_t                  vbo;
  uint32_t                  ebo;
};

static SFRrenderer_t*       _renderer = NULL;                     // reference to _pipeline->renderer just that its called a lot

void                        _sfr_renderer_init();
void                        _sfr_renderer_flush();

void                        _sfr_renderer_push_data();
void                        _sfr_renderer_clear_buffers();

void                        _sfr_renderer_free();




void sfr_pipeline_init(const char* window_title, int window_width, int window_height, bool fullscreen) {
  _pipeline = (SFRpipeline_t*)malloc(sizeof(SFRpipeline_t));

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
  glm_mat4_identity(_pipeline->transform2D);
  glm_translate(_pipeline->transform2D, (vec3){ 0.0f, 0.0f, 0.0f });
  glm_scale(_pipeline->transform2D, (vec3){ 1.0f, 1.0f, 1.0f });
  glm_rotate(_pipeline->transform2D, 0.0f, (vec3){ 0.0f, 0.0f, 1.0f });

  // creating the default projection matrix
  sfr_pipeline_set_projection_matrix(SFR_PIPELINE_PROJECTION_ORTHOGRAPHIC);
}

SFRpipeline_t* sfr_pipeline_instance() {
  return _pipeline;
}

void sfr_pipeline_render() {
  _sfr_renderer_flush();
  
  sfr_window_swap_buffers(_pipeline->window);
  sfr_window_clear(_pipeline->window);  
}

bool sfr_pipeline_window_closing() {
  return sfr_window_closing(_pipeline->window);
}

void sfr_pipeline_free() {
  _sfr_renderer_free();
  _pipeline->renderer = NULL;

  sfr_window_free(_pipeline->window);
  
  free(_pipeline);
  _pipeline = NULL;
  glfwTerminate();
}

void sfr_pipeline_set_projection_matrix(int mode) {
  if (mode == SFR_PIPELINE_PROJECTION_PERSPECTIVE) {
    // TODO: add perspective mode for 3D games
    // pipeline->projection = glm_perspective();
    // pipeline->projection_mode = mode;
  } else if (mode == SFR_PIPELINE_PROJECTION_ORTHOGRAPHIC) {
    glm_ortho(
      -9.0f, 9.0f, -4.5f, 4.5f,
      -1.0f, 1.0f, _pipeline->projection
    );
    _pipeline->projection_mode = mode;
  } else {
    printf("[SAFIRE_WARNING::PIPELINE] projection matrix can either be orthographic or perspective\n");
  }
}

void sfr_pipeline_get_projection_matrix(mat4 projection) {

}

SFRshader_t** sfr_pipeline_get_shaders() {
  return NULL;
}

SFRtexture_t** sfr_pipeline_get_textures() {
  return NULL;
}

SFRwindow_t* sfr_pipeline_get_window() {
  return _pipeline->window;
}

SFRshader_t* sfr_pipeline_get_target_shader(const char* name) {
  return NULL;
}

SFRtexture_t* sfr_pipeline_get_target_texture(const char* texture) {
  return NULL;
}

void sfr_pipeline_push_shader(SFRshader_t* shader) {
  if (shader != NULL) {
    _pipeline->shaders_count++;

    if (_pipeline->shaders != NULL) {
      _pipeline->shaders = (SFRshader_t**)realloc(_pipeline->shaders, sizeof(SFRshader_t*) * _pipeline->shaders_count);
      SAFIRE_ASSERT(_pipeline->shaders, "[SAFIRE::PIPELINE_SHADER_PUSH] failed to resize shader buffer stack");
    } else {
      _pipeline->shaders = (SFRshader_t**)malloc(sizeof(SFRshader_t*) * _pipeline->shaders_count);
      SAFIRE_ASSERT(_pipeline->shaders, "[SAFIRE::PIPELINE_SHADER_PUSH] failed to assign memory to shader buffer stack");
    }

    _pipeline->shaders[_pipeline->shaders_count - 1] = shader;
  } else {
    printf("[SAFIRE_WARNING::PIPELINE_SHADER_PUSH] cannot push shader, as the shader doesn't exist");
  }
}

void sfr_pipeline_push_texture(SFRtexture_t* texture) {
  if (texture != NULL) {
    _pipeline->textures_count++;

    if (_pipeline->textures != NULL) {
      _pipeline->textures = (SFRtexture_t**)realloc(_pipeline->textures, sizeof(SFRtexture_t*) * _pipeline->textures_count);
      SAFIRE_ASSERT(_pipeline->textures, "[SAFIRE::PIPELINE_TEXTURE_PUSH] failed to resize texture buffer stack");
    } else {
      _pipeline->textures = (SFRtexture_t**)malloc(sizeof(SFRtexture_t*) * _pipeline->textures_count);
      SAFIRE_ASSERT(_pipeline->textures, "[SAFIRE::PIPELINE_TEXTURE_PUSH] failed to assign memory to texture buffer stack");
    }

    _pipeline->textures[_pipeline->textures_count - 1] = texture;
  } else {
    printf("[SAFIRE_WARNING::PIPELINE_TEXTURE_PUSH] cannot push texture, as the texture doesn't exist");
  }
}

void sfr_pipeline_clear_assets_stack() {
  if (_pipeline->shaders != NULL) {
    for (uint32_t i = 0; i < _pipeline->shaders_count; i++) {
      sfr_shader_free(_pipeline->shaders[i]);
    }
    free(_pipeline->shaders);
    _pipeline->shaders = NULL;
    _pipeline->shaders_count = 0;
  }

  if (_pipeline->textures != NULL) {
    for (uint32_t i = 0; i < _pipeline->textures_count; i++) {
      sfr_texture_free(_pipeline->textures[i]);
    }
    free(_pipeline->textures);
    _pipeline->textures = NULL;
    _pipeline->textures_count = 0;
  }
}

void sfr_pipeline_push_vertices(SFRvertex_t* vertices, uint32_t count, SFRshader_t* shader) {
  _sfr_renderer_push_data(vertices, count, shader);
}

void _sfr_renderer_init() {
  _pipeline->renderer = (SFRrenderer_t*)malloc(sizeof(SFRrenderer_t));
  _renderer = _pipeline->renderer;

  _renderer->vertices = NULL;
  _renderer->indices = NULL;
  _renderer->vertices_count = 0;
  _renderer->indices_count = 0;

  _renderer->active_shader = 0; // the index to the shader in the pipeline

  glGenVertexArrays(1, &_renderer->vao);
  glBindVertexArray(_renderer->vao);
  glGenBuffers(1, &_renderer->vbo);
  glGenBuffers(1, &_renderer->ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderer->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, _renderer->vbo);
  glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);         // vertices       (vec3)
  glEnableVertexAttribArray(1);         // uv coords      (vec2)
  glEnableVertexAttribArray(2);         // overlay colour (vec4)
  glEnableVertexAttribArray(3);         // texture coord  (float)

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)(sizeof(float) * 3));
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)(sizeof(float) * 5));
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)(sizeof(float) * 9));

  glBindVertexArray(0);
}

void _sfr_renderer_flush() {
  if (_renderer->vertices != NULL) {
    // shader and texture binding
    sfr_shader_bind(_pipeline->shaders[0]);
    sfr_texture_bind(_pipeline->shaders[0], _pipeline->textures[0], 0);

    uint32_t u_transform = glGetUniformLocation(_pipeline->shaders[0]->id, "u_transform");
    uint32_t u_projection = glGetUniformLocation(_pipeline->shaders[0]->id, "u_projection");

    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &_pipeline->projection[0][0]);
    glUniformMatrix4fv(u_transform, 1, GL_FALSE, &_pipeline->transform2D[0][0]);

    // calling draw call
    glBindVertexArray(_renderer->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);

    sfr_shader_unbind();
    sfr_texture_unbind(0);
  }
}

void _sfr_renderer_push_data(SFRvertex_t* vertices, uint32_t count, SFRshader_t* shader) {
  /* 
  example vertex data in float buffer: 
     float vertices[] = {
        (vertex)              (uv)           (overlay colour)        (texture id)
        1.0f,  1.0f, 0.0f,    1.0f, 1.0f,     1.0f,  1.0f, 0.0f,     0.0f,            (top      right)
        1.0f, -1.0f, 0.0f,    1.0f, 0.0f,     1.0f, -1.0f, 0.0f,     0.0f,            (bottom   right)
      -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,    -1.0f, -1.0f, 0.0f,     0.0f,             (bottom   left )
      -1.0f,  1.0f, 0.0f,    0.0f, 1.0f,    -1.0f,  1.0f, 0.0f,     0.0f              (top      left )
    };

  example of indices data for the current vertex buffer:
    uint32_t indices[] = { 
      0, 1, 3,              (first triangle )
      1, 2, 3               (second triangle)
    };
  */
}

void _sfr_renderer_clear_buffers() {
  if (_renderer->vertices != NULL) {
    free(_renderer->vertices);
    free(_renderer->indices);

    _renderer->vertices = NULL;
    _renderer->indices = NULL;
    
    _renderer->vertices_count = 0;
    _renderer->indices_count = 0;
  }
}

void _sfr_renderer_free() {
  glDeleteVertexArrays(1, &_renderer->vao);
  glDeleteBuffers(1, &_renderer->vbo);
  glDeleteBuffers(1, &_renderer->ebo);

  _sfr_renderer_clear_buffers();

  free(_renderer);
  _renderer = NULL;
}

SFRwindow_t* sfr_window(const char* window_title, int window_width, int window_height, bool fullscreen, bool transparent) {
  // tells glfw what version of glsl we are using
  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 5);

#ifdef __APPLE__
  // I have no clue what this is doing, I just know you have to add this if on Mac 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

  SFRwindow_t* window = (SFRwindow_t*)malloc(sizeof(SFRwindow_t));
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] something went wrong ...");

  window->title = window_title != NULL ? sfr_str(window_title) : NULL;
  window->fullscreen = fullscreen;
  window->transparent = transparent;

  if (window->transparent) {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  } else {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
  }

  // important for fullscreen and putting the window in the centre of the monitor on windows cause for some reason windows likes to not do that by default
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  if (window->fullscreen) {
    window->width = vidmode->width;
    window->height = vidmode->height;
    window->window = glfwCreateWindow(window->width, window->height, window->title, monitor, NULL);
    SAFIRE_ASSERT(window->window, "[SAFIRE::PIPELINE_WINDOW] something went wrong ...");
  } else {
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

  glfwMakeContextCurrent(window->window);
  SAFIRE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "[SAFIRE::PIPELINE_WINDOW] something when wrong when initialising glad ...");
  glfwSwapInterval(0); // by setting it to 0, it should disable vsync, this might cause screen tearing, but most likely it won't

  glViewport(0, 0, window->width, window->height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // this is some blend function, idk too much about this

  glm_vec4_copy((vec4){ 0.0f, 0.0f, 0.0f, 1.0f }, window->clear_colour);

  return window;
}

void sfr_window_free(SFRwindow_t* window) {
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to free window");
  sfr_str_free(&window->title);
  glfwDestroyWindow(window->window);
  free(window);
}

void sfr_window_set_clear(SFRwindow_t* window, vec4 clear_colour) {
  glm_vec4_copy(clear_colour, window->clear_colour);
}

void sfr_window_set_title(SFRwindow_t* window, const char* title) {
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to set title");
  SAFIRE_ASSERT(title, "[SAFIRE::PIPELINE_WINDOW] failed to set title");
  glfwSetWindowTitle(window->window, title);
  if (window->title != NULL) {
    sfr_str_free(&window->title);
  }
  window->title = sfr_str(title);
}

void sfr_window_clear(SFRwindow_t* window) {
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to clear window");
  glClearColor(window->clear_colour[R], window->clear_colour[G], window->clear_colour[B], window->clear_colour[A]);
  glClear(GL_COLOR_BUFFER_BIT); // don't need to include the GL_DEPTH_BUFFER_BIT as it is a 2D renderer not a 3D renderer
  glfwGetFramebufferSize(window->window, &window->width, &window->height);
  glViewport(0, 0, window->width, window->height);
}

void sfr_window_swap_buffers(SFRwindow_t* window) {
  SAFIRE_ASSERT(window, "[SAFIRE::PIPELINE_WINDOW] failed to swap buffers");
  glfwSwapBuffers(window->window);
}

bool sfr_window_closing(SFRwindow_t* window) {
  return (bool)glfwWindowShouldClose(window->window);
}

void sfr_window_quit(SFRwindow_t* window) {
  glfwSetWindowShouldClose(window->window, 0);
}

SFRshader_t* sfr_shader(const char* name, const char* vertex_path, const char* fragment_path) {
  const char* paths[] = { fragment_path, vertex_path };
  uint32_t shaders[2] = { 0, 0 };

  for (uint32_t i = 0; i < 2; i++) {
    FILE* file = fopen(paths[i], "rb");
    if (file == NULL) {
      printf("file at path '%s' doesn't exist\n", paths[i]);
      SAFIRE_ASSERT(file, "failed to open file");
      return NULL;
    }

    fseek(file, 0, SEEK_END);
    uint32_t length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* source = (char*)malloc(sizeof(char*) * length);
    fread(source, length, 1, file);
    fclose(file);

    shaders[i] = glCreateShader(GL_FRAGMENT_SHADER + i);
    glShaderSource(shaders[i], 1, (const char* const*)&source, NULL);
    glCompileShader(shaders[i]);

    int result;
    glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &result);
    if (!result) {
      glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &result);
      char* message = (char*)malloc(sizeof(char*) * result);
      glGetShaderInfoLog(shaders[i], result, &result, message);
      printf(
        "[SAFIRE::SHADER] failed to compile shader at path '%s', OpenGL Error:\n%s\n", 
        paths[i], message
      );

      if (i == 1) {
        glDeleteShader(shaders[0]);
      }

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

  SFRshader_t* program = (SFRshader_t*)malloc(sizeof(SFRshader_t));
  program->id = glCreateProgram();
  glAttachShader(program->id, shaders[0]); // fragment shader
  glAttachShader(program->id, shaders[1]); // vertex shader
  glLinkProgram(program->id);

  glDeleteShader(shaders[0]);
  glDeleteShader(shaders[1]);

  int result;
  glGetProgramiv(program->id, GL_LINK_STATUS, &result);
  if (!result) {
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

void sfr_shader_bind(SFRshader_t* shader) {
  glUseProgram(shader->id);
}

void sfr_shader_unbind() {
  glUseProgram(0);
}

void sfr_shader_free(SFRshader_t* shader) {
  if (shader != NULL) {
    if (shader->id != 0) {
      glDeleteProgram(shader->id);

      sfr_str_free(&shader->name);

      free(shader);
    }
  }
}

SFRtexture_t* sfr_texture(const char* name, const char* path, bool flip, uint32_t channels, bool linear) {
  if (name != NULL) {
    if (sfr_str_length(name) > 0) {
      if (path != NULL) {
        stbi_set_flip_vertically_on_load(flip);

        int width, height, channel;
        uint8_t* buffer = stbi_load(path, &width, &height, &channel, channels);

        SAFIRE_ASSERT(buffer, "[SAFIRE::PIPELINE_TEXTURE] failed to load texture, probably because it doesn't exist");

        uint32_t id = 0;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        if (linear) {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        } else {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        uint32_t format = channels > 3 ? GL_RGBA : GL_RGB;
        glTexImage2D(
          GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        SFRtexture_t* texture = (SFRtexture_t*)malloc(sizeof(SFRtexture_t));
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

void sfr_texture_bind(SFRshader_t* shader, SFRtexture_t* texture, uint32_t id) {
  uint32_t location = glGetUniformLocation(shader->id, "textures");
  glActiveTexture(GL_TEXTURE0 + id);
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

void sfr_texture_unbind(uint32_t id) {
  glActiveTexture(GL_TEXTURE0 + id);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void sfr_texture_free(SFRtexture_t* texture) {
  if (texture != NULL) {
    if (texture->id != 0) {
      glDeleteTextures(1, &texture->id);

      sfr_str_free(&texture->name);

      free(texture);
    }
  }
}
