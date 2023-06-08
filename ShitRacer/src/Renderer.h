#pragma once

#include "Walnut\Image.h"
#include <glm/gtc/type_ptr.hpp>

#include <memory>

namespace HTM
{
	class Renderer
	{
	public:
		Renderer() = default;
		//~Renderer();

		void OnResize(uint32_t, uint32_t);
		void Render();

		const std::shared_ptr<Walnut::Image>& GetFinalImage() const;
	private:
		uint32_t PerPixel(glm::vec2 coord);

	private:
		std::shared_ptr<Walnut::Image> m_FinalImage{};
		uint32_t* m_FinalImageData{ nullptr }
		;
	};
}