//
// Created by Serhii Pustovit on 30.11.2025.
//

#include "resident_miner.hpp"

#include <iostream>

vsa::village::ResidentMiner::ResidentMiner(bool is_male, std::size_t age_days) : Resident(is_male, age_days) {}

void vsa::village::ResidentMiner::eat()
{
    std::cout << "ResidentMiner eat" << std::endl;
}