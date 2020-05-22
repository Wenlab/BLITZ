#include "talk2screen.h"


using namespace std;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

float ScreenData::cross_product(float x0, float y0, float x1, float y1, float x, float y) {
	float result;
	result = (x1 - x0) * (y - y0) - (x - x0) * (y1 - y0);
	return result;
}

void ScreenData::get_endpoint(float x1, float y1, float x2, float y2) {
	if (x1 == x2) {
		endpoint[0][0] = x1;
		endpoint[0][1] = 1.0;
		endpoint[1][0] = x1;
		endpoint[1][1] = -1.0;
	}
	else if (y1 == y2) {
		endpoint[0][0] = 1.0;
		endpoint[0][1] = y1;
		endpoint[1][0] = -1.0;
		endpoint[1][1] = y1;
	}
	else {
		endpoint[0][0] = (x2 + y1 * x2 - x1 - x1 * y2) / (y1 - y2);
		endpoint[0][1] = -1.0;
		endpoint[1][0] = (x1 - x2 + y1 * x2 - x1 * y2) / (y1 - y2);
		endpoint[1][1] = 1.0;
	}
}


bool ScreenData::PatternInitialize()
{

	for (int i = 0; i < 4; i++) {
		if (cross_product(point_1[0], point_1[1], point_2[0], point_2[1], border_pts[i][0], border_pts[i][1]) * left_true > 0) {
			bright_pts.push_back(border_pts[i][0]);
			bright_pts.push_back(border_pts[i][1]);
		}
		else {
			dark_pts.push_back(border_pts[i][0]);
			dark_pts.push_back(border_pts[i][1]);
		}
	}

	get_endpoint(point_1[0], point_1[1], point_2[0], point_2[1]);



	// vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

	float vertices1[] = {
		 endpoint[0][0], endpoint[0][1], 0.0f,  // top right
		 endpoint[1][0], endpoint[1][1], 0.0f,  // bottom right
		bright_pts[0], bright_pts[1], 0.0f,  // bottom left
	};

	float vertices2[12] = {
		 endpoint[0][0], endpoint[0][1], 0.0f,  // top right
		 endpoint[1][0], endpoint[1][1], 0.0f,  // bottom right
		0.0f, 0.0f,0.0f ,  // bottom left
		0.0f,  0.0f, 0.0f   // top left 
	};

	float vertices3[] = {
		 endpoint[0][0], endpoint[0][1], 0.0f,
		 endpoint[1][0], endpoint[1][1], 0.0f,
		dark_pts[0], dark_pts[1], 0.0f,
	};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	switch (int(bright_pts.size() / 2)) {
	case 1:
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, bright[0], bright[1], bright[2], bright[3]);
		background_color = dark;
		num_triangles = 1;
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

		break;
	case 2:

		vertices2[6] = bright_pts[0];
		vertices2[7] = bright_pts[1];
		vertices2[9] = bright_pts[2];
		vertices2[10] = bright_pts[3];

		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, bright[0], bright[1], bright[2], bright[3]);
		background_color = dark;
		num_triangles = 2;

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

		break;
	case 3:

		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, dark[0], dark[1], dark[2], dark[3]);
		background_color = bright;
		num_triangles = 1;

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);
		break;
	default:
		cout << "VBO/EBO setting error:" << int(bright_pts.size() / 2) << endl;
		return false;

	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	return true;
}

void ScreenData::reversePattern() {
	if (background_color == bright) {
		background_color = dark;
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, bright[0], bright[1], bright[2], bright[3]);
	}
	else {
		background_color = bright;
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, dark[0], dark[1], dark[2], dark[3]);
	}
}

bool ScreenData::ScreenInitialize() {
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
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return false;
	}
	return true;

}

void ScreenData::renderTexture() {
	//processInput(window);


	glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
	glClear(GL_COLOR_BUFFER_BIT);


	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, num_triangles * 3, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}