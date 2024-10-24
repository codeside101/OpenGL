#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"


 
//verteices coordinates
GLfloat vertices[] =
{    //     COORDINATES                     //     COLORS           
    -0.5f, -0.5f, -0.5f,				       1.0f, 0.6f, 0.2f, 
	 0.5f, -0.5f, -0.5f,					   1.0f, 0.5f, 0.1f,
	 0.5f,  0.5f, -0.5f,					   1.0f, 0.7f, 0.3f, 
	-0.5f,  0.5f, -0.5f,                    1.0f, 0.65f, 0.25f, 

	-0.5f, -0.5f,  0.5f,                    1.0f, 0.55f, 0.15f, 
	 0.5f, -0.5f,  0.5f,                     1.0f, 0.6f, 0.2f,  
	 0.5f,  0.5f,  0.5f,					   1.0f, 0.8f, 0.4f,  
	-0.5f,  0.5f,  0.5f,					   1.0f, 0.7f, 0.3f   
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,  0, 2, 3, // Back face
	4, 5, 6,  4, 6, 7, // Front face
	0, 1, 5,  0, 5, 4, // Bottom face
	2, 3, 7,  2, 7, 6, // Top face
	0, 3, 7,  0, 7, 4, // Left face
	1, 2, 6,  1, 6, 5  // Right face
};

const unsigned int width=1000;
const unsigned int height=800;

int main()
{
	//initializing GLFW
	glfwInit();

	//telling GLFW what version of opengl we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//telling GLFW we are using the Core profile so that mean we only have the modern function
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a glfwWindow object of 1000 by 800 pixels, naming it "STL_Render"
	GLFWwindow* window = glfwCreateWindow(width, height, "STL_Render", NULL, NULL);

	//error check if the window fail to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//introduce the window into the current context
	glfwMakeContextCurrent(window);

	//load GLAD so it configures OpenGL
	gladLoadGL();

	//specify the viewport of opengl in the window
	//in this case the viewport goes x=0,y=0 to x=1000,y=800
	glViewport(0, 0, width, height);

	// generates shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// generates vertex array object and binds it
	VAO VAO1;
	VAO1.Bind();

	// generates vertex buffer object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// generates element buffer object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);
	//main while loop
	while (!glfwWindowShouldClose(window))
	{
		// specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
		//bind the VAO so opengl know to use it
		shaderProgram.Activate();

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}


		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);


		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(90.0f), (float)(width / height), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		// assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.4f);

		// bind the VAO so OpenGL knows to use it
		VAO1.Bind();

		// draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		//take care of all GLFW events
		glfwPollEvents();
	}

	// delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	//delete window before ending the program
	glfwDestroyWindow(window);
	//terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}