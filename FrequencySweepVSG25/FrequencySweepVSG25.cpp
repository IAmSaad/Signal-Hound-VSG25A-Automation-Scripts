#include <VSG25A/sg_api.h>
#include <iostream>
#include <Windows.h>
#include <synchapi.h>

using std::cout;

// the following macros set the sweep type of the program
#define FREQ_SWEEP
#define POW_SWEEP

// the follwoing macros set the paramters of the function
#define START_FREQUENCY		100e6
#define STOP_FREQUENCY		500e6
#define NUMBER_OF_SWEEPS	1
#define STEP_SIZE			10e6
#define DWELL_TIME			1
#define AMPLITUDE			-20

#define LOOP_COUNT			(STOP_FREQUENCY-START_FREQUENCY)/STEP_SIZE



BOOL WINAPI consoleHandler(DWORD signal);

// command line paramters list 
// -fSweep:	start_freq to end_freq
// -pSweep:	min_power to max_power
// -u:		upper limit of power or frequency sweep
// -l:		lower limit of power or frequency sweep
// -n:		number of sweeps [special value INFIN for infinite sweep]
// -d:		delay time between frequency changes
// -offset: frequency offset for sweep. [default is 1MHz or 1e6]


enum errorStatus {
	Success			=  0,
	NotFound		=  1,
	RfSetFail		=  2,
	RfParamSetFail	=  3,
	RfCWSetFail		=  4,
	RfSweepSetFail	=  5,
	VSGCloseFail	=  6,
	ConsoleCtrlErr	= -1,
};

// made global so that console handler can access it on CTRL_C event
int deviceHandle;

int main(int argc, char** argv)
{
	// set flag for program error detection
	errorStatus programFlag = Success;

	// enable console exit with CTRL_C
	if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
		printf("\nERROR: Could not set console handler");
		return ConsoleCtrlErr;
	}

	int loopCount = 0;

	// start frequency
	double startFreq = START_FREQUENCY;
	// stop frequency
	double stopFreq = STOP_FREQUENCY;
	// number of full sweeps
	int sweepTime = NUMBER_OF_SWEEPS;
	// freq span
	double sweepStep = STEP_SIZE;
	// amplidtude for the whole sweep
	double amplitude = AMPLITUDE;
	
	// open device
	sgStatus deviceStatus = sgOpenDevice(&deviceHandle);

	if (deviceStatus != sgUnableToFindDevice)
	{
		// assign start freq to current freq on start of program/loop
		double currentFreq = startFreq;

		while (loopCount <= LOOP_COUNT)
		{
			// Turn RF off
			if (sgRFOff(deviceHandle) != sgNoError)
			{
				cout << "Error on RF off\n";
				programFlag = RfSetFail;
				break;
			}

			// Set parameters
			if (sgSetFrequencyAmplitude(deviceHandle, currentFreq, amplitude) != sgNoError)
			{
				cout << "Error on setting RF parameters\n";
				programFlag = RfParamSetFail;
				break;
			}

			// Turn CW RF signal on
			if (sgSetCW(deviceHandle) != sgNoError)
			{
				cout << "Error on turning ON CW RF signal\n";
				programFlag = RfCWSetFail;
				break;
			}

			cout << "Current Freq is: " << currentFreq/1e6 << "MHz with power "
				<< amplitude << "dBm\n";

			Sleep(DWELL_TIME * 1000);
			currentFreq += sweepStep;
			loopCount++;
		}
	}
	else
	{
		cout << "Device failed to open!\n";
		programFlag = NotFound;
	}

	// only close device if it was opened before
	if (programFlag != NotFound) {
		if (sgCloseDevice(deviceHandle) != sgNoError)
		{
			cout << "Error closing device\n";
			programFlag = VSGCloseFail;
		}
	}

	return programFlag;
}

// safely close device on CTRL_C event
BOOL WINAPI consoleHandler(DWORD signal) {
	if (signal == CTRL_C_EVENT)
		sgCloseDevice(deviceHandle);

	return TRUE;
}

