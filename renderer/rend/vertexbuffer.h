/*
 * vertexbuffer.h
 *
 *      Author: flamingo
 *      E-mail: epiforce57@gmail.com
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "../math/vertex.h"
#include "boundingsphere.h"
#include "../math/m44.h"

namespace rend
{

struct Material;
class Mesh;

//! Container for vertices and indices with same material.
/*!
  * Submesh.
  */
class VertexBuffer
{
public:
    //! Some anti-boilerplate typedefs.
    typedef std::vector<math::vertex> VertexArray;
    typedef std::vector<uint16_t> IndexArray;

    enum VertexBufferType
    {
        UNDEFINED,
        INDEXEDTRIANGLELIST,    /*!< Index triplet in vertex buffer define the triangle. */
        TRIANGLELIST,           /*!< Vertex triplets in vertex buffer define the triangle. */
        LINELIST                /*!< Each two vertices define the line. */
    };

private:
    friend class Mesh; // because mesh is vertex buffer container.

    //! Type of vertices.
    VertexBufferType m_type;
    //! Material of this vertices;
    sptr(Material) m_material;
    //! Vertices of the submesh.
    VertexArray m_vertices;
    //! Indices for the vertices.
    IndexArray m_indices;

    //! UV coords of the submesh.
    std::vector<math::vec2> m_uvs;
    //! Indices for uv coords.
    IndexArray m_uvsIndices;

public:
    //! Default ctor.
    VertexBuffer(VertexBufferType type = UNDEFINED);
    //! Dtor.
    ~VertexBuffer();

    //! Sets the material to this submesh.
    void            setMaterial(sptr(Material) material);
    //! Gets material.
    sptr(Material)  getMaterial() const { return m_material; }

    //! Sets the type.
    void                setType(VertexBufferType type) { m_type = type; }
    //! Gets the type.
    VertexBufferType    getType() const { return m_type; }

    //! Appends vertices to this submesh. Also computes vertex normals and bounding sphere.
    void appendVertices(const std::vector<math::vertex> &vertices, const std::vector<int> &indices,
                        const std::vector<math::vec2> &uvs, const std::vector<int> &uvinds, bool isNormalsComputed = false);
    void appendVertices(const std::vector<math::vertex> &vertices, const std::vector<int> &indices, bool isNormalsComputed = false);
    void appendVertices(const std::vector<math::vertex> &vertices, bool isNormalsComputed = false);

    //! Helper to compute vertex normals.
    void computeVertexNormals();

    //! How many vertices in the buffer?
    int numVertices() const { return m_vertices.size(); }
    //! How many indices in the buffer?
    int numIndices() const { return m_indices.size(); }

    //! Gets i-th vertex from the array.
    math::vertex    vertex(unsigned i) const { return m_vertices[i]; }
    //! Gets i-th index from the array.
    int             index(unsigned i) const { return m_indices[i]; }

    //! Returns reference to all vertices of the submesh.
    const VertexArray&  getVertices() const { return m_vertices; }
    //! Returns reference to all indices of the submesh.
    const IndexArray&   getIndices() const { return m_indices; }

    //!
    const std::vector<math::vec2> &getUVs() const { return m_uvs; }
    //!
    const IndexArray &getUVIndices() const { return m_uvsIndices; }

    bool operator< (const VertexBuffer &vb);
};

}

#endif // VERTEXBUFFER_H
