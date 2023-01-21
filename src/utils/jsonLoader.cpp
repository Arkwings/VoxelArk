#include "common.hpp"

JsonLoader::JsonLoader(const std::string& filepath)
    : filepath_(filepath) {

    std::string props_path;
#ifdef DEBUG
    props_path = __FILE__;
    props_path = props_path.substr(0, props_path.rfind("\\") + 1) + "..\\..\\extern\\";
    DEBUG_NAME_PRINT("json properties", props_path + "properties\\textures.json");
#endif
    std::ifstream f(props_path + filepath);
    if (!f)  std::cerr << "ERROR: texture properties path doesn't exist" << std::endl;

    data_ = nlohmann::json::parse(f);
}

JsonLoader::~JsonLoader() {}

const std::string JsonLoader::Get(const std::string& json_path) {
    auto& ret(data_[nlohmann::json::json_pointer(json_path)]);
    return ret.is_null() ? "NULL" : ret.dump();
}
