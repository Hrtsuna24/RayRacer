#include "Renderer.h"

namespace Utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r{ uint8_t(color.r * 255.f) };
		uint8_t g{ uint8_t(color.g * 255.f) };
		uint8_t b{ uint8_t(color.b * 255.f) };
		uint8_t a{ uint8_t(color.a * 255.f) };
	
		return  (uint32_t)(a << 24) | (b << 16) | (g << 8) | (r);
	}
}

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
			glm::vec4 color{ PerPixel(coord) };
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
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


glm::vec4 HTM::Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 rayOrigin(0.0f, 0.0f, -2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	//rayDirection = glm::normalize(rayDirection);
	float radius = 0.5f;
	
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float dicriminant = b * b - 4.0f * a * c;
	if (dicriminant < 0.0f)
	{
		return glm::vec4{ 0, 0, 0, 1 };
	}
	
	float t0{}, closestT{};
	glm::vec3 h0{}, hitPoint{};
	if (dicriminant == 0.0f)
	{
		t0 = -b / (2.0f * a);
		closestT = t0;

		h0 = rayOrigin + rayDirection * t0;
		hitPoint = h0;
	}
	else
	{
		t0 = (-b + glm::sqrt(dicriminant)) / (2.0f * a);
		closestT = (-b - glm::sqrt(dicriminant)) / (2.0f * a);

		h0 = rayOrigin + rayDirection * t0;
		hitPoint = rayOrigin + rayDirection * closestT;
	}

	return glm::vec4{ hitPoint , 1.0f };
}