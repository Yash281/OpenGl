#include<iostream>
using namespace std;
#include<glad\glad.h>
#include<glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "ShaderH.h"



glm::mat4 transform;
glm::mat4 model;
glm::mat4 project;
glm::mat4 view;

int height = 600;
int width = 800;
float resoultion = (float)width / (float)height;



glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -4.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float cameraSpeed = 2.5 * deltaTime;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		transform = glm::translate(transform, glm::vec3(0.0f, 0.05f, 0.0f));
	
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		transform = glm::translate(transform, glm::vec3(0.0f, -0.05f, 0.0f));
	
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		transform = glm::translate(transform, glm::vec3(0.05f, 0.0f, 0.0f));
	
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		transform = glm::translate(transform, glm::vec3(-0.05f, 0.0f, 0.0f));
	
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		transform = glm::rotate(transform, (float)glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		transform = glm::rotate(transform, (float)glm::radians(50.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		transform = glm::scale(transform, glm::vec3(0.9f,0.9f,0.9f));
	
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
		transform = glm::scale(transform, glm::vec3(1.1f,1.1f,1.1f));
	
	if(key== GLFW_KEY_O && action ==GLFW_PRESS)
	project =	glm::ortho(-4.0f,4.0f,-4.0f,4.0f,0.1f,100.0f);
	
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
    project = glm::perspective(glm::radians(100.0f), resoultion, 0.1f, 100.0f);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	
		cameraPos += cameraSpeed * cameraFront;
	
	
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	
		cameraPos -= cameraSpeed * cameraFront;
	
	
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		
	
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		




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

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("vertexShader.glsl", "fragmentShader.glsl");

	

			//Vertices
				float vertices[] = {
				0.5f,  0.5f, 0.5f, 1.0f,0.0f,0.0f, //0
				0.5f, -0.5f, 0.5f, 0.0f,1.0f,1.0f, //1
			   -0.5f, -0.5f, 0.5f, 0.0f,0.0f,1.0f,  //2
			   -0.5f,  0.5f, 0.5f, 1.0f,0.0f,0.0f, //3
				0.5f,  0.5f, -0.5f,0.0f,1.0f,0.0f,  //4
				0.5f, -0.5f, -0.5f,0.0f,0.0f,1.f, //5
			   -0.5f, -0.5f, -0.5f,1.0f,0.0f,0.0f, //6
			   -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,0.0f  //7

			};
			//Indices
			GLuint indices[] = {  
				0, 1, 2, //1
				0, 2, 3, //2 
				0 ,3, 7, //3
				0, 4, 7, //4
				3, 7, 2, //5
				7, 2, 6, //6
				1, 6 ,2 , //7
				1 ,6, 5, //8
				1 ,0 ,4, //9
				1 ,4 ,5, //10
				4 , 5, 6, //11
				4, 6 ,7, //12
				

			};

			

			
			
			transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -3.0f));
			unsigned int VBO, VAO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			//Position Attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			//color Attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	project=glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 100.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSetKeyCallback(window, key_callback);
		glClearColor(0.09f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		ourShader.use();
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", project);
		ourShader.setMat4("model", model);
		ourShader.setMat4("transform", transform);


		glBindVertexArray(VAO);
	


		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}
