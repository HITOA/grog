#include "Node.hpp"


Grog::Node::Node(std::shared_ptr<VCL::Source> source, std::shared_ptr<VCL::Logger> logger) : VCLG::Node{ source, logger } {

}

const std::string& Grog::Node::GetName() {
    return name;
}

void Grog::Node::SetName(const std::string& name) {
    this->name = name;
}
