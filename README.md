# Physics System with Collision
## Junxuan Hu 

This project is simple 2D game developed with custom Engine codes and GLib Graphics support.

## How to run the program

Open the solution file, set ``MonsterChase`` as the start up project, then click run.

You will use WASD to control the character. Once hitting the monster, the monster will move away aligning with the force direction.

The collision box is marked.

## Program features

The Engine codes follow the The Entity Component System (ECS) architecture, consisting of ``CollisionSystem``, ``MovableSystem``, 
``PhysicsSystem``, ``RenderSystem``, and ``GameObjectControllerSystem``. ``GameObjectFactory`` reads data from json files in 
the MonsterChase game folder and creates game objects and components.

A simple ``PlayerController`` is created to read input from keyboard (WASD).

The application exits without memory leak.