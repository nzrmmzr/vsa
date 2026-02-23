#pragma once

#include <string>

namespace vsa::village {

class ResidentHerbalist
{
public:
    static std::string get_id() { return "herbalist_id"; }
    static std::string get_name() { return "Herbalist"; }
};

}