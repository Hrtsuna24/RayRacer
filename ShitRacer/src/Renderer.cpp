#include "Renderer.h"
#include "Walnut/Random.h"

namespace HTM
{
	void Renderer::Render()
	{
		//	render every pixel
		
		for (
			uint32_t y = 0;
			y < m_FinalImage->GetHeight();
			y++)
		{
			for (
				uint32_t x = 0;
				x < m_FinalImage->GetWidth();
				x++)
			{
				glm::vec2 coord = {
					x / (float)m_FinalImage->GetWidth(),
					y / (float)m_FinalImage->GetHeight()
				};
				coord *= coord * 2.f - 1.f; // map from -1 to 1
				

				m_FinalImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord); // Set pixel
			}
		}

		m_FinalImage->SetData(m_FinalImageData);
	}

	const std::shared_ptr<Walnut::Image>& Renderer::GetFinalImage() const
	{
		// TODO: insert return statement here
		return m_FinalImage;
	}


	void Renderer::OnResize(uint32_t w, uint32_t h)
	{

		if (m_FinalImage) // if exist
		{
			if (m_FinalImage->GetWidth() == w && m_FinalImage->GetHeight() == h) return; // no resize neccesary

			m_FinalImage->Resize(w, h); //...change the size
		}
		else
		{
			m_FinalImage = std::make_shared<Walnut::Image>(w, h, Walnut::ImageFormat::RGBA); // ...else create with new param
		}

		delete[]m_FinalImageData; // clear buffer
		m_FinalImageData = new uint32_t[ w * h ]; //R G B A -> 4
	}


	uint32_t Renderer::PerPixel(glm::vec2 coord)
	{
		glm::vec3 rayDirection{ coord.x , coord.y , -1.0f };
		glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
		rayDirection = glm::normalize(rayDirection);
		// 
		//sphere
		// (b *(x*x + y*y)) * t * t + 2*t*(a*b* (x*x + y*y) ) + (a * (x*x + y*y) - r * r) = 0
		
		// a = ray origin
		// b = ray direction
		// r radius of sphere
		// t = hit distance

		float a = glm::dot(rayDirection, rayDirection);
			/*{rayDirection.x * rayDirection.x +
			rayDirection.y * rayDirection.y +
			rayDirection.z * rayDirection.z }
			*/
		float b = 2.0f * glm::dot(rayOrigin, rayDirection);
		float radius = 0.5f ;
		float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

		//discrim: b^2 - 4ac
		float discriminant{ b * b - 4.0f * a * c };
		uint32_t res{};

		if (discriminant >= 0.0f)
		{
			return 0xffff00ff;
		}
		else
		{
			res = 0xff000000;
		}

		return 0xff000000;
	}
}