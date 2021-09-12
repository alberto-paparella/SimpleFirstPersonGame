# sfpg
## SIMPLE FIRST PERSON GAME

An implementation of a simple camera used to navigate into a 3D world.
It is possible to rotate the camera moving the mouse, and to move around
using W, A, S and D keys (ESC to close the application).
It can be used, for example, in first person games or in CAD applications.
As a little showcase, we created a very simple labyrinth.
Both grass and walls are rendered using shaders.
We also implemented a collision system, such as the user won't go "inside" a
wall, but it will stop right before it.

## Development
Final project for the Computer Graphics course (A.Y. 2020/2021) of the University of Ferrara.

```
Teacher:  Antonino Casile

Students: Alberto Paparella   Badge number: 144261
          Martina Tenani      Badge number: 144739
```

## About the project          
There are two ways to move to a point in space.
The first is changing the position of the camera and moving it to that point
(not possible in OpenGL), the other is changing the position of the point
and bringing it to the camera.
Simply speaking, to produce the effect of moving forward, you either go
forward yourself, or bring the world backwards.
So to produce the effect of going to the point (x,y) in space, it is possible
to translate the world to the point (-x,-y) instead, and it will be
indistinguishable.
If you wish to look to the right side, rotate the world to the left.
That is why we use the negative values of the angles while rotating.
Important note: always rotate first and then translate.

For the collission system, we implemented the followuing logic: check if the
next step will locate the object around you; if that is the case, don't
update the matrices in that direction.
 
