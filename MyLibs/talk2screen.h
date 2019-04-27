/*
* Copyright 2019 Wenbin Yang <bysin7@gmail.com> (This project started from Jan., 2018.)
* This file is part of [BLITZ (Behavioral Learning In The Zebrafish)](https://github.com/Wenlab/BLITZ),
* which is adapted from MindControl (Andrew Leifer et al., 2011) <leifer@fas.harvard.edu>
*
* BLITZ is a free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the license, or
* (at your option) any later version.
*
* Filename: talk2screen.h
* Abstract: this file contains all classes and functions' declarations
*			used to present visual patterns
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 15, 2018
* Modified on: Apr. 20, 2019
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


/* Class to set up GLFW-OpenGL environment for rendering patterns */
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

public:
	Screen() // constructor
	{

	}
	/* Initilize screen environment and coordinates for a single area */
	void initialize(
		std::string imgName, // name of the image to show
		std::vector<float> boundBox, // bounding area of the pattern x,y (-1.0f<->1.0f)
		std::string renderType // type of rendering, full, half, rotating
	);

	// TODO: write the implementation
	/* Initilize screen environment with pre-defined multi-bounding areas */
	void initialize(
		std::vector<std::string> imgNames, // name of the images to show
		std::vector<std::vector<float>> boundBoxes, // bounding boxes of all the areas
		std::string renderType, // type of rendering, full, half, rotating
		std::vector<int> patchesInAreas // number of patches in each area
	);

	/* Show all patterns on the screen */
	void show();

	/* Show pattern for a specific area */
	void show(int idxArea);

	/* Render black pattern */
	void showBlackPattern();

	/* Reverse patterns on the top and on the bottom,
	 	only for halfSplit rendering */
	void reverse();

	/* Given the indices of patches, reverse patterns on the top and on the bottom,
	  only for halfSplit rendering */
	void reverse(std::vector<std::vector<bool>> patchIndices); // indices to indicate the patches to reverse

	/* Reverse all patterns periodically */
	void reverse(
		int sec2start, // time to the beginning in second
		int period // the interval between two reversals
	);

	/* Reverse patterns periodically */
	void reverse(
		int sec2start, // time to the beginning in second
		int period, // the interval between two reversals
		std::vector<std::vector<bool>> patchIndices // indices to indicate the patches to reverse
	);

	// properties
	std::vector<Area> allAreas;
	int numAreas; //TODO: consider to get this via a get method "numAreas = allAreas.size();"
};

/* represent pattern changes of an entire local area,
 which consists of many patches
*/
class Area
{
private:
	std::string renderType; /* 1. full: full rendering of a single pattern;
							   2. half: half pattern, half background (pure-color) rendering
							   3. rotation: render rotating pattern with a fixed rotating rate */
	unsigned int textureID; // texture ID

	const int numPatches;
	const std::vector<float> boundBox; // upper-left corner (x, y, width, height)

public:
	// methods
	/* Enquire the number of patches in an arena */
	Area(
		std::vector<float> rect,
		string RT = "full",
		int n = 1
	)
		: boundBox(rect)
		, renderType(RT)
		, numPatches(n)
	{

	}
	/* Initialize memory */
	void initialize(std::string imgName); // name of the pattern to show

	/* load texture (image) to GPU buffers */
	void loadTextureIntoBuffers(std::string imgName);

	/* Update idxCases for all patches in this area,
	 	only for halfSplit rendering type */
	void updateIdxCase(int value);

	// TODO: consider to have all manipulation methods at this level?
	/* Negate idxCases for all patches in this area,
	 	only for halfSplit rendering type  */
	void negateIdxCase();

	/* Given idxPatchess, negate idxCases for all patches in this area,
	 	only for halfSplit rendering type  */
	void negateIdxCase(int patchIndex);

	/* Render designed pattern on the screen
	The keyword `render` is reserved for bottom-level rendering,
	DO NOT use this word in high level "showPattern" methods */
	/* Render pattern for the entire area via textureID */
	void renderTexture(int areaIdx);


	// properties
	template<class P> // test this generic function
	std::vector<P> allPatches;
	// TODO: test whether this initiation works?
};

/* This an example class for inheritances.
A wrapper class of the Shader class */
class Patch
{
protected:
/* the following properties or methods should not be accessed by users,
but can be accessed by inherited classes */
	// Properties
	Shader shader;
	unsigned int VAO, VBO, EBO;
	const std::vector<float> boundBox; // upper-left corner (x, y, width, height)

	// Methods
	/* Initialize vertices and their buffers with given pos(x,y) */
	void initVertices();



public:
	Patch(
		std::vector<float> patchRect, // bounding box
		const char vertexPath[] = "3rdPartyLibs/OpenGL/full.vs", // path to the vertex shader file
		const char fragmentPath[] = "3rdPartyLibs/OpenGL/full.fs" // path to the vertex fragment file
	)
		: shader(vertexPath, fragmentPath)
		, boundBox(patchRect)
	{

	}
	/* Initialize memory for patch */
	void initialize();

	/* Upload an int variable to GPU from CPU */
	void uploadInt2GPU(std::string varName, int varValue);

	/* Upload a float variable to GPU from CPU */
	void uploadFloat2GPU(std::string varName, float varValue);
};

/* Patch-area to render a full pattern without anything else */
class FullPatch : public Patch
{
public:
	// methods
	FullPatch() : Patch( boundBox ,"3rdPartyLibs/OpenGL/full.vs", "3rdPartyLibs/OpenGL/full.fs")
	{

	}

	/* Initialize memory for patch */
	// initialize(); inherited from parent class Patch
};

/* Patch-area to render a half-texture-half-background pattern */
class HalfSplitPatch : public Patch
{
private:
	// properties
	const int yDivide; // the dividing position in y

public:
	// Methods
	HalfSplitPatch( int yDiv ) 
		: Patch(boundBox, "3rdPartyLibs/OpenGL/halfSplit.vs", "3rdPartyLibs/OpenGL/halfSplit.fs")
		, yDivide(yDiv)
	{
		shader.use();
		shader.setInt("idxCase",idxCase);
	}

	// Properties
	int idxCase; // to select the case in f-shader

};

class RotatingPatch : public Patch
{
private:
	// properties
	; // nothing for now

public:
	// Methods
	RotatingPatch( 
		int radVelo = 0 // the rotating velocity of the pattern
	)
	: Patch(boundBox, "3rdPartyLibs/OpenGL/rotating.vs", "3rdPartyLibs/OpenGL/rotating.fs")
	{
		shader.use();
		shader.setInt("idxCase",idxCase);
	}



	// Properties
	float radVelo; // rotating radian velocity
	int idxCase;
	// TODO: consider to make it private?
};



// global functions
/* Case insensitive comparasion
Adapted from Timmmm, https://stackoverflow.com/a/4119881
*/
bool iequals(const string& a, const string& b);
//TODO: how to reduce the duplication of this method in both files?
// Create a new class? move to errorHandling class

#endif // !_GUARD_TALK2SCREEN_H
