Welcome to BLITZ (Behavioral Learning In The Zebrafish)
======================
Introduction
------------
BLITZ (Behavioral Learning In The Zebrafish) is a software that allows researchers to train larval zebrafish to associate visual pattern with electric shock in a fully automated way, which is adapted from [MindControl][1] 
(Leifer, A.M., Fang-Yen, C., Gershow, M., Alkema, M., and Samuel A. D.T.,
 "Optogenetic manipulation of neural activity with high spatial resolution
 in freely moving Caenorhabditis elegans," Nature Methods, 2011.) <br/>
 
BLITZ processes multiple video streams of freely swimming larveas in real time. As feedback, it presents different visual patterns and give electric shocks to individual fish as user designed. With inputs and outputs in a close loop, researchers can training dozens of fish simultaneously with pre-designed behavioral paradigm in a fully automatic way. Detailed information about fish's position, the type of visual pattern, timing of given electric shocks and other system's state can be recorded in a user-defined way in YAML file, which is a human and computer readable file format. Also the raw video would be saved. The [ABLITZER][2] - "Analyzer of BLITZe Result" (MATLAB) produces quantitative analysis figures and formated quantification structure data based on the output from BLITZ.

  
  
  
Authors
-------

BLITZ is written by Wenbin Yang. It is originally a  product of the [Wen Lab][3] in the [Department of Life Science](http://biox.ustc.edu.cn/) and the [HEFEI NATIONAL LABORATORY FOR PHYSICAL SCIENCES AT THE MICROSCALE](http://en.hfnl.ustc.edu.cn/) at [University of Science and Technology of China](http://en.ustc.edu.cn/). 

Wanzhuo Shi is now helping to improve the readability and the code quality.

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
* Install OpenCV 3.0 or later;Click [here](https://opencv.org/) and say hello to OpenCV
  * Check this [link](https://docs.opencv.org/) and find what you need. There are many great tutorials in this website.
  * If you configure all libs and includes with OpenCV successfully, installation of OpenGL and Pylon 5 will be very easy for you.
    Other two processes are similar to this one.
* Install OpenGL with GLFW3.0 or later
  * Follow the tutorial in [this website](https://learnopengl.com/). Do not forget to install GLAD after installation of GLFW. It won't     cost you too much time.
  * tips: Download the correct glad.c based on your OpenGL version from [this website](https://glad.dav1d.de/) and replace the original     glad.c file. More details are provided in the tutorial. 
* Install Basler Pylon 5 and configure the VS with this [tutorial](https://www.baslerweb.com/fp-1476182890/media/downloads/documents/application_notes/AW00136801000_Getting_Started_with_pylon5_and_OpenCV.pdf) to get started with         OpenCV and pylon.
* Clone the repository to a new empty c++ project in VS2017
* At the project Property Pages, Set "Character Set" -> "Use UniCode Character Set" and configure all libs and includes with OpenCV, OpenGL and Pylon 5. 
* (optional) In the settings, electroshocks delivery are controlled by a RS232 relay. You can download a RS232 driver at [here](http://www.prolific.com.tw/US/ShowProduct.aspx?p_id=225&pcid=41).
* Then build and run, you are all set.
