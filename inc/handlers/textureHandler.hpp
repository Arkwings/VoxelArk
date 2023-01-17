#pragma once

class Texture;

class TextureHandler: public Singleton<TextureHandler> {
    public:
    TextureHandler();
    ~TextureHandler();
    TextureHandler(const TextureHandler&) = delete;
    TextureHandler& operator=(const TextureHandler&) = delete;

    Texture* GetTex(const unsigned int& ID) { return textures_[ID]; };
    Texture* operator [](const unsigned int& ID) { return textures_[ID]; };
    private:
    std::vector<Texture*> textures_;
};