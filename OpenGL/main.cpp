#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"

#include <iostream>

// Declaring the window resize callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Window settings
const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "LearnOpenGL";

float mixValue = 0.2f;

int main()
{
	#pragma region GLFW Window

	// Initialize GLFW and tell it which OpenGL version we're using (3.3 Core)
	// Core means we'll get access to a smaller subset of OpenGL features (without backwards-compatible features we no longer need)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);

	// Registering the window resize callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLAD with the OS-specific function to get function addresses and load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	#pragma endregion

	#pragma region Shaders

	Shader shader("vertex.s", "fragment.s");

	#pragma endregion

	#pragma region Data

	//float vertices[] = {
	//	//x      y     z     r     g     b
	//	0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom left
	//	0.0f, 0.5f, 0.0f,	1.0f, 1.0f, 1.0f // top
	//};

	// Rectangle
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3
	};

	#pragma endregion

	#pragma region Textures

	#pragma region Texture 1

	// Create and bind texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Flip vertically to match coordinate system
	stbi_set_flip_vertically_on_load(true);

	// Load texture image
	int width, height, nrChannels;
	unsigned char *data = stbi_load("res\\container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		// Generate texture and mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free image memory
	stbi_image_free(data);

	#pragma endregion

	#pragma region Texture 2

	// Create and bind texture
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Set wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture image
	width, height, nrChannels = 0;
	data = stbi_load("res\\awesomeface.png", &width, &height, &nrChannels, 0);

	if (data) {
		// Generate texture and mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free image memory
	stbi_image_free(data);

	#pragma endregion

	#pragma endregion

	#pragma region VAO

	unsigned int VAO, VBO, EBO;

	// Generating vertex array object to hold our VBO configuration and attributes
	glGenVertexArrays(1, &VAO);

	// Generating vertex buffer
	glGenBuffers(1, &VBO);

	// Generating index buffer
	glGenBuffers(1, &EBO);

	// Binding the VAO to record the VBO data and vertex attributes configuration
	glBindVertexArray(VAO);

	// VBO

	// Binding the VBO and copying the vertices to the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_STATIC_DRAW means this data will change very rarely, if at all.

	// Tell OpenGL how to read the VBO:
	// We passed in vertices structured as an array of floats, where every first triplet of floats is a vertex, and every second triplet of floats is a color.
	// This also registers the VBO as the vertex attribute's bound vertex buffer object
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute	 (location, size, type, ?, stride (space between same attribute of next vertex), pointer (offset from start of vertex to attribute))
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// EBO

	// Binding the EBO and copying the indices to the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbind current buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	#pragma endregion

	#pragma region Render Loop

	// Start using the shader program with our defined shaders
	shader.use();
	shader.setInt("texture2", 1);

	// Bind textures
	glActiveTexture(GL_TEXTURE0); // container
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1); // smiley
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Start render loop
	while (!glfwWindowShouldClose(window))
	{
		// Check for specific input
		processInput(window);

		// Set background color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//// Compute position offset
		//float timeValue = glfwGetTime();
		//float vertical = sin(timeValue) / 2.0f;
		//float horizontal = cos(timeValue) / 2.0f;
		//
		//shader.setVec3("offset", horizontal, vertical, 0);

		//// Set color gradient
		//shader.setVec4("gradient", 0, sin(timeValue) / 2.0f + 0.5f, 0, 0);

		shader.setFloat("mixValue", mixValue);

		// Bind VAO to get the vertex attribute configuration and vertex data
		glBindVertexArray(VAO);
		
		// Draw indexed vertices
		// Bind the EBO and draw the indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap the buffers
		// Swapping refers to the Double Buffer - read more at https://learnopengl.com/#!Getting-started/Hello-Window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	#pragma endregion

	#pragma region Cleanup

	glfwTerminate();
	return 0;

	#pragma endregion
}

// Callback function to reset the viewport with the correct coordinates whenever a window resize occurrs
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Initialize the viewport in which OpenGL will draw 
	// Basically mapping OpenGLs' [-1, 1] coordinates to our windows' [0, WIDTH] and [0, HEIGHT] coordinates
	glViewport(0, 0, width, height);
}

// Process keyboard input
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	// Exit the application when the Escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}