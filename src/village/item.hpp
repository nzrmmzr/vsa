#pragma once

#include <string>

namespace vsa::village {

class Item
{
public:
    virtual const std::string& get_id() const = 0;
    virtual const std::string& get_name() const = 0;
};

}