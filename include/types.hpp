#pragma once

#include <FreeImage.h>
#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>
#include <pybind11/attr.h>

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

namespace colmapeasy {

class Image : public Eigen::Matrix<std::uint8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> {
  public:
    using Matrix::Matrix;

    FIBITMAP* view_as_FreeImage() {
        return FreeImage_ConvertFromRawBits(
            this->data(), 
            this->cols(), 
            this->rows(), 
            this->cols(), 
            8, 
            0, 
            0, 
            0
        );
    }
};

using image_t = Image;

class Kwds {
  public:

    explicit Kwds(const pybind11::kwargs& kwargs) 
      : args{Kwds::to_args(kwargs)} {
        this->argv = std::vector<char*>{};
        for (auto&& arg : this->args) {
            this->argv.push_back(const_cast<char*>(&arg.front()));
        }
    }

    int count() const {
        return static_cast<int>(this->args.size());
    }

    auto size() const {
        return this->args.size();
    }

    char** view_as_argv() {
        return this->argv.data();
    }

  private:
    static std::vector<std::string> to_args(const pybind11::kwargs& kwargs) {
        auto args = std::vector<std::string>{};
        args.emplace_back("colmap");

        for (auto&& [k, value] : kwargs) {
            auto option = k.cast<std::string>();
            boost::replace_all(option, "__", ".");
            args.emplace_back("--" + option);
            args.emplace_back(value.cast<std::string>());
        }

        return args;
    }

  private:
    const std::vector<std::string> args;
    std::vector<char*> argv;
};

// return image_t view_as_image(FIBITMAP* const& fi_bitmap) {
//     FreeImage_ConvertFromRawBits(data, cols, rows, scan_width, bpp, 0, 0, 0);

// }

}