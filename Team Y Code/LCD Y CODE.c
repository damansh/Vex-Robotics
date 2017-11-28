#pragma config(Sensor, in1,    claw_s,         sensorPotentiometer)
#pragma config(Sensor, in3,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  arms_s,         sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  right,          sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  left,           sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  button,         sensorTouch)
#pragma config(Motor,  port1,           ArmR1,         tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           ArmR2,         tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           ArmR3,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           ClawA,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           ClawB,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           DriveL,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           DriveR,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           ArmL1,         tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           ArmL2,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          ArmL3,         tmotorVex393HighSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

int claw;
int arms;
float target;
float error;
float derivative;
float lastError;
float Kp = 0.0001;
float Ki = 0.002;
float Kd = 0.002;
const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;
int count = 0;
//Manitha stuff
int claw_;
int lift;
int drive_l;
int drive_r;
float a_value = 0.000062;
int locked = 1285;
int opened_pos = 2300;
bool close_claw = false;
bool open_claw = false;
int arms_return = false;

void shootManitha(int arms, int wait, bool arms_return)
{
	bool arms_up = false;
	bool arms_done = false;

	while(arms_done == false)
	{


		if(SensorValue[arms_s] > arms && arms_up == false)
		{
			motor[ArmL1] = 127;
			motor[ArmL2] = 127;
			motor[ArmL3] = 127;
			motor[ArmR1] = 127;
			motor[ArmR2] = 127;
			motor[ArmR3] = 127;

		}
		else if(SensorValue[arms_s] < arms && arms_up == false)
		{

			motor[ArmL1] = 1;
			motor[ArmL2] = 1;
			motor[ArmL3] = 1;
			motor[ArmR1] = 1;
			motor[ArmR2] = 1;
			motor[ArmR3] = 1;
			wait10Msec(wait);
			close_claw = false;
			open_claw = true;
			wait10Msec(100);
			close_claw = false;
			open_claw = false;
			wait10Msec(wait);
			arms_up = true;


		}
		else if(arms_up == true)
		{

		}
		if(arms_return == true && arms_up == true)
		{
			if(SensorValue[arms_s] < -20 )
			{
				motor[ArmL1] = -60;
				motor[ArmL2] = -60;
				motor[ArmL3] = -60;
				motor[ArmR1] = -60;
				motor[ArmR2] = -60;
				motor[ArmR3] = -60;
			}
			else
			{
				motor[ArmL1] = 0;
				motor[ArmL2] = 0;
				motor[ArmL3] = 0;
				motor[ArmR1] = 0;
				motor[ArmR2] = 0;
				motor[ArmR3] = 0;
				arms_done = true;
			}
		}
		else if(arms_up == true && arms_return == false)
		{
			motor[ArmL1] = 0;
			motor[ArmL2] = 0;
			motor[ArmL3] = 0;
			motor[ArmR1] = 0;
			motor[ArmR2] = 0;
			motor[ArmR3] = 0;
			arms_done = true;
		}
	}
}
task clawManitha()
{
	while(true)
	{

		if(vexRT[Btn6U] == 1 || close_claw == true)
		{
			;
			claw_ = (SensorValue[claw_s] - locked)/2;
		}
		else if(vexRT[Btn5U] == 1 || open_claw == true)
		{
			claw_ = (SensorValue[claw_s] - opened_pos);
			locked = 1000;
		}
		else if(vexRT[Btn6D] == 1)
		{
			if(SensorValue[claw_s] < locked)
			{
				claw_ = 0;
			}
			else
			{
				claw_ = (SensorValue[claw_s] - locked);
			}
		}
		else
		{
			if(vexRT[Btn7L] == 1)
			{
				claw_ = 127;
			}
			else if(vexRT[Btn7R] == 1)
			{
				claw_ = -127;
			}
			else
			{
				claw_ = 0;
			}
		}
		motor[ClawA] = claw_;
		motor[ClawB] = claw_;
	}

}

void drive_tManitha(int time, int power)
{
	motor[DriveL] = power;
	motor[DriveR] = power;
	wait10Msec(time);
	motor[DriveL] = 0;
	motor[DriveR] = 0;

}
void gyro_turnManitha(int angle)
{
	int motorpower = 1;
	bool turned = false;
	int i;
	int tol_gyro = 20;
	int counter_g;
	SensorValue[in3] = 0;
	while(turned == false)
	{
		if(motorpower == 0)
		{
			turned = true;
		}
		else
		{

			counter_g ++;
			motorpower = (SensorValue[in3] - angle)*0.55;
			if(motorpower < 25 &&  motorpower > -25 && motorpower != 0)
			{
				motorpower = 0;
				counter_g = 0;
			}
			motor[DriveL] = -motorpower - motorpower*(counter_g/200);
			motor[DriveR] = motorpower + motorpower*(counter_g/200);
		}
	}
}

void waitForPress()
{
	while(nLCDButtons == 0){}
	wait1Msec(5);
}

