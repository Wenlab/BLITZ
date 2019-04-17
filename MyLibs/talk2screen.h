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

protected:
/* the following properties or methods should not be accessed by users,
but can be accessed by inherited classes */
	 /* GLFW initialize and configure window,
	 this method is used in initialize() only */
	 void init_glfw_window();

	 /* glad: load all OpenGL function pointers,
	 this method is used in initialize() only */
	 void init_glad();

	 /* Render designed pattern on the screen
	 The keyword `render` is reserved for bottom-level rendering,
	 DO NOT use this word in high level "showPattern" methods */
	 void renderTexture();

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
	/* TODO: what if I want to show a single pattern rendering?
		 write a demo method?
		 Answer:
		 Screen screenObj;
		 // TODO: should I write an overload function to simplify user's work?
		 screenObj.initialize(string::fileName); or vector<string>::fileName?
		 // TODO: this is an inconsistent abstraction level, unify it to the screen level
		 // e.g., screenObj.reversePatternIdx(int interval);
		 screen.showPattern();

	*/

	/* Initilize screen environment and coordinates for a single patch */
	void initialize(
		std::string imgName, // name of the image to show
		std::vector<float> boundBox, // bounding area of the pattern x,y (-1.0f<->1.0f)
		string renderType /* 1. full: full rendering of a single pattern;
		2. half: half pattern, half background (pure-color) rendering
		3. rotation: render rotating pattern with a fixed rotating rate */
	);

	/* Initilize screen environment and coordinates for multiple patterns */
	void initialize(
		std::vector<std::string> filenames, // names of the images to show
		std::vector<int> patchesOfAreas = {4,4,4} // number of patches in each area
	);


	/* Update pattern for a specific area */
	void showPattern(int areaIdx); // TODO: get a better name since it is a little confusing with render
	/* Update patternIdx for all shaders in the screen */
	void updatePattern();
	/* Reverse patterns on the top and on the bottom */
	void reversePattern();
	/* Given the indices of patches, reverse patterns on the top and on the bottom */
	void reversePattern(std::vector<int> patchIndices); // TODO: -> reversePatternIndices, maybe some overload input argument type?
	/* Reverse patterns periodically */
	void reversePattern(
		int sec2start, // time to the beginning in second
		int period // the interval between two reversals
	);
	/* Render black pattern */
	void showBlackPattern();

	// properties
	std::vector<AreaData> allAreas;
	int numAreas; //TODO: consider to get this via a get method "numAreas = allAreas.size();"
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
	Area(std::vector<float> rect, int n = 1)
		: boundBox(rect)
		, numPatches(n)
	{

	}
	/* TODO: add descriptions */
	void initialize(
		std::string imgName, // name of the pattern to show
		std::string renderType, /* 1. full: full rendering of a single pattern;
		2. half: half pattern, half background (pure-color) rendering
		3. rotation: render rotating pattern with a fixed rotating rate */
		int numPatches = 1; // number of patches in the area
	)


	void initialize(std::vector<int> yDivideVec, std::string imgName);
	void loadTextureIntoBuffers(std::string imgName);
	void reverseAllPatches(); // TODO: -> reverse();
	void renderTexture(int areaIdx);


	// properties
	std::vector<PatchData> allPatches;
	unsigned int textureID; // texture ID // TODO: update this name in other files (texture0 -> textureID)
	const int numPatches;
	const std::vector<float> boundBox; // upper-left corner (x, y, width, height)
	// TODO: -> boundBox
};


/* represent pattern changes for a single patch (shader) */
class Patch
{
private:
	Shader shader;
	unsigned int VAO, VBO, EBO;
protected:
/* the following properties or methods should not be accessed by users,
but can be accessed by inherited classes */
	/* Initialize vertices and their buffers with given pos(x,y) */
	void initVertices();

public:
	// methods
	Patch(std::vector<float> patchRect)
			: boundBox(patchRect)
	{
		/* Frequent updating variable */
		pIdx = 0;
		radVelo = 0;
	}

	/* Initialize memory for patch */
	void initialize();

	/* Update pattern by giving the shader new pattern index */
	void updatePattern(); // TODO: consider to change the name

	// properties
	const std::vector<float> boundBox; // upper-left corner (x, y, width, height)
	const int yDivide; // TODO: calculate this variable from boundBox
	int pIdx; // pattern index
	float radVelo; // rotating radian velocity


};

// global functions
// TODO: consider to move this function to error handling class
bool iequals(const string& a, const string& b);

#endif // !_GUARD_TALK2SCREEN_H
