# VoxGL
This is a OpenGL graphics engine framework with the data structures neccesary for ray casting through voxel grids and octrees.  The voxel data consists of 32 bit color data and includes support for alpha transparency. Examples of applications which rely on this library can be found at [ModelToVoxel](https://github.com/jfriedson/modeltovoxel) and [3DPowderToy](https://github.com/jfriedson/3DPowderToy).

## Data Structures
Grids: Uniform 3D grids that store the value of every voxel without compression. It has the largest footprint in the hard disk and memory and results in the longest load times.

Octrees: Trees containing the array index of 8 children. This minimizes the unnecessary storage of empty space.

(In Progress) Directed Acyclic Graphs: Octrees that have duplicate subtrees merged from the bottom-up, further compressing overall voxel data on disk and memory.

## Future Plans
I was in the process of adding Directed Acyclic Graphs (DAGs) to further compress Octress in order to save disk space and minimize loading times as evident in the source code; however, I moved on to a more productive project. I will revisit this project soon, but in the meantime, I have made a compromise between octrees and DAGs by reusing leaf nodes to avoid leaf data duplication.

## Dependencies
ASSIMP
GLFW
GL3W
GLM
STB_IMAGE
