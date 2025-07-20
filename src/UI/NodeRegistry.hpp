#pragma once

#include <VCL/Source.hpp>

#include <map>
#include <string>


namespace Grog {

    struct NodeSource {
        std::shared_ptr<VCL::Source> source{};
    };

    // It will do for now
    using NodeRegistry = std::map<std::string, NodeSource>;

}
