#include "../include/safire/components.h"
#include "../include/safire/ecs.h"

/*
  add more components that will be added to every entity when it is created
*/
void sfr_ecs_attach_default_comps(SFRentity_t* entity) {
  // list of default components
  sfr_ecs_push_component(entity, sfr_transform()); // really shouldn't remove this component 
}
