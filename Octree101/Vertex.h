#ifndef VERTEX_H
#define VERTEX_H

#include "Vec3.h"
#include "Vec2.h"
#include "Colour.h"

struct Vertex
{
	Vertex::Vertex()
	{
	}

	Vertex::Vertex(const Vec3f& point) : m_point(point)
	{
	}

	void Draw()
	{
		//m_colour.Draw();
		//glTexCoord2f(m_uv.m_x, m_uv.m_y);
		//glNormal3f(m_normal.x, m_normal.y, m_normal.z);
		glVertex3f(m_point.x, m_point.y, m_point.z);
	}

	Vec3f m_point;

    // TODO Add these other attributes if you like.
	//Vec2f m_uv;
	//Colour m_colour;
	//Vec3f m_normal;

	operator Vec3f()
	{
		return m_point;
	}
};

#endif
