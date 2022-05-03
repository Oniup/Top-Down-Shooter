#include "../include/safire/pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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

