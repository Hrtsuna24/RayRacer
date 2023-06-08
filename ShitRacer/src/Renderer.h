#pragma once

#include "Walnut\Image.h"
#include <memory>

namespace HTM
{
	class Renderer
	{
	public:
		Renderer() = default;
		//~Renderer();

		void OnResize(uint32_t[2]);
		void Render();

		const std::shared_ptr<Walnut::Image>& GetFinalImage() const;
	private:
		std::shared_ptr<Walnut::Image> m_FinalImage{};
		uint32_t* m_FinalImageData{ nullptr }
		;
	};
}