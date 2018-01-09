#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include "mesh.h"

#include <string>
#include <unordered_map>

struct AssetManager
{
    void loadMesh(const std::string& name,
                  const std::string& path,
                  const std::string& vertex_shader,
                  const std::string& fragment_shader);


private:
};

#endif // _ASSET_MANAGER_H_
