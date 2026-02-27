#pragma once
#include <memory>

#include <village/resident.hpp>

namespace vsa::village {

class ResidentFactory
{
public:
    static std::shared_ptr<Resident> create_resident(const std::string& id, bool is_male, std::size_t age_days);
};

} // village