#include "common.hpp"

void vec_string_to_vec_char_array(const std::vector<std::string>& tab, std::vector<const char*>& new_tab) {
    for (std::vector<std::string>::const_iterator it(tab.begin()); it != tab.end(); ++it) {
        new_tab.push_back(it->data());
    }
}

std::vector<std::string> string_to_vec_string(std::string tab, const char& separator, const std::string& remove_chars) {
    std::vector<std::string> res;

    for (unsigned int i = 0U; i < remove_chars.size(); ++i)
        tab.erase(std::remove(tab.begin(), tab.end(), remove_chars[i]), tab.end());

    std::size_t start(0), end(tab.find(separator));
    while (end != std::string::npos) {
        res.push_back(tab.substr(start, end - start));
        start = end + 1;
        end = tab.find(separator, start);
    }
    res.push_back(tab.substr(start, end - start));

    return res;
}
