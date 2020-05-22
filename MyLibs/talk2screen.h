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

using namespace std;

class ScreenData
{
private:

	GLFWmonitor** monitors;
	GLFWwindow* window;

public:
	// methods
	ScreenData(vector<float> startPoint, vector<float> endPoint, bool lefttrue)
		: point_1(startPoint)
		, point_2(endPoint)
		, left_true(lefttrue)
	{



	}

	bool PatternInitialize();

	bool ScreenInitialize();

	float cross_product(float x0, float y0, float x1, float y1, float x, float y);

	void get_endpoint(float x1, float y1, float x2, float y2);

	void reversePattern();

	// Render designed pattern on the screen
	void renderTexture();
	// properties

	vector<float> point_1;
	vector<float> point_2;

	float  border_pts[4][2] = {
		1.0f,1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f
	};
	float endpoint[2][2];

	vector<float> bright_pts;
	vector<float> dark_pts;

	float bright[4] = { 1.0f, 0.5f, 0.2f, 1.0f };
	float dark[4] = { 0.2f, 0.3f, 0.3f, 1.0f };
	float* background_color;

	int vertexShader, fragmentShader, shaderProgram, vertexColorLocation;
	int num_triangles;

	bool left_true;
	unsigned int VAO, VBO, EBO;
	//EBO
	unsigned int indices1[3] = {
		0, 1, 2,
	};
	unsigned int indices2[6] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int indices3[3] = {
		0, 1, 2,
	};
	const GLFWvidmode* mode;

};

// global functions

#endif // !_GUARD_TALK2SCREEN_H