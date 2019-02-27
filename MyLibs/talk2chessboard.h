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

#ifndef _GUARD_TALK2CHESSBOARD_H
#define _GUARD_TALK2CHESSBOARD_H

/* Disable warning to function std::copy */
// ///CAN NOT Understand this statement!!!!! (USE "// ///" as the mark)
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
// ///#define NUM_ARENA 3
// ///#define PATCHES_PER_ARENA 4
// ///#define NUM_SHADER 12
// ///#define VERTEXS_PER_SHADER 4
// ///#define TRIANGLES_PER_PATCH 2

class ChessData
{
private:
	; // nothing for now
public:
	// methods
	ChessData(std::vector<float> ChessRect, // ///const int patchYdivide,
		const char vertexPath[] = "3rdPartyLibs/OpenGL/shader.vs",
		const char fragmentPath[] = "3rdPartyLibs/OpenGL/shader1.fs")
		: shader(vertexPath, fragmentPath)
		, rect(ChessRect)
		// ///, yDivide(patchYdivide)
	{
		/* Frequent updating variable */
		pIdx = 0;
		shader.use();
		// ///shader.setInt("yDivide", yDivide);
		shader.setInt("patternIdx", pIdx);
	}

	bool initialize(std::string imgName);
	/* Initialize vertices and their buffers with providing pos(x,y) */
	void initVertices();
	/* Update pattern by giving the shader new pattern index */
	void updatePattern();

	bool loadTextureIntoBuffers(std::string imgName);

	void renderTexture();

	/* Update pattern in test experiment */
	void updatePatternInTest(int sElapsed);
	/* Update pattern in baseline experiment */
	void updatePatternInBaseline(int sElapsed);
	/* Update pattern in the blackout experiment */
	void updatePatternInBlackout();


	// properties
	const std::vector<float> rect; // upper-left corner (x, y, width, height)
	// ///const int yDivide;
	int pIdx; // pattern index
	unsigned int texture0; // texture ID 
	Shader shader;
	unsigned int VAO, VBO, EBO;

	int lastScreenPatternUpdate;
	/* Interval for updating pattern in baseline session, which is a random number in range */
	int baselineInterval;
};




class BoardData
{
private: // only used within class
	GLFWmonitor * * monitors;
	GLFWwindow* window;

public:
	// methods
	BoardData(
	          
	) // constructor
		

	{

	}
	/* Initilize screen environment and coordinates */
	bool initialize(std::string filename);
	/* GLFW initialize and configure window */
	bool init_glfw_window();
	/* glad: load all OpenGL function pointers */
	bool init_glad();
	/* Update pattern for specific area */
	// ///void updatePattern(int cIdx);

	void renderTexture();
	


	// properties
	const GLFWvidmode* mode;
	std::vector<ChessData> Chess;

};

#endif
