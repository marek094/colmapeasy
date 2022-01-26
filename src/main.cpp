#include "feature_extractor.hpp"
#include "matcher.hpp"
#include "verifier.hpp"
#include "incremental_mapper.hpp"
#include <Eigen/Dense>

#include <filesystem>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/pytypes.h>

#include <colmap/feature/types.h>
#include <colmap/estimators/two_view_geometry.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(colmapeasy_impl, m) {

    m.def("extract_features", colmapeasy::extract_features);

    m.def("match", colmapeasy::match);
    
    m.def("map_incremental", colmapeasy::map_incremental);

    m.def("verify", colmapeasy::verify);

    py::class_<colmap::FeatureKeypoint>(m, "FeatureKeypoint")
        .def(py::init<const float, const float, 
                      const float, const float, 
                      const float, const float>()
            )
        .def_readwrite("x", &colmap::FeatureKeypoint::x)
        .def_readwrite("y", &colmap::FeatureKeypoint::y)
        .def_readwrite("a11", &colmap::FeatureKeypoint::a11)
        .def_readwrite("a12", &colmap::FeatureKeypoint::a12)
        .def_readwrite("a21", &colmap::FeatureKeypoint::a21)
        .def_readwrite("a22", &colmap::FeatureKeypoint::a22)
        .def("__repr__",
            [](const colmap::FeatureKeypoint& fkp) {
                return "<colmapeasy.FeatureKeypoint [" +
                    std::to_string(fkp.x) + " " + std::to_string(fkp.y) +
                    "]>";
            }
        );

    py::class_<colmap::FeatureMatch>(m, "FeatureMatch")
        .def(py::init<const colmap::point2D_t, const colmap::point2D_t>())
        .def_readwrite("point2D_idx1", &colmap::FeatureMatch::point2D_idx1)
        .def_readwrite("point2D_idx2", &colmap::FeatureMatch::point2D_idx2)
        .def("__repr__", 
            [](const colmap::FeatureMatch& fm) {
                return "<colmapeasy.FeatureMatch of " +
                    std::to_string(fm.point2D_idx1) + " and " + std::to_string(fm.point2D_idx2) +
                    ">";
            }
        );        


    py::class_<colmap::TwoViewGeometry>(m, "TwoViewGeometry")
        .def(py::init<>())
        .def_readwrite("config", &colmap::TwoViewGeometry::config)
        .def_readwrite("E", &colmap::TwoViewGeometry::E)
        .def_readwrite("F", &colmap::TwoViewGeometry::F)
        .def_readwrite("H", &colmap::TwoViewGeometry::H)
        .def_readwrite("tvec", &colmap::TwoViewGeometry::tvec)
        .def_readwrite("qvec", &colmap::TwoViewGeometry::qvec)
        .def_readwrite("inlier_matches", &colmap::TwoViewGeometry::inlier_matches)
        .def_readwrite("tri_angle", &colmap::TwoViewGeometry::tri_angle)
        .def("__repr__",
            [](const colmap::TwoViewGeometry& twg) {
                return "<colmapeasy.TwoViewGeometry of " +
                    std::to_string(std::size(twg.inlier_matches)) +
                    " inliers>";
            }
        );




    // py::class_<std::filesystem::path>(m, "Path")
    //     .def(py::init<std::string>());
    // py::implicitly_convertible<std::string, std::filesystem::path>();



#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
