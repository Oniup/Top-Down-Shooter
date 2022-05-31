#include "../include/topdown_shooter/tile.h"




void                                    _tds_tile_late_update(SFR_Component* component, float late_delta_time);




SFR_Component* tds_tile(vec2 location) 
{
  SFR_Component* component = sfr_ecs_component(
    TDS_TILE, NULL, _tds_tile_late_update, NULL, NULL
  );

  return component;
}

void _tds_tile_late_update(SFR_Component* component, float late_delta_time) 
{
  
}