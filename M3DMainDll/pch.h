// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

// 添加要在此处预编译的标头
#include "framework.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "tinyxml2.h"

#include <jemalloc/jemalloc.h>
#include <jemalloc/jemalloc_mangle.h>


#include <vector>
#include <map>
#include <unordered_map>
#include <atomic>
#include <string>
#include <iostream>
#include <fstream>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <FTGL/ftgl.h>

#include "Global.h"
#include "SharedFunc.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <PxConfig.h>
#include <PxPhysicsAPI.h>

#include <lua.hpp>
#include <sol/sol.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H  

#endif //PCH_H
