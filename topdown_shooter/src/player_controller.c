#include <topdown_shooter/player_controller.h>

void tds_player_controller_update(SFRcomponent_t* component, float delta_time);
void tds_player_controller_free(SFRcomponent_t* component);

SFRcomponent_t* tds_attach_player_controller(SFRentity_t* entity) {
  SFRcomponent_t* comp = sfr_ecs_component(
    TDS_PLAYER_CONTROLLER, tds_player_controller_update, NULL, tds_player_controller_free
  );
  return NULL;
}

void tds_player_damage(SFRcomponent_t* player_controller_comp, uint32_t damage) {

}

void tds_player_controller_update(SFRcomponent_t* component, float delta_time) {

}

void tds_player_controller_free(SFRcomponent_t* component) {

}

