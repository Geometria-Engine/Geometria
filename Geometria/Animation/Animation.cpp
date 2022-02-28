#include "Animation.h"

std::vector<Animation::AnimThread*> Animation::allThreads;

Animation::Trigger* Animation::Timeline::AddTrigger(int& p, int to, int atFrame, int duration)
{
	return AddTrigger(p, to, atFrame, duration, Easing::Linear);
}

Animation::Trigger* Animation::Timeline::AddTrigger(float& p, float to, int atFrame, int duration)
{
	return AddTrigger(p, to, atFrame, duration, Easing::Linear);
}

Animation::Trigger* Animation::Timeline::AddTrigger(double& p, double to, int atFrame, int duration)
{
	return AddTrigger(p, to, atFrame, duration, Easing::Linear);
}

Animation::Trigger* Animation::Timeline::AddTrigger(std::function<void()> theEvent, int atFrame, int duration)
{
	Animation::Trigger* newT = new Animation::Trigger();
	newT->void_Trigger = theEvent;
	newT->frame = atFrame;
	newT->t_Duration = duration;

	newT->owner = this;
	allTriggers.push_back(newT);
	ChangeEndOfTimeline(newT);

	return newT;
}

Animation::Trigger* Animation::Timeline::AddTrigger(int& p, int to, int atFrame, int duration, Animation::Easing easing)
{
	Animation::Trigger* newT = new Animation::Trigger();
	newT->int_Object = &p;
	newT->int_Result = to;
	newT->frame = atFrame;
	newT->t_Duration = duration;
	newT->easingT = easing;
	
	newT->owner = this;
	allTriggers.push_back(newT);
	ChangeEndOfTimeline(newT);
	return newT;
}

Animation::Trigger* Animation::Timeline::AddTrigger(float& p, float to, int atFrame, int duration, Animation::Easing easing)
{
	Animation::Trigger* newT = new Animation::Trigger();
	newT->float_Object = &p;
	newT->float_Result = to;
	newT->frame = atFrame;
	newT->t_Duration = duration;
	newT->easingT = easing;

	newT->owner = this;
	allTriggers.push_back(newT);
	ChangeEndOfTimeline(newT);
	return newT;
}

Animation::Trigger* Animation::Timeline::AddTrigger(double& p, double to, int atFrame, int duration, Animation::Easing easing)
{
	Animation::Trigger* newT = new Animation::Trigger();
	newT->double_Object = &p;
	newT->double_Result = to;
	newT->frame = atFrame;
	newT->t_Duration = duration;
	newT->easingT = easing;

	newT->owner = this;
	allTriggers.push_back(newT);
	ChangeEndOfTimeline(newT);
	return newT;
}

void Animation::Timeline::ChangeEndOfTimeline(Animation::Trigger* t)
{
	int t_final = t->frame + t->t_Duration;
	if (t_final > duration)
		duration = t_final;
}

void Animation::Timeline::Update()
{
	int step = framerate / 100;
	if (step < 1)
		step = 1;

	if (time <= duration)
	{
		for (auto i : allTriggers)
		{
			if (i->frame <= time && !i->isFinished)
			{
				i->Play();
			}
		}
		time += step;
	}
	else
	{
		Stop();
	}
}

void Animation::AnimThread::UpdateLoop()
{
	for (auto i : allTimelines)
	{
		if (i->IsPlaying())
		{
			i->Update();
		}
	}
}

void Animation::Trigger::SetUp()
{
	if (int_Object != nullptr)
	{
		int int_Begin = *int_Object;
		int_Tween = tweeny::from(int_Begin).to(int_Result).during(t_Duration);
		SetIntEasing();
		usingInt = true;
	}

	if (float_Object != nullptr)
	{
		float float_Begin = *float_Object;
		float_Tween = tweeny::from(float_Begin).to(float_Result).during(t_Duration);
		SetFloatEasing();
		usingFloat = true;
	}

	if (double_Object != nullptr)
	{
		double double_Begin = *double_Object;
		double_Tween = tweeny::from(double_Begin).to(double_Result).during(t_Duration);
		SetDoubleEasing();
		usingDouble = true;
	}

	isSet = true;
}

void Animation::Trigger::Play()
{
	if (!isSet)
		SetUp();

	Animation::Timeline* t = (Animation::Timeline*)owner;

	int step = t->framerate / 100;
	if (step < 1)
		step = 1;

	if (usingInt)
	{
		SetIntPointer(int_Object, int_Tween.step(step));

		if (int_Tween.progress() >= 1)
			Finished();
	}
	else if (usingFloat)
	{
		SetFloatPointer(float_Object, float_Tween.step(step));

		if (float_Tween.progress() >= 1)
			Finished();
	}
	else if (usingDouble)
	{
		SetDoublePointer(double_Object, double_Tween.step(step));

		if (double_Tween.progress() >= 1)
			Finished();
	}
	else
	{
		void_Trigger();
		Finished();
	}
}

void Animation::Trigger::Finished()
{
	usingInt = usingDouble = usingFloat = false;
	isFinished = true;
}

