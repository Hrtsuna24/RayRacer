#include "Renderer.h"


void HTM::Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord{
				(float)x / (float)m_FinalImage->GetWidth(),
				(float)y / (float)m_FinalImage->GetHeight()
			};
			coord = coord * 2.0f - 1.0f;

			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

void HTM::Renderer::OnResize(uint32_t w, uint32_t h)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetHeight() == h && m_FinalImage->GetWidth() == w)
			return;
		
		m_FinalImage->Resize(w, h);
	}
	else
	{
		m_FinalImage = std::make_shared<Image>(
			w,
			h,
			ImageFormat::RGBA);
	}

	delete[]m_ImageData;
	m_ImageData = new uint32_t[w * h];
}

std::shared_ptr<Image> HTM::Renderer::GetFinalImage() const
{
	return m_FinalImage;
}


uint32_t HTM::Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 rayOrigin(0.0f, 0.0f, -2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	//rayDirection = glm::normalize(rayDirection);
	float radius = 0.5f;
	
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float dicriminant = b * b - 4.0f * a * c;
	if (dicriminant >= 0.0f)
	{
		return 0xffff00ff;
	}
	else
	{
		return 0xff000000;
	}
}