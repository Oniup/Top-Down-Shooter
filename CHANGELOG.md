# Change logs from beginning to end

## VERSION 0.0.5 (25/5/2022)

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
  * stole some code from my utils library safire-utils: https://github.com/Oniup/Safire-Utils.git
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
