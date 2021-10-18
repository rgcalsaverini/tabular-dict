#include "flatten.h"

#include <vector>
#include <iostream>

using std::vector;
using std::string;

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

string makePath(const string &current_path, const string &key, const string &separator, EscapeType escape) {
    string new_key;
    if (escape == EscapeType::DOUBLE) new_key = replaceSubstr(key, separator, separator + separator);
    else if (escape == EscapeType::SLASH) {
        new_key = replaceSubstr(key, "\\", "\\\\");
        new_key = replaceSubstr(new_key, separator, "\\" + separator);
    } else new_key = key;
    return current_path.empty() ? new_key : current_path + separator + new_key;
}

std::vector<std::variant<int, std::string>> splitPath(const std::string &path,
                                                      const std::string &separator,
                                                      EscapeType escape,
                                                      bool mark_arrays) {
    std::vector<std::variant<int, std::string>> path_parts;
    unsigned long start = 0;
    unsigned long pos = path.find(separator);
    string acc;

    while (pos != string::npos) {
        std::cerr << "Pos: " << pos << "\n";
        if (escape == EscapeType::DOUBLE) {

        } else if (escape == EscapeType::SLASH) {
            if (pos > 0 and path[pos - 1] != '\\') {
                string new_key = path.substr(start, pos);
                replaceSubstr(new_key, "\\", "");
                path_parts.emplace_back(new_key);
                start = pos + separator.size();
                pos = path.find(separator, start);

            } else {
                start = pos + separator.size();
                pos = path.find(separator, start);

            }
        } else {
            path_parts.emplace_back(path.substr(start, pos));
            start = pos + separator.size();
            pos = path.find(separator, start);
        }
    }

    if (key_start < path.size()) path_parts.emplace_back(path.substr(key_start, pos));
    return path_parts;
}

//unsigned long next_sep = path.find(separator, pos + separator.size());
//            std::cerr << "Next Sep: " << next_sep << "\n";
//            if (next_sep > pos + separator.size()) {
//                std::cerr << "Not escaped " << key_start << ":" << pos << "\n";
//                path_parts.emplace_back(path.substr(key_start, pos));
//                key_start = next_sep + separator.size();
//                find_start = key_start;
//                pos = next_sep + separator.size();
//                continue;
//            }
//            std::cerr << "Escaped\n";
//            find_start = pos + separator.size();
//            pos = next_sep;

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

    return result;
}