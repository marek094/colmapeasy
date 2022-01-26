#pragma once

#include "types.hpp"

#include <colmap/util/option_manager.h>
#include <colmap/estimators/two_view_geometry.h>
#include <colmap/base/camera.h>
#include <colmap/feature/utils.h>
#include <colmap/feature/types.h>


namespace colmapeasy {

inline 
auto
verify(
    colmap::FeatureKeypoints keypoints1,
    colmap::FeatureKeypoints keypoints2,
    colmap::FeatureMatches matches,
    py::kwargs kwargs) {
    Kwds args{std::move(kwargs)};

    auto options = colmap::OptionManager{};
    options.Parse(args.count(), args.view_as_argv());
    options.Check();
    assert(!!options.sift_matching);
    

    auto tw_options = colmap::TwoViewGeometry::Options{};
    tw_options.min_num_inliers = static_cast<size_t>(options.sift_matching->min_num_inliers);
    tw_options.ransac_options.max_error = options.sift_matching->max_error;
    tw_options.ransac_options.confidence = options.sift_matching->confidence;
    tw_options.ransac_options.min_num_trials = static_cast<size_t>(options.sift_matching->min_num_trials);
    tw_options.ransac_options.max_num_trials = static_cast<size_t>(options.sift_matching->max_num_trials);
    tw_options.ransac_options.min_inlier_ratio = options.sift_matching->min_inlier_ratio;
    
    auto tw_geometry = colmap::TwoViewGeometry{};

    auto camera = colmap::Camera();
    camera.SetCameraId(1);

    const auto points1 = colmap::FeatureKeypointsToPointsVector(keypoints1);
    const auto points2 = colmap::FeatureKeypointsToPointsVector(keypoints2);

    tw_geometry.Estimate(
        camera, points1,
        camera, points2,
        matches,
        tw_options);

    return tw_geometry;
}

} // namespace colmapeasy