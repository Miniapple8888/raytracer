// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "ray.h"
#include "hittable.hpp"
#include "hittable_list.hpp"

#include "SceneNode.hpp"
#include "GeometryNode.hpp"
#include "Primitive.hpp"
#include "Light.hpp"
#include "Image.hpp"

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		double time,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);
