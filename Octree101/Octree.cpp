#include "Octree.h"

// Set to false to draw all octants; true to draw leaves only
static bool s_drawLeavesOnly = false;

OctreeComposite::OctreeComposite(const BBox& box)
{
    m_box = box;
	colour[0] = 1;
	colour[1] = 1;
	colour[2] = 1;
}

void OctreeComposite::Draw()
{
    if (!s_drawLeavesOnly)
    {
        float c = 0.7f;
        glColor3f(colour[0]*c, colour[1]*c, colour[2]*c);
        glLineWidth(1);
        m_box.Draw();
    }
    
    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->Draw();
    }
}

void OctreeComposite::AddChild(Octree* pChild)
{
    children.push_back(pChild);
}

OctreeLeaf::OctreeLeaf(const Tris& tris, const BBox& box) 
    : m_tris(tris)
{
    m_box = box;
	colour[0] = 1;
	colour[1] = 1;
	colour[2] = 1;
}

void OctreeLeaf::Draw() 
{
    
    glColor3f(colour[0], colour[1], colour[2]);
    glLineWidth(3);
    m_box.Draw();
    

    glBegin(GL_TRIANGLES);

    for (unsigned int i = 0; i < m_tris.size(); i++)
    {
        m_tris[i].Draw();
    }

    glEnd();
}

void OctreeComposite::getIntersectingLeaves(const Capsule &capsule, Leaves *pLeaves)
{
	for(std::vector<Octree*>::iterator it = children.begin(); it != children.end(); it++)
	{
		if (Intersects(capsule,(*it)->getBox()))
		{
			(*it)->getIntersectingLeaves(capsule, pLeaves);
		}
		else
		{
			(*it)->highlight(false);
		}
	}
}

void OctreeLeaf::getIntersectingLeaves(const Capsule &capsule, Leaves *pLeaves)
{
	this->highlight(true);

}

