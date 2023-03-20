# VoxGL
This is a OpenGL graphics engine framework with the data structures neccesary for ray casting through voxel grids and sparse octrees. Examples of applications which rely on this library can be found at [ModelToVoxel](https://github.com/jfriedson/modeltovoxel) and [3DPowderToy](https://github.com/jfriedson/3DPowderToy).

## Data Structures
Grids: Uniform 3D grids that store the value of every voxel without compression. It has the largest footprint in the hard disk and memory and results in the longest load times.

Sparse Octrees: Trees containing the array index of 8 children. This minimizes the unnecessary storage of empty space and enables high resolution voxel space when sparsity of voxels is present.

(In Progress) Directed Acyclic Graphs: Octrees that have duplicate subtrees merged from the bottom-up, further compressing overall voxel data on disk and memory.

## Sparse Octree Architechture
Each octree contains two vectors: one containing subtree indices, and one containing leaf color data.  The inner tree node vector holding children tree indices is of 32 bit integers, so the maximum number of tree nodes is 2^31 because the left-most bit is used as a signal for a leaf index. I decided to use 32 bit indices because GLSL uses 32 bit resolution variables by default without the use of extensions.

Leaf nodes can contain any amount of data desired. For the mesh to voxel conversion program 64 bit leaves are utilized to contain 32 bits of diffuse color and transparency while the other 32 bits possess the normal of the triangle's face derived at conversion time.

In the future, the SVO class will be templated and the inner node and leaf node struct type will defined by the dependent code.

## Future Plans
Template SVO inner node and leaf nodes
Finish implementing DAG

## Dependencies
ASSIMP
GLFW
GL3W
GLM
STB_IMAGE
