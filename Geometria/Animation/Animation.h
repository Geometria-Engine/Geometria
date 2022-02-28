#pragma once
#include "Multithreading/Multithreading.h"
#include "Tweeny/tweeny.h"
#include <iostream>

#ifndef ANIMATION_H
#define ANIMATION_H
struct Animation
{
	enum Easing
	{
		Linear,
		Back,
		BackIn,
		BackOut,
		Bounce,
		BounceIn,
		BounceOut,
		Circular,
		CircularIn,
		CircularOut,
		Cubic,
		CubicIn,
		CubicOut,
		Elastic,
		ElasticIn,
		ElasticOut,
		Exponential,
		ExponentialIn,
		ExponentialOut,
		Quadratic,
		QuadraticIn,
		QuadraticOut,
		Quartic,
		QuarticIn,
		QuarticOut,
		Quintic,
		QuinticIn,
		QuinticOut,
		Sinusoidal,
		SinusoidalIn,
		SinusoidalOut
	};

	struct Trigger
	{
		unsigned int frame = 0;
		unsigned int t_Duration = 1000;
		bool isSet = false;

		void* owner = nullptr;

		// Int
		tweeny::tween<int> int_Tween;
		int* int_Object = nullptr;
		int int_Result = 0;
		bool usingInt = false;

		// Float
		tweeny::tween<float> float_Tween;
		float* float_Object = nullptr;
		float float_Result = 0;
		bool usingFloat = false;

		// Double
		tweeny::tween<double> double_Tween;
		double* double_Object = nullptr;
		double double_Result = 0;
		bool usingDouble = false;

		// Void
		std::function<void()> void_Trigger;

		Easing easingT = Easing::Linear;

		void SetIntEasing();
		void SetFloatEasing();
		void SetDoubleEasing();

		void SetUp();
		void Play();
		void Finished();

		bool isFinished = false;

		void SetIntPointer(int* p, int r)
		{
			*p = r;
		}

		void SetFloatPointer(float* p, float r)
		{
			*p = r;
		}

		void SetDoublePointer(double* p, double r)
		{
			*p = r;
		}
	};

	struct Timeline
	{
		unsigned int time = 0;
		unsigned int framerate = 0;

		int duration = 0;
		int triggerAmount = 0;
		std::vector<Animation::Trigger*> allTriggers;
		bool play = false;

		bool Play()
		{
			play = true;
			time = 0;

			for (auto i : allTriggers)
			{
				i->isSet = false;
				i->isFinished = false;
			}

			return play;
		}

		bool Stop()
		{
			play = false;
			time = 0;
			return play;
		}

		bool IsPlaying()
		{
			return play;
		}

		Animation::Trigger* AddTrigger(int& p, int to, int atFrame, int duration);
		Animation::Trigger* AddTrigger(float& p, float to, int atFrame, int duration);
		Animation::Trigger* AddTrigger(double& p, double to, int atFrame, int duration);

		Animation::Trigger* AddTrigger(std::function<void()> theEvent, int atFrame, int duration);

		Animation::Trigger* AddTrigger(int& p, int to, int atFrame, int duration, Animation::Easing easing);
		Animation::Trigger* AddTrigger(float& p, float to, int atFrame, int duration, Animation::Easing easing);
		Animation::Trigger* AddTrigger(double& p, double to, int atFrame, int duration, Animation::Easing easing);

		int InSeconds(float sec)
		{
			return sec * framerate;
		}

		void ChangeEndOfTimeline(Animation::Trigger* t);

		void Update();
	};

	struct AnimThread
	{
		std::vector<Animation::Timeline*> allTimelines;
		Thread* owner = nullptr;
		int timelineLimit = 20;
		void UpdateLoop();

		bool HasReachedLimit()
		{
			return allTimelines.size() >= timelineLimit;
		}
	};

	static std::vector<Animation::AnimThread*> allThreads;
	
	static Animation::AnimThread* Create();
	static Animation::AnimThread* FindAvailableAnimationThread();

	static Animation::Timeline* New(int framerate);
};
#endif