Animation::AnimThread* Animation::Create()
{
	Animation::AnimThread* newTh = new Animation::AnimThread();
	newTh->owner = Multithreading::RunUpdateThread([newTh]() { newTh->UpdateLoop(); }, 250);
	allThreads.push_back(newTh);
	return newTh;
}

Animation::AnimThread* Animation::FindAvailableAnimationThread()
{
	for (auto i : allThreads)
	{
		if (!i->HasReachedLimit())
		{
			return i;
		}
	}

	return Animation::Create();
}

Animation::Timeline* Animation::New(int framerate)
{
	Animation::Timeline* newTi = new Animation::Timeline();

	newTi->framerate = framerate;

	Animation::AnimThread* a = FindAvailableAnimationThread();
	if (a != nullptr)
	{
		a->allTimelines.push_back(newTi);

		int milliseconds = 1000 / framerate;

		if (milliseconds < a->owner->milliseconds)
		{
			a->owner->ChangeMilliseconds(milliseconds);
		}
	}

	return newTi;
}

#pragma region IntEasing
void Animation::Trigger::SetIntEasing()
{
	switch (easingT)
	{
	case Linear:
		int_Tween.via(tweeny::easing::linear);
		break;

	case Back:
		int_Tween.via(tweeny::easing::backInOut);
		break;
	case BackIn:
		int_Tween.via(tweeny::easing::backIn);
		break;
	case BackOut:
		int_Tween.via(tweeny::easing::backOut);
		break;

	case Bounce:
		int_Tween.via(tweeny::easing::bounceInOut);
		break;
	case BounceIn:
		int_Tween.via(tweeny::easing::bounceIn);
		break;
	case BounceOut:
		int_Tween.via(tweeny::easing::bounceOut);
		break;

	case Circular:
		int_Tween.via(tweeny::easing::circularInOut);
		break;
	case CircularIn:
		int_Tween.via(tweeny::easing::circularIn);
		break;
	case CircularOut:
		int_Tween.via(tweeny::easing::circularOut);
		break;

	case Cubic:
		int_Tween.via(tweeny::easing::cubicInOut);
		break;
	case CubicIn:
		int_Tween.via(tweeny::easing::cubicIn);
		break;
	case CubicOut:
		int_Tween.via(tweeny::easing::cubicOut);
		break;

	case Elastic:
		int_Tween.via(tweeny::easing::elasticInOut);
		break;
	case ElasticIn:
		int_Tween.via(tweeny::easing::elasticIn);
		break;
	case ElasticOut:
		int_Tween.via(tweeny::easing::elasticOut);
		break;

	case Exponential:
		int_Tween.via(tweeny::easing::exponentialInOut);
		break;
	case ExponentialIn:
		int_Tween.via(tweeny::easing::exponentialIn);
		break;
	case ExponentialOut:
		int_Tween.via(tweeny::easing::exponentialOut);
		break;

	case Quadratic:
		int_Tween.via(tweeny::easing::quadraticInOut);
		break;
	case QuadraticIn:
		int_Tween.via(tweeny::easing::quadraticIn);
		break;
	case QuadraticOut:
		int_Tween.via(tweeny::easing::quadraticOut);
		break;

	case Quartic:
		int_Tween.via(tweeny::easing::quarticInOut);
		break;
	case QuarticIn:
		int_Tween.via(tweeny::easing::quarticIn);
		break;
	case QuarticOut:
		int_Tween.via(tweeny::easing::quarticOut);
		break;

	case Quintic:
		int_Tween.via(tweeny::easing::quinticInOut);
		break;
	case QuinticIn:
		int_Tween.via(tweeny::easing::quinticIn);
		break;
	case QuinticOut:
		int_Tween.via(tweeny::easing::quinticOut);
		break;

	case Sinusoidal:
		int_Tween.via(tweeny::easing::sinusoidalInOut);
		break;
	case SinusoidalIn:
		int_Tween.via(tweeny::easing::sinusoidalIn);
		break;
	case SinusoidalOut:
		int_Tween.via(tweeny::easing::sinusoidalOut);
		break;
	}
}
#pragma endregion

