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
* Abstract: this file contains all function definitions
*			used to present visual stimulus to fish
*
* A great portion of code was adapted from learnopengl.com, which is a great
* website to learn OpenGL
*
* Current Version: 2.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: May 12, 2018

* Replaced Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/


// Include user-defined libraries
#include "talk2screen.h"
#include <algorithm>
#include <string>

using namespace std;

void Screen::initialize(
	std::vector<string> imgNames, // image file names
	vector<int> patchesOfAreas
	)
{
	const vector<vector<float>> allAreaPos =
	{
		{ 0.068f, 0.300f, 0.258f, 0.668f },
		{ 0.840f, -0.810f, 0.258f, 0.73f },
		{ -0.668f, -0.810f, 0.258f, 0.73f }
	};

	//y dividing positions for all patches
	const vector<vector<int>> yPatternDivs =
	{
		{ 818, 818, 942, 942 },
		{ 247, 247, 365, 365 },
		{ 238, 238, 358, 358 }
	};

	cout << "Initializing the projector screen .. " << endl;
	/* GLFW initialize and configure */
	init_glfw_window();

	/* glad: load all OpenGL function pointers */
	init_glad();

	// Initialize all areas
	numAreas = imgNames.size();
	allAreas.reserve(numAreas);
	for (int i = 0; i < numAreas; i++)
	{
		AreaData area(allAreaPos[i], patchesOfAreas[i]);
		area.initialize(yPatternDivs[i], imgNames[i]);
		allAreas.push_back(area);
	}
	cout << "Screen initialization succeeded." << endl << endl;

}

/* Initialize GLFW window */
void Screen::init_glfw_window() // TODO: consider to make it void by using exception to handle errors.
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	int count;
	monitors = glfwGetMonitors(&count);
	mode = glfwGetVideoMode(monitors[1]);

	// glfw window creation
	try {
		window = glfwCreateWindow(mode->width, mode->height, "VR", monitors[1], NULL);
		tryCatchNull(window, "Failed to create a GLFW window!");
	} catch (string msg) {
		cout << msg << endl;
		glfwTerminate();
		waitUserInput2exit();
	}

	cout << "Screen width: " << mode->width << endl;
	cout << "Screen height: " << mode->height << endl;
	glfwMakeContextCurrent(window);

}

/* Initiate glad for using OpenGL functions */
bool Screen::init_glad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return false;
	}
	return true;
}

void Screen::updatePattern()
{
	for (int i = 0; i < allAreas.size(); i++)
	{
		AreaData area = allAreas[i];
		for (int j = 0; j < area.numPatches; j++)
		{
			area.allPatches[j].updatePattern();
		}
	}
	renderTexture();
}

void Screen::updatePattern(int cIdx)
{

	AreaData area = allAreas[cIdx];
	for (int j = 0; j < area.numPatches; j++)
	{
		area.allPatches[j].updatePattern();
	}
	renderTexture();
}

void Screen::reversePattern()
{

}

void Screen::showBlackPattern() {
	for (int i = 0; i < numAreas; i++)
	{
		for (int j = 0; j < allAreas[i].numPatches; j++)
		{
			allAreas[i].allPatches[j].pIdx = 2;
			allAreas[i].allPatches[j].updatePattern();
		}
	}
	renderTexture();
}

void Screen::renderTexture()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int  i = 0; i < allAreas.size(); i++)
		allAreas[i].renderTexture(i);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();// DO NOT DELETE!!! It processes all pending events, such as mouse move

}

void Area::initialize(vector<int> yDivideVec, string imgName)
{
	for (int i = 0; i < numPatches; i++)
	{
		vector<float> patchRect = rect;
		switch (i) {
			case 0:
				break;// do nothing
			case 1:
			{
				patchRect[0] -= patchRect[2] / 2; // minus half area width
				break;
			}
			case 2:
			{
				patchRect[1] += patchRect[3] / 2;
				break;
			}
			case 3:
			{
				patchRect[0] -= patchRect[2] / 2;
				patchRect[1] += patchRect[3] / 2;
				break;
			}
			default:;
		}
		patchRect[2] /= 2; // the width of patch is half of area width
		patchRect[3] /= 2; // the height of patch is half of area width
		PatchData patch(patchRect, yDivideVec[i]);
		patch.initialize();
		allPatches.push_back(patch);
	}
	loadTextureIntoBuffers(imgName);
}

/* Reverse the position of the CS pattern */
void Area::reverseAllPatches() {
	for (int j = 0; j < numPatches; j++)
	{
		allPatches[j].pIdx = !allPatches[j].pIdx;
		allPatches[j].updatePattern();
	}

}


void Area::loadTextureIntoBuffers(string imgName)
{
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	try{
		unsigned char *data = stbi_load(imgName.c_str(), &width, &height, &nrChannels, 0);
		if (!data)
			throw "Failed to load texture!";
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	} catch (const char* errorMsg) {
		cout << errorMsg << endl;
		waitUserInput2exit();
	}

}

void Area::renderTexture(int areaIdx)
{
	glActiveTexture(GL_TEXTURE0 + areaIdx);
	glBindTexture(GL_TEXTURE_2D, texture0);
	for (int j = 0; j < numPatches; j++)
	{
		allPatches[j].shader.use();
		glUniform1i(glGetUniformLocation(allPatches[j].shader.ID, "texture0"), areaIdx);
		glBindVertexArray(allPatches[j].VAO);
		glDrawElements(GL_TRIANGLES, TRIANGLES_PER_PATCH * 3, GL_UNSIGNED_INT, 0);
	}
}

bool Patch::initialize()
{
	initVertices();
	return true;
}

void Patch::initVertices()
{
	const unsigned int indices[TRIANGLES_PER_PATCH * 3] =
	{
		0, 1, 2,
		0, 3, 2
	};

	float vertices[32] =
	{	// positions (0-2)      // colors (3-5)         // texture coordinates (6-7)
		rect[0], rect[1], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top left
		rect[0], rect[1] + rect[3], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, rect[3], // bottom left
		rect[0] - rect[2], rect[1] + rect[3], 0.0f, 0.0f, 0.0f, 0.0f, rect[2], rect[3], // bottom right
		rect[0] - rect[2], rect[1], 0.0f, 0.0f, 0.0f, 0.0f, rect[2], 0.0f // top right
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Patch::updatePattern()
{
	shader.use();
	shader.setInt("patternIdx", pIdx);
}
