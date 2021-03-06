/*
 * camera.h
 *
 *  Created on: Mar 10, 2012
 *      Author: flamingo
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "math/vec3.h"
#include "math/m44.h"

namespace rend
{

class Viewport;
class RenderList;
class SceneObject;

//! Scene camera.
/*!
  *
  */
class Camera
{
    friend class Viewport; // when resizing the viewport, we need to update some cam params

    //! Position of the camera in the world.
    math::vec3 m_position;
    //! Right vector.
    math::vec3 m_right;
    //! Up vector.
    math::vec3 m_up;
    //! Direction vector.
    math::vec3 m_dir;

    float m_yaw, m_pitch, m_roll;

    //! Field of view. Default is 90 grad.
    float m_fov;

    //! Near Z-clipping plane position.
    float m_nearZ;
    //! Far Z-clipping plane position.
    float m_farZ;

    //! Normalized projected plane size.
    float m_viewPlaneWidth;
    float m_viewPlaneHeight;
    //! Distance from the (0, 0, 0) to the projected plane along Z axis.
    float m_distance;

    math::M44 m_worldToCamera;
    math::M44 m_projection;
    math::M44 m_screen;

    // helpers
    void toScreen(math::vec3 &v, const Viewport &viewport) const;

    void buildCamMatrix();

public:
    //! Default ctor.
    /*! Initialize camera with position,
      * field of view (default 90 gr.), near and far clipping planes */
    Camera(const math::vec3 position,
           float fov = 90.0,
           float nearZ = 5.0,
           float farZ = 15000.0);
    ~Camera() { }

    void        setPosition(const math::vec3 &pos);
    math::vec3  getPosition() const;

    void        setDirection(const math::vec3 &dir);
    math::vec3  getDirection() const;

    math::vec3  getRightVector() const;
    math::vec3  getUpVector() const;

    void setEulerAnglesRotation(float yaw, float pitch, float roll);

    void toCamera(RenderList *rendList) const;
    void toScreen(RenderList *rendList, const Viewport &viewport) const;

    void frustumCull(RenderList *rendList) const;
    bool culled(const sptr(SceneObject) obj) const;
};

}

#endif // CAMERA_H
