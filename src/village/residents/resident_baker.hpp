#pragma once

#include <string>
#include <memory>

#include <village/resident.hpp>

namespace vsa::village {

class ResidentBaker : public Resident
{
public:
    static std::string s_get_id() { return "r_baker"; }
    static std::string s_get_name() { return "Baker"; }
    std::string get_id() override { return s_get_id(); }
    std::string get_name() override { return s_get_name(); }

public:
    ResidentBaker(bool is_male = true, std::size_t age_days = 0);

    void eat() override;
    void produce() override {}
    void iterate_impl() override {}
};

} // village