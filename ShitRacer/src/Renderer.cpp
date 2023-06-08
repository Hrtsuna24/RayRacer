#include "Renderer.h"
#include "Walnut/Random.h"

namespace HTM
{
	void Renderer::Render()
	{
		//	render every pixel
		for (
			uint32_t i = 0;
			i < m_FinalImage->GetWidth() * m_FinalImage->GetHeight();
			i++)
		{
			m_FinalImageData[i] = Walnut::Random::UInt() | 0xff000000;
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
}