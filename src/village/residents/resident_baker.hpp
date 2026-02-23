#pragma once

#include <string>

namespace vsa::village {

class ResidentBaker
{
public:
    static std::string get_id() { return "baker_id"; }
    static std::string get_name() { return "Baker"; }

};

} // village