#pragma once

#include "types.hpp"

#include <colmap/base/database_cache.h>
#include <colmap/base/database.h>
#include <colmap/base/image.h>

#include <iostream>
#include <cassert>
#include <tuple>

namespace colmapeasy {

class EasyCache : public colmap::DatabaseCache {
  public:  
    EasyCache() = default;
    EasyCache(const EasyCache&) = default;
    EasyCache(EasyCache&&) = default;
    EasyCache& operator=(const EasyCache&) noexcept = default;
    EasyCache& operator=(EasyCache&&) noexcept = default;
    
    template<typename Options>
    static EasyCache FromMatches(FeatureMatchesMap map, const Options& options) {
        auto result = EasyCache{};
        
        for (colmap::image_t im : to_uniq_image_ids(map)) {
            auto image = colmap::Image();
            image.SetImageId(im);
            result.AddImage(image);
        }

        for (auto& [im_pair, matches] : map) {
            auto pair_id = std::apply(colmap::Database::ImagePairToPairId, im_pair);
            
            // if (matches.size() < static_cast<size_t>(options.min_num_inliers)) {
            //     matches = {};
            // }

            // if ()
        }


        return result;
    }

    void Load(
        const colmap::Database& database,
        const size_t min_num_matches,
        const bool ignore_watermarks,
        const std::unordered_set<std::string>& image_names) override {
        
        // pass
        assert(false);
    }

    void Load(
        const size_t min_num_matches,
        const bool ignore_watermarks,
        const std::unordered_set<std::string>& image_names) {
        // pass
    }

    virtual inline size_t NumCameras() const override {
        return 0;
    }

    virtual inline size_t NumImages() const override {
        return 0;
    }

};



} // namespace colmapeasy