#pragma region FloatEasing
void Animation::Trigger::SetFloatEasing()
{
	switch (easingT)
	{
	case Linear:
		float_Tween.via(tweeny::easing::linear);
		break;

	case Back:
		float_Tween.via(tweeny::easing::backInOut);
		break;
	case BackIn:
		float_Tween.via(tweeny::easing::backIn);
		break;
	case BackOut:
		float_Tween.via(tweeny::easing::backOut);
		break;

	case Bounce:
		float_Tween.via(tweeny::easing::bounceInOut);
		break;
	case BounceIn:
		float_Tween.via(tweeny::easing::bounceIn);
		break;
	case BounceOut:
		float_Tween.via(tweeny::easing::bounceOut);
		break;

	case Circular:
		float_Tween.via(tweeny::easing::circularInOut);
		break;
	case CircularIn:
		float_Tween.via(tweeny::easing::circularIn);
		break;
	case CircularOut:
		float_Tween.via(tweeny::easing::circularOut);
		break;

	case Cubic:
		float_Tween.via(tweeny::easing::cubicInOut);
		break;
	case CubicIn:
		float_Tween.via(tweeny::easing::cubicIn);
		break;
	case CubicOut:
		float_Tween.via(tweeny::easing::cubicOut);
		break;

	case Elastic:
		float_Tween.via(tweeny::easing::elasticInOut);
		break;
	case ElasticIn:
		float_Tween.via(tweeny::easing::elasticIn);
		break;
	case ElasticOut:
		float_Tween.via(tweeny::easing::elasticOut);
		break;

	case Exponential:
		float_Tween.via(tweeny::easing::exponentialInOut);
		break;
	case ExponentialIn:
		float_Tween.via(tweeny::easing::exponentialIn);
		break;
	case ExponentialOut:
		float_Tween.via(tweeny::easing::exponentialOut);
		break;

	case Quadratic:
		float_Tween.via(tweeny::easing::quadraticInOut);
		break;
	case QuadraticIn:
		float_Tween.via(tweeny::easing::quadraticIn);
		break;
	case QuadraticOut:
		float_Tween.via(tweeny::easing::quadraticOut);
		break;

	case Quartic:
		float_Tween.via(tweeny::easing::quarticInOut);
		break;
	case QuarticIn:
		float_Tween.via(tweeny::easing::quarticIn);
		break;
	case QuarticOut:
		float_Tween.via(tweeny::easing::quarticOut);
		break;

	case Quintic:
		float_Tween.via(tweeny::easing::quinticInOut);
		break;
	case QuinticIn:
		float_Tween.via(tweeny::easing::quinticIn);
		break;
	case QuinticOut:
		float_Tween.via(tweeny::easing::quinticOut);
		break;

	case Sinusoidal:
		float_Tween.via(tweeny::easing::sinusoidalInOut);
		break;
	case SinusoidalIn:
		float_Tween.via(tweeny::easing::sinusoidalIn);
		break;
	case SinusoidalOut:
		float_Tween.via(tweeny::easing::sinusoidalOut);
		break;
	}
}
#pragma endregion

#pragma region DoubleEasing
void Animation::Trigger::SetDoubleEasing()
{
	switch (easingT)
	{
	case Linear:
		double_Tween.via(tweeny::easing::linear);
		break;

	case Back:
		double_Tween.via(tweeny::easing::backInOut);
		break;
	case BackIn:
		double_Tween.via(tweeny::easing::backIn);
		break;
	case BackOut:
		double_Tween.via(tweeny::easing::backOut);
		break;

	case Bounce:
		double_Tween.via(tweeny::easing::bounceInOut);
		break;
	case BounceIn:
		double_Tween.via(tweeny::easing::bounceIn);
		break;
	case BounceOut:
		double_Tween.via(tweeny::easing::bounceOut);
		break;

	case Circular:
		double_Tween.via(tweeny::easing::circularInOut);
		break;
	case CircularIn:
		double_Tween.via(tweeny::easing::circularIn);
		break;
	case CircularOut:
		double_Tween.via(tweeny::easing::circularOut);
		break;

	case Cubic:
		double_Tween.via(tweeny::easing::cubicInOut);
		break;
	case CubicIn:
		double_Tween.via(tweeny::easing::cubicIn);
		break;
	case CubicOut:
		double_Tween.via(tweeny::easing::cubicOut);
		break;

	case Elastic:
		double_Tween.via(tweeny::easing::elasticInOut);
		break;
	case ElasticIn:
		double_Tween.via(tweeny::easing::elasticIn);
		break;
	case ElasticOut:
		double_Tween.via(tweeny::easing::elasticOut);
		break;

	case Exponential:
		double_Tween.via(tweeny::easing::exponentialInOut);
		break;
	case ExponentialIn:
		double_Tween.via(tweeny::easing::exponentialIn);
		break;
	case ExponentialOut:
		double_Tween.via(tweeny::easing::exponentialOut);
		break;

	case Quadratic:
		double_Tween.via(tweeny::easing::quadraticInOut);
		break;
	case QuadraticIn:
		double_Tween.via(tweeny::easing::quadraticIn);
		break;
	case QuadraticOut:
		double_Tween.via(tweeny::easing::quadraticOut);
		break;

	case Quartic:
		double_Tween.via(tweeny::easing::quarticInOut);
		break;
	case QuarticIn:
		double_Tween.via(tweeny::easing::quarticIn);
		break;
	case QuarticOut:
		double_Tween.via(tweeny::easing::quarticOut);
		break;

	case Quintic:
		double_Tween.via(tweeny::easing::quinticInOut);
		break;
	case QuinticIn:
		double_Tween.via(tweeny::easing::quinticIn);
		break;
	case QuinticOut:
		double_Tween.via(tweeny::easing::quinticOut);
		break;

	case Sinusoidal:
		double_Tween.via(tweeny::easing::sinusoidalInOut);
		break;
	case SinusoidalIn:
		double_Tween.via(tweeny::easing::sinusoidalIn);
		break;
	case SinusoidalOut:
		double_Tween.via(tweeny::easing::sinusoidalOut);
		break;
	}
}
#pragma endregion
