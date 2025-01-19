#pragma once

#include <pbrlib/scene/visitor.hpp>

#include <sstream>

namespace pbrlib::testing
{
    class GraphvizGenerator :
        public SceneVisitor
    {
        void process(SceneItem* ptr_item) override;
    public:
        GraphvizGenerator();

        std::string source() 
        {
            return _src.str();
        }

    private:
        std::stringstream _src;

        SceneItem* _ptr_root_item = nullptr;
    };
}