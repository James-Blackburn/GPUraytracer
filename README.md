# GPUraytracer
GPU accelerated pathtracer with C++ and OpenCL

## Usage
Building the program will produce a command line tool enabling raytraced images to be produced from customisable scene (.scn) files. The tool outputs to a file, ```output.ppm```, located in the current working directory. Command line usage of the tool is as follows:

```raytrace <scene path> <width> <height> [samples=500] [max depth=50]```
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

## Scene File Format

Scene files begin with a listing of the Materials used. Each entry has the following format:\
```MATERIAL <material name> <r> <g> <b> <fuzz> <material type>```
- With material name being a string and r,g,b,fuzz being decimal numbers from 0.0 - 1.0.
- Material type is an integer that can be one of the following options:
    - 0 - Diffuse
    - 1 - Diffuse Chequered
    - 2 - Metallic
    - 3 - Light Source

A listing of the Spheres present in the scene follows the material listing. Each entry has the following format:\
```SPHERE <material name> <x> <y> <z> <radius>```
- With material name being a string, x,y,z being decimal numbers from 0.0 - 1.0 and radius being a decimal number 0.0 - MAX_FLOAT.

.scn file for mirror room 1:
```
MATERIAL chequeredWhite 0.9 0.9 0.9 0.0 1
MATERIAL metalShinyGrey 0.8 0.8 0.8 0.0 2
MATERIAL diffuseWhite 	0.9 0.9 0.9 0.0 0
MATERIAL lightsource 	2.0 2.0 2.0 0.0 3 

SPHERE chequeredWhite 	0.0 -100.5 -1.0 100.0 	# floor
SPHERE metalShinyGrey 	0.0 0.0 -1.0 0.5 	# middle sphere
SPHERE metalShinyGrey 	0.0 0.0 -104.0 100.0 	# back wall
SPHERE metalShinyGrey 	-103.0 0.0 -1.0 100.0 	# left wall
SPHERE metalShinyGrey 	103.0 0.0 -1.0 100.0 	# right wall
SPHERE metalShinyGrey 	0.0 0.0 104.0 100.0 	# front wall
SPHERE diffuseWhite 	0.0 104.0 -1.0 100.0 	# ceiling
SPHERE lightsource 	0.0 3.0 -1.0 1.0 	# light source
```
