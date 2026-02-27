#pragma once

#include <functional>
#include <memory>
#include <string>

#include <village/resident.hpp>

namespace vsa::village {

class ResidentHerbalist : public Resident
{
public:
    static std::string s_get_id() { return "r_herbalist"; }
    static std::string s_get_name() { return "Herbalist"; }
    std::string get_id() override { return s_get_id(); }
    std::string get_name() override { return s_get_name(); }

public:
    ResidentHerbalist(bool is_male = true, std::size_t age_days = 0);

    void eat() override;
    void produce() override {}
    void iterate_impl() override {}
};

}