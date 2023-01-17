#include "common.hpp"
#include "shader.hpp"
#include "handlers/shaderHandler.hpp"

ShaderHandler::ShaderHandler() {
    shaders_.push_back(new Shader("shaders/main3D.out", "shaders/main3D.vs", "shaders/main3D.fs"));
    shaders_.push_back(new Shader("shaders/lines3D.out", "shaders/lines3D.vs", "shaders/lines3D.fs"));
    shaders_.push_back(new Shader("shaders/main2D.out", "shaders/main2D.vs", "shaders/main2D.fs"));
    shaders_.push_back(new Shader("shaders/bloc3D.out", "shaders/bloc3D.vs", "shaders/bloc3D.fs"));
    shaders_.push_back(new Shader("shaders/env3D.out", "shaders/env3D.vs", "shaders/env3D.fs"));
}

ShaderHandler::~ShaderHandler() {
    for (std::vector<Shader*>::iterator it = shaders_.begin(); it != shaders_.end(); ++it) {
        delete (*it);
    }
}

void ShaderHandler::RecompileShaders() {
    for (std::vector<Shader*>::iterator it = shaders_.begin(); it != shaders_.end(); ++it) {
        (*it)->Recompile();
    }
}