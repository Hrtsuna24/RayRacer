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


	void Renderer::OnResize(uint32_t wh[2])
	{

		if (m_FinalImage) // if exist
		{
			if (m_FinalImage->GetWidth() == wh[0] && m_FinalImage->GetHeight() == wh[1]) return; // no resize neccesary

			m_FinalImage->Resize(wh[0], wh[1]); //...change the size
		}
		else
		{
			m_FinalImage = std::make_shared<Walnut::Image>(wh[0], wh[1], Walnut::ImageFormat::RGBA); // ...else create with new param
		}

		delete[]m_FinalImageData; // clear buffer
		m_FinalImageData = new uint32_t[ wh[1] * wh[0] ]; //R G B A -> 4
	}


	uint32_t Renderer::PerPixel(glm::vec2 coord)
	{
		uint8_t
			r{ static_cast<uint8_t>(coord.x * 255.f) },
			b{},
			g{ static_cast<uint8_t>(coord.y * 255.f) },
			a{}
		;
		return 0xff000000 | (g <<8) | r;
	}
}