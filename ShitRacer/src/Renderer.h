#pragma once
#include "Walnut/Image.h"
using Walnut::Image;
using Walnut::ImageFormat;

#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

namespace HTM
{
	class Renderer
	{

		struct Settings
		{
			bool bAccumulate = true;
		};

		Settings m_Settings;

		std::shared_ptr<Image> m_FinalImage{};
		
		uint32_t* m_ImageData{ nullptr };

		glm::vec4* m_AccumulationData{ nullptr };

		uint32_t m_FrameIndex = 1;

		std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

	public:
		Renderer() = default;

		void OnResize(uint32_t, uint32_t);
		void Render(const Scene&, const Camera&);

		std::shared_ptr<Image> GetFinalImage() const;


		void ResetFrameIndex() { m_FrameIndex = 1; }
		Settings& GetSettings() { return m_Settings; }
	private:
		struct HitPayload
		{
			float HitDistance{};
			glm::vec3 WorldNormal, WorldPosition;
			int ObjectIndex{};
		};

		const Scene* m_Scene{nullptr};

		const Camera* m_Camera{nullptr};

	private:
		struct HitPayload TraceRay(const Ray&);
		struct HitPayload ClosestHit(const Ray&, float, int);
		struct HitPayload Miss(const Ray&);


		glm::vec4 PerPixel(uint32_t, uint32_t); //RayGen shader

	};
}
