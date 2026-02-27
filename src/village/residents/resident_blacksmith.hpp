#pragma once

#include <string>
#include <memory>

#include <village/resident.hpp>

namespace vsa::village {

class ResidentBlacksmith : public Resident
{
public:
    static std::string s_get_id() { return "r_blacksmith"; }
    static std::string s_get_name() { return "Blacksmith"; }
    std::string get_id() override { return s_get_id(); }
    std::string get_name() override { return s_get_name(); }

public:
    ResidentBlacksmith(bool is_male = true, std::size_t age_days = 0);

    void eat() override;
    void produce() override {}
    void iterate_impl() override {}
};

} // village