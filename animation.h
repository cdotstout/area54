#pragma once

#include "log.h"

class Animation {
public:
	Animation(uint32_t start = 0, uint32_t end = 255, uint32_t duration = 1000) : start_(start), end_(end), duration_(duration) {}

	uint32_t get(uint32_t time) 
	{ 
		if (time >= duration_ || duration_ == 0)
			return end_;
		int32_t delta = end_ - start_;
		return start_ + delta * (int32_t) time / (int32_t) duration_;
	}

	uint32_t duration() { return duration_; }

private:
	uint32_t start_;
	uint32_t end_;
	uint32_t duration_;
};
