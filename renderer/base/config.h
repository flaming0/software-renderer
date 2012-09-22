/*
 * config.h
 *
 *  Created on: Mar 10, 2012
 *      Author: flamingo
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "vec3.h"
#include "color.h"

namespace YAML
{
class Node;
}

namespace base
{

class Controller;
class ResourceMgr;

struct RendererConfig
{
    math::vec3  camPosition;
    int         width;
    int         height;
    string      pathToTheAssets;

    void makeDefaults();
};

struct SceneConfig
{
    struct ObjInfo
    {
        math::vec3 position;
        string pathToTheModel;
    };

    // scene objects
    vector<ObjInfo> objects;

    // ambient light intensity
    rend::Color3 ambIntensity;

    struct DirLightInfo
    {
        math::vec3 direction;
        rend::Color3 intensity;
    };

    // directional lights
    vector<DirLightInfo> dirLights;

    SceneConfig() { ambIntensity.reset(); }
};

class Config
{
    RendererConfig m_rendererConfig;
    std::stringstream m_rendererConfigData;
    void parseRendererConfig();

    SceneConfig m_sceneConfig;
    std::stringstream m_sceneConfigData;
    void parseSceneConfig();
    void parseLights(const YAML::Node &doc);

public:
    Config(const string &cfgDir = "");

    const RendererConfig &getRendererConfig() const;
    const SceneConfig &getSceneConfig() const;

    Config(const Config &) = delete;
    Config &operator= (const Config &) = delete;
};

}

#endif // CONFIG_H
