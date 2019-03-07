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

/* Disable warning to function std::copy */
#pragma warning(disable : 4996) 


// Include 3rd-party libraries
#include "../3rdPartyLibs/OpenGL/shader_s.h"
#include "../3rdPartyLibs/OpenGL/stb_image.h"

// Include OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include standard libraries
#include <vector>
#include <iostream>

// User-defined macros
#define NUM_ARENA 1
#define PATCHES_PER_ARENA 1
#define NUM_SHADER 1
#define VERTEXS_PER_SHADER 4
#define TRIANGLES_PER_PATCH 2

/* represent pattern changes for a single patch (shader) */
class PatchData 
{
private:
	; // nothing for now
public:
	// methods
	PatchData(std::vector<float> patchRect,
			  const char vertexPath[] = "3rdPartyLibs/OpenGL/shader.vs",
			  const char fragmentPath[] = "3rdPartyLibs/OpenGL/shader1.fs")
			: shader(vertexPath, fragmentPath)
			, rect(patchRect)
			//, yDivide(patchYdivide)
	{
		/* Frequent updating variable */
		pIdx = 0;	
		xDis = 0;
		yDis = 0;
		theta = 0;
		shader.use();
		//shader.setInt("yDivide", yDivide);
		shader.setInt("patternIdx", pIdx);
		shader.setFloat("xDis", xDis);
		shader.setFloat("yDis", yDis);
		shader.setFloat("theta", theta);
	}
	
	bool initialize();
	/* Initialize vertices and their buffers with providing pos(x,y) */
	void initVertices();
	/* Update pattern by giving the shader new pattern index */
	void updatePattern();
	
	

	// properties
	const std::vector<float> rect; // upper-left corner (x, y, width, height)
	//const int yDivide;
	int pIdx; // pattern index

	float xDis, yDis;
	float theta;
	
	Shader shader;
	unsigned int VAO, VBO, EBO;
};
/* represent pattern changes of an entire local area,
 which consists of many patches
*/
class AreaData 
{
private:
	; // nothing for now
public:
	// methods
	/* Enquire the number of patches in an arena */
	AreaData(std::vector<float> areaRect, int n = 1)
		: rect(areaRect)
		, numPatches(n)
	{
		
	}
	bool initialize(std::string imgName);
	bool loadTextureIntoBuffers(std::string imgName);
	void reverseAllPatches();
	void renderTexture(int areaIdx);
	// properties
	std::vector<PatchData> allPatches;
	unsigned int texture0; // texture ID 
	const int numPatches;
	const std::vector<float> rect; // upper-left corner (x, y, width, height)
};

class ScreenData 
{
private: // only used within class
	GLFWmonitor * * monitors;
	GLFWwindow* window;

public:
	// methods
	ScreenData() // constructor
	{
		
	}
	/* Initilize screen environment and coordinates */
	bool initialize(std::string filename);
	/* GLFW initialize and configure window */
	bool init_glfw_window();
	/* glad: load all OpenGL function pointers */
	bool init_glad();
	/* Update pattern for specific area */
	void updatePattern(int cIdx);
	/* Update patternIdx for all shaders in the screen */
	void updatePattern();
	/* Render designed pattern on the screen */
	void renderTexture();
	/* Update pattern in test experiment */
	void updatePatternInTest(int sElapsed);
	/* Update pattern in baseline experiment */
	void updatePatternInBaseline(int sElapsed);
	/* Update pattern in the blackout experiment */
	void updatePatternInBlackout();

	void getTheta(float theta);
	void getXDis(float xDis);
	void getYDis(float yDis);

	// properties
	const GLFWvidmode* mode;

	/* 3(#arenas) * 4(patchesPerArena)
		Scheme for fish positions in arena
		|		|		|
		|	0	|	1	|
		|		|		|
		|---------------|
		|		|		|
		|	2	|	3	|
		|		|		|
	*/
	std::vector<AreaData> allAreas;
	
	int numAreas;
	int lastScreenPatternUpdate;
	/* Interval for updating pattern in baseline session, which is a random number in range */
	int baselineInterval;
};

#endif // !_GUARD_TALK2SCREEN_H

