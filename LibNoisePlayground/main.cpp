#include <iostream>
#include "glfw/include/glfw3.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"

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

	ImGui_ImplGlfw_Init(window, true);

	ImVec4 clear_color = ImColor(114, 144, 154);

	// Main Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplGlfw_NewFrame();



		int dW, dH;
		glfwGetFramebufferSize(window, &dW, &dH);
		glViewport(0, 0, dW, dH);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}