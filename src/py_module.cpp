#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/operators.h"
#include "pybind11/complex.h"
#include "pybind11_json/pybind11_json.h"
#include "flatten.h"

PYBIND11_MODULE(tabular_dict, module) {
    module.doc() = "JSON flattening utilities";

    py::enum_<EscapeType>(module, "escape_type")
            .value("NO_ESCAPE", EscapeType::NO_ESCAPE)
            .value("DOUBLE", EscapeType::DOUBLE)
            .value("SLASH", EscapeType::SLASH)
            .export_values();

    module.def("flatten",
               [](py::object &root, const std::string &separator, EscapeType escape, bool mark_arrays) {
                   json json_obj = pyorderedjson::to_json(root);
                   return pyorderedjson::from_json(flatten(json_obj, separator, escape, mark_arrays));
               },
               py::return_value_policy::take_ownership,
               py::arg("data"),
               py::arg("separator") = ".",
               py::arg("escape") = EscapeType::SLASH,
               py::arg("mark_arrays") = true
    );

    module.def("inflate",
               [](py::object &flat_json, const std::string &separator, EscapeType escape, bool mark_arrays) {
                   json json_obj = pyorderedjson::to_json(flat_json);
                   return pyorderedjson::from_json(inflate(json_obj, separator, escape, mark_arrays));
               },
               py::return_value_policy::take_ownership,
               py::arg("flat_json"),
               py::arg("separator") = ".",
               py::arg("escape") = EscapeType::SLASH,
               py::arg("mark_arrays") = true
    );

    module.def("make_path",
               &makePath,
               py::return_value_policy::take_ownership,
               py::arg("path"),
               py::arg("key"),
               py::arg("separator") = ".",
               py::arg("escape") = EscapeType::SLASH
    );

    module.def("split_path",
               &splitPath,
               py::return_value_policy::take_ownership,
               py::arg("path"),
               py::arg("separator") = ".",
               py::arg("escape") = EscapeType::SLASH,
               py::arg("mark_arrays") = true
    );

}