#pragma once

#include "easy_cache.hpp"

#include <colmap/base/database_cache.h>
#include <colmap/base/reconstruction.h>
#include <colmap/feature/types.h>
#include <colmap/sfm/incremental_mapper.h>
#include <colmap/controllers/incremental_mapper.h>

namespace colmapeasy {

class EasyIncrementalMapper {
  public:  
    EasyIncrementalMapper(EasyCache& ec) : easy_cache{ec}, reconstruction{}, mapper{&easy_cache} {

    }

    const colmapeasy::EasyCache& Cache() const {
        return this->easy_cache;
    }

    const colmap::Reconstruction& Reconstruction() const {
        return this->reconstruction;
    }

    const colmap::IncrementalMapper& Mapper() const {
        return this->mapper;
    }

    colmapeasy::EasyCache& Cache() {
        return this->easy_cache;
    }

    colmap::Reconstruction& Reconstruction() {
        return this->reconstruction;
    }

    colmap::IncrementalMapper& Mapper() {
        return this->mapper;
    }

  protected:
    colmapeasy::EasyCache& easy_cache;
    colmap::Reconstruction reconstruction;
    colmap::IncrementalMapper mapper;        
};

} // namespace colmapeasy