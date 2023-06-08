#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Timer.h"

#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		
		ImGui::Text("Last Render : %.3fms", m_LastRenderTime);

		if(ImGui::Button("Render"))
		{
			Render();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

		ImGui::Begin("Viewport");

		m_ViewportWidth = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		m_ViewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

		const auto im = m_renderer.GetFinalImage();
		if (im)
		{
			ImGui::Image(im->GetDescriptorSet(), { float(im->GetWidth()), float(im->GetHeight()) });
		}
		
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render()
	{
		Timer timer;

		// Change Size of Image
		uint32_t tmp_WH[2]{ m_ViewportWidth , m_ViewportHeight };
		m_renderer.OnResize(tmp_WH);

		// Draw Image
		m_renderer.Render();


		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	HTM::Renderer m_renderer{};

	uint32_t m_ViewportWidth{},
		m_ViewportHeight{}
	;

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