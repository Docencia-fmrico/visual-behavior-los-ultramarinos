#ifndef CPIDCONTROLLER_H
#define CPIDCONTROLLER_H

#include <string>
#include <limits>

class CPidController{
public:
	CPidController(const char* name = "", float samplingTime = 100, float kp = 1, float ti = 0, float td = 0, float comp = 1, float lowerSaturation = -std::numeric_limits<float>::infinity(), float upperSaturation = std::numeric_limits<float>::infinity());
	virtual ~CPidController();

	int GetSystemInput(float measure, float* output);

	float GetProportionalGain(void);
	void SetProportionalGain(float kp);

	float GetIntegrationTime(void);
	void SetIntegrationTime(float ti);

	float GetDerivativeTime(void);
	void SetDerivativeTime(float td);

	float GetSamplingTime(void);
	void SetSamplingTime(float time);

	float GetDerivativeFilter(void);
	void SetDerivativeFilter(float comp);

	float GetLowerSaturationLimit(void);
	void SetLowerSaturationLimit(float saturation);

	float GetUpperSaturationLimit(void);
	void SetUpperSaturationLimit(float saturation);

	float GetError(void);

	float GetTarget(void);
	void SetTarget(float target);

	float GetLastInput(void);
	void Reset(void);

private:
	static constexpr int NONE = -1;
	std::string name;
	int iteration;
	float kp;
	float ti;
	float td;
	float comp;

	float lowerSaturation;
	float upperSaturation;

	bool firstIteration;

	float sp;

	float ts;
	float uk_1;
	float ek_1;
	float ek_2;
	float ek;

};
#endif