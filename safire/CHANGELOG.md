# Safire Change logs

## VERSION 0.0.9 (2/5/2022)

* implementation of the batch renderer
  * designed the framework *(VERSION 0.0.8.1)*
  * create vertex buffers
  * vertex data stack

## VERSION 0.0.8 (2/5/2022)

* implemented component update, late_update, and render loops (ECS) *(VERSION 0.0.7.1)*
* implementation of shaders *(VERSION 0.0.7.2)*
* implementation of textures  

## VERSION 0.0.7 (26/4/2022 - 30/4/2022)

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
