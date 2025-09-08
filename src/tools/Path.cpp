#include "Path.h"
static std::string rootPath;
std::string GetResDir() {
    if(!rootPath.empty()){
        return rootPath;
    }
    // 当前目录
    fs::path currentPath = fs::current_path();

    // 在当前目录下直接查找 res 目录
    fs::path resPath = currentPath / "res";
    if (fs::exists(resPath) && fs::is_directory(resPath)) {
        rootPath = (currentPath / "").make_preferred().string();
        return rootPath;
    }

    // 如果当前目录未找到，到上一层目录中查找 res 目录
    fs::path parentPath = currentPath.parent_path();
    if (parentPath != currentPath) { // 确保有上一层目录
        resPath = parentPath / "res";
        if (fs::exists(resPath) && fs::is_directory(resPath)) {
            rootPath = (parentPath / "").make_preferred().string();
            return rootPath;
        }
    }

    // 仍未找到，返回空字符串
    rootPath =  "";
    return rootPath;
}
