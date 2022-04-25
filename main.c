#include <safire/safire.h>
#include <scenes/scene_start_manager.h>
#include <topdown_shooter/manager.h>
int main(int argc, char** argv) {
  print_message();
  print_message_from_manager();
  
  safire("Top-Down Shooter game", 1920 / 2, 1080 / 2, false, NULL);
  return 0;
}