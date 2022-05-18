#ifndef __TDS_TILE_H__
#define __TDS_TILE_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include <safire/safire.h>

#define TDS_TILE                        "tile"

typedef struct TDS_Tile                 TDS_Tile;




SAFIRE_API SFR_Component*               tds_tile(vec2 location);




#ifdef __cplusplus
}
#endif 

#endif // __TDS_TILE_H__