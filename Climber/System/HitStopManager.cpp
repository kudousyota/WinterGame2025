#include "HitStopManager.h"

void HitStopManager::Start(int frame)
{
	if (frame > m_frame)
	{
        m_frame = frame;
	}
}

void HitStopManager::Update()
{
    if (m_frame > 0)
    {
        --m_frame;
    }
}

bool HitStopManager::IsActive() const
{
    return m_frame > 0;
}
