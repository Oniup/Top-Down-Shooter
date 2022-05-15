/**
 * NOTE:  This isn't how I would load scenes if I had more time.
 *        I would use a custom file type like .scene and have the 
 *        opening data in there.
*/

#ifndef __SAFIRE_SCENE_START_MANAGER_H__
#define __SAFIRE_SCENE_START_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <safire/safire.h>




SAFIRE_API SFR_Scene**                  sfr_get_scenes(uint32_t* count);




#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SAFIRE_SCENE_START_MANAGER_H__