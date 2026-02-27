#include "resident_factory.hpp"


#include <tools/logger.hpp>
#include <village/residents/resident_baker.hpp>
#include <village/residents/resident_blacksmith.hpp>
#include <village/residents/resident_miner.hpp>
#include <village/residents/resident_herbalist.hpp>

namespace vsa::village {

std::shared_ptr<Resident> ResidentFactory::create_resident(const std::string& id, bool is_male, std::size_t age_days)
{
    if(id == ResidentBaker::s_get_id()) { return std::make_shared<ResidentBaker>(is_male, age_days); }
    if(id == ResidentBlacksmith::s_get_id()) { return std::make_shared<ResidentBlacksmith>(is_male, age_days); }
    if(id == ResidentMiner::s_get_id()) { return std::make_shared<ResidentMiner>(is_male, age_days); }
    if(id == ResidentHerbalist::s_get_id()) { return std::make_shared<ResidentHerbalist>(is_male, age_days); }
    VSA_LOG_ERROR("village", "Unknown resident id: {};", id);
    return nullptr;
}

} // village