# MazeRunner
Neuroscience modeling of spatial memory using a 3d mouse maze simulation.

## 3d Maze

A maze has been built in Unreal Engine 4 (UE4).  UE4 offers many advantages in simulation capabilities as well as being able to be controlled directly through C++ source code.

Currently included:
* Wall collision detection 
* Ability to include different mazes. Could be expanded into the morris water maze, etc.
* Movement controls are working successfully
* Maze has been tested and gone through to the exit successfully

## Neural Computation

Grid and place cells, and oscillatory rhythms are modeled. Those areas of modeling are evolving areas of science. Contained in the work are continuous attractor network (CAN) models and ramping membrane potential dynamics [1][2]. The CAN formulas are from work by Dr. Mathew Nolan’s lab [3]. A known issue with CAN models are the often lack of inclusion of mechanisms for phase procession relative to theta rhythms. A possible method to address that is to have a hybrid model with oscillatory interference [4][5]. Work by Dr. Michael Hasselmo’s lab is being researched for inclusion for such purposes [6]. Oscillatory interference has had some challenges presented to it by studies on bats, mice, and others; learning from such studies will be done to address the knowledge found [7][8]. The simulation can potentially include multiple models and compare and contrast them. The system is being designed with a goal to flexibly add more models over time.

Similar methods to those of an existing Sheynikhovich, et al., 3d maze simulation will be included [9]. Recreation of observed mouse neural activity will be used in the maze tasks as a ground truth for training and testing models. A source of in vivo recorded activity will be the open access data from Dr. Gyorgy Buzsáki's lab that was obtained in spatial memory task experiments [10]. Another source of data can be from the Kavli Institute of Systems Neuroscience [11]. Theories of neural mechanisms can be explored through modifying neural properties. Eventual work can include addressing open questions existing in the current state of spatial memory models.

Presently functional:
* Saved images from the maze are processed by a Gabor filter using OpenCV. That will be visual stimulus input into neural networks of spatial memory cells to learn the maze.
* Differential equations solver is working with odeint, that can generate synapse signaling differential equations if needed. Plotting is operational with matplotlibcpp. Exponential integrate-and-fire neuron current with refractory period has been tested.
* Grid cell populations are generated as multidimentional struct arrays which contain pertinant variables (i.e., membrane voltage and neuron location).
Presently 6 populations of 5x5 neuron grid formations are included.
* Synaptic current for each neuron has been implemented through corresponding formulas. All-to-all connections within grid cell populations have been tested over several time 
steps and found to be computing successfully. Some simplifications were made (i.e., refractory period and dirac delta) but more complex methods can be
included in the implementation in the future.
* Efficient memory management is included through pointers avoiding hard copying values. E.g., voltages and synaptic currents.
* Basic sample movement testing has been added. Place cells increase firing in a test location movement pattern. This sequence will be expanded on to include the bump attractor initialisation component present in the methods this work is based from. The external current variable that is affected by place cells is used to correspondingly increase the grid cells firing activity. A simple form of traveling increased activity as a bump in the grid formation of grid cells has been tested and observed, and further bump dynamics will be added through additional math formulas.

## Methods to Work with the Code
./3d_simulated_maze/

Import the 3d maze folder into UE4 and it will compile the project for you.

./neural_engine/

The project is in C++ and created with Eclipse C++ edition. Import it with that IDE or one of your choice.
Dependencies needed for compiling: Boost, Odeint, OpenCV, Matplotlibcpp. Also plplot and python2.7 but those are planning to be removed.

## Video Samples:

<center>
<a href="https://gfycat.com/ThornyAcceptableIndianglassfish" target="_blank"><img src="http://i.imgur.com/nultxhH.png" alt="gameplay video image" width="360" height="240" border="10"></a>
<a href="https://gfycat.com/SnarlingIllinformedGibbon" target="_blank"><img src="http://i.imgur.com/ssjSamF.png" alt="gameplay video image" width="360" height="240" border="10"></a>
<a href="https://gfycat.com/InfantileVapidIvorygull" target="_blank"><img src="http://i.imgur.com/uNXdr0H.png" alt="gameplay video image" width="360" height="240" border="10"></a>
<a href="https://gfycat.com/ComplicatedAppropriateIrishdraughthorse" target="_blank"><img src="http://i.imgur.com/nJyybdQ.png" alt="gameplay video image" width="360" height="240" border="10"></a>

Gabor Filtering:

<a href="http://i.imgur.com/qPpUyNv.jpg" target="_blank"><img src="http://i.imgur.com/qPpUyNv.jpg" alt="gabor normal" width="360" height="240" border="10"></a><a href="http://i.imgur.com/bizKrcK.jpg" target="_blank"><img src="http://i.imgur.com/bizKrcK.jpg" alt="gabor filtered" width="360" height="240" border="10"></a>
</center>

### References

\[1\] Shipston-sharman O, Solanka L, Nolan MF. Continuous attractor network models of grid cell firing based on excitatory-inhibitory interactions. J Physiol (Lond). 2016;594(22):6547-6557.

\[2\] Schmidt-hieber C, Häusser M. How to build a grid cell. Philos Trans R Soc Lond, B, Biol Sci. 2014;369(1635):20120520.

\[3\] Solanka L., van Rossum M.C.W., Nolan, M.F. Noise promotes independent control of gamma oscillations and grid firing within a recurrent attractor network. eLife 2015;10.7554/eLife.06444.

\[4\] Schmidt-hieber C, Häusser M. Cellular mechanisms of spatial navigation in the medial entorhinal cortex. Nat Neurosci. 2013;16(3):325-31.

\[5\] Bush D, Burgess N. A hybrid oscillatory interference/continuous attractor network model of grid cell firing. J Neurosci. 2014;34(14):5065-79.

\[6\] Shay, C. F., Ferrante, M., Chapman, G. W., & Hasselmo, M. E. (2016). Rebound spiking in layer II medial entorhinal cortex stellate cells: Possible mechanism of grid cell function. Neurobiology of Learning and Memory, 129, 83–98. https://doi.org/10.1016/j.nlm.2015.09.004

\[7\] Yartsev MM, Witter MP, Ulanovsky N. Grid cells without theta oscillations in the entorhinal cortex of bats. Nature. 2011;479(7371):103-7.

\[8\] Domnisoru C, Kinkhabwala AA, Tank DW. Membrane potential dynamics of grid cells. Nature. 2013;495(7440):199-204.

\[9\] Sheynikhovich D, Chavarriaga R, Strösslin T, Arleo A, Gerstner W. Is there a geometric module for spatial orientation? Insights from a rodent navigation model. Psychol Rev. 2009;116(3):540-66.

\[10\] Diba K, Buzsáki G. Hippocampal network dynamics constrain the time lag between pyramidal cells across modified environments. J Neurosci. 2008;28(50):13448-56. Open Access Data: [hc-3 at crcns.org](http://crcns.org/data-sets/hc/hc-3)

\[11\] Hafting T, Fyhn M, Bonnevie T, Moser MB, Moser EI. Hippocampus-independent phase precession in entorhinal grid cells. Nature. 2008;453(7199):1248-52. Open Access Data: [data at ntnu.edu](ntnu.edu/kavli/research/grid-cell-data)





