#pragma once

#include <random>


namespace vsa::village {

class Resident
{
public:
    Resident() = delete;
    explicit Resident(bool is_male, std::size_t age_days);

    virtual ~Resident() = default;

    virtual std::string get_id() = 0;
    virtual std::string get_name() = 0;

    virtual void iterate_impl() = 0;

    virtual void eat() = 0;
    virtual void produce() = 0;

    bool iterate();

    [[nodiscard]] bool is_male() const { return m_is_male; }
    [[nodiscard]] bool is_dead() const { return m_is_dead; }
    [[nodiscard]] std::size_t get_age_days() const { return m_age_days; }
    [[nodiscard]] std::size_t get_age_years() const { return m_age_days / 365; }

private:
    bool m_is_male = true;
    std::size_t m_age_days = 0;
    float m_death_probability = 0.0f;

    bool m_is_dead = false;
};

} // village