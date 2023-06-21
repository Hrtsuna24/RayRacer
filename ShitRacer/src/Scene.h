#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <vector>


struct Material 
{

	glm::vec3 Albedo{ 1.f };
	float Roughness{ 1.f };
	float Metallic{ 0.f };
};


struct Sphere
{
	glm::vec3 Position{ 0.f };
	float Radius{ 0.5 };


	size_t MaterialIndex = 0;
};

struct Scene
{
	std::vector<Sphere> spheres;
	std::vector<Material> Materials;
};