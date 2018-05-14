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



using namespace std;


bool PatchData::initialize(float* pos)
{
	initVertices(pos);
	return true;
}

void PatchData::initVertices(float* pos)
{
	const unsigned int indices[TRIANGLES_PER_PATCH * 3] =
	{
		0, 1, 2,
		0, 3, 2
	};

	float vertices[32] =
	{	// positions (0-2)      // colors (3-5)         // texture coordinates (6-7)
		pos[0], pos[1], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top left
		pos[0], pos[1] + height, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, height, // bottom left
		pos[0] - width, pos[1] + height, 0.0f, 0.0f, 0.0f, 0.0f, width, height, // bottom right
		pos[0] - width, pos[1], 0.0f, 0.0f, 0.0f, 0.0f, width, 0.0f // top right
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

void PatchData::updatePattern()
{
	shader.use();
	shader.setInt("patternIdx", pIdx);
}

void ScreenData::updatePattern()
{
	for (int i = 0; i < allAreas.size(); i++)
	{
		AreaData area = allAreas[i];
		for (int j = 0; j < area.numPatches; j++)
		{
			area.allPatches[j].updatePattern();
		}
	}

}

/* update pattern for specific area */
void ScreenData::updatePattern(int cIdx)
{
	
		AreaData area = allAreas[cIdx];
		for (int j = 0; j < area.numPatches; j++)
		{
			area.allPatches[j].updatePattern();
		}
	

}

bool AreaData::initialize(const int* yDivideArr)
{
	for (int i = 0; i < numPatches; i++)
	{
		PatchData patch(width/2,height/2,yDivideArr[i]);
		switch (i){
			case 0: 
				break;// do nothing
			case 1:
			{
				pos[0] -= patch.width;
				break;
			}
			case 2:
			{
				pos[1] += patch.height;
				break;
			}
			case 3:
			{
				pos[0] -= patch.width;
				pos[1] += patch.height;
				break;
			}
			default:;
		}

		patch.initialize(pos);
		allPatches.push_back(patch);
	}
	return true;
}

bool ScreenData::initGLFWenvironment()
{
	/* Initialize GLFW environment */
	if (!init_glfw_window())
		return false;
	if (!init_glad())
		return false;
	return true;
}

// allAreaPos is a 2D array
bool ScreenData::initialize(const char* imgName, int numAreas)
{
	float areaWidth = 0.24f; // In screen coordinates
	float areaHeight = 0.88f;

	const float allAreaPos[][4] =
	{
		{ 0.233f, 0.470f, 0.30f, areaHeight }, // midlle arena
		{ 0.800f, -0.680f, 0.28f, areaHeight }, // left arena
		{ -0.760f, -0.680f, areaWidth, areaHeight } // right arena
	};
	const int yDivideArr[][4] = // in pixels
	{
		{ 894, 894, 1000, 1000 }, // middle arena
		{ 305, 305, 400, 400 }, // left arena
		{ 305, 305, 400, 400 } // right arena
	};

	initGLFWenvironment();

	

	for (int i = 0; i < numAreas; i++)
	{
		//const int arr[] = { 100,100,200,200 }; // array of delimY positions
		//vector<int> delimYVec(arr, arr + sizeof(arr) / sizeof(arr[0]));
		
		AreaData area(allAreaPos[i],2); // make it a (x,y,width,height) list
		area.initialize(yDivideArr[i]);
		allAreas.push_back(area);
	}

	
	if (!loadTextureIntoBuffers(imgName))
		return false;
	return true;
}

/* Initialize GLFW window */
bool ScreenData::init_glfw_window()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	int count;
	monitors = glfwGetMonitors(&count);
	mode = glfwGetVideoMode(monitors[1]);
	// glfw window creation
	// --------------------
	window = glfwCreateWindow(mode->width, mode->height, "VR", monitors[1], NULL);
	cout << "Screen width: " << mode->width << endl;
	cout << "Screen height: " << mode->height << endl;
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	return true;
}

/* Initiate glad for using OpenGL functions */
bool ScreenData::init_glad() 
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return false;
	}
	return true;
}

/* Load one texture for the entire screen */
bool ScreenData::loadTextureIntoBuffers(const char* imgName)
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
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis
	unsigned char *data = stbi_load(imgName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return false;
	}
	stbi_image_free(data);
	return true;
}

/* Render designed pattern on the screen */
void ScreenData::renderTexture()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	// render in shaders
	for (int i = 0; i < allAreas.size(); i++)
	{
		for (int j = 0; j < allAreas[i].numPatches; j++)
		{
			allAreas[i].allPatches[j].shader.use();
			glBindVertexArray(allAreas[i].allPatches[j].VAO);
			glDrawElements(GL_TRIANGLES, TRIANGLES_PER_PATCH * 3, GL_UNSIGNED_INT, 0);
		}
	}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();// DO NOT DELETE!!! It processes all pending events, such as mouse move 

}

