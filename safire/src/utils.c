#include "../include/safire/utils.h"

#include "../include/safire/components.h"

#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>




void                                    _sfr_qsort_uint32_swap(uint32_t* a, uint32_t* b);
int                                     _sfr_qsort_uint32_partition(uint32_t* arr, int lower, int higher);
void                                    _sfr_qsort_uint32(uint32_t* arr, int lower, int higher);




SFR_Timer sfr_timer_start(float duration) 
{
  SFR_Timer timer = (float)glfwGetTime() + duration;
  return timer;
}

void sfr_timer_add_time(SFR_Timer* timer, float additional) 
{
  *timer += additional;
}

bool sfr_timer_finished(SFR_Timer* timer) 
{
  float current_time = (float)glfwGetTime();
  if (current_time > *timer) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}

void sfr_clear_terminal() 
{
#ifndef WIN32
  system("clear");
#else
  system("cls");
#endif
}




void _sfr_qsort_uint32_swap(uint32_t* a, uint32_t* b) 
{
  uint32_t temp = *a;
  *a = *b;
  *b = temp;
}

int _sfr_qsort_uint32_partition(uint32_t* arr, int lower, int higher) 
{
  uint32_t pivot = arr[higher];
  int i = (lower - 1);
  for (int j = lower; j < higher; j++) 
  {
    if (arr[j] <= pivot) 
    {
      i++;
      _sfr_qsort_uint32_swap(&arr[i], &arr[j]);
    }
  }
  _sfr_qsort_uint32_swap(&arr[i + 1], &arr[higher]);
  return (i + 1);
}

void _sfr_qsort_uint32(uint32_t* arr, int lower, int higher) 
{
  if (lower < higher) 
  {
    int pi  =_sfr_qsort_uint32_partition(arr, lower, higher);

    // recursive call on the left of the array
    _sfr_qsort_uint32(arr, lower, pi - 1);
    _sfr_qsort_uint32(arr, pi + 1, higher);
  }
}

void sfr_qsort_uint32(uint32_t* arr, uint32_t size) 
{
  _sfr_qsort_uint32(arr, 0, (int)size - 1);
}




float sfr_look_at_target(vec2 position, vec2 target) 
{
  vec2 direction;
  glm_vec2_sub(target, position, direction);
  float angle = 0.0f;

  if (direction[X] < 0.0f && direction[Y] < 0.0f) 
  {
    angle = -atanf(direction[X] / direction[Y]) - 1.5708f;
  } 
  else if (direction[X] >= 0.0f) 
  {
    float mag = sqrtf(glm_vec2_dot(direction, direction));
    angle = asinf(direction[Y] / mag);
  } 
  else 
  {
    float mag = sqrtf(glm_vec2_dot(direction, direction));
    angle = acosf(direction[X] / mag);
  }

  return angle;
}

float sfr_look_at_target_flip(SFR_Transform* transform, vec2 target, bool* flip) 
{
  float angle = sfr_look_at_target(transform->position, target);

  if (angle < 1.5286f && angle > -1.616f && *flip) 
  {
    transform->scale[Y] = -transform->scale[Y];
    *flip = false;
  } 
  else if (angle <= -1.616f && !*flip) 
  {
    transform->scale[Y] = -transform->scale[Y];
    *flip = true;
  } 
  else if (angle >= 1.5286f && !*flip) 
  {
    transform->scale[Y] = -transform->scale[Y];
    *flip = true;
  }

  if (*flip) 
    angle = -angle;

  return angle;
}




int sfr_rand_int32() 
{
  time_t t1; 
  srand ((unsigned)time(&t1));
  // https://stackoverflow.com/questions/7622887/generating-a-random-32-bit-hexadecimal-value-in-c
  int r = rand() & 0xff;
  r |= (rand() & 0xff) << 8;
  r |= (rand() & 0xff) << 16;
  r |= (rand() & 0xff) << 24;
  
  return r;
}

uint32_t sfr_rand_uint32() 
{
  time_t t1; 
  srand ((unsigned)time(&t1));
  uint32_t r = (unsigned)rand() & 0xff;
  r |= ((unsigned)rand() & 0xff) << 8;
  r |= ((unsigned)rand() & 0xff) << 16;
  r |= ((unsigned)rand() & 0xff) << 24;

  return r;
}

// https://stackoverflow.com/questions/33010010/how-to-generate-random-64-bit-unsigned-integer-in-c
#define IMAX_BITS(m) ((m)/((m)%255+1) / 255%255*8 + 7-86/((m)%255+12))
#define RAND_MAX_WIDTH IMAX_BITS(RAND_MAX)
uint64_t sfr_rand_uint64(void) 
{
  uint64_t r = 0;
  for (int i = 0; i < 64; i += RAND_MAX_WIDTH) 
  {
    r <<= RAND_MAX_WIDTH;
    r ^= (unsigned) rand();
  }

  return r;
}

char* sfr_str(const char* str) 
{
  uint32_t length = sfr_str_length(str);
  char* new_str = (char*)malloc(sizeof(char*) * length);
  memcpy(new_str, str, length);
  new_str[length] = '\0';

  return new_str;
}

char* sfr_str_alloc(uint32_t length) 
{
  char* str = (char*)malloc(sizeof(char*) * length);
  str[length] = '\0';

  return str;
}

uint32_t sfr_str_set(char* dest, const char* src) 
{
  if (dest != NULL) 
    sfr_str_free(&dest);

  uint32_t length = sfr_str_length(src);
  dest = (char*)malloc(sizeof(char*) * length);
  memcpy(dest, src, length);
  dest[length] = '\0';

  return length;
}

void sfr_str_set_length(char* dest, const char* src, uint32_t length) 
{
  if (dest != NULL) 
    sfr_str_free(&dest);

  dest = (char*)malloc(sizeof(char*) * length);
  memcpy(dest, src, length);
  dest[length] = '\0';
}

uint32_t sfr_str_length(const char* src) 
{
  uint32_t length = 0;
  while (src[length] != '\0') 
  { 
    length++;
  }

  return length;
}

bool sfr_str_cmp(const char* str1, const char* str2) 
{
  uint32_t i = 0;
  while (i < 100000) {
    if (str1[i] == '\0' && str2[i] == '\0') 
      return true;
    else if(str1[i] != str2[i]) 
      return false;
    i++;
  }
  return false;
}

bool sfr_str_cmp_length(const char* str1, const char* str2, uint32_t length) 
{
  for (uint32_t i = 0; i < length; i++) 
  {
    if (str1[i] != str2[i]) 
    {
      return false;
    }
  }
  
  return true;
}

void sfr_str_free(char** str) 
{
  SAFIRE_ASSERT(*str, "failed to terminate string as the str doesn't have any memory assigned to it");
  free(*str);
  *str = NULL;
}
