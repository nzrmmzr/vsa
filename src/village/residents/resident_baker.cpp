//
// Created by Serhii Pustovit on 30.11.2025.
//

#include "resident_baker.hpp"

#include <iostream>

namespace vsa::village {

ResidentBaker::ResidentBaker(bool is_male, std::size_t age_days) : Resident(is_male, age_days) {}

void vsa::village::ResidentBaker::eat()
{
    std::cout << "ResidentBaker eat" << std::endl;
}

} // village