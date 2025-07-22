#pragma once

#include <VCLG/Node.hpp>

#include <imgui_node_editor.h>


namespace Grog {

    class Node : public VCLG::Node {
    public:
        Node(std::shared_ptr<VCL::Source> source, std::shared_ptr<VCL::Logger> logger = nullptr);

        const std::string& GetName();
        void SetName(const std::string& name);

    private:
        std::string name;
    };

}