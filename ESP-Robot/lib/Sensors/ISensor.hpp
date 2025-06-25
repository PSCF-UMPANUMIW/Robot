#pragma once

class ISensor
{
public:
    virtual void read() = 0;

protected:
    template<typename T>
    void updateWithExpFilter(T& toUpdate, T newValue, float newValueShare)
    {
        toUpdate = (newValueShare * newValue) + ((1.f-newValueShare) * toUpdate);
    }
};