void waitForRelease()
{
	while(nLCDButtons != 0){}
	wait1Msec(5);
}

task clawControl() {
	while(true) {
		while (vexRT[Btn5U] == 1) {
			claw = 70;
		}
		while (vexRT[Btn5D] == 1) {
			claw = -70;
		}
		claw = 0;
	}
}

task arm() {
	while(true) {
		if (vexRT[Btn7R] == 1) {
			motor[DriveL] = vexRT[Ch2];
			motor[DriveR] = vexRT[Ch3];
			target = 300;
			float integral = 0;
			SensorValue[arms_s] = 0;
			while(vexRT[Btn6D] == 0) {
				motor[ClawA] = 50;
				motor[ClawB] = 50;
				error = target - SensorValue[arms_s];
				integral = integral + error;

				if (error == 0) {
					integral = 0;
				}
				/* if (abs(error) > 65) {
				integral = 0;
				} */
				derivative = error - lastError;
				lastError = error;
				arms = (Kp*error) + (Ki*integral) + (Kd*derivative);
			}
		}

		while (vexRT[Btn6U] == 1) {
			motor[DriveL] = vexRT[Ch2];
			motor[DriveR] = vexRT[Ch3];
			arms = 127;
		}
		while(vexRT[Btn6D] == 1 && SensorValue[button] == 0) {
			motor[DriveL] = vexRT[Ch2];
			motor[DriveR] = vexRT[Ch3];
			arms = -127;
		}
		while (vexRT[Btn7D] == 1) {
			motor[DriveL] = vexRT[Ch2] - vexRT[Ch1];
			motor[DriveR] = vexRT[Ch2] + vexRT[Ch1];
			arms = vexRT[Ch3];
		}
		motor[DriveL] = vexRT[Ch2];
		motor[DriveR] = vexRT[Ch3];
		arms = 0;
	}
}

void autoDriveTicksShaft(int masterPower, int left_ticks) { //Drive forward based on sensor values.
	int totalTicks = 0;
	int Autoerror = 0;
	int slavePower = masterPower - 5;
	bool type_drive;

	SensorValue[right] = 0;
	SensorValue[left] = 0;

	if (left_ticks > 0) {
		type_drive = true;
		} else {
		type_drive = false;
	}
	if (type_drive == true) {
		while(abs(totalTicks) < left_ticks) {
			motor[DriveL] = masterPower;
			motor[DriveR] = slavePower;

			Autoerror = (abs(SensorValue[left]) - SensorValue[right]);
			slavePower += Autoerror * 0.5;
			totalTicks += abs(SensorValue[left]);
			SensorValue[right] = 0;
			SensorValue[left] = 0;
			wait1Msec(100);
		}
		motor[DriveL] = 0;
		motor[DriveR] = 0;
	}

	if (type_drive == false) {
		while(totalTicks > left_ticks) {
			motor[DriveL] = masterPower;
			motor[DriveR] = slavePower;

			Autoerror = (SensorValue[left] - SensorValue[right]);
			slavePower += Autoerror * 0.5;
			totalTicks += SensorValue[left];
			SensorValue[right] = 0;
			SensorValue[left] = 0;
			wait1Msec(100);
		}
		motor[DriveL] = 0;
		motor[DriveR] = 0;
	}
}

void gyroTurnLeft (int degrees, int speedLeft, int speedRight) {
	SensorType[gyro] = sensorNone;
	int degrees10 = degrees;
	int acceptableError = 5;
	wait1Msec(1000);
	while(abs(SensorValue[gyro]) < degrees10 - 100)  {
		//		motor[ClawA] = 70;
		//		motor[ClawB] = 70;
		motor[left] = speedLeft;
		motor[right] = speedRight;
	}
	motor[left] = -5;
	motor[right] = 5;
	wait1Msec(100);
	while(abs(SensorValue[gyro]) > degrees10 + acceptableError || abs(SensorValue[gyro]) < degrees10 - acceptableError)
	{
		if(abs(SensorValue[gyro]) > degrees10)
		{
			motor[right] = -30;
			motor[left] = 30;
			//		motor[ClawA] = 70;
			//		motor[ClawB] = 70;
		}
		else
		{
			//		motor[ClawA] = 70;
			//		motor[ClawB] = 70;
			motor[right] = 30;
			motor[left] = -30;
		}
		motor[right] = 0;
		motor[left] = 0;
		wait1Msec(250);
	}
}

task battery()
{
	bLCDBacklight = true;									// Turn on LCD Backlight
	string mainBattery, backupBattery;

	while(true)														// An infinite loop to keep the program running until you terminate it
	{
		clearLCDLine(0);											// Clear line 1 (0) of the LCD
		clearLCDLine(1);											// Clear line 2 (1) of the LCD

		//Display the Primary Robot battery voltage
		displayLCDString(0, 0, "Primary: ");
		sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
		displayNextLCDString(mainBattery);

		//Display the Backup battery voltage
		displayLCDString(1, 0, "Backup: ");
		sprintf(backupBattery, "%1.2f%c", BackupBatteryLevel/1000.0, 'V');	//Build the value to be displayed
		displayNextLCDString(backupBattery);

		//Short delay for the LCD refresh rate
		wait1Msec(100);
	}
}


