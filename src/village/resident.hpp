#pragma once

#include <random>


namespace vsa::village {

class Resident : public std::enable_shared_from_this<Resident>
{
public:
    Resident() = delete;
    explicit Resident(bool is_male, std::size_t age_days);

    virtual ~Resident();

    virtual std::string get_id() = 0;
    virtual std::string get_name() = 0;

    virtual void iterate_impl() = 0;

    virtual void eat() = 0;
    virtual void produce() = 0;

    bool iterate();

    void set_mother(std::shared_ptr<Resident> mother) { m_mother = mother; }
    void set_father(std::shared_ptr<Resident> father) { m_father = father; }
    void set_spouse(std::shared_ptr<Resident> spouse) { m_spouse = spouse; }
    void add_child(std::shared_ptr<Resident> child) { m_children.push_back(child); }

    void remove_mother() { m_mother.reset(); }
    void remove_father() { m_father.reset(); }
    void remove_spouse() { m_spouse.reset(); }
    void remove_child(std::shared_ptr<Resident> child) { std::erase_if(m_children, [child](const std::shared_ptr<Resident>& r) {return r == child; }); };

    void remove_relations();

    bool has_spouse(){ return m_spouse != nullptr; }
    [[nodiscard]] bool is_male() const { return m_is_male; }
    [[nodiscard]] bool is_dead() const { return m_is_dead; }
    [[nodiscard]] std::size_t get_age_days() const { return m_age_days; }
    [[nodiscard]] std::size_t get_age_years() const { return m_age_days / 365; }
    std::size_t count_children(){ return m_children.size(); }

private:
    bool m_is_male = true;
    std::size_t m_age_days = 0;
    float m_death_probability = 0.0f;

    std::shared_ptr<Resident> m_mother;
    std::shared_ptr<Resident> m_father;
    std::shared_ptr<Resident> m_spouse;
    std::vector<std::shared_ptr<Resident>> m_children;

    bool m_is_dead = false;
};

} // village