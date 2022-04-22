#ifndef __SAFIRE_ECS_H__
#define __SAFIRE_ECS_H__

#include "utils.h"

typedef struct SFRecs SFRecs_t;
typedef struct SFRentity SFRentity_t;
typedef struct SFRcomponent SFRcomponent_t;

struct SFRecs {
  SFRentity_t** entities;
  uint32_t entities_count;
  SFRcomponent_t** components;
  uint32_t component_count;
};

void sfr_ecs_init();
void sfr_ecs_free();

SFRentity_t* sfr_ecs_push_entity(const char* name);
SFRentity_t* sfr_ecs_find_entity_name(const char* name);
SFRentity_t* sfr_ecs_find_entity_uuid(uint64_t uuid);

SFRcomponent_t* sfr_ecs_push_components(SFRentity_t* entity, SFRcomponent_t* component);
SFRcomponent_t** sfr_ecs_find_list_components_name(const char* name);

#endif // __SAFIRE_ECS_H__