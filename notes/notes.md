#IDEAS



## Map View
* Toggled with a keypress
* Rendered to a separate framebuffer and displayed when key pressed
  (https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/)
? Possibly visible as minimap

* *Fog of War*
    - Terrain in black until it comes within light-of-sight
    - Grey fog-of-war on terrain not currently in LoS.


* *Memory*
    - Features, items and creatures which have previously entered LoS are 
      visible in map view (with an indicator symbol)
    - Memory of features decays as time passes. Length of time depends on
      players memory attribute
        ? Possibly draw a circle around them which grows as time passes

