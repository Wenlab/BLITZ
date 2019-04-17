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

// Include standard libraries
#include <vector>
#include <iostream>

// User-defined macros; TODO: consider to convert some macros to local variables
#define NUM_ARENA 3
#define PATCHES_PER_ARENA 4
#define NUM_SHADER 12
#define VERTEXS_PER_SHADER 4
#define TRIANGLES_PER_PATCH 2



class Screen
{
private: // only used within class
	GLFWmonitor * * monitors;
	GLFWwindow* window;
	const GLFWvidmode* mode;

public:
	// methods
	/* Basically, there are only 3 kinds of methods
			1. initiation
			2. updatePatternIdx
			3. render
	*/
	Screen() // constructor
	{

	}
	/* Initilize screen environment and coordinates */
	bool initialize(std::vector<std::string> filenames, std::vector<int> patchesOfAreas = {4,4,4});
	/* GLFW initialize and configure window */
	void init_glfw_window();
	/* glad: load all OpenGL function pointers */
	void init_glad();
	/* Update pattern for specific area */
	void updatePatternIdx(int areaIdx); // TODO: get a better name since it is a little confusing with render
	/* Update patternIdx for all shaders in the screen */
	void updatePattern();
	/* Render designed pattern on the screen */
	void renderTexture();
	/* Reverse patterns on the top and on the bottom */
	void reverse();
	/* Given the indices of patches, reverse patterns on the top and on the bottom */
	void reverse(std::vector<int> patchIndices);
	/* Reverse patterns periodically */
	void reversePeriodically(int sec2start, int period);//TODO: add this method to screen class
	/* Render black pattern */
	void renderBlackPattern();

	// properties
	std::vector<AreaData> allAreas;
	int numAreas;
	int lastScreenPatternUpdate;
};

/* represent pattern changes of an entire local area,
 which consists of many patches
*/
class Area
{
private:
	; // nothing for now
public:
	// methods
	/* Enquire the number of patches in an arena */
	Area(std::vector<float> areaRect, int n = 1)
		: rect(areaRect)
		, numPatches(n)
	{

	}
	/* TODO: add descriptions */
	bool initialize(std::vector<int> yDivideVec, std::string imgName);
	bool loadTextureIntoBuffers(std::string imgName);
	void reverseAllPatches(); // TODO: -> reverse();
	void renderTexture(int areaIdx);


	// properties
	std::vector<PatchData> allPatches;
	unsigned int textureID; // texture ID // TODO: update this name in other files (texture0 -> textureID)
	const int numPatches;
	const std::vector<float> rect; // upper-left corner (x, y, width, height)
};


/* represent pattern changes for a single patch (shader) */
class Patch
{
private:
	; // nothing for now
public:
	// methods
	Patch(std::vector<float> patchRect, const int patchYdivide,
			  const char vertexPath[] = "3rdPartyLibs/OpenGL/shader.vs",
			  const char fragmentPath[] = "3rdPartyLibs/OpenGL/shader.fs")
			: shader(vertexPath, fragmentPath)
			, rect(patchRect)
			, yDivide(patchYdivide)
	{
		/* Frequent updating variable */
		pIdx = 0;
		shader.use();
		shader.setInt("yDivide", yDivide);
		shader.setInt("patternIdx", pIdx);
	}

	/* Initialize memory for patch */
	bool initialize();
	/* Initialize vertices and their buffers with given pos(x,y) */
	void initVertices();
	/* Update pattern by giving the shader new pattern index */
	void updatePattern();



	// properties
	const std::vector<float> rect; // upper-left corner (x, y, width, height)
	const int yDivide;
	int pIdx; // pattern index

	Shader shader;
	unsigned int VAO, VBO, EBO;
};




#endif // !_GUARD_TALK2SCREEN_H
