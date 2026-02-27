#pragma once

#include <map>

#include <village/item.hpp>

namespace vsa::village {

class Storage
{
public:
    Storage() = default;

private:
    std::map<std::string, std::size_t> m_id_items_count;
};

} // village