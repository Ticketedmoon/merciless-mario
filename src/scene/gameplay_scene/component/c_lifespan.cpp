#include "component/c_lifespan.h"

CLifespan::CLifespan(uint8_t timeToLive) : m_timeToLive(timeToLive)
{

}

bool CLifespan::isAlive() const
{
    return m_timeToLive > 0;
}

void CLifespan::decreaseTimeToLive()
{
    m_timeToLive--;
}
