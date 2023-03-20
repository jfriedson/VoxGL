# VoxGL
This is a OpenGL graphics engine framework with the data structures neccesary for ray casting through voxel grids and sparse octrees. Examples of applications which rely on this library can be found at [ModelToVoxel](https://github.com/jfriedson/modeltovoxel) and [Voxel-Particle-Sim](https://github.com/jfriedson/Voxel-Particle-Sim).

Screenshot from ModelToVoxel demo
![Screenshot from ModelToVoxel demo](screenshots/house.png?raw=true)

Screenshot from Voxel-Particle-Sim demo
![Screenshot from Voxel-Particle-Sim demo](screenshots/waterfall.png?raw=true)


## Data Structures
Grids: Uniform 3D grids that store the value of every voxel without compression. It has the largest footprint in the hard disk and memory and results in the longest load times.

Sparse Octrees: Trees containing the array index of 8 children. This minimizes the unnecessary storage of empty space and enables high resolution voxel space when sparsity of voxels is present.

(In Progress) Directed Acyclic Graphs: Octrees that have duplicate subtrees merged from the bottom-up, further compressing overall voxel data on disk and memory.


## Sparse Octree Architechture
Each octree contains two vectors: one containing subtree indices, and one containing leaf data.  The way data is defined is left up to the dependent project through the use of templating.


## Future Plans
Finish implementing DAG


## Dependencies
ASSIMP\
GLFW\
GL3W\
GLM\
STB_IMAGE
