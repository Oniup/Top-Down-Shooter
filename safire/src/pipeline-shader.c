#include "../include/safire/pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
