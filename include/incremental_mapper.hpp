#pragma once

#include "types.hpp"

#include <colmap/feature/types.h>

#include <pybind11/attr.h>
#include <pybind11/stl.h>

#include <vector>
#include <map>


namespace colmapeasy {

namespace py = pybind11;


using FeatureMatchesMap = std::map<std::pair<size_t, size_t>, colmap::FeatureMatches>;

inline
auto
map_incremental(py::dict matches, py::kwargs kwargs) {
    Kwds args{std::move(kwargs)};

    auto options = colmap::OptionManager{};
    options.AddMapperOptions();
    options.Parse(args.count(), args.view_as_argv());
    options.Check();
    assert(!!options.mapper);

    auto matches_map = matches.cast<FeatureMatchesMap>();


  
    return matches_map.size();
} 




} // namespace colmapeasy