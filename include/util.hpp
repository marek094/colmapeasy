#pragma once

#include <boost/algorithm/string.hpp>
#include <pybind11/pybind11.h>

inline auto encode_kwargs(const pybind11::kwargs& kwargs) {
    auto args = std::vector<std::string>{};
    args.emplace_back("colmap");

    for (auto&& [k, value] : kwargs) {
        auto option = k.cast<std::string>();
        boost::replace_all(option, "__", ".");
        args.emplace_back("--" + option);
        args.emplace_back(value.cast<std::string>());
    }

    auto argv = std::vector<char*>{};
    for (auto&& arg : args) {
        argv.push_back(&arg.front());
    }

    return make_pair(args, argv);
}