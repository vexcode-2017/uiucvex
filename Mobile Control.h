//MOBILE GOAL INTAKE FUNCTIONS
void mobileIn(int speed) {
	motor[mobileRight] = speed;
	motor[mobileLeft] = speed;
}

void mobileOut(int speed) {
	motor[mobileRight] = 0 - speed;
	motor[mobileLeft] = 0 - speed;
}

void mobileStop() {
	motor[mobileRight] = 0;
	motor[mobileLeft] = 0;
}

void manualMobile() {
	if (vexRT[Btn7U] == 1) {
		mobileIn(100);
	}
	else if (vexRT[Btn7D] == 1) {
		mobileOut(100);
	}
	else {
		mobileStop();
	}
}
