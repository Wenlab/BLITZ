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

// User-defined macros
#define NUM_ARENA 3
#define PATCHES_PER_ARENA 4
#define NUM_SHADER 12
#define VERTEXS_PER_SHADER 4
#define TRIANGLES_PER_PATCH 2

/* represent pattern changes for a single patch (shader) */
class PatchData 
{
private:
	; // nothing for now
public:
	// methods
	PatchData(float patchWidth, float patchHeight, int patchYdivide,
			  const char vertexPath[] = "3rdPartyLibs/OpenGL/shader.vs",
			  const char fragmentPath[] = "3rdPartyLibs/OpenGL/shader.fs")
			: shader(vertexPath, fragmentPath)
	{
		width = patchWidth;
		height = patchHeight;
		yDivide = patchYdivide;
		/* Frequent updating variable */
		pIdx = 0;
		
		shader.use();
		shader.setInt("yDivide",yDivide);
		shader.setInt("patternIdx", pIdx);
	}
	
	bool initialize(float* pos);
	/* Initialize vertices and their buffers with providing pos(x,y) */
	void initVertices(float* pos);
	/* Update pattern by giving the shader new pattern index */
	void updatePattern();
	// properties
	float width, height; // in screen coordinates
	int yDivide;
	int pIdx; // pattern index
	
	Shader shader;
	unsigned int VAO, VBO, EBO;
	unsigned int texture;
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
	AreaData(const float* areaPos, int n = PATCHES_PER_ARENA)
	{
		numPatches = n;
		pos[0] = areaPos[0];
		pos[1] = areaPos[1];
		width = areaPos[2]; // Make it argument
		height = areaPos[3];
	}
	bool initialize(const int* yDivideArr);
	// properties
	std::vector<PatchData> allPatches;
	int numPatches;
	float pos[2]; // upper-left corner
	float width, height;
};

class ScreenData 
{
private:
	; // nothing for now
public:
	// methods
	ScreenData() // constructor
	{
		
	}
	bool initialize(const char* filename, int numAreas);
	/* Initialize GLFW environment*/
	bool initGLFWenvironment();
	bool init_glfw_window();
	bool init_glad();
	bool loadTextureIntoBuffers(const char* filename);
	//Shader init_shader(const char* vsName, const char* fsName, int delimY);
	//void init_texture_buffers(int idxArena, int idxPatch, const float* vertex);
	//bool load_texture_into_buffers(const char* fileName);

	/* Update pattern for specific area */
	void updatePattern(int cIdx);
	/* Update patternIdx for all shaders in the screen */
	void updatePattern();
	/* Render designed pattern on the screen */
	void renderTexture();
	/* Update pattern for a single shader */
	//void updatePatternInShader(int idxArena, int idxPatch, int patternIdx);


	// properties
	GLFWmonitor** monitors;
	const GLFWvidmode* mode;
	GLFWwindow* window;
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
	unsigned int texture0;
	int numAreas;

};

#endif // !_GUARD_TALK2SCREEN_H

