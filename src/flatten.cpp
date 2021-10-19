#include "flatten.h"

#include <vector>
#include <iostream>

using std::vector;
using std::variant;
using std::string;
using std::optional;


std::string replaceSubstr(const std::string &str, const std::string &find_str, const std::string &replace_str) {
    string new_str;
    unsigned long start = 0;
    unsigned long pos = str.find(find_str);

    while (pos != string::npos) {
        new_str += str.substr(start, pos - start) + replace_str;
        start = pos + find_str.size();
        pos = str.find(find_str, start);
    }
    if (start < str.size()) new_str += str.substr(start, str.size() - start);

    return new_str;
}

variant<long int, string> intOrString(string key) {
    bool is_digit_key = key[0] == '['
                        && key[key.size() - 1] == ']'
                        && std::all_of(key.begin() + 1, key.end() - 1, [](char c) { return c >= '0' && c <= '9'; });
    if (!is_digit_key) return key;
    return std::stoi(key.substr(1, key.size() - 2));
}

string makePath(const string &current_path, const string &key, const string &separator, EscapeType escape) {
    string new_key;
    if (escape == EscapeType::DOUBLE) new_key = replaceSubstr(key, separator, separator + separator);
    else if (escape == EscapeType::SLASH) {
        new_key = replaceSubstr(key, "\\", "\\\\");
        new_key = replaceSubstr(new_key, separator, "\\" + separator);
    } else new_key = key;
    return current_path.empty() ? new_key : current_path + separator + new_key;
}

vector<variant<long int, string>> splitPath(const string &path,
                                            const string &separator,
                                            EscapeType escape,
                                            bool mark_arrays) {
    if (escape == EscapeType::SLASH) return makePathSlash(path, separator, mark_arrays);
}

vector<variant<long int, string>> makePathSlash(const string &path, const string &separator, bool mark_arrays) {
    string current_key;
    std::vector<std::variant<long int, std::string>> path_parts;
    int64_t start = 0;
    int64_t pos = path.find(separator);
    int64_t len;

    while (pos != string::npos) {
        bool is_escaped = false;
        for (int64_t i = pos - 1; i >= 0 && path[i] == '\\'; i--) is_escaped = !is_escaped;

        if (is_escaped) {
            len = pos - 1 - start;
            current_key += replaceSubstr(path.substr(start, len), "\\\\", "\\") + separator;
        } else {
            len = pos - start;
            current_key += replaceSubstr(path.substr(start, len), "\\\\", "\\");

            path_parts.emplace_back(intOrString(current_key));
            current_key = "";
        }
        start = pos + separator.size();
        pos = path.find(separator, start);
    }

    if (start < path.size()) {
        current_key += replaceSubstr(path.substr(start, pos), "\\\\", "\\");
        path_parts.emplace_back(intOrString(current_key));
        current_key = "";
    }

    if (!current_key.empty()) {
        path_parts.emplace_back(intOrString(current_key));
    }

    return path_parts;
}

void flattenPath(json &root,
                 json &result,
                 const std::string &path,
                 const std::string &separator,
                 EscapeType escape,
                 bool mark_arrays) {
    if (root.is_primitive()) {
        result[path] = root;
        return;
    }

    if (root.is_array()) {
        uint16_t i = 0;
        for (auto &array_item: root) {
            std::string array_key = mark_arrays ? "[" + std::to_string(i++) + "]" : std::to_string(i++);
            std::string array_path = makePath(path, array_key, separator, escape);
            flattenPath(array_item, result, array_path, separator, escape, mark_arrays);
        }
        return;
    }

    for (auto const &item: root.items()) {
        string new_path = makePath(path, item.key(), separator, escape);
        flattenPath(item.value(), result, new_path, separator, escape, mark_arrays);
    }
}

json flatten(json &root, const std::string &separator, EscapeType escape, bool mark_arrays) {
    json result;
    flattenPath(root, result, "", separator, escape, mark_arrays);
    return result;
}

json inflate(json &flat_json, const std::string &separator, EscapeType escape, bool mark_arrays) {
    json result;

    for (const auto &item: flat_json.items()) {
        vector<variant<long int, string>> path = splitPath(item.key(), separator, escape, mark_arrays);
        deepSetJson(result, path.begin(), path.end(), item.value());
    }
    return result;
}

void deepSetJson(json &obj, const PathIt &path_start, const PathIt &path_end, const json &value) {
    if (path_start == path_end) {
        obj = value;
        return;
    }

    if (path_start->index() == 0) {
        // It is a list
        long int idx = std::get<long int>(*path_start);

        if (!obj.empty() && !obj.is_array()) {
            throw std::invalid_argument("Conflicting path, trying to fit an array.");
        }
        if (obj.empty() || obj.size() < idx) {
            for (uint64_t i = obj.size(); i <= idx; i++) {
                obj.push_back(nullptr);
            }
        }
        deepSetJson(obj[idx], path_start + 1, path_end, value);
    } else {
        // It is a dict
        string key = std::get<string>(*path_start);
        if (!obj.empty() && !obj.is_object()) {
            throw std::invalid_argument("Conflicting path, trying to fit an object.");
        }
        deepSetJson(obj[key], path_start + 1, path_end, value);
    }
}
