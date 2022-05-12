#include <topdown_shooter/player_controller.h>

void _tds_player_controller_update(SFRcomponent_t* component, float delta_time);




TDSplayer_controller_t* tds_player_controller_create_instance() {
  SFRentity_t* player = sfr_ecs_push_entity("Player1", "player");
  SFRentity_t* player_gun = sfr_ecs_push_entity("Player1 Gun", "player gun");

  player->layer = 20;
  player_gun->layer = 21;

  SFRcomponent_t* player_controller = sfr_ecs_push_component(player, tds_player_controller(player_gun));

  SFRcomponent_t* renderer = sfr_ecs_push_component(player, sfr_sprite_renderer());
  SFRcomponent_t* animator = sfr_ecs_push_component(player, sfr_sprite_animator(player));

  sfr_sprite_renderer_set_texture(renderer, "player");

  ivec2 idle_animation[] = { { 0, 0 }, { 1, 0 } };
  float idle_time[]      = {   1.0f,     1.0f   };
  ivec2 run_animation[] = { { 0, 0 }, { 1, 0 }, { 2, 0 } };
  float run_time[]      = {   0.1f,     0.1f,     0.2f   };

  sfr_sprite_animator_slice(animator, (ivec2) { 16, 16 });
  sfr_sprite_animator_load_animation(animator, "idle", idle_animation, idle_time, 2);
  sfr_sprite_animator_load_animation(animator, "run", run_animation, run_time, 3);

  SFRcomponent_t* player_collider = sfr_ecs_push_component(player, sfr_collider2d());  
  TDSplayer_controller_t* controller = SFR_COMPONENT_CONVERT(TDSplayer_controller_t, player_controller);


  // setting up the gun
  SFRcomponent_t* gun_sprite_renderer = sfr_ecs_push_component(player_gun, sfr_sprite_renderer());

  sfr_sprite_renderer_set_texture(gun_sprite_renderer, "player_gun");
  vec2 uv_size = { 0.5f, 1.0f };
  sfr_sprite_renderer_set_uv(
    gun_sprite_renderer, 
    (vec2) {
      uv_size[X] * 0.0f,
      uv_size[Y] * 0.0f,
    },
    (vec2) {
      uv_size[X] * 1.0f,
      uv_size[Y] * 1.0f,
    }
  );

  return controller;
}

void tds_player_controller_load_assets() {
  sfr_pipeline_push_texture(sfr_texture(
    "player", "./art/player-sprite-sheet.png", true, 4, false
  ));
    sfr_pipeline_push_texture(sfr_texture(
    "player_gun", "./art/gun-sprite-sheet.png", true, 4, false
  ));
}

SFRcomponent_t* tds_player_controller(SFRentity_t* player_gun) {
  SFRcomponent_t* component = sfr_ecs_component(
    TDS_PLAYER_CONTROLLER, _tds_player_controller_update, NULL, NULL
  );

  component->data = (TDSplayer_controller_t*)malloc(sizeof(TDSplayer_controller_t));
  TDSplayer_controller_t* player_controller = SFR_COMPONENT_CONVERT(TDSplayer_controller_t, component);

  player_controller->damage = 1;
  player_controller->move_speed = 5.0f;
  player_controller->gun = player_gun;

  return component;
}

void tds_player_damage(SFRcomponent_t* player_controller_comp, uint32_t damage) {

}

void _tds_player_controller_update(SFRcomponent_t* component, float delta_time) {
  uint32_t animation = 0;
  static uint32_t last_animation = 0;

  TDSplayer_controller_t* controller = ((TDSplayer_controller_t*)component->data);

  vec3 direction = { 0, 0, 0 };
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_A)) {
    direction[X] = -1;
  } 
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_D)) {
    direction[X] = 1;
  } 
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_S)) {
    direction[Y] = -1;
  } 
  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_W)) {
    direction[Y] = 1;
  } 


  SFRtransform_t* transform = SFR_COMPONENT_CONVERT(SFRtransform_t, component->owner->components[0]);
  static vec3 offset_gun = {
    -0.3f, 0.5f, 0.0f
  };

  if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_UP)) {
    transform->scale[Y] *= 0.9999f;
  } else if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_DOWN)) {
    transform->scale[Y] *= 1.0001f;
  } else if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_LEFT)) {
    transform->rotation[W] += 1.0f * delta_time;
  } else if (sfr_input_keyboard(SFR_INPUT_PRESS, SFR_KEY_RIGHT)) {
    transform->rotation[W] -= 1.0f * delta_time;
  }


  if (direction[X] != 0 || direction[Y] != 0) {

    static bool flipped_x = false;

    if (direction[X] < -0.0f && !flipped_x) {
      transform->scale[X] = -transform->scale[X];
      offset_gun[X] = -offset_gun[X];
      flipped_x = true;
    } else if (direction[X] > 0.0f && flipped_x) {
      transform->scale[X] = -transform->scale[X];
      offset_gun[X] = -offset_gun[X];
      flipped_x = false;
    }
    
    glm_vec3_scale(direction, controller->move_speed * delta_time, direction);
    glm_vec3_add(direction, transform->position, transform->position);

    animation = 1;
  }

  if (animation != last_animation) {
    SFRcomponent_t* animator = sfr_get_component(component, SFR_SPRITE_ANIMATOR);

    sfr_sprite_animator_start_animation_index(animator, animation);
    last_animation = animation;
  }

  SFRtransform_t* gun_transform = SFR_COMPONENT_CONVERT(SFRtransform_t, controller->gun->components[0]);
  glm_vec3_copy(transform->position, gun_transform->position);
  glm_vec3_sub(gun_transform->position, offset_gun, gun_transform->position);
}
