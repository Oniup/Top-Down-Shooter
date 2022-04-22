#ifndef __SAFIRE_PIPELINE_H__
#define __SAFIRE_PIPELINE_H__

#include "utils.h"

#include <cglm/cglm.h>

typedef struct SFRpipeline SFRpipeline_t;

typedef struct SFRshader SFRshader_t;
typedef struct SFRtexture SFRtexture_t;
typedef struct SFRvertex SFRvertex_t;

extern void sfr_pipeline_init();
extern void sfr_pipeline_free();

extern void sfr_pipeline_push_shader(const char* name, const char* vertex_path, const char* fragment_path);
extern void sfr_pipeline_push_texture(const char* name, const char* path, bool flip, uint32_t channels);

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