task codeChooser () {
	//Declare count variable to keep track of our choice
	//Clear LCD

	clearLCDLine(0);
	clearLCDLine(1);
	//Loop while center button is not pressed
	while(nLCDButtons != centerButton)
	{
		//Switch case that allows the user to choose from 4 different options
		switch(count){
		case 0:
			//Display first choice
			displayLCDCenteredString(0, "Autonomous 1");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count = 4;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 1:
			//Display second choice
			displayLCDCenteredString(0, "Autonomous 2");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 2:
			//Display third choice
			displayLCDCenteredString(0, "Autonomous 3");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 3:
			//Display fourth choice
			displayLCDCenteredString(0, "Autonomous 4");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count = 0;
			}
			break;
		default:
			count = 0;
			break;
		}
	}
}

void pre_auton()
{

}

task autonomous()
{
	clearLCDLine(0);
	clearLCDLine(1);
	//Switch Case that actually runs the user choice
	switch(count){
	case 0:
		//If count = 0, run the code correspoinding with choice 1
		displayLCDCenteredString(0, "Autonomous 1");
		displayLCDCenteredString(1, "is running!");
		wait1Msec(1000);						// Robot waits for 2000 milliseconds

		// INSERT AUTONOMOUS 1 CODE HERE BEFORE BREAK
		// MANITHA AUTONOMOUS
		int kill = false;
		int auton_s = 2;
		while(auton_s == 1 && kill == false)
		{
			startTask(clawManitha);
			shootManitha(-100,10,true);
			drive_tManitha(60,127);
			gyro_turnManitha(-750);
			close_claw = false;
			open_claw = true;
			drive_tManitha(100,127);
			close_claw = true;
			open_claw = false;
			gyro_turnManitha(-800);
			drive_tManitha(70,-127);
			arms_return = false;
			shootManitha(-1000,10,false);
			kill = true;
		}
		while(auton_s == 2 && kill == false)
		{
			startTask(clawManitha);
			drive_tManitha(60,127);
			gyro_turnManitha(-750);
			close_claw = false;
			open_claw = true;
			drive_tManitha(100,127);
			close_claw = true;
			open_claw = false;
			gyro_turnManitha(-800);
			drive_tManitha(70,-127);
			arms_return = false;
			shootManitha(-1000,10,true);
			close_claw = false;
			open_claw = true;
			drive_tManitha(150,127);
			close_claw = true;
			open_claw = false;
			drive_tManitha(150,-127);
			shootManitha(-1000,10,true);
			kill = true;
		}
		//END OF FIRST AUTONOMOUS
		break;
	case 1:
		//If count = 1, run the code correspoinding with choice 2
		displayLCDCenteredString(0, "Autonomous 2");
		displayLCDCenteredString(1, "is running!");
		wait1Msec(1000);						// Robot waits for 2000 milliseconds

		// INSERT AUTONOMOUS 2 CODE HERE BEFORE BREAK
		// Just the middle cube

		break;
	case 2:
		//If count = 2, run the code correspoinding with choice 3
		displayLCDCenteredString(0, "Autonomous 3");
		displayLCDCenteredString(1, "is running!");
		wait1Msec(1000);						// Robot waits for 2000 milliseconds

		//INSERT AUTONOMOUS 3 CODE HERE BEFORE BREAK
		// Three stars off of the fence

		break;
	case 3:
		//If count = 3, run the code correspoinding with choice 4
		displayLCDCenteredString(0, "Autonomous 4");
		displayLCDCenteredString(1, "is running!");
		wait1Msec(1000);						// Robot waits for 2000 milliseconds

		//INSERT AUTONOMOUS 4 CODE HERE BEFORE BREAK
		//Three stars off of the fence and the middle cube

		break;
	default:
		displayLCDCenteredString(0, "No valid choice");
		displayLCDCenteredString(1, "was made!");
		break;
	}
}


task usercontrol()
{
	//	startTask(drive);
	startTask(codeChooser);
	startTask(arm);
	startTask(clawControl);
	while (true)
	{
		motor[ClawA] = claw;
		motor[ClawB] = claw;
		motor[ArmR1] = arms;
		motor[ArmR2] = arms;
		motor[ArmR3] = arms;
		motor[ArmL1] = arms;
		motor[ArmL2] = arms;
		motor[ArmL3] = arms;
	}
}

/*
void generalClaw (int target) {
while((target - 50) < SensorValue[claw_s] < (target + 50)) {
motor[ClawA] = (SensorValue[claw_s] - target);
motor[ClawB] = (SensorValue[claw_s} - target);
}
motor[ClawA] = 0;
motor[ClawB] = 0;
} */