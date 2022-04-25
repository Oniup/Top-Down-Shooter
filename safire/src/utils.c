#include "../include/safire/utils.h"

#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

SFRtimer_t sfr_timer_start(float duration) {
  SFRtimer_t timer = (float)glfwGetTime() + duration;
  return timer;
}

void sfr_timer_add_time(SFRtimer_t* timer, float additional) {
  *timer += additional;
}

bool sfr_timer_finished(SFRtimer_t* timer) {
  float current_time = (float)glfwGetTime();
  if (current_time > *timer) {
    return true;
  } else {
    return false;
  }
}

int sfr_rand_int32() {
  time_t t1; 
  srand ((unsigned)time(&t1));
  // https://stackoverflow.com/questions/7622887/generating-a-random-32-bit-hexadecimal-value-in-c
  int r = rand() & 0xff;
  r |= (rand() & 0xff) << 8;
  r |= (rand() & 0xff) << 16;
  r |= (rand() & 0xff) << 24;
  return r;
}

uint32_t sfr_rand_uint32() {
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

uint64_t sfr_rand_uint64() {
  time_t t1; 
  srand ((unsigned)time(&t1));
  uint64_t r = 0;
  for (int i = 0; i < 64; i += RAND_MAX_WIDTH) {
   r <<= RAND_MAX_WIDTH;
   r ^= (unsigned)rand();
  }
  return r;
}

char* sfr_str(const char* str) {
  uint32_t length = sfr_str_length(str);
  char* new_str = (char*)malloc(sizeof(char*) * length);
  memcpy(new_str, str, length);
  new_str[length] = '\0';
  return new_str;
}

char* sfr_str_alloc(uint32_t length) {
  char* str = (char*)malloc(sizeof(char*) * length);
  str[length] = '\0';
  return str;
}

uint32_t sfr_str_set(char* dest, const char* src) {
  if (dest != NULL) {
    sfr_str_free(&dest);
  }
  uint32_t length = sfr_str_length(src);
  dest = (char*)malloc(sizeof(char*) * length);
  memcpy(dest, src, length);
  dest[length] = '\0';
  return length;
}

void sfr_str_set_length(char* dest, const char* src, uint32_t length) {
  if (dest != NULL) {
    sfr_str_free(&dest);
  }
  dest = (char*)malloc(sizeof(char*) * length);
  memcpy(dest, src, length);
  dest[length] = '\0';
}

uint32_t sfr_str_length(const char* src) {
  uint32_t length = 0;
  while (src[length] != '\0') { 
    length++;
  }
  return length;
}

bool sfr_str_cmp(const char* str1, const char* str2) {
  uint32_t i = 0;
  while (i < 100000) {
    if (str1[i] == '\0' && str2[i] == '\0') {
      return true;
    } else if(str1[i] != str2[i]) {
      return false;
    }
    i++;
  }
  return false;
}

bool sfr_str_cmp_length(const char* str1, const char* str2, uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    if (str1[i] != str2[i]) {
      return false;
    }
  }
  return true;
}

void sfr_str_free(char** str) {
  SAFIRE_ASSERT(*str, "failed to terminate string as the str doesn't have any memory assigned to it");
  free(*str);
  *str = NULL;
}
