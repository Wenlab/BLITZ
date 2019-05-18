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

#include <vector> // include range-based for loop
#include <algorithm>
#include <string>

#include "talk2screen.h"


using namespace std;

// TODO: decide the renderType after the initialization
void Screen::initialize(string imgName, vector<float> boundBox, string renderType)
{
	cout << "Initializing the projector screen .. " << endl;
	/* GLFW initialize and configure */
	init_glfw_window();
	/* glad: load all OpenGL function pointers */
	init_glad();

	// initialize an area object
	Area areaObj(boundBox, renderType);
	areaObj.initialize(imgName);
	allAreas.push_back(areaObj);

	cout << "Screen initialization succeeded." << endl << endl;

}

void Screen::init_glfw_window()
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
	window = glfwCreateWindow(mode->width, mode->height, "VR", monitors[1], NULL);
	tryCatchNull(window, "Failed to create a GLFW window!"); // glfwTerminate(); Check if we need to run this method

	cout << "Screen width: " << mode->width << endl;
	cout << "Screen height: " << mode->height << endl;
	glfwMakeContextCurrent(window);

}

void Screen::init_glad()
{
	tryCatchFalse(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
		"Failed to initialize GLAD!");
}

void Screen::show()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < allAreas.size(); i++)
		allAreas[i].renderTexture(i);
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();// DO NOT DELETE!!! It processes all pending events, such as mouse move

}

void Screen::show(int idxArea)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	allAreas[idxArea].renderTexture(idxArea);// TODO: consider to use instance variable instead
											 // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();// DO NOT DELETE!!! It processes all pending events, such as mouse move

}

void Screen::showBlackPattern()
{
	int idxBlack = 2;
	for (Area& area : allAreas)
		area.updateIdxCase(idxBlack);

	show();
}

void Screen::reverse()
{
	for (Area& area : allAreas)
		area.negateIdxCase();

	show();
}

void Screen::reverse(vector<vector<bool>> patchIndices)
{
	for (int i = 0; i < patchIndices.size(); i++)
	{
		vector<bool> onstatus = patchIndices[i];
		for (int j = 0; j < onstatus.size(); j++)
		{
			if (onstatus[j])
				allAreas[i].negateIdxCase(j);
		}
	}
	show();
}

void Screen::reverse(int sec2start, int period)
{
	// TODO: check this code
	vector<vector<bool>> patchIndices(numAreas);
	for (int i = 0; i < numAreas; i++)
	{// TODO: deal with situation that areas have different patches
		vector<bool> vec(PATCHES_PER_ARENA, true);
		patchIndices[i] = vec;
	}
	if ((sec2start / period) % 2) // has to be odd-times period to reverse
		reverse(patchIndices);
}

void Screen::reverse(int sec2start, int period, vector<vector<bool>> patchIndices)
{
	if ((sec2start / period) % 2) // has to be odd-times period to reverse
		reverse(patchIndices);
}


void Area::initialize(string imgName)
{
	// nx - Maximum number of images in a row
	// ny - Maximum number of images in a column
	int nx, ny;

	// If the number of patches is lesser than 0 or greater than 4
	// return without displaying
	if (numPatches <= 0)
	{
		printf("Number of patches too small....\n");
		return;
	}
	else if (numPatches > 4) {
		printf("Number of arguments too large, can only handle maximally 4 images at a time ...\n");
		return;
	}
	else if (numPatches == 1) {
		nx = ny = 1;
	}
	else if (numPatches == 2) {
		nx = 2; ny = 1;
	}
	else if (numPatches == 3) {
		nx = ny = 2;
	}
	else if (numPatches == 4) {
		nx = ny = 2;
	}
	float xDivLen = boundBox[2] / nx; // the length of each division in x
	float yDivLen = boundBox[3] / ny; // the length of each division in y

	for (int i = 0; i < numPatches; i++)
	{
		vector<float> patchBoundBox = boundBox;
		patchBoundBox[2] = xDivLen;
		patchBoundBox[3] = yDivLen;
		switch (i) {
		case 0: // the first patch at the upper-left corner
			break;
		case 1:
		{
			patchBoundBox[0] -= xDivLen;// minus the division length
			break;
		}
		case 2:
		{
			patchBoundBox[1] += yDivLen;// minus the division length
			break;
		}
		case 3:
		{
			patchBoundBox[0] -= xDivLen;// minus the division length
			patchBoundBox[1] += yDivLen;// minus the division length
			break;
		}
		if (renderType.compare("full") == 0)
		{
			FullPatch patchObj(patchBoundBox);
			patchObj.initialize();
			allPatches.push_back(&patchObj);
		}
		else if (renderType.compare("half") == 0)
		{
			int yDiv = 380;
			HalfSplitPatch patchObj(patchBoundBox, yDiv);
			patchObj.initialize();
			allPatches.push_back(&patchObj);
		}
		else if (renderType.compare("rotation") == 0)
		{
			RotatingPatch patchObj(patchBoundBox);
			patchObj.initialize();
			allPatches.push_back(&patchObj);
		}
		else {
			cout << "Unknown renderType! Please select one of the following:\n"
				<< "full, half, rotation" << endl;
			waitUserInput2exit();
		}

		}
		loadTextureIntoBuffers(imgName);
	}
}

