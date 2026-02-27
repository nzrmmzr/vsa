//
// Created by Serhii Pustovit on 30.11.2025.
//

#include "resident_herbalist.hpp"

#include <iostream>

namespace vsa::village {

ResidentHerbalist::ResidentHerbalist(bool is_male, std::size_t age_days) : Resident(is_male, age_days) {}

void vsa::village::ResidentHerbalist::eat()
{
    std::cout << "ResidentHerbalist eat" << std::endl;
}

} // village