#include "../include/topdown_shooter/destroy_target.h"



void                                    _tds_destroy_target_update(SFR_Component* component, float delta_time);




SFR_Component* tds_destroy_target(float timer_length)
{
  SFR_Component* component = sfr_ecs_component(
    TDS_DESTROY_TARGET, _tds_destroy_target_update, NULL, NULL, NULL
  );

  component->data = (TDS_DestroyTarget*)malloc(sizeof(TDS_DestroyTarget));
  TDS_DestroyTarget* destroy_target = SFR_COMPONENT_CONVERT(TDS_DestroyTarget, component);

  destroy_target->timer_started = false;
  destroy_target->timer = timer_length;

  return component;
}

void tds_destroy_target_start(SFR_Component* component)
{
  TDS_DestroyTarget* destroy_target = SFR_COMPONENT_CONVERT(TDS_DestroyTarget, component);
  destroy_target->timer = sfr_timer_start(destroy_target->timer);
  destroy_target->timer_started = true;
}




void _tds_destroy_target_update(SFR_Component* component, float delta_time)
{
  TDS_DestroyTarget* destroy_target = SFR_COMPONENT_CONVERT(TDS_DestroyTarget, component);
  if (destroy_target->timer_started)
  {
    if (sfr_timer_finished(&destroy_target->timer))
    {
      uint32_t index = sfr_ecs_entity_find_index_uuid(0, component->owner->uuid);
      sfr_ecs_erase_entity(index);
    }
  }
}
