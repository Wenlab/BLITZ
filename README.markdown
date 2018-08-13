Welcome to BLITZ (Behavioral Learning In The Zebrafish)
======================
Introduction
------------
BLITZ (Behavioral Learning In The Zebrafish) is a software that allows researchers to train larval zebrafish to associate visual pattern with electric shock in a fully automated way, which is adapted from [MindControl][1] 
(Leifer, A.M., Fang-Yen, C., Gershow, M., Alkema, M., and Samuel A. D.T.,
 "Optogenetic manipulation of neural activity with high spatial resolution
 in freely moving Caenorhabditis elegans," Nature Methods, Submitted
 (2010).) <br/>
 
BLITZ processes multiple video streams of freely swimming larveas in real time. As feedback, it presents different visual patterns and give electric shocks to individual fish as user designed. With inputs and outputs in a close loop, researchers can training dozens of fish simultaneously with pre-designed behavioral paradigm in a fully automatic way. Detailed information about fish's position, the type of visual pattern, timing of given electric shocks and other system's state can be recorded in a user-defined way in YAML file, which is a human and computer readable file format. Also the raw video would be saved. The [ABLITZER][2] - "Analyzer of BLITZe Result" (MATLAB) produces quantitative analysis figures and formated quantification structure data based on the output from BLITZ.

  
  
  
Authors
-------

BLITZ is written by Wenbin Yang. It is originally a  product of the [Wen Lab][3] in the [Department of Life Science](http://biox.ustc.edu.cn/) and the [HEFEI NATIONAL LABORATORY FOR PHYSICAL SCIENCES AT THE MICROSCALE](http://en.hfnl.ustc.edu.cn/) at [University of Science and Technology of China](http://en.ustc.edu.cn/). 

  [1]:http://github.com/samuellab/mindcontrol
  [2]:https://github.com/Wenlab/ABLIZTER
  [3]:http://www.wenlab.org/
   
  
  
License
-------
With the exception of certain libraries in the `3rdPartyLibs/` folder, all of BLITZ is released under the GNU Public License. This means you are free to copy, modify and redistribute this software. 

Contact
-------
Please contact Wenbin Yang, bysin7 (at) gmail (dot) com with questions or feedback.


Step by step install With Visual Studio 2017 for Windows 7/10 64 Bit
-----------------------------------------------------------------
* Install OpenCV 3.0 or later
* Install OpenGL with GLFW3.0; (Check [this great website](https://learnopengl.com/) for more details.)
* Install Balser Pylon 5
* Clone the repository to a new empty c++ project in VS2017
* At the project Property Pages, Set "Character Set" -> "Use UniCode Character Set" and configure all libs and includes with OpenCV, OpenGL and Pylon 5. (More details at their official websites.)
* Then build and run, you are all set.
