#pragma once

#include <functional>
#include <memory>
#include <string>

#include <village/resident.hpp>

namespace vsa::village {

class ResidentMiner : public Resident
{
public:
    static std::string s_get_id() { return "r_miner"; }
    static std::string s_get_name() { return "Miner"; }
    std::string get_id() override { return s_get_id(); }
    std::string get_name() override { return s_get_name(); }

public:
    ResidentMiner(bool is_male = true, std::size_t age_days = 0);

    void eat() override;
    void produce() override {}
    void iterate_impl() override {}
};

}