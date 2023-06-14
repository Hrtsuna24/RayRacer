#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Sphere
{
	glm::vec3 Position{ 0.f };
	float Radius{ 0.5 };


	glm::vec3 Albedo{ 1.f };
};

struct Scene
{
	std::vector<Sphere> spheres;
};