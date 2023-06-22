#include "Renderer.h"
#include "Walnut/Random.h"

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
	m_Camera = &camera;
	m_Scene = &scene;
		
		
	if (m_FrameIndex == 1)
	{
		//m_AccumulationData[x + y * m_FinalImage->GetWidth()] = gkm::vec(0.0f);
		memset(m_AccumulationData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec4));
	}

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec4 color{ PerPixel(x, y) };
			m_AccumulationData[x + y * m_FinalImage->GetWidth()] += color;

			glm::vec4 AcumulatedColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
			AcumulatedColor /= float(m_FrameIndex);

			AcumulatedColor = glm::clamp(AcumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(AcumulatedColor);
		}
	}

	m_FinalImage->SetData(m_ImageData);

	if (m_Settings.bAccumulate)
	{
		m_FrameIndex++;
	}
	else
	{
		m_FrameIndex = 1;
	}
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

	delete[]m_AccumulationData;
	m_AccumulationData = new glm::vec4[w * h];
}

std::shared_ptr<Image> HTM::Renderer::GetFinalImage() const
{
	return m_FinalImage;
}

HTM::Renderer::HitPayload HTM::Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;


	const Sphere& closestSphere = m_Scene->spheres[objectIndex];
	
	glm::vec3 origin = ray.Origin - closestSphere.Position;
	payload.WorldPosition = origin + ray.Direction * hitDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);

	payload.WorldPosition += closestSphere.Position;
	return payload;
}


HTM::Renderer::HitPayload HTM::Renderer::Miss(const Ray& ray)
{
	HitPayload payload;
	payload.HitDistance = -1.f;
	return payload;
}


HTM::Renderer::HitPayload HTM::Renderer::TraceRay(const Ray& ray)
{

	 
	//rayDirection = glm::normalize(rayDirection);
	

	int closestSphere = -1;
	float hitDistance = std::numeric_limits<float>::max();

	for (size_t i = 0; i < m_Scene->spheres.size(); i++)
	{
		const Sphere& sphere = m_Scene->spheres[i];
		
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
		if (closestt < hitDistance &&
			closestt > 0.f)
		{
			closestSphere = (int)i;
			hitDistance = closestt;
		}
		
	}

	if (closestSphere < 0) 
		return Miss(ray);

	return ClosestHit(ray, hitDistance, closestSphere);
}

glm::vec4 HTM::Renderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.Origin = m_Camera->GetPosition();
	ray.Direction = m_Camera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];


	glm::vec3 color(0.f);
	float multiplier = 1.0f;

	size_t bounces = 5;
	for (size_t i = 0; i < bounces; i++)
	{
		HitPayload payload = TraceRay(ray);
		if (payload.HitDistance < 0.f)
		{
			glm::vec3 skyColor = glm::vec3(0.6f, 0.7f, 0.9f);
			color += skyColor * multiplier;
			break;
		}

		glm::vec3 lightDirecton = glm::normalize(glm::vec3(1, -1, -1));
		float d = glm::max(glm::dot(payload.WorldNormal, -lightDirecton), 0.0f);// cos(angel) between n and lD

		const Sphere& sphere = m_Scene->spheres[payload.ObjectIndex];
		const Material& material = m_Scene->Materials[sphere.MaterialIndex];

		glm::vec3 sc = material.Albedo;
		sc *= d;
		color += sc * multiplier;

		multiplier *= 0.5f;

		ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
		ray.Direction = glm::reflect(ray.Direction,
			payload.WorldNormal + material.Roughness * Walnut::Random::Vec3(-0.5f, 0.5f) );
	}
	return glm::vec4(color, 1.f);
}

