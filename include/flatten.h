#ifndef FLATTEN_JSON_FLATTEN_H
#define FLATTEN_JSON_FLATTEN_H

#include "json/single_include/nlohmann/json.hpp"

#include <string>
#include <utility>
#include <variant>

using json = nlohmann::ordered_json;

enum EscapeType {
    NO_ESCAPE, DOUBLE, SLASH
};


std::string replaceSubstr(const std::string &str, const std::string &find_str, const std::string &replace_str);

std::string makePath(const std::string &current_path,
                     const std::string &key,
                     const std::string &separator,
                     EscapeType escape);

std::vector<std::variant<int, std::string>> splitPath(const std::string &path,
                                                      const std::string &separator,
                                                      EscapeType escape,
                                                      bool mark_arrays);

void flattenPath(json &root,
                 json &result,
                 const std::string &path,
                 const std::string &separator,
                 EscapeType escape,
                 bool mark_arrays);

json flatten(json &root, const std::string &separator, EscapeType escape, bool mark_arrays);

json inflate(json &flat_json, const std::string &separator, EscapeType escape, bool mark_arrays);

#endif //FLATTEN_JSON_FLATTEN_H
