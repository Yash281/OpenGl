#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SHADERH.h"


#include <iostream>
#define _USE_MATH_DEFINES
#include<math.h>

//Globals
glm::mat4 model, cam;

const GLuint SCR_WIDTH = 900;
const GLuint SCR_HEIGHT = 900;
GLfloat aspect = SCR_WIDTH / SCR_HEIGHT;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Mode
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
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

	Shader Head("vertexShader.glsl", "fsHead.glsl");
	Shader Body("vertexShader.glsl", "fsBody.glsl");
	Shader Limb("vertexShader.glsl", "fsLimb.glsl");
	Shader LimbAnim("vsArm2.glsl", "fsArm2.glsl");

	//Points
	GLfloat Body1[] =
	{
		0, 0, 0,
		-5, 0, 0,
		-5, -6, 0,
		0, -6, 0
	};
	GLfloat Arm1[] =
	{
		-5, 0, 0,
		-6, 1, 0,
		-8, -5, 0,
		-7, -6, 0
	};

	GLfloat Arm2[] =
	{
		0, 0, 0,
		1, 1, 0,
		3, -5, 0,
		2, -6, 0
	};

	GLfloat Legs[] =
	{
		-5, -6, 0,
		-4, -6, 0,
		-4, -9, 0,
		-5, -9, 0,
		0, -6, 0,
		-1, -6, 0,
		-1, -9, 0,
		0, -9, 0
	};

	GLuint LIndices[] =
	{
		0,1,2,
		0,3,2,
		4,5,6,
		4,7,6
	};

	GLuint Indices[] =
	{
		0,1,2,
		0,3,2
	};

	GLfloat Head1[36];
	Head1[0] = -2.5;
	Head1[1] = 2;
	Head1[2] = 0;

	int j = 1;
	float twoPI = 2 * M_PI;

	for (int i = 3; i<34; ++i)
	{
		Head1[i++] = -2.5 + 2 * cos(j*twoPI / 10);
		Head1[i++] = 2 + 2 * sin(j*twoPI / 10);
		Head1[i] = 0;
		++j;
	}

	//Buffers
	GLuint VBO[5], VAO[5], EBO[2];
	glGenVertexArrays(5, VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(2, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Body1), Body1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Arm1), Arm1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Arm2), Arm2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Legs), Legs, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(LIndices), LIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Head1), Head1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	//Cam
	cam = glm::ortho(-10.0f, 5.0f, -10.0f, 5.0f, 0.0f, 100.0f);

	//Loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0, 0, 1));

		Head.use();
		Head.setMat4("projection", cam);
		glBindVertexArray(VAO[4]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 12);


		Body.use();
		Body.setMat4("projection", cam);

		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		Limb.use();
		Limb.setMat4("projection", cam);

		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);


		LimbAnim.use();
		LimbAnim.setMat4("projection", cam);
		LimbAnim.setMat4("transform", model);

		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean-up
	glDeleteVertexArrays(5, VAO);
	glDeleteBuffers(4, VBO);
	glDeleteBuffers(2, EBO);



	glfwTerminate();
	return 0;
}
