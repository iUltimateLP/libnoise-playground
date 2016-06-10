#include <iostream>
#include "glfw/include/glfw3.h"

// Error Callback for OpenGL
static void errorCallback(int error, const char* desc)
{
	fprintf(stderr, "[OpenGL Error] %d: %s\n", error, desc);
}

// Main Entry
int main()
{
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
		return 1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "LibNoise Playground", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Main Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		int dW, dH;
		glfwGetFramebufferSize(window, &dW, &dH);
		glViewport(0, 0, dW, dH);
		glClearColor(0, 255, 0, 255);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}