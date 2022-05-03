#include "../include/safire/pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct SFRpipeline {
  SFRshader_t**             shaders;
  uint32_t                  shaders_count;

  SFRtexture_t**            textures;
  uint32_t                  textures_count;

  SFRwindow_t*              window;

  mat4                      projection;
  int                       projection_mode;
  mat4                      transform2D;

  uint32_t                  vao;
  uint32_t                  vbo;
  uint32_t                  ebo;
};

static SFRpipeline_t* pipeline = NULL;

void sfr_pipeline_init(const char* window_title, int window_width, int window_height, bool fullscreen) {
  pipeline = (SFRpipeline_t*)malloc(sizeof(SFRpipeline_t));

  SAFIRE_ASSERT(glfwInit(), "[SAFIRE::PIPELINE] failed to initialise glfw for some reason ...");

  pipeline->window = sfr_window_create(window_title, window_width, window_height, fullscreen, false);
  pipeline->shaders = NULL;
  pipeline->textures = NULL;
  pipeline->shaders_count = 0;
  pipeline->textures_count = 0;

  // create vertex buffers
    float vertices[] = {
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f,  // top right
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f   // top left 
  };

  uint32_t indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };

  glGenVertexArrays(1, &pipeline->vao);
  glBindVertexArray(pipeline->vao);
  glGenBuffers(1, &pipeline->vbo);
  glGenBuffers(1, &pipeline->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pipeline->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, pipeline->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(sizeof(float) * 3));

  sfr_pipeline_push_shader(
    sfr_shader(
      "shader", 
      "./safire/shaders/sfr_default_shader.vert", "./safire/shaders/sfr_default_shader.frag"
    )
  );

  sfr_pipeline_push_texture(
    sfr_texture(
      "test_texture", "./safire/textures/test.png", false, 4, true
    )
  );

  // creating the global transform matrix for 2D games
  glm_mat4_identity(pipeline->transform2D);
  glm_translate(pipeline->transform2D, (vec3){ 0.0f, 0.0f, 0.0f });
  glm_scale(pipeline->transform2D, (vec3){ 1.0f, 1.0f, 1.0f });
  glm_rotate(pipeline->transform2D, 0.0f, (vec3){ 0.0f, 0.0f, 1.0f });

  // creating the default projection matrix
  sfr_pipeline_set_projection_matrix(SFR_PIPELINE_PROJECTION_ORTHOGRAPHIC);
}

SFRpipeline_t* sfr_pipeline_instance() {
  return pipeline;
}

void sfr_pipeline_render() {
  // shader and texture binding
  sfr_shader_bind(pipeline->shaders[0]);
  sfr_texture_bind(pipeline->shaders[0], pipeline->textures[0], 0);

  uint32_t u_transform = glGetUniformLocation(pipeline->shaders[0]->id, "u_transform");
  uint32_t u_projection = glGetUniformLocation(pipeline->shaders[0]->id, "u_projection");

  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &pipeline->projection[0][0]);
  glUniformMatrix4fv(u_transform, 1, GL_FALSE, &pipeline->transform2D[0][0]);

  // calling draw call
  glBindVertexArray(pipeline->vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);

  sfr_shader_unbind();
  sfr_texture_unbind(0);
  
  sfr_window_swap_buffers(pipeline->window);
  sfr_window_clear(pipeline->window);  
}

bool sfr_pipeline_window_closing() {
  return sfr_window_closing(pipeline->window);
}

void sfr_pipeline_free() {
  glDeleteVertexArrays(1, &pipeline->vao);
  glDeleteBuffers(1, &pipeline->vbo);
  glDeleteBuffers(1, &pipeline->ebo);

  

  sfr_window_free(pipeline->window);
  
  free(pipeline);
  pipeline = NULL;
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
      -1.0f, 1.0f, pipeline->projection
    );
    pipeline->projection_mode = mode;
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
  return pipeline->window;
}

void sfr_pipeline_push_shader(SFRshader_t* shader) {
  if (shader != NULL) {
    pipeline->shaders_count++;

    if (pipeline->shaders != NULL) {
      pipeline->shaders = (SFRshader_t**)realloc(pipeline->shaders, sizeof(SFRshader_t*) * pipeline->shaders_count);
      SAFIRE_ASSERT(pipeline->shaders, "[SAFIRE::PIPELINE_SHADER_PUSH] failed to resize shader buffer stack");
    } else {
      pipeline->shaders = (SFRshader_t**)malloc(sizeof(SFRshader_t*) * pipeline->shaders_count);
      SAFIRE_ASSERT(pipeline->shaders, "[SAFIRE::PIPELINE_SHADER_PUSH] failed to assign memory to shader buffer stack");
    }

    pipeline->shaders[pipeline->shaders_count - 1] = shader;
  } else {
    printf("[SAFIRE_WARNING::PIPELINE_SHADER_PUSH] cannot push shader, as the shader doesn't exist");
  }
}

void sfr_pipeline_push_texture(SFRtexture_t* texture) {
  if (texture != NULL) {
    pipeline->textures_count++;

    if (pipeline->textures != NULL) {
      pipeline->textures = (SFRtexture_t**)realloc(pipeline->textures, sizeof(SFRtexture_t*) * pipeline->textures_count);
      SAFIRE_ASSERT(pipeline->textures, "[SAFIRE::PIPELINE_TEXTURE_PUSH] failed to resize texture buffer stack");
    } else {
      pipeline->textures = (SFRtexture_t**)malloc(sizeof(SFRtexture_t*) * pipeline->textures_count);
      SAFIRE_ASSERT(pipeline->textures, "[SAFIRE::PIPELINE_TEXTURE_PUSH] failed to assign memory to texture buffer stack");
    }

    pipeline->textures[pipeline->textures_count - 1] = texture;
  } else {
    printf("[SAFIRE_WARNING::PIPELINE_TEXTURE_PUSH] cannot push texture, as the texture doesn't exist");
  }
}

void sfr_pipeline_clear_assets_stack() {
  if (pipeline->shaders != NULL) {
    for (uint32_t i = 0; i < pipeline->shaders_count; i++) {
      sfr_shader_free(pipeline->shaders[i]);
    }
    free(pipeline->shaders);
    pipeline->shaders = NULL;
    pipeline->shaders_count = 0;
  }

  if (pipeline->textures != NULL) {
    for (uint32_t i = 0; i < pipeline->textures_count; i++) {
      sfr_texture_free(pipeline->textures[i]);
    }
    free(pipeline->textures);
    pipeline->textures = NULL;
    pipeline->textures_count = 0;
  }
}
