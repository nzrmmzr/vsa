#pragma once

#include <string>
#include <map>
#include <functional>

#include <village/resident.hpp>

namespace vsa::village {

class EntitiesRegistry
{
public:
    static EntitiesRegistry& get_instance();

private:
    EntitiesRegistry();

    struct ResidentEntity
    {
        std::string name;
    };
public:
    void register_resident(const std::string& id, const std::string& name);
    [[nodiscard]] const std::map<std::string, ResidentEntity>& get_residents() const;
    [[nodiscard]] const std::vector<std::string>& get_residents_ids() const;
private:

    std::map<std::string, ResidentEntity> m_residents;
    std::vector<std::string> m_residents_ids;
};

template <typename T>
class ResidentRegistrator
{
public:
    ResidentRegistrator()
    {
        auto& r = EntitiesRegistry::get_instance();
        r.register_resident(T::s_get_id(), T::s_get_name());
    }
};

} // vsa