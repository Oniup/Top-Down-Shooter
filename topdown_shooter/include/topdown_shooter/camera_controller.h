#ifndef __TDS_CAMERA_CONTROLLER_H__
#define __TDS_CAMERA_CONTROLLER_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include <safire/safire.h>

#define TDS_CAMERA_CONTROLLER           "camera controller"

typedef struct TDS_CameraController     TDS_CameraController;




struct TDS_CameraController 
{
  SFR_Transform*                        player;
  SFR_Transform*                        cursor;
};

SAFIRE_API SFR_Component*               tds_camera_controller();



#ifdef __cplusplus
}
#endif 

#endif // __TDS_CAMERA_CONTROLLER_H__