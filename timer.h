#include <chrono> 

//"Timer" class definition 
class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using Clock = std::chrono::steady_clock;
	using Second = std::chrono::duration<int, std::ratio<1> >;

	std::chrono::time_point<Clock> m_beg { Clock::now() };

	bool paused{false};
	int totalElapsed{0};

public:
	void reset()
	{
		m_beg = Clock::now();
		totalElapsed = 0;
	}
	
	void pause()
	{
		if (!paused)
		{
			paused = true;
			totalElapsed += std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
		}
		else
		{
			paused = false;
			m_beg = Clock::now();
		}
	}

	int elapsed() //const
	{
		if (!paused)
		{
			return totalElapsed + std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
		}
		else
		{
			return totalElapsed;
		}

	}
};