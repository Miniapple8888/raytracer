// Termm--Fall 2020

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Rigidbody.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );
	void setTexture ( Texture *texture );
	Texture* getTexture ();
	void setRigidbody ( RigidBody *rb );
	RigidBody* getRigidbody ();

	Material *m_material;
	Primitive *m_primitive;
	Texture *m_texture = nullptr;
	RigidBody *m_rb = nullptr;
};
