/*
 * controller.h
 *
 *      Author: flamingo
 *      E-mail: epiforce57@gmail.com
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

namespace rend
{
class RenderMgr;
class Viewport;
class Camera;
}

namespace platform
{
class BaseApp;
}

namespace base
{

class Config;
class ResourceMgr;

DECLARE_EXCEPTION(ControllerException)

class Controller
{
    friend class platform::BaseApp;

    sptr(rend::RenderMgr)       m_rendmgr;
    sptr(rend::Viewport)        m_viewport;
    sptr(rend::Camera)          m_mainCam;
    sptr(ResourceMgr)           m_resourceMgr;

    void update();

    Config *m_controllerConfig;
    std::pair<int, int> getViewportSize();

    void createRenderManager();
    bool viewportExist();

public:
    Controller(int argc, const char *argv[]);
    virtual ~Controller();

    template<typename T>
    void createViewport();

    sptr(rend::Camera)      getCamera();
    sptr(ResourceMgr)       getResmgr();
    sptr(rend::RenderMgr)   getRendmgr();
    sptr(rend::Viewport)    getViewport();

    void resize(int w, int h);

    NONCOPYABLE(Controller)
};

// because there is no `export' keyword
template<typename T>
void Controller::createViewport()
{
    if (viewportExist())
        return;

    auto size = getViewportSize();

    // creating the viewport
    m_viewport = std::make_shared<T>(size.first, size.second, m_mainCam);
    // initializing the rendering manager
    createRenderManager();
}

}

#endif // CONTROLLER_H
