#ifndef __SAFIRE_UTILS_H__
#define __SAFIRE_UTILS_H__

#ifdef __cplusplus 
extern "C" {
#endif

// this should work on mac, I think, I've never tried it, also I have no idea what shared and static library file extention it use
#if defined(_WIN32) && defined(SAFIRE_BUILD_DLL)  
  // build a shared library for windows (.dll)
# define SAFIRE_API __declspec(dllexport)
#elif defined(_WIN32) && defined(SAFIRE_DLL)
  // using the safire.dll/.so for the program
# define SAFIRE_API __declspec(dllimport)
# define GLFW_DLL // so we can use the shared glfw libray
#elif defined(__GNUC__) && defined(SAFIRE_BUILD_DLL)
  // build shared library for linux (.so)
# define SAFIRE_API __attribute__((visibility("default")))
#else
 // build static safire library (.lib/.a) 
 // NOTE: this is the one you really should be using
# define SAFIRE_API extern
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>

#include <cglm/cglm.h>

#define X 0
#define Y 1
#define Z 2
#define W 3

#define R 0
#define G 1
#define B 2
#define A 3




#if !defined(NDEBUG) || defined(__MINGW32__) || !defined(__WIN32)

// for some reason MSVC doesn't like this syntax
#define SFR_COMPONENT_CONVERT(Ty, component) ({\
  SAFIRE_ASSERT(component != NULL, "[SAFIRE::COMPONENT_CONVERT]: failed to convert component custom data as the component given doesn't exist");\
  Ty* result = ((Ty*)component->data);\
  SAFIRE_ASSERT(result != NULL, "[SAFIRE::COMPONENT_CONVERT]: failed to convert component custom data as the component's custom data isn't the target type");\
  result;\
})\

#else

#define SFR_COMPONENT_CONVERT(Ty, component) ((Ty*)component->data);

#endif




typedef float                           SFR_Timer;

SAFIRE_API SFR_Timer                    sfr_timer_start(float duration);
SAFIRE_API void                         sfr_timer_add_time(SFR_Timer* timer, float additional);
SAFIRE_API bool                         sfr_timer_finished(SFR_Timer* timer);




SAFIRE_API void                         sfr_clear_terminal();
SAFIRE_API void                         sfr_qsort_uint32(uint32_t* arr, uint32_t size);



typedef struct SFR_Transform            SFR_Transform;

SAFIRE_API float                        sfr_look_at_target(vec2 position, vec2 target);
SAFIRE_API float                        sfr_look_at_target_flip(SFR_Transform* transform, vec2 target, bool* flip);




/* ==================== the following is from safire-utils.h =================== */
/* ================= https://github.com/Oniup/Safire-Utils.git ================= */




#ifndef NDEBUG
# include <assert.h>
# define SAFIRE_ASSERT(x, message)            assert(x && message)
# define SAFIRE_ASSERT2(x, y, message)        assert(x && y && message)
# define SAFIRE_ASSERT3(x, y, z, message)     assert(x && y && z && message)
# define SAFIRE_ASSERT4(x, y, z, w, message)  assert(x && y && z && w && message)
#else
# define SAFIRE_ASSERT(x, message)
# define SAFIRE_ASSERT2(x, y, message)
# define SAFIRE_ASSERT3(x, y, z, message)
# define SAFIRE_ASSERT4(x, y, z, w, message)
#endif // NDEBUG




SAFIRE_API int                          sfr_rand_int32();
SAFIRE_API uint32_t                     sfr_rand_uint32();
SAFIRE_API uint64_t                     sfr_rand_uint64();




/*!
 * @brief allocates memory and copies over the str to the new string
 * @param[in] str desired string to be copied into new string
 * @returns (char*) a new c style string
*/
SAFIRE_API char*                        sfr_str(const char* str);
/*!
 * @brief allocates uninitialised memory to the char* buffer
 * @param[in] length the desired length that will be allocated
 * @returns (char*) a new c style string of uninitialised memory
*/
SAFIRE_API char*                        sfr_str_alloc(uint32_t length);
/*!
 * @brief allocates and copies the desired string to the destination string
 * @param[in] src string to be copied into the destired string
 * @param[in] dest the desired string that will have src message copied over to
 * @returns (uint32_t) the length of the string that was copied over to the dest
*/
SAFIRE_API uint32_t                     sfr_str_set(char* dest, const char* src);
/*!
 * @brief allocates and copies the desired string to the destination string
 * @param[in] src string to be copied into the destired string
 * @param[in] dest the desired string that will have src message copied over to
 * @param[in] length the length of the src string, optimisation over 'sfr_str_set' if known length
*/
SAFIRE_API void                         sfr_str_set_length(char* dest, const char* src, uint32_t length);
/*!
 * @brief returns the length of a string
 * @param[in] src desired string to find the length of
 * @returns (uint32_t ) the length of the string
*/
SAFIRE_API uint32_t                     sfr_str_length(const char* src);
/*!
 * @brief returns true if the two strings are the same
 * @param[in] str1 the string that will check if same as str2
 * @param[in] str2 the string that will check if same as str1
 * @returns (bool) true if the two strings are the same
*/
SAFIRE_API bool                         sfr_str_cmp(const char* str1, const char* str2);
/*!
 * @brief returns true if the two strings are the same
 * @param[in] str1 the string that will check if same as str2
 * @param[in] str2 the string that will check if same as str1
 * @param[in] length the length of both strings, optimisation over 'str_strcmp' if known lengths are the same
 * @returns (bool) true if the two strings are the same
*/
SAFIRE_API bool                         sfr_str_cmp_length(const char* str1, const char* str2, uint32_t length);
/*!
 * @brief frees the strings memory and sets the strings pointer to NULL
 * @param[in] src desired string to be freed
*/
SAFIRE_API void                         sfr_str_free(char** str);




#ifdef __cplusplus 
}
#endif

#endif // __SAFIRE_UTILS_H__