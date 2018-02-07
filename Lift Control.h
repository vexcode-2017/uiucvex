#include "Utility Functions.h"

int liftTarget;
bool newLiftTarget = true;

void liftUp(int speed) {
	motor[lift] = speed;
}

void liftStop() {
	motor[lift] = 0;
}

void manualLift() {
	if (vexRT[Btn6U] == 1) {
		liftUp(100);
	}
	else if (vexRT[Btn6D] == 1) {
		liftUp(-100);
	}
	else {
		liftStop();
	}
}

void setLift(int target) {
	liftTarget = target;
	newLiftTarget = true;
}

task liftControl(){
	int liftState = 1;

	const float Kp = 0.0;
	const float Kd = 0.0;

	int proportion;
	int derivative;
	int error;
	int prevError;
	int raw;

	while(true) {
		if (isAuton) liftState = 1;
		else liftState = 0;

		switch(liftState) {
			case 0:
				manualLift();
				break;
			case 1:
				error = calcError(liftTarget, liftPos);

				proportion = calcP(Kp, error);

				derivative = calcD(Kd, error, prevError);
				prevError = error;
				if (newLiftTarget) derivative = 0;

				raw = proportion + derivative;

				liftUp(raw);

				newLiftTarget = false;
				break;
		}
	}
}
