# MazeRunner
Neuroscience modeling of spatial memory using a 3d mouse maze simulation.

Note: this project is currently in an early development stage.

## 3d Maze

A maze has been built in Unreal Engine 4 (UE4).  UE4 offers many advantages in simulation capabilities as well as being able to be controlled directly through C++ source code.

Currently included:
* Wall collision detection 
* Ability to include different mazes. Could be expanded into the morris water maze, etc.
* Movement controls are working successfully
* Maze has been tested and gone through to the exit successfully

## Neural Computation

Grid and place cells, and oscillatory rhythms will be modeled. Initially the work will be from oscillatory interference formulas by Dr. Neil Burgess \[1\]. Alternatively spatial memory formulas from Dr. Michael Hasselmo may be used instead, that effort is being looked into \[2\]. Similar methods to those of an existing Sheynikhovich, et al., 3d maze simulation will be included \[3\]. Recreation of observed mouse neural activity can be used in the maze tasks and theories of neural mechanisms can be explored. Eventual work can include addressing open questions existing in the current state of spatial memory models.

Presently functional:
* Saved images from the maze are processed by a Gabor filter using OpenCV. That will be visual stimulus input into neural networks of spatial memory cells to learn the maze.
* Differential equations solver is working with odeint, that can generate synapse signalling. Plotting is operational with matplotlibcpp.
* Grid cell populations are able to be generated as multidimentional arrays with a placeholder struct to contain the pertinant variables.

## Methods to Work with the Code
./3d_simulated_maze/

Import the 3d maze folder into UE4 and it will compile the project for you.

./neural_engine/

The project is in C++ and created with Eclipse C++ edition. Import it with that IDE or one of your choice.
Dependencies needed for compiling: Boost, Odeint, OpenCV, Matplotlibcpp. Also plplot and python2.7 but those are planning to be removed.

### References

\[1\] Burgess N. Grid cells and theta as oscillatory interference: theory and predictions. Hippocampus. 2008;18(12):1157-74.

\[2\] Hasselmo ME. A model of episodic memory: mental time travel along encoded trajectories using grid cells. Neurobiol Learn Mem. 2009;92(4):559-73.

\[2\] Sheynikhovich D, Chavarriaga R, Strösslin T, Arleo A, Gerstner W. Is there a geometric module for spatial orientation? Insights from a rodent navigation model. Psychol Rev. 2009;116(3):540-66.

Video Samples:

<center>
<a href="https://gfycat.com/ThornyAcceptableIndianglassfish" target="_blank"><img src="http://i.imgur.com/nultxhH.png" alt="gameplay video image" width="360" height="240" border="10"></a>
<a href="https://gfycat.com/SnarlingIllinformedGibbon" target="_blank"><img src="http://i.imgur.com/ssjSamF.png" alt="gameplay video image" width="360" height="240" border="10"></a>
<a href="https://gfycat.com/InfantileVapidIvorygull" target="_blank"><img src="http://i.imgur.com/uNXdr0H.png" alt="gameplay video image" width="360" height="240" border="10"></a>
<a href="https://gfycat.com/ComplicatedAppropriateIrishdraughthorse" target="_blank"><img src="http://i.imgur.com/nJyybdQ.png" alt="gameplay video image" width="360" height="240" border="10"></a>

Gabor Filtering:

<a href="http://i.imgur.com/qPpUyNv.jpg" target="_blank"><img src="http://i.imgur.com/qPpUyNv.jpg" alt="gabor normal" width="360" height="240" border="10"></a><a href="http://i.imgur.com/bizKrcK.jpg" target="_blank"><img src="http://i.imgur.com/bizKrcK.jpg" alt="gabor filtered" width="360" height="240" border="10"></a>
</center>
