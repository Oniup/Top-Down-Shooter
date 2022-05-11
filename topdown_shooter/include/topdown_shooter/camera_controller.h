#ifndef __TDS_CAMERA_CONTROLLER_H__
#define __TDS_CAMERA_CONTROLLER_H__

#include <safire/safire.h>

#define TDS_CAMERA_CONTROLLER "camera controller"

typedef struct TDScamera_controller TDScamera_controller_t;




struct TDScamera_controller {
  SFRtransform_t* target;
};

SAFIRE_API SFRcomponent_t*  tds_camera_controller();




#endif // __TDS_CAMERA_CONTROLLER_H__