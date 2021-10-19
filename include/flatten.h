#ifndef FLATTEN_JSON_FLATTEN_H
#define FLATTEN_JSON_FLATTEN_H

#include "json/single_include/nlohmann/json.hpp"

#include <string>
#include <utility>
#include <variant>

using json = nlohmann::ordered_json;
typedef std::vector<std::variant<long int, std::string>>::iterator PathIt;
enum EscapeType {
    NO_ESCAPE, DOUBLE, SLASH
};


std::string replaceSubstr(const std::string &str, const std::string &find_str, const std::string &replace_str);

std::variant<long int, std::string> intOrString(std::string key);


std::string makePath(const std::string &current_path,
                     const std::string &key,
                     const std::string &separator,
                     EscapeType escape);


std::vector<std::variant<long int, std::string>> splitPath(const std::string &path,
                                                      const std::string &separator,
                                                      EscapeType escape,
                                                      bool mark_arrays);

std::vector<std::variant<long int, std::string>> makePathSlash(const std::string &path,
                                                          const std::string &separator,
                                                          bool mark_arrays);

void flattenPath(json &root,
                 json &result,
                 const std::string &path,
                 const std::string &separator,
                 EscapeType escape,
                 bool mark_arrays);

json flatten(json &root, const std::string &separator, EscapeType escape, bool mark_arrays);

json inflate(json &flat_json, const std::string &separator, EscapeType escape, bool mark_arrays);

void deepSetJson(json &obj, const PathIt &path_start, const PathIt &path_end, const json& value);

#endif //FLATTEN_JSON_FLATTEN_H
