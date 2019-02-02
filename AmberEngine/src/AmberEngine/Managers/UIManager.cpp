#include "AmberEngine/Managers/UIManager.h"

#include "AmberEngine/ImGUI/imgui.h"
#include "AmberEngine/ImGUI/imgui_impl_glfw_gl3.h"

AmberEngine::Managers::UIManager::UIManager(Context::Device& p_device) : m_device(p_device)
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_device.GetContextWindow(), true);
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 12.0f;
}

AmberEngine::Managers::UIManager::~UIManager()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void AmberEngine::Managers::UIManager::BeginFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

void AmberEngine::Managers::UIManager::EndFrame()
{
	ImGui::Render();
}

void AmberEngine::Managers::UIManager::BeginWindow(const std::string& p_title)
{
	ImGui::Begin(p_title.c_str());
}

void AmberEngine::Managers::UIManager::EndWindow()
{
	ImGui::End();
}

void AmberEngine::Managers::UIManager::Render()
{
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void AmberEngine::Managers::UIManager::DisplayDeviceInfos()
{
	ImGui::Begin("Device");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}