#include<iostream>
using namespace std;
#include<glad\glad.h>
#include<glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include<math.h>


#include "ShaderH.h"

glm::mat4 project;

glm::mat4 transform;
int height = 600;
int width = 800;
float resoultion = (float)width / (float)height;


unsigned int VBO[4];
unsigned int VAO[4], EBO;



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


	

int main()
{	
	
	//Intializing GlFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	
	//Window Creation && Error Check
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "failed to Create Window " << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	
	//Intializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initalize GLAD";
		return -1;
	}

	Shader Body("vertexShader.glsl", "fsBody.glsl");
	Shader Head("vertexShader.glsl", "fsHead.glsl");
	Shader Limb("vertexShader.glsl", "fsLimb.glsl");
	Shader Arm("vsArm2.glsl", "fsArm2.glsl");
	Shader Arm3("vertexShader.glsl", "fsLimb.glsl");

	glGenVertexArrays(4, VAO);
	glGenBuffers(4, VBO);
	glGenBuffers(1, &EBO);

	float BodyVertex[] = { 0, 0, 0,
						-5, 0, 0,
						-5, -6, 0,
						0, -6, 0
						};
	GLuint BodyIndex[] = { 0,1,2,
						   0,3,2
						  };

	GLfloat Arm1[] =
	{  -5, 0, 0,
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



	float LimbVertex[] = { -5, -6, 0,
		-4, -6, 0,
		-4, -9, 0,
		-5, -9, 0,
		0, -6, 0,
		-1, -6, 0,
		-1, -9, 0,
		0, -9, 0
	};
	GLuint LimbIndex[] = { 0,1,2,
		0,3,2,
		4,5,6,
		4,7,6 };

	float Head1[36];
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
	glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BodyVertex), BodyVertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BodyIndex), BodyIndex, GL_STATIC_DRAW);
	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LimbVertex), LimbVertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(LimbIndex), LimbIndex, GL_STATIC_DRAW);
	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Head1), Head1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Arm1), Arm1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Arm2), Arm2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/


	glBindVertexArray(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	project = glm::ortho(-10.0f, 5.0f, -10.0f, 5.0f, 0.0f, 100.0f);
	while (!glfwWindowShouldClose(window))
	{
		glfwSetKeyCallback(window, key_callback);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT );


		transform = glm::rotate(transform, glm::radians(5.0f), glm::vec3(0, 0, 1));

		Body.use();
		Body.setMat4("projection", project);
		glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		Limb.use();
		Limb.setMat4("projection", project);
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		Arm3.use();
		Arm3.setMat4("projection", project);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		Head.use();
		Head.setMat4("projection", project);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

		/*Arm.use();
		Arm.setMat4("projection", project);
		Arm.setMat4("transform", transform);

		glBindVertexArray(VAO[4]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
	
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(4, VAO);
	glDeleteBuffers(4, VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}