void Area::loadTextureIntoBuffers(string imgName)
{

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	unsigned char *data = stbi_load(imgName.c_str(), &width, &height, &nrChannels, 0);
	tryCatchFalse(data, "Failed to load texture!");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

}

void Area::updateIdxCase(int value)
{

	tryCatchFalse(!renderType.compare("full"),
		"Wrong renderType! This method is for full rendering only!");

	/* TODO: test this range-based loop implementation
	for (auto patch : allPatches)
	{
	patch.idxCase = value; // TODO: consider to have a method?
	patch.uploadInt2GPU("idxCase",idxCase);
	}
	*/
	for (int i = 0; i < numPatches; i++)
	{
		allPatches[i]->idxCase = value;
		allPatches[i]->uploadInt2GPU("idxCase", value);
	}
}

void Area::negateIdxCase()
{

	tryCatchFalse(!renderType.compare("full"),
		"Wrong renderType! This method is for full rendering only!");

	// TODO: consider to use range-based loop implementation?
	for (int i = 0; i < numPatches; i++)
	{
		allPatches[i]->idxCase = !allPatches[i]->idxCase;
		allPatches[i]->uploadInt2GPU("idxCase", allPatches[i]->idxCase);
	}
}

void Area::negateIdxCase(int patchIdx)
{

	tryCatchFalse(!renderType.compare("full"),
		"Wrong renderType! This method is for full rendering only!");

	allPatches[patchIdx]->idxCase = !allPatches[patchIdx]->idxCase;
	allPatches[patchIdx]->uploadInt2GPU("idxCase", allPatches[patchIdx]->idxCase);


}

void Area::renderTexture(int areaIdx)
{
	glActiveTexture(GL_TEXTURE0 + areaIdx);
	glBindTexture(GL_TEXTURE_2D, textureID);
	for (int j = 0; j < numPatches; j++) // TODO: consider to use range-based loop implementation?
	{
		allPatches[j]->uploadInt2GPU("textureID", areaIdx);
		glBindVertexArray(allPatches[j]->VAO); // TODO: write a method?
		glDrawElements(GL_TRIANGLES, TRIANGLES_PER_PATCH * 3, GL_UNSIGNED_INT, 0);
	}
}

void Patch::initialize()
{
	initVertices();
	// Space for updates
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
		boundBox[0], boundBox[1], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top left
		boundBox[0], boundBox[1] + boundBox[3], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, boundBox[3], // bottom left
		boundBox[0] - boundBox[2], boundBox[1] + boundBox[3], 0.0f, 0.0f, 0.0f, 0.0f, boundBox[2], boundBox[3], // bottom right
		boundBox[0] - boundBox[2], boundBox[1], 0.0f, 0.0f, 0.0f, 0.0f, boundBox[2], 0.0f // top right
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

void Patch::uploadInt2GPU(string varName, int varValue)
{
	shader.use();
	shader.setInt(varName, varValue);
}

void Patch::uploadFloat2GPU(string varName, float varValue)
{
	shader.use();
	shader.setFloat(varName, varValue);
}

void Patch::setIdxCase(int value)
{
	idxCase = value;
}