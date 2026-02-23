#include <village/entities_registry.hpp>

#include <village/residents/resident_baker.hpp>
#include <village/residents/resident_miner.hpp>
#include <village/residents/resident_blacksmith.hpp>
#include <village/residents/resident_herbalist.hpp>

#include <village/tools/tool_sickle.hpp>
#include <village/tools/tool_triphammer.hpp>

namespace vsa::village {

const static ResidentRegistrator<ResidentBaker> r_baker;
const static ResidentRegistrator<ResidentMiner> r_miner;
const static ResidentRegistrator<ResidentBlacksmith> r_blacksmith;
const static ResidentRegistrator<ResidentHerbalist> r_herbalist;

const static ItemRegistrator<ToolSickle> i_sickle;
const static ItemRegistrator<ToolTriphammer> i_triphammer;

}