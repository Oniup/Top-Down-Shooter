#include "../include/topdown_shooter/dash.h"




void                                    _tds_dash_update(SFR_Component* component, float delta_time);




SFR_Component* tds_dash(float duration, float cool_down_duration, float speed)
{
  SFR_Component* component = sfr_ecs_component(TDS_DASH, _tds_dash_update, NULL, NULL, NULL);

  component->data = (TDS_Dash*)malloc(sizeof(TDS_Dash));
  TDS_Dash* dash = SFR_COMPONENT_CONVERT(TDS_Dash, component);

  dash->duration = duration;
  dash->cool_down_duration = cool_down_duration;
  dash->speed = speed;
  glm_vec2_copy((vec2){ 0.0f, 0.0f }, dash->direction);
  dash->is_dashing = false;
  dash->timer = 0.0f;
  dash->trail_life_time = 0.0f;
  dash->trail_texture = NULL;
  dash->trail_instantiate = NULL;

  return component;
}

void tds_set_trail(SFR_Component* component, tds_trail_instantiate trail_instantiate, SFR_Texture* trail_texture, float trail_life_time)
{
  component = sfr_get_component(component, TDS_DASH);
  TDS_Dash* dash = SFR_COMPONENT_CONVERT(TDS_Dash, component);

  if (trail_instantiate != NULL)
  {
    dash->trail_instantiate = trail_instantiate;
    dash->trail_texture = trail_texture;
    dash->trail_life_time = trail_life_time;
  }
}

bool tds_enable_dash(SFR_Component* component,  vec2 direction)
{
  component = sfr_get_component(component, TDS_DASH);
  TDS_Dash* dash = SFR_COMPONENT_CONVERT(TDS_Dash, component);

  // checking if the cool down has finished
  if (sfr_timer_finished(&dash->timer))
  {
    dash->is_dashing = true;
    glm_vec2_copy(direction, dash->direction);

    dash->timer = sfr_timer_start(dash->duration);

    return true;
  }

  return false;
}

void _tds_dash_update(SFR_Component* component, float delta_time)
{
  TDS_Dash* dash = SFR_COMPONENT_CONVERT(TDS_Dash, component);
  if (dash->is_dashing)
  {
    if (sfr_timer_finished(&dash->timer))
    {
      dash->timer = sfr_timer_start(dash->cool_down_duration);
      dash->is_dashing = false;
      return;
    }

    SFR_Transform* transform = SFR_COMPONENT_CONVERT(SFR_Transform, component->owner->components[0]);
    vec2 direction;
    glm_vec2_scale(dash->direction, dash->speed * delta_time, direction);
    glm_vec2_add(transform->position, direction, transform->position);

    if (dash->trail_instantiate != NULL)
      dash->trail_instantiate(dash->trail_texture, dash->trail_life_time);
  }
}
