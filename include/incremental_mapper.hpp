#pragma once

#include "types.hpp"
#include "easy_mapper.hpp"

#include <colmap/base/database_cache.h>
#include <colmap/base/reconstruction.h>
#include <colmap/feature/types.h>
#include <colmap/sfm/incremental_mapper.h>
#include <colmap/controllers/incremental_mapper.h>
#include <colmap/base/image.h>

#include <pybind11/attr.h>
#include <pybind11/stl.h>

#include <vector>
#include <map>
#include <unordered_set>
#include <optional>
#include <iostream>

namespace colmapeasy {

namespace py = pybind11;


inline
std::optional<EasyIncrementalMapper>
map_incremental(py::dict matches, py::kwargs kwargs) {
    Kwds args{std::move(kwargs)};

    auto options = colmap::OptionManager{};
    options.AddMapperOptions();
    options.Parse(args.count(), args.view_as_argv());
    options.Check();
    assert(!!options.mapper);

    auto matches_map = matches.cast<FeatureMatchesMap>();

    auto cache = EasyCache::FromMatches(matches_map, *options.mapper);

    // std::unordered_set<std::string> image_names; //= options.mapper->image_names;
    // const size_t min_num_matches = static_cast<size_t>(options.mapper->min_num_matches);
    // cache.Load(
    //     min_num_matches,
    //     options.mapper->ignore_watermarks,
    //     image_names    
    // );
        
    auto easy_mapper = EasyIncrementalMapper{cache};
    auto init_mapper_options = options.mapper->Mapper();
    auto&& mapper = easy_mapper.Mapper();
    auto&& reconstruction = easy_mapper.Reconstruction();
    // auto&& easy_cache = easy_mapper.EasyCache();

    mapper.BeginReconstruction(&reconstruction);

    for (colmap::image_t im : to_uniq_image_ids(matches_map)) {
        auto image = colmap::Image();
        image.SetImageId(im);
        reconstruction.AddImage(image);
        // easy_cache.AddImage(image);
    }


    if (reconstruction.NumRegImages() == 0) {
        colmap::image_t image_id1 = static_cast<colmap::image_t>(options.mapper->init_image_id1);
        colmap::image_t image_id2 = static_cast<colmap::image_t>(options.mapper->init_image_id2);

        if (options.mapper->init_image_id1 == -1 || options.mapper->init_image_id2 == -1) {
            const bool find_init_success = mapper.FindInitialImagePair(
                init_mapper_options,
                &image_id1,
                &image_id2);

            if (!find_init_success) {
                std::cout << "No initial pair. " << image_id1 << " " << image_id2  << std::endl;
                mapper.EndReconstruction(true);
                return std::nullopt;
            }
        }

        const bool reg_init_success = mapper.RegisterInitialImagePair(
            init_mapper_options,
            image_id1,
            image_id2);
        
        if (!reg_init_success) {
            std::cout << "Init failed." << std::endl;
            mapper.EndReconstruction(true);
            return std::nullopt;
        }
    }


    std::cout << "Success?!" << std::endl;
    return easy_mapper;
}


} // namespace colmapeasy