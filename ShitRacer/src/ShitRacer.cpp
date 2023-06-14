#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Timer.h"

#include "Renderer.h"
using HTM::Renderer;

#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include "Camera.h"

#include "Walnut/Image.h"
using namespace Walnut;


class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() : m_Camera(45.f,0.1f, 100.f){};

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		
		ImGui::Text("Last Render : %.3fms", m_LastRenderTime);

		if(ImGui::Button("Render"))
		{
			Render();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_Viewport_Width = ImGui::GetContentRegionAvail().x;
		m_Viewport_Heigth = ImGui::GetContentRegionAvail().y;

		auto im = m_Renderer.GetFinalImage();
		if (im)
		{
			ImGui::Image(
				im->GetDescriptorSet(),
				{ float(im->GetWidth()), float(im->GetHeight()) },
				ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::End();
		ImGui::PopStyleVar();
		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_Viewport_Width, m_Viewport_Heigth);
		m_Camera.OnResize(m_Viewport_Width, m_Viewport_Heigth);
		m_Renderer.Render(m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_Renderer{};
	Camera m_Camera;

	uint32_t
		m_Viewport_Width{},
		m_Viewport_Heigth{};

	float m_LastRenderTime{};
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "ShitRacer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}