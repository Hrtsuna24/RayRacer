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

void HTM::Renderer::Render(const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color{ TraceRay(ray) };
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


glm::vec4 HTM::Renderer::TraceRay(const Ray& ray)
{
	//rayDirection = glm::normalize(rayDirection);
	float radius = 0.5f;
	
	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * glm::dot(ray.Origin, ray.Direction);
	float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

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

		h0 = ray.Origin + ray.Direction * t0;
		hitPoint = h0;
	}
	else
	{
		t0 = (-b + glm::sqrt(dicriminant)) / (2.0f * a);
		closestT = (-b - glm::sqrt(dicriminant)) / (2.0f * a);

		h0 = ray.Origin + ray.Direction * t0;
		hitPoint = ray.Origin + ray.Direction * closestT;
	}

	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDirecton = glm::normalize(glm::vec3(-1, -1, -1));

	float d = glm::max(glm::dot(normal, -lightDirecton), 0.0f);// cos(angel) between n and lD

	glm::vec3 sc{1, 0, 1};
	sc *= d;
	return glm::vec4{ sc , 1.0f };
}