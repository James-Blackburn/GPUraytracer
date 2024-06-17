# GPUraytracer
GPU accelerated pathtracer with C++ and OpenCL

## Usage
```raytrace <scene path> <width> <height> [samples=500] [max depth=50]```\
- Scene path - Path to a .scn file, example scenes located in ./scenes
- Samples - The amount of random ray samples performed per pixel of the image.
- Max depth - The maximum amount of bounces a ray is able to perform before termination.

## Scenes
![chequered-high-res-2](https://github.com/James-Blackburn/GPUraytracer/assets/32494995/63231c07-d32f-4e15-ae3e-c59272008f5f)
|:--:| 
| *Chequered Room 1* |

![mirror-room-high-res](https://github.com/James-Blackburn/GPUraytracer/assets/32494995/92363b02-957e-47ae-831a-4eed507912cb)
|:--:| 
| *Mirror Room 1* |
