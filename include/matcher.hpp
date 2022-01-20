#pragma once

#include "types.hpp"

#include <colmap/feature/types.h>
#include <colmap/feature/sift.h>

#include <pybind11/attr.h>

#include <vector>
#include <utility>
#include <cassert>

namespace colmapeasy {

namespace py = pybind11;


inline 
colmap::FeatureMatches
match(
    colmap::FeatureDescriptors descriptors1, 
    colmap::FeatureDescriptors descriptors2, 
    py::kwargs kwargs
) {
    auto args = Kwds{std::move(kwargs)};

    auto options = colmap::OptionManager{};
    options.AddMatchingOptions();
    options.Parse(args.count(), args.view_as_argv());
    options.Check();
    assert(!!options.sift_matching);

    auto matches = colmap::FeatureMatches{};
    colmap::MatchSiftFeaturesCPU(
        *options.sift_matching,
        descriptors1,
        descriptors2,
        &matches
    );

    return matches;
}


} // namespace colmapeasy