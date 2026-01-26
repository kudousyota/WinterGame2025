#pragma once
class HitStopManager
{
public:
	void Start(int frame);
	bool IsActive() const;
	void Update();

private:
	int m_frame;
};

