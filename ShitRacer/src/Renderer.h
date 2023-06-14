#pragma once
#include "Walnut/Image.h"
using Walnut::Image;
using Walnut::ImageFormat;

#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Ray.h"

namespace HTM
{
	class Renderer
	{
		std::shared_ptr<Image> m_FinalImage{};
		
		uint32_t* m_ImageData{ nullptr };

	public:
		Renderer() = default;

		void OnResize(uint32_t, uint32_t);
		void Render(const Camera&);

		std::shared_ptr<Image> GetFinalImage() const;

	private:
		glm::vec4 TraceRay(const Ray&);
	};
}
