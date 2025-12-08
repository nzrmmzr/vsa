#pragma once


namespace vsa::village {

class Resident
{
public:
    virtual ~Resident() = default;
    virtual void eat() = 0;
    virtual void produce() = 0;
};

} // village
