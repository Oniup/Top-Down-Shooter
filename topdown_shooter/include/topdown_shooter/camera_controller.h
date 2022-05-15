#ifndef __TDS_CAMERA_CONTROLLER_H__
#define __TDS_CAMERA_CONTROLLER_H__

#include <safire/safire.h>

#define TDS_CAMERA_CONTROLLER           "camera controller"

typedef struct TDS_CameraController     TDS_CameraController;




struct TDS_CameraController 
{
  SFR_Transform*                        target;
};

SAFIRE_API SFR_Component*               tds_camera_controller();




#endif // __TDS_CAMERA_CONTROLLER_H__