# Top-Down Shooter Game Change logs

## VERSION 0.0.5 (22/5/2022)

* added enemy death animation
* blood spills when the enemy and the player lose health
* better camera movement
* started development of the wave system
  * kill count
  * score count
  * wave number
* player death
  * enemy handler stops spawning enemies in
  * death animation
* reload scene

## VERSION 0.0.4 (19/5/2022)

* enemy animation and different types setup
* more interesting enemy movement for demon type
* added player health
  * ui
  * damage
  * just no death/game over yet

## VERSION 0.0.3 (14/5/2022)

* fixed the bug where the enemy wouldn't be killed when touching the player
* player shooting
  * gun animation
  * bullet animation
  * destroys enemy when it hits the desired target
* enemy handler
  * started spawning system

## VERSION 0.0.2 (13/5/2022)

* added some custom assets
* added the gun asset
* camera lerp to correct position

## VERSION 0.0.1 (11/5/2022)

* added player controller
  * just the basic WASD movement
  * destroys the enemies when they touch (this is temporary)
* added tile map
  * draws sand for the "walkable" areas and a stone thing for the walls (there is no collision detection here yet tho)
* added enemy
  * basic test movement for the time being, just moves towards the player
* added enemy handler
  * spawns enemies in the centre of the map every second
