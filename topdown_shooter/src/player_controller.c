#include <topdown_shooter/player_controller.h>

void _tds_player_controller_update(SFRcomponent_t* component, float delta_time);

void _tds_trigger_check(SFRcomponent_t* component);




SFRcomponent_t* tds_player_controller() {
  SFRcomponent_t* component = sfr_ecs_component(
    TDS_PLAYER_CONTROLLER, _tds_player_controller_update, NULL, NULL
  );

  component->data = (TDSplayer_controller_t*)malloc(sizeof(TDSplayer_controller_t));
  TDSplayer_controller_t* player_controller = sfr_component_convert(TDSplayer_controller_t, component);

  player_controller->damage = 1;
  player_controller->move_speed = 5.0f;

  return component;
}

void tds_player_damage(SFRcomponent_t* player_controller_comp, uint32_t damage) {

}

void _tds_player_controller_update(SFRcomponent_t* component, float delta_time) {
  vec3 direction = { 0, 0, 0 };
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_A)) {
    direction[X] = -1;
  } 
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_D)) {
    direction[X] = 1;
  } 
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_S)) {
    direction[Y] = -1;
  } 
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_W)) {
    direction[Y] = 1;
  } 


  SFRtransform_t* transform = sfr_component_convert(SFRtransform_t, component->owner->components[0]);
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_UP)) {
    transform->scale[Y] *= 0.9999f;
  } else if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_DOWN)) {
    transform->scale[Y] *= 1.0001f;
  } else if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_LEFT)) {
    transform->rotation[W] += 1.0f * delta_time;
  } else if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_RIGHT)) {
    transform->rotation[W] -= 1.0f * delta_time;
  }


  if (direction[X] != 0 || direction[Y] != 0) {
    TDSplayer_controller_t* controller = ((TDSplayer_controller_t*)component->data);

    static bool flipped_x = false;
    // printf("direction: %f, %f   :   %d", direction[X], direction[Y], (int)flipped_x);
    // printf("   :   transform: %f, %f\n", transform->scale[X], transform->scale[Y]);

    if (direction[X] < -0.0f && !flipped_x) {
      transform->scale[X] = -transform->scale[X];
      flipped_x = true;
    } else if (direction[X] > 0.0f && flipped_x) {
      transform->scale[X] = -transform->scale[X];
      flipped_x = false;
    }
    
    glm_vec3_scale(direction, controller->move_speed * delta_time, direction);
    glm_vec3_add(direction, transform->position, transform->position);

  }

  _tds_trigger_check(component);
}

void _tds_trigger_check(SFRcomponent_t* component) {
  bool result = sfr_collider2d_trigger_enter_tag(component, "enemy");
  if (result) {
    static uint32_t count = 0;
    count++;
    printf("[%u] is in trigger collider\n", count);
  }
}
