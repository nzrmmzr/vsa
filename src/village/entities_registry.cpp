#include "entities_registry.hpp"

namespace vsa::village {

EntitiesRegistry& EntitiesRegistry::get_instance()
{
    static EntitiesRegistry instance;
    return instance;
}

EntitiesRegistry::EntitiesRegistry() = default;

void EntitiesRegistry::register_resident(const std::string& id, const std::string& name) { m_residents[id] = name; }
void EntitiesRegistry::register_item(const std::string& id, const std::string& name) { m_items[id] = name; }

const std::map<std::string, std::string>& EntitiesRegistry::get_residents() const { return m_residents; }
const std::map<std::string, std::string>& EntitiesRegistry::get_items() const { return m_items; }

} // vsa