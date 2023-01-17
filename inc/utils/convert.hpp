#pragma once

void vec_string_to_vec_char_array(const std::vector<std::string>& tab, std::vector<const char*>& new_tab);
std::vector<std::string> string_to_vec_string(std::string tab, const char& separator = ',', const std::string& remove_chars = "[]\"");
