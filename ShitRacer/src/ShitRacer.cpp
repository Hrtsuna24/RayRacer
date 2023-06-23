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
	ExampleLayer() : m_Camera(45.f,0.1f, 100.f)
	{
		Material& pinkSphere =
			m_Scene.Materials.emplace_back();
		pinkSphere.Albedo = glm::vec3{ 1.f,0.f,1.f };
		pinkSphere.Roughness = 0.f;


		Material& blueSphere = m_Scene.Materials.emplace_back();
		blueSphere.Albedo = glm::vec3{ 0.2f,0.3f,1.f };
		blueSphere.Roughness = 0.1f;
		
		Material& orangeSphere = m_Scene.Materials.emplace_back();
		orangeSphere.Albedo = glm::vec3{ 0.8f,0.5f,0.2f };
		orangeSphere.Roughness = 0.1f;
		orangeSphere.EmmisionColor = orangeSphere.Albedo;
		orangeSphere.EmmisionPower = 2.f;

		m_Scene.spheres.push_back(Sphere{ glm::vec3{0.f,2.f,0.f}, 0.75f,  size_t(0) });

		Sphere tmp;
		tmp.MaterialIndex = size_t(1);
		tmp.Radius = 2.f;
		m_Scene.spheres.push_back(tmp);

		tmp.MaterialIndex = size_t(2);
		tmp.Position = glm::vec3{ 0.8f,1.f,0.2f };
		m_Scene.spheres.push_back(tmp);

	};

	virtual void OnUpdate(float ts) override
	{
		if (m_Camera.OnUpdate(ts))
		{
			m_Renderer.ResetFrameIndex();
		}
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		
		ImGui::Text("Last Render : %.3fms", m_LastRenderTime);

		ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().bAccumulate);

		if(ImGui::Button("Render"))
		{
			Render();
		}

		if (ImGui::Button("Reset"))
		{
			m_Renderer.ResetFrameIndex();
		}

		ImGui::End();
		
		////
		ImGui::Begin("Scene");

		for (size_t i = 0; i < m_Scene.spheres.size(); ++i)
		{
			ImGui::PushID(i);

			Sphere& sphere = m_Scene.spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			ImGui::DragInt("MaterialIndex", &(int)sphere.MaterialIndex, 1.0f, 0, (int)m_Scene.Materials.size() - 1);

			ImGui::Separator();
			ImGui::PopID();
		}

		for (size_t i = 0; i < m_Scene.Materials.size(); ++i)
		{
			ImGui::PushID(i);

			Material& material = m_Scene.Materials[i];
			
			ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
			ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.f, 1.f);
			ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.f, 1.f);

			ImGui::ColorEdit3("Emmision Color", glm::value_ptr(material.EmmisionColor));
			ImGui::DragFloat("Emmision Power", &material.EmmisionPower, 0.05f, 0.f, FLT_MAX);

			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::End();
		//////
		
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
		m_Renderer.Render(m_Scene, m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_Renderer{};
	Camera m_Camera;
	Scene m_Scene;
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