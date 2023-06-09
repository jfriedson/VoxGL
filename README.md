# VoxGL
This is a OpenGL graphics engine framework with the data structures and methods neccesary for constructing and modifying voxel grids and sparse octrees on CPU. Examples of applications which rely on this library can be found at [ModelToVoxel](https://github.com/jfriedson/modeltovoxel) and [Voxel-Particle-Sim](https://github.com/jfriedson/Voxel-Particle-Sim).

Screenshot from ModelToVoxel demo
![Screenshot from ModelToVoxel demo](screenshots/house.png?raw=true)

Screenshot from Voxel-Particle-Sim demo
![Screenshot from Voxel-Particle-Sim demo](screenshots/waterfall.png?raw=true)


## Data Structures
Grids: Uniform 3D grids that store the value of every voxel without compression. It has the largest footprint in the hard disk and memory and typically results in the longest load times, but it's very cheap to read and modify as memory is allocated for both filled and empty space. Ray traversal through this structure is very expensive, however.

Sparse Octrees: Tree implemented as an index array where each node contains the array index of it's 8 children. Nodes without children will not consume additional space - instead, the branch will point to the invisible leaf at index 0 for this node. This minimizes the unnecessary storage of empty space and enables high resolution voxel space and quick ray traversal when moderate to high sparsity of voxels is present at the expense of modification speed due to tree traversal and up-keeping.

(In Progress) Directed Acyclic Graphs: Octrees with duplicate subtrees are merged from the bottom-up, further compressing overall voxel data on disk and memory. Ray traversal will likely not be much quicker compared to the sparse octree, but modification of the data structure will be much more expensive due to sub-tree comparisons.


## Sparse Octree Architechture
Each octree contains two vectors: one containing subtree indices, and one containing leaf data.  The way node and leaf data is stored is left up to the dependent project through the use of templating. Additionally, to minimize space consumption and provide a compromise between sparse octrees and DAGs, leaves are merged based on exact match of their properties using a quick robinhood hash map.


## Future Plans
Finish implementing DAG


## Dependencies
ASSIMP\
GLFW\
GL3W\
GLM\
STB_IMAGE
