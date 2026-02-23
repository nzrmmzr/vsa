#pragma once

#include <string>

namespace vsa::village {

class ResidentBlacksmith
{
public:
    static std::string get_id() { return "blacksmith_id"; }
    static std::string get_name() { return "Blacksmith"; }
};

} // village