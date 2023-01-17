#pragma once

class JsonLoader {
    public:
    JsonLoader() = delete;
    JsonLoader(const std::string& filepath);
    ~JsonLoader();
    JsonLoader(const JsonLoader&) = delete;
    JsonLoader& operator=(const JsonLoader&) = delete;

    const std::string Get(const std::string& json_path);

    private:
    std::string filepath_;
    nlohmann::json data_;
};
