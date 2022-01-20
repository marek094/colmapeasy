#pragma once

#include "types.hpp"

#include <colmap/util/option_manager.h>
#include <colmap/feature/types.h>
#include <colmap/feature/sift.h>

#include <FreeImage.h>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

#include <vector>
#include <cassert>
#include <utility>

namespace colmapeasy {

namespace py = pybind11;

inline 
std::tuple<colmap::FeatureKeypoints, colmap::FeatureDescriptors>
extract_features(Image image, py::kwargs kwargs) {
    Kwds args{kwargs};

    auto options = colmap::OptionManager{};
    options.AddExtractionOptions();
    options.Parse(args.count(), args.view_as_argv());
    options.Check();
    assert(!!options.sift_extraction);

    auto keypoints = colmap::FeatureKeypoints{};
    auto descriptors = colmap::FeatureDescriptors{};

    // std::cout << extraction_options.max_num_orientations << std::endl;

    auto bitmap = colmap::Bitmap{image.view_as_FreeImage()};
    bool result = colmap::ExtractSiftFeaturesCPU(
        *options.sift_extraction,
        bitmap,
        &keypoints,
        &descriptors
    );

    return {keypoints, descriptors};
}

} // namespace colmapeasy

