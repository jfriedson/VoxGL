# VoxGL
This is a OpenGL graphics engine framework with the data structures neccesary for ray casting through voxel grids and octrees.  The voxel data consists of 32 bit color data and includes support for alpha transparency. Examples of applications which rely on this library can be found at [ModelToVoxel](https://github.com/jfriedson/modeltovoxel) and [3DPowderToy](https://github.com/jfriedson/3DPowderToy).

## Data Structures
Grids: Uniform 3D grids that store the value of every voxel without compression. It has the largest footprint in the hard disk and memory and results in the longest load times.

Octrees: Trees containing the array index of 8 children. This minimizes the unnecessary storage of empty space.

(In Progress) Directed Acyclic Graphs: Octrees that have duplicate subtrees merged from the bottom-up, further compressing overall voxel data on disk and memory.

## Technicalities
Each octree "root" contains two vectors: one containing subtree indices, and one containing leaf color data.  The vector holding subtree indices is of 32 bit integers, so the maximum number of tree nodes is 2^31 because the left-most bit is used as a signal for a leaf index. I decided to use 32 bit indices because GLSL uses 32 bit resolution variables by default without the use of extensions.  In my [ModelToVoxel](https://github.com/jfriedson/modeltovoxel) project I have found that octrees with a depth of 9 do not run out of indices with the 3D models that I have tested.

## Future Plans
I was in the process of adding Directed Acyclic Graphs (DAGs) to further compress Octress in order to save disk space and minimize loading times as evident in the source code; however, I moved on to a more productive project. I will revisit this project soon, but in the meantime, I have made a compromise between octrees and DAGs by reusing leaf nodes to avoid leaf data duplication.

## Dependencies
ASSIMP
GLFW
GL3W
GLM
STB_IMAGE
