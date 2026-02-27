#include "residents/resident_herbalist.hpp"

#include <village/entities_registry.hpp>

#include <village/residents/resident_baker.hpp>
#include <village/residents/resident_miner.hpp>
#include <village/residents/resident_blacksmith.hpp>

namespace vsa::village {

const static ResidentRegistrator<ResidentBaker> r_rb;
const static ResidentRegistrator<ResidentMiner> r_rm;
const static ResidentRegistrator<ResidentBlacksmith> r_rbs;
const static ResidentRegistrator<ResidentHerbalist> r_hb;

}