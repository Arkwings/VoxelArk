#pragma once

class Shader;

class ShaderHandler: public Singleton<ShaderHandler> {
    public:
    ShaderHandler();
    ~ShaderHandler();
    ShaderHandler(const ShaderHandler&) = delete;
    ShaderHandler& operator=(const ShaderHandler&) = delete;

    Shader* GetShader(const unsigned int& index) { return shaders_[index]; };
    Shader* operator [](const unsigned int& index) { return shaders_[index]; }
    void RecompileShaders();

    private:
    std::vector<Shader*> shaders_;
};