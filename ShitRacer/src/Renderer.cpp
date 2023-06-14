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

void HTM::Renderer::Render(const Scene& scene, const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color{ TraceRay(scene, ray) };
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


glm::vec4 HTM::Renderer::TraceRay(const Scene& scene, const Ray& ray)
{
	//rayDirection = glm::normalize(rayDirection);
	if (scene.spheres.empty())
		return glm::vec4{ 0, 0, 0, 1 };

	const Sphere* closestSphere = nullptr;
	float hitDistance = std::numeric_limits<float>::max();

	for (const Sphere& sphere : scene.spheres)
	{
		
		const float& radius = sphere.Radius;

		glm::vec3 origin = ray.Origin - sphere.Position;

		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		float c = glm::dot(origin, origin) - radius * radius;

		float dicriminant = b * b - 4.0f * a * c;
		if (dicriminant < 0.0f)
		{
			continue;
		}

		float t0, closestt;
		glm::vec3 h0, hitpoint;
		if (dicriminant == 0.0f)
		{
			t0 = -b / (2.0f * a);
			closestt = t0;

			h0 = origin + ray.Direction * t0;
			hitpoint = h0;
		}
		else
		{
			//t0 = (-b + glm::sqrt(dicriminant)) / (2.0f * a);
			closestt = (-b - glm::sqrt(dicriminant)) / (2.0f * a);

			//h0 = origin + ray.Direction * t0;
			hitpoint = origin + ray.Direction * closestt;
		}
		if (closestt < hitDistance)
		{
			closestSphere = &sphere;
			hitDistance = closestt;
		}
		
	}

	if (!closestSphere) 
		return glm::vec4{ 0, 0, 0, 1 };

	glm::vec3 hitPoint;
	glm::vec3 origin = ray.Origin - closestSphere->Position;
	hitPoint = origin + ray.Direction * hitDistance;

	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDirecton = glm::normalize(glm::vec3(-1, -1, -1));

	float d = glm::max(glm::dot(normal, -lightDirecton), 0.0f);// cos(angel) between n and lD

	glm::vec3 sc = closestSphere->Albedo;
	sc *= d;
	return glm::vec4{ sc , 1.0f };
}