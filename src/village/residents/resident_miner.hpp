#pragma once

#include <string>

namespace vsa::village {

class ResidentMiner
{
public:
    static std::string get_id() { return "miner_id"; }
    static std::string get_name() { return "Miner"; }
};

}