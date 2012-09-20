/*
 * mesh.cpp
 *
 *  Created on: Mar 10, 2012
 *      Author: flamingo
 */

#include "mesh.h"

#include "m33.h"
#include "renderlist.h"
#include "vertexbuffer.h"

namespace rend
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::appendSubmesh(const VertexBuffer &submesh)
{
    m_submeshes.push_back(submesh);
}

void Mesh::computeBoundingSphere(const math::M44 &transform)
{
    int sz = 0;

    for (auto &vb : m_submeshes)
        sz += vb.numVertices();

    vector<math::vec3> points(sz);

    int j = 0;
    for (auto &vb : m_submeshes)
    {
        for (size_t i = 0; i < vb.m_vertices.size(); i++, j++)
            points[j] = vb.m_vertices[i].p;
    }

    // apply transformation
    const math::M33 &rotsc = transform.getM();
    std::for_each(points.begin(), points.end(), [&](math::vec3 &v) { v = v * rotsc; });

    m_boundingSphere.calculate(points);
}

const BoundingSphere &Mesh::getBoundingSphere() const
{
    return m_boundingSphere;
}

int Mesh::numVertices() const
{
    int n = 0;
    std::for_each(m_submeshes.begin(), m_submeshes.end(),
                  [&](const VertexBuffer &buffer) { n += buffer.numVertices(); });

    return n;
}

int Mesh::numSubMeshes() const
{
    return m_submeshes.size();
}

sptr(Mesh) Mesh::clone() const
{
    sptr(Mesh) objMesh = make_shared<Mesh>();

    for (auto &submesh : m_submeshes)
    {
        VertexBuffer vb;
        vb.m_type = submesh.m_type;
        vb.m_material = submesh.m_material;
        vb.m_vertices = submesh.m_vertices;
        vb.m_indices = submesh.m_indices;

        objMesh->appendSubmesh(vb);
    }

    return objMesh;
}

}
