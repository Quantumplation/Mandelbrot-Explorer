#ifndef ANIMATED_VALUE_H
#define ANIMATED_VALUE_H

#include <Windows.h>

enum CurvatureType
{
	CURVE_LINEAR,
	CURVE_HOLD,
	CURVE_EASEIN,
	CURVE_EASEOUT,
	CURVE_SMOOTH,
	CURVE_TWIST,
};

enum InteruptType
{
	INTERRUPT_NONE,
	INTERRUPT_RESTART,
	INTERRUPT_CONTINUE,
};

template<typename T>
class AnimatedValue
{
public:
	AnimatedValue(T* value, const T& start, const T& end, float time, CurvatureType how = CURVE_LINEAR) 
		: start_value_(start), end_value_(end), animated_value_(value), interval_length_(time), curve_type_(how)
	{
	}
	~AnimatedValue();

	void Start()
	{
		(*animated_value_) = start_value_;
		start_time_ = timeGetTime();
		elapsed_time_ = 0;
	}

	void Update(float timeScalar = 1.0f)
	{
		if(start_value_ == end_value_)
			return;

		elapsed_time_ += ((timeGetTime() - start_time_) - elapsed_time_) * timeScalar;

		if(elapsed_time_ >= interval_length_)
		{
			End();
			return;
		}

		float t = elapsed_time_ / interval_length_;
		float u = 1.0f - t;

		float start = start_value_;
		float end = end_value_;

		switch(curve_type_)
		{
		default:
		case CURVE_LINEAR:
			(*animated_value_) = start + t * (end - start);
			break;
		case CURVE_HOLD:
			break;
		case CURVE_EASEIN:
			(*animated_value_) = (start - end) * u * u * u + end;
			break;
		case CURVE_EASEOUT:
			(*animated_value_) = start - (start - end) * t * t * t;
			break;
		case CURVE_SMOOTH:
			(*animated_value_) = (u*u*u*start) + 
				                 (3*u*u*t*start) +
								 (3*t*t*u*end) +
								 (t*t*t*end);
			break;
		case CURVE_TWIST:
			(*animated_value_) = (u*u*u*start) + 
				                 (3*u*u*t*end) +
								 (3*t*t*u*start) +
								 (t*t*t*end);
			break;
		}
	}
	const T& GetValue() { return *animated_value_; }
	const T& GetStartValue() { return start_value_; }
	const T& GetEndValue() { return end_value_; }

	void End()
	{
		(*animated_value_) = end_value_;
		elapsed_time_ = interval_length_;
	}

	void Interrupt(const T& new_end, float new_time)
	{	
		start_value_ = (*animated_value_);
		end_value_ = new_end;
		interval_length_ = new_time;
		Start();
	}
	bool IsAnimating()
	{
		return elapsed_time_ < interval_length_;
	}

private:
	CurvatureType curve_type_;
	T* animated_value_;
	T start_value_;
	T end_value_;
	float start_time_;
	float elapsed_time_;
	float interval_length_;
};

#endif