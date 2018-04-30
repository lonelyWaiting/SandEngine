#pragma once

class STimer
{
public:
	STimer();
	~STimer();

	void Reset();
	void Update();

	float TotalTime() const;
	float DeltaTime() const;

	void SetFixedTimeStep( float FixedTimeStep );

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	float m_FixedTimeStep;
	bool m_bUseFixedTimeStep;
};