# PVR DMA RENDERING 

## Features
 - Uses KOS's `pvr_vertbuf_tail()`/`pvr_vertbuf_written()` API for DMA scene submission.
 - Uses SH4ZAM to perform all matrix math/transforms.
 - Provides a real-world tested implementation of near-z clipping.
 - Shows off PVR hardware fog.

## Controls
- analog stick up: move forward 
- analog stick down: move backward 
- analog stick left: turn left
- analog stick right: turn right
- dpad up: move vertically up
- dpad down: move vertically down
- dpad left: decrease PVR fog far-plane
- dpad right: increase PVR fog far-plane 
- L trigger: decrease PVR fog near-plane
- R trigger: increase PVR fog near-plane
- A button: disable texturing/enable debug vertex coloring
- Y button: reset scene to initial defaults
- Start button: exit

## Building
- Compile/install KOS.
- Clone kos-ports and install sh4zam, zlib, libjpeg, libpng and libkmg.
- Run `make`.

## Author
jnmartin64