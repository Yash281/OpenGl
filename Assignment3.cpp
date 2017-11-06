#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SHADER_H.h"

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include<math.h>


//Globals
glm::mat4 model, cam;

GLuint SCR_WIDTH = 900;
GLuint SCR_HEIGHT = 900;
GLuint VBO[4], VAO[4], EBO;

float zeroX = SCR_WIDTH / 2, zeroY = SCR_HEIGHT / 2;
float twoPI = 2 * M_PI;

double x, y, piY, piX;

int circledef = 20;
int circlecount = 0;

int mode = 2;
int call = 0;
int indices = 0;
int edit = -1;
int editloc;

std::vector<float> Circle;
std::vector<int> Line, Triangle, Quad, Indices, Ccount, Cstart;

template <class T>
void find_closest(std::vector<T>& a)
{
	double dist = 10000;
	for (int z = 0; z < a.size(); z += 3)
	{
		double comp = sqrt(pow(x - a[z], 2) + pow(y - a[z + 1], 2));
		if (dist > comp)
		{
			dist = comp;
			editloc = z;
		}
	}
}

void draw_line()
{
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, Line.size() * sizeof(int), &Line.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_triangle()
{
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, Triangle.size() * sizeof(int), &Triangle.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_quad(bool u = 1)
{
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, Quad.size() * sizeof(int), &Quad.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);
	if (u)
	{
		//Indices.insert(Indices.end(), { indices, indices + 1, indices + 2, indices, indices + 2, indices + 3 });
		Indices.push_back(indices);
		Indices.push_back(indices + 1);
		Indices.push_back(indices + 2);
		Indices.push_back(indices);
		Indices.push_back(indices + 2);
		Indices.push_back(indices + 3);
		indices += 4;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), &Indices.front(), GL_DYNAMIC_DRAW);
	}
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_circle()
{
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, Circle.size() * sizeof(float), &Circle.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void edit_circle()
{
	int cx, cy;
	float rx, ry, radius;

	if (editloc % ((circledef + 2) * 3) == 0)
	{
		cx = Circle[editloc];
		cy = Circle[editloc + 1];

		Circle[editloc++] = x;
		Circle[editloc++] = y;
		++editloc;

		rx = Circle[editloc];
		ry = Circle[editloc + 1];

		radius = sqrt(pow(cx - rx, 2) + pow(cy - ry, 2));

		cx = x;
		cy = y;
	}

	else
	{
		rx = x;
		ry = y;

		while (editloc % ((circledef + 2) * 3) != 0)
			--editloc;

		cx = Circle[editloc++];
		cy = Circle[editloc++];
		++editloc;

		radius = sqrt(pow(cx - rx, 2) + pow(cy - ry, 2));
	}

	for (int i = 1; i <= circledef + 1; ++i)
	{
		rx = cx + radius*cos(i*twoPI / circledef);
		ry = cy + radius*sin(i*twoPI / circledef);
		Circle[editloc++] = rx;
		Circle[editloc++] = ry;
		++editloc;
	}
	draw_circle();
}

void fill_circle()
{
	int cx, cy;
	float rx, ry;

	rx = Circle[Circle.size() - 3];
	ry = Circle[Circle.size() - 2];

	Circle.erase(Circle.end() - 3, Circle.end());

	cx = Circle[Circle.size() - 3];
	cy = Circle[Circle.size() - 2];

	float radius = sqrt(pow(cx - rx, 2) + pow(cy - ry, 2));

	for (int i = 1; i <= circledef + 1; ++i)
	{
		rx = cx + radius*cos(i*twoPI / circledef);
		ry = cy + radius*sin(i*twoPI / circledef);
		Circle.push_back(rx);
		Circle.push_back(ry);
		Circle.push_back(0);
	}

	Cstart.push_back(circlecount*(circledef + 2));
	++circlecount;
	Ccount.push_back(circledef + 2);

	draw_circle();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	cam = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.0f, 100.0f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Mode
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		if (call == 0)
			mode = 1;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		if (call == 0)
			mode = 2;

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		if (call == 0)
			mode = 3;
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		if (call == 0)
			mode = 4;
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		if (call == 0)
			edit = 0;
}

void mbutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		x = piX;
		y = piY;
		y = -y + SCR_HEIGHT;

		if (edit == -1)
		{
			switch (mode)
			{
			case 1:
				Line.push_back(x);
				Line.push_back(y);
				Line.push_back(0);
				break;
			case 2:
				Triangle.push_back(x);
				Triangle.push_back(y);
				Triangle.push_back(0);
				break;
			case 3:
				Quad.push_back(x);
				Quad.push_back(y);
				Quad.push_back(0);
				break;
			case 4:
				Circle.push_back(x);
				Circle.push_back(y);
				Circle.push_back(0);
				break;
			}
			call++;
		}

		if (edit == 1)
		{
			switch (mode)
			{
			case 1:
				Line[editloc] = x;
				Line[editloc + 1] = y;
				draw_line();
				break;
			case 2:
				Triangle[editloc] = x;
				Triangle[editloc + 1] = y;
				draw_triangle();
				break;
			case 3:
				Quad[editloc] = x;
				Quad[editloc + 1] = y;
				draw_quad(0);
				break;
			case 4:
				edit_circle();
				break;
			}
			edit = -1;
			call = 0;
		}
		if (edit == 0)
		{
			switch (mode)
			{
			case 1:
				find_closest(Line);
				break;
			case 2:
				find_closest(Triangle);
				break;
			case 3:
				find_closest(Quad);
				break;
			case 4:
				find_closest(Circle);
				break;
			}
			edit = 1;
			call++;
		}
	}
}

int main()
{
	//Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mbutton_callback);


	Shader S("vs.glsl", "fs.glsl");

	//Buffers
	glGenVertexArrays(4, VAO);
	glGenBuffers(4, VBO);
	glGenBuffers(1, &EBO);

	//Cam
	cam = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.0f, 100.0f);
	glfwSetCursorPos(window, zeroX, zeroY);

	//Loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwGetCursorPos(window, &piX, &piY);

		S.setMat4("proj", cam);

		S.use();

		switch (mode)
		{
		case 1:
			if (call == 2)
			{
				call = 0;
				draw_line();
			}
			break;
		case 2:
			if (call == 3)
			{
				call = 0;
				draw_triangle();
			}
			break;
		case 3:
			if (call == 4)
			{
				call = 0;
				draw_quad();
			}
			break;
		case 4:
			if (call == 2)
			{
				call = 0;
				fill_circle();
			}
			break;
		}

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_LINES, 0, Line.size() / 3);

		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, Triangle.size() / 3);

		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

		if (circlecount > 0)
		{
			glBindVertexArray(VAO[3]);
			glMultiDrawArrays(GL_TRIANGLE_FAN, &Cstart.front(), &Ccount.front(), circlecount);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean-up
	glDeleteVertexArrays(4, VAO);
	glDeleteBuffers(4, VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}