#pragma once

#include "Walnut/Image.h" 
#include "Camera.h"
#include "Ray.h"

#include "memory"
#include <glm/fwd.hpp>

class Renderer {
public: 
	Renderer() = default;

	void OnResize(uint16_t width, uint16_t height);
	void Render(const Camera& camera, float _r, float _g, float _b, float _i);

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
private:
	glm::vec4 TraceRay(const Ray& ray, float r, float g, float b, float i);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};