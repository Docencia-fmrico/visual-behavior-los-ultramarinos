#include "CPidController.h"

CPidController::CPidController(const char* name, float samplingTime, float kp, float ti, float td, float comp, float lowerSaturation, float upperSaturation){
	this->name = std::string(name);
	this->ts = samplingTime;

	this->kp = kp;
	this->ti = ti;
	this->td = td;

	this->comp = comp;
	this->upperSaturation = upperSaturation;
	this->lowerSaturation = lowerSaturation;

	Reset();
}

CPidController::~CPidController(){
	
}

int CPidController::GetSystemInput(float measure, float* output){
	float uk = 0;
	float ki = 0;
	float kd = 0;

	if(firstIteration){
		this->uk_1 = 0;
		this->ek = 0;
		firstIteration = false;
	}
	this->ek_2 = this->ek_1;
	this->ek_1 = this->ek;
	this->ek = this->sp - measure;
	if(kp != 0.0){
		if(ti == 0.0 and td == 0.0){
			uk = kp * ek;
		} else if(ti != 0.0 and td == 0.0){
			ki = (kp / ti) * ts;
			uk = this->uk_1 + ((kp + ki) * this->ek) - (kp * this->ek_1);
		} else if(ti == 0.0 and td != 0.0){
			kd = (kp * td) / ts;
			uk = ((kp + kd) * this->ek) - (kd * this->ek_1);
		} else if(ti != 0.0 and td != 0.0){
			ki = (kp / ti) * ts;
			kd = (kp * td) / ts;
			uk = this->uk_1 + ((kp + ki + kd) * this->ek) - ((kp + 2.0 * kd) * this->ek_1) + ((kd) * this->ek_2);
		}
	}
	if(uk > upperSaturation){
		uk = upperSaturation;
	}
	if(uk < lowerSaturation){
		uk = lowerSaturation;
	}
	this->uk_1 = uk;
	*output = uk;
	this->iteration++;
	return this->iteration;
}

void CPidController::Reset(void){
	firstIteration = true;
	this->uk_1 = std::numeric_limits<float>::infinity();
	this->ek_1 = 0;
	this->ek_2 = 0;
	this->ek = std::numeric_limits<float>::infinity();
	this->iteration = NONE;
}

float CPidController::GetProportionalGain(void){
	return this->kp;
}

void CPidController::SetProportionalGain(float kp){
	this->kp = kp;
}

float CPidController::GetIntegrationTime(void){
	return this->ti;
}

void CPidController::SetIntegrationTime(float ti){
	this->ti = ti;
}

float CPidController::GetDerivativeTime(void){
	return this->td;
}

void CPidController::SetDerivativeTime(float td){
	this->td = td;
}

float CPidController::GetSamplingTime(void){
	return this->ts;
}

void CPidController::SetSamplingTime(float ts){
	this->ts = ts;
}

float CPidController::GetDerivativeFilter(void){
	return this->comp;
}

void CPidController::SetDerivativeFilter(float comp){
	this->comp = comp;
}

float CPidController::GetLowerSaturationLimit(void){
	return this->lowerSaturation;
}

void CPidController::SetLowerSaturationLimit(float saturation){
	this->lowerSaturation = saturation;
}

float CPidController::GetUpperSaturationLimit(void){
	return upperSaturation;
}

void CPidController::SetUpperSaturationLimit(float saturation){
	this->upperSaturation = saturation;
}

float CPidController::GetTarget(void){
	return this->sp;
}

float CPidController::GetError(void){
	return this->ek;
}

void CPidController::SetTarget(float target){
	this->sp = target;
}

float CPidController::GetLastInput(void){
	return this->uk_1;
}