#pragma once

#include <vector>

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

class AnimationSequence {
public:
	void Add(Animation animation) 
	{		
		sequence_.push_back(animation); 
		duration_ += animation.duration();
		iter_ = sequence_.begin();
	}

	// time must be nondecreasing
	uint32_t get(uint32_t time) 
	{
		uint32_t time_delta = time - time_base_;
		uint32_t value = iter_->get(time_delta);
		if (time_delta >= iter_->duration()) {
			time_base_ += iter_->duration();
			iter_++;
			if (iter_ == sequence_.end())
				iter_ = sequence_.begin();
		}
		return value;
	}

	uint32_t duration() { return duration_; }

private:
	std::vector<Animation> sequence_;
	std::vector<Animation>::iterator iter_;
	uint32_t duration_ = 0;
	uint32_t time_base_ = 0;
};