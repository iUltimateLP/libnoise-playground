#include <iostream>
#include "libs/glfw/include/glfw3.h"
#include "libs/ImGui/imgui.h"
#include "libs/ImGui/imgui_impl_glfw.h"
#include "libs/libnoise/noise.h"
#include "libs/libnoise/noiseutils.h"

using namespace noise;

// Error Callback for OpenGL
static void errorCallback(int error, const char* desc)
{
	fprintf(stderr, "[OpenGL Error] %d: %s\n", error, desc);
}

// Main Entry
int main()
{
	static bool bMenu_ClearNodes = false;
	static bool bMenu_SaveNoise = false;
	static bool bMenu_ShowInfo = false;
	static bool bMenu_ShouldClose = false;

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

		ImGui::GetStyle().WindowTitleAlign = ImGuiAlign_Center;

		// Closing
		if (bMenu_ShouldClose)
			glfwSetWindowShouldClose(window, true);

		// Info window
		if (bMenu_ShowInfo)
		{
			ImGui::Begin("LibNoise Playground", &bMenu_ShowInfo, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::Text("Compiled at : %s - %s", __DATE__, __TIME__);
			ImGui::Text("Created by  : iUltimateLP");
			ImGui::Text("Uses        : ImGui (ocornut)\n              GLFW (Marcus Geelnard & Camilla Berglund)\n              LibNoise (Jason Bevins)");
			ImGui::End();
		}

		// Menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::MenuItem("Clear Nodes", NULL, &bMenu_ClearNodes);
				ImGui::MenuItem("Save noise to picture", NULL, &bMenu_SaveNoise);
				ImGui::Separator();
				ImGui::MenuItem("Info", NULL, &bMenu_ShowInfo);
				ImGui::MenuItem("Close", NULL, &bMenu_ShouldClose);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}


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