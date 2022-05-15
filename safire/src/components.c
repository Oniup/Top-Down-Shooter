#include "../include/safire/components.h"

#include "../include/safire/ecs.h"




SFR_Component* sfr_get_component(SFR_Component* component, const char* name) 
{
  SAFIRE_ASSERT(component, "[SAFIRE::GET_CORRECT_COMPONENT] failed to find the correct type as the component passed doesn't have any memory assigned to it...");
  SAFIRE_ASSERT(name, "[SAFIRE::GET_CORRECT_COMPONENT] failed to find the correct type as the name passed doesn't have any memory assigned to it...");

  uint32_t length = sfr_str_length(name);
  if (length > 0) 
  {
    for (uint32_t i = 0; i < component->owner->components_count; i++) 
    {
      SFR_Component* target = component->owner->components[i];
      if (sfr_str_cmp_length(name, target->name, length)) 
      {
        return target;
      }
      
    }
  } 
  else 
  {
    printf("[SAFIRE::GET_CORRECT_COMPONENT] failed to find the correct type as the name's length is 0 ...\n");
  }

  return NULL;
}
