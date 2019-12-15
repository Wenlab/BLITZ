/*
* Copyright 2018 Wenbin Yang <bysin7@gmail.com>
* This file is part of BLITZ (Behavioral Learning In The Zebrafish),
* which is adapted from MindControl (Andrew Leifer et al <leifer@fas.harvard.edu>
* Leifer, A.M., Fang-Yen, C., Gershow, M., Alkema, M., and Samuel A. D.T.,
* 	"Optogenetic manipulation of neural activity with high spatial resolution in
*	freely moving Caenorhabditis elegans," Nature Methods, Submitted (2010).
*
* BLITZ is a free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the license, or
* (at your option) any later version.
*
* Filename: talk2screen.h
* Abstract: this file contains all classes and functions' declarations
*			used to present visual stimulus to fish
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018
* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/

#ifndef _GUARD_TALK2SCREEN_H
#define _GUARD_TALK2SCREEN_H

// Include OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Include standard libraries
#include <vector>
#include <iostream>



/* Class to set up GLFW-OpenGL environment for rendering patterns */
class Screen
{
private: // only used within class
	//GLFWmonitor** monitors;
	//GLFWwindow* window;
	//const GLFWvidmode* mode;


public:
	Screen()
	{

	}
	
	void initialize();

	/* Show all patterns on the screen */
	void bindVBOandVAO();

	void showLeftPatch();
	void showRightPatch();
	/* Render black pattern */
	void showBlackPattern();

	/* Reverse patterns on the top and on the bottom,
		only for halfSplit rendering */
	void reverse();
	void processInput(GLFWwindow* window);

	float leftPatch[9] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};
	float rightPatch[9] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};
private:
	unsigned int VBOs[2], VAOs[2];
	int shaderProgram;
	GLFWwindow* window;
};

// global functions

#endif // !_GUARD_TALK2SCREEN_H