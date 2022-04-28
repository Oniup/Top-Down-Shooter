#include "../include/safire/components.h"

#include "../include/safire/ecs.h"

/*
  add more components that will be added to every entity when it is created
*/
SFRcomponent_t** sfr_attach_default_comps(SFRentity_t* entity, uint32_t* count) {
  *count = 1; // change this to the correct number of default components
  SFRcomponent_t** components = (SFRcomponent_t**)malloc(sizeof(SFRcomponent_t*) * *count);

  // list of default components
  components[0] = sfr_ecs_push_component(entity, sfr_transform()); // really shouldn't remove this component 

  return components;
}
