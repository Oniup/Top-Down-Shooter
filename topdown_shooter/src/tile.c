#include "../include/topdown_shooter/tile.h"

void _tds_tile_late_update(SFRcomponent_t* component, float late_delta_time);

SFRcomponent_t* tds_tile(vec2 location) {
  SFRcomponent_t* component = sfr_ecs_component(
    TDS_TILE, NULL, _tds_tile_late_update, NULL
  );

  return component;
}

void _tds_tile_late_update(SFRcomponent_t* component, float late_delta_time) {
  
}