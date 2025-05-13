// Termm--Fall 2020
#include <iostream>
#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	glm::vec3& kd, 
	glm::vec3& ks, 
	glm::vec3& ka, 
	double shininess)
	: m_kd(kd)
	, m_ks(ks)
	, m_ka(ka)
	, m_shininess(shininess)
{}

glm::vec3 halfway_vector(const glm::vec3& v, const glm::vec3& l) {
	glm::vec3 vl = v + l;
	return glm::normalize(vl); // (v+l) / ||v+l||
}

bool PhongMaterial::scatter(const ray& r_in, const hit_record& rec, ray& r_out, glm::vec3& attenuation, std::list<Light*>& lights, const glm::vec3& ambient) {
	glm::vec3 n = rec.normal;
	glm::vec3 v = glm::normalize(r_in.origin() - rec.p);
	glm::vec3 I_ambient = ambient;
	glm::vec3 L_ambient = m_ka * I_ambient;
	float p = static_cast<float>(m_shininess);

	glm::vec3 total_L_out = glm::vec3(0.0f,0.0f,0.0f);

	for (Light* light : lights) {
		double c1 = light->falloff[0];
		double c2 = light->falloff[1];
		double c3 = light->falloff[2];
		glm::vec3 I_light = light->colour;
		glm::vec3 l = glm::normalize(light->position - rec.p);
		double r = glm::length(l);
		float attenuation = (c1 + c2 * r + c3 * r*r);
		glm::vec3 L_in = I_light / attenuation;
		float ln = glm::dot(l,n);
		ln = ln < 0 ? 0 : ln; // remove negative light contributions

		glm::vec3 L_diffuse = m_kd * L_in * glm::dot(l, n);
		
		// Texture stuff to be put in a conditional
		if (texture != nullptr) {
			glm::vec3 text_colour = texture->getColor(rec.u,rec.v);
			L_diffuse *= text_colour;
			// std::cout<<"texture map"<<text_colour.x<<","<<text_colour.y<<","<<text_colour.z<<std::endl;
		} else {
			L_diffuse *= glm::vec3(1.0f,0.0f,0.0f); // will be red by default
		}
		
		
		glm::vec3 h = halfway_vector(v, l);
		glm::vec3 L_specular = m_ks * glm::pow(glm::dot(h, n), p) * I_light;
		
		
		glm::vec3 L_out = L_diffuse + L_ambient + L_specular;
		total_L_out += L_out;
		// total_L_out = L_diffuse;
	}
	
	r_out = ray(rec.p, total_L_out);
	return true;
}

PhongMaterial::~PhongMaterial(){
	std::cout<<"deallocate"<<std::endl;
}
