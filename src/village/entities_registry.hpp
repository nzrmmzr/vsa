#pragma once

#include <string>
#include <map>

namespace vsa::village {

class EntitiesRegistry
{
public:
    static EntitiesRegistry& get_instance();

private:
    EntitiesRegistry();

public:
    void register_resident(const std::string& id, const std::string& name);
    void register_item(const std::string& id, const std::string& name);
    [[nodiscard]] const std::map<std::string, std::string>& get_residents() const;
    [[nodiscard]] const std::map<std::string, std::string>& get_items() const;

private:
    std::map<std::string, std::string> m_residents;
    std::map<std::string, std::string> m_items;
};

template <typename T>
class ResidentRegistrator
{
public:
    ResidentRegistrator()
    {
        EntitiesRegistry::get_instance().register_resident(T::get_id(), T::get_name());
    }
};

template <typename T>
class ItemRegistrator
{
public:
    ItemRegistrator()
    {
        EntitiesRegistry::get_instance().register_item(T::get_id(), T::get_name());
    }
};

} // vsa