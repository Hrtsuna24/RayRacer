#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		
		if(ImGui::Button("Render"))
		{
			Render();
		}

		ImGui::End();

		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		ImGui::End();
	}

	void Render()
	{
		uint32_t tmp_NewImDataSize = m_ViewportWidth * m_ViewportHeight;

		if (!m_Image ||
			m_ViewportWidth != m_Image->GetWidth() ||
			m_ViewportHeight != m_Image->GetHeight())
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);
						
			delete[]m_ImageData;
			m_ImageData = new uint32_t[tmp_NewImDataSize]; //R G B A -> 4
		}

		for (size_t index{ 0 }; index < tmp_NewImDataSize; ++index)
		{

		}
	}

private:
	std::shared_ptr<Image> m_Image{};

	uint32_t m_ViewportWidth{},
		m_ViewportHeight{},
	
		*m_ImageData{nullptr}
	;
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