#pragma once

#include <chrono>
#include <thread>
#include <iostream>


class Timer
{
private:
	std::chrono::duration<double, std::micro> desiredPeriod;
	std::chrono::high_resolution_clock::time_point prevTime, time;

public:
	Timer(unsigned int framerate) {
		desiredPeriod = std::chrono::duration<double, std::micro>(1e6 / framerate);
		prevTime = time = std::chrono::high_resolution_clock::now();
	}

	unsigned int tick() {
		prevTime = time;
		time += std::chrono::duration_cast<std::chrono::microseconds>(desiredPeriod);

		if (std::chrono::high_resolution_clock::now() < time)
			std::this_thread::sleep_until(time);

		return (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - prevTime)).count();
	}
};
