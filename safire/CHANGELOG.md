# Safire Change logs

## VERSION 0.1.1 (18/5/2022)

* added mouse position
  * screen and world coords
  * implementation of the view matrix
* collider now takes in the transform of the entity so that the size of the collider is set to the scale automatically
* added screen culling
* more quality of life stuff
* no entity can have to same name
  * if they use the same name, then it just adds an number to the end of it

## VERSION 0.1.0 (13/5/2022)

* implemented sprite animator
* more quality of life features
  * getting a component name by passing a single component
  * moved the default components into another file (config.c)

## VERSION 0.0.12 (11/5/2022)

* many bug fixes
  * fixed (windows) the compiler errors for SFR_COMPONENT_CONVERT (also change it from sfr_component_convert because of proper conventions with macros)
  * fixed the bug when the deleting multiple entities in one frame, it would crash (the lower and higher in sfr_qsort_uint32 was an uint32_t when they should be int)
  * fixed rendering more than 2 textures, it was because the uniform location wasn't using the dynamic location but was using a test string "u_textures[1]"  
* quality of life
  * implemented the other collider2d trigger functions (for uuid and name)
  * added error logs when failing to find an entity or component (these functions: sfr_ecs_entity_find_index_uuid)

*note: development on the engine should be slower because working on the game part now...*

## VERSION 0.0.11 (9/5/2022)

* components
  * collider component
    * trigger
    * collide

## VERSION 0.0.10 (8/5/2022)

* component converter, so you don't have to write ((Ty*)component->data) every time you want to access the custom data
* textures and different shaders in batch renderer working
* sprite renderer
  * scaling and rotating
* fixed indices bug when there were more than 5 quads been draw

## VERSION 0.0.9 (6/5/2022)

* implementation of the batch renderer
  * designed the framework *(VERSION 0.0.8.1)*
  * create vertex buffers *(VERSION 0.0.8.2)*
    * batch renderer *(VERSION 0.0.9)*
    * vertex data stack
    * indices data stack
    * clear when flushing

## VERSION 0.0.8 (2/5/2022)

* implemented component update, late_update, and render loops (ECS) *(VERSION 0.0.7.1)*
* implementation of shaders *(VERSION 0.0.7.2)*
* implementation of textures  

## VERSION 0.0.7 (30/4/2022)

* made code more cleaner *(VERSION 0.0.6.1)*
  * spacing and tabbing for functions and type defines in the header files
* updated README
  * instructions for building program
* ecs implementation
  * finding entities and components *(VERSION 0.0.6.2)*
  * create entities *(VERSION 0.0.6.4)*
  * create and add components to entities *(VERSION 0.0.6.5) (memory corruption when creating component)*
  * set the default components for every entity
  * erase entity and its components from the scene

## VERSION 0.0.6 (25/4/2022)

* ecs's scene implementation
  * create scenes and add them to the stack
    * start (initialises adds all the starting entities to the scene)
    * update loop (acts kind of like a game manager)
    * late update loop
    * custom data for scene
      * free pointer function for handling this data
  * can swap between scenes and show free all entities and then load that scene
* also made the code more readable for function declarations and data types in the structs

## VERSION 0.0.5 (25/4/2022)

* setup ecs
  * more in depth framework
  * setup scenes
* creates the game folder
  * include
  * scenes
  * src

## VERSION 0.0.4 (24/4/2022)

* basic mockup designs for ecs
  * showing how I am going to store the entities and components
  * showed how the update loops are going to work
* updated utils library
  * better compatibility support
    * build library into static and shared for windows and linux
    * dll support
  * stole some code from my utils library safire-utils, can be found on [Github](https://github.com/Oniup/Safire-Utils.git)
    * better strings
    * generating random numbers
* framework for ecs setup (mostly)
* added cpp support standard at the beginning of every header file

## VERSION 0.0.3 (24/4/2022)

* very basic window input
  * added keybindings

## VERSION 0.0.2 (23/4/2022)

* safire's window implementation using GLFW as the window handler
  * creates an instance and frees the instance
  * handles the frame buffer and swapping it after flushing all the data to the GPU for rendering

## VERSION 0.0.1 (22/4/2022)

* basic mockup designs for the structure of the engine and the game working together
  * game-engine-framework.png
  * game.png
* safire
  * basic setup of framework for ecs, pipeline and the safire context

## VERSION 0.0.0 (22/4/2022)

* basic setup
  * cmake
    * build glfw, glad and custom engine (safire) into a static library that the game will use
  * installed clang (as that is my favourite compiler)
  * basic window setup (making sure that glfw, glad and OpenGL are all working)
