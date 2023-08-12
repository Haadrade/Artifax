#include "axpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Artifax/Application.h"
//TEMPORARY
#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include <Artifax/KeyCodes.h>

namespace Artifax
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Hazel key codes
		io.KeyMap[ImGuiKey_Tab] = AX_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = AX_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = AX_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = AX_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = AX_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = AX_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = AX_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = AX_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = AX_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = AX_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = AX_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = AX_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = AX_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = AX_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = AX_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = AX_KEY_A;
		io.KeyMap[ImGuiKey_C] = AX_KEY_C;
		io.KeyMap[ImGuiKey_V] = AX_KEY_V;
		io.KeyMap[ImGuiKey_X] = AX_KEY_X;
		io.KeyMap[ImGuiKey_Y] = AX_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = AX_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Events::Event& event)
	{
		Events::EventDispatcher dispacher(event);
		
		dispacher.Dispatch<Events::MouseButtonPressedEvent>(AX_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispacher.Dispatch<Events::MouseButtonReleasedEvent>(AX_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispacher.Dispatch<Events::MouseMovedEvent>(AX_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispacher.Dispatch<Events::MouseScrolledEvent>(AX_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispacher.Dispatch<Events::KeyPressedEvent>(AX_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispacher.Dispatch<Events::KeyTypedEvent>(AX_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispacher.Dispatch<Events::KeyReleasedEvent>(AX_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispacher.Dispatch<Events::WindowResizeEvent>(AX_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}
	bool ImGuiLayer::OnMouseButtonPressedEvent(Events::MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(Events::MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(Events::MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(Events::KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[AX_KEY_LEFT_CONTROL] || io.KeysDown[AX_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[AX_KEY_LEFT_SHIFT] || io.KeysDown[AX_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[AX_KEY_LEFT_ALT] || io.KeysDown[AX_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[AX_KEY_LEFT_SUPER] || io.KeysDown[AX_KEY_RIGHT_SUPER];

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(Events::KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(Events::KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned int)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(Events::WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)e.GetWidth(), (float)e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}