#include <topdown_shooter/player_controller.h>

void tds_player_controller_update(SFRcomponent_t* component, float delta_time);

SFRcomponent_t* tds_player_controller() {
  SFRcomponent_t* component = sfr_ecs_component(
    TDS_PLAYER_CONTROLLER, tds_player_controller_update, NULL, NULL
  );

  component->data = (TDSplayer_controller_t*)malloc(sizeof(TDSplayer_controller_t));
  TDSplayer_controller_t* player_controller = ((TDSplayer_controller_t*)component->data);

  player_controller->damage = 1;
  player_controller->move_speed = 5.0f;

  return component;
}

void tds_player_damage(SFRcomponent_t* player_controller_comp, uint32_t damage) {

}

void tds_player_controller_update(SFRcomponent_t* component, float delta_time) {
  vec2 direction = { 0, 0 };
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

  // printf("movement [%f, %f]\n", direction[X], direction[Y]);
}
