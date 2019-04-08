#ifndef SG_API_H
#define SG_API_H

#if defined(_WIN32) || defined(_WIN64)
#  ifdef SG_EXPORTS
#    define SG_API _declspec(dllexport)
#  else 
#    define SG_API _declspec(dllimport)
#  endif
#else /* Linux */
#  define SG_API
#endif

#define SG_FALSE (0)
#define SG_TRUE (1)

#define SG_MAX_DEVICES (16)

#define SG_MIN_FREQUENCY (80.0e6)
#define SG_MAX_FREQUENCY (2.55e9)
#define SG_MIN_OUTPUT_POWER (-50.0)
#define SG_MAX_OUTPUT_POWER (+13.0)

#define SG_MIN_AM_FREQ (30.0)
#define SG_MAX_AM_FREQ (50.0e6)

#define SG_MIN_SYMBOL_RATE (53.334e3)
#define SG_MAX_SYMBOL_RATE (180.0e6)

enum sgMode {
    sgModeCW = 0,
    sgModeAM = 1,
    sgModeFM = 2,
    sgModePulse = 3,
    sgModePM = 4,
    sgModeStepSweep = 5,
    sgModeListSweep = 6,
    sgModeNoise = 7,
    sgCustomIQ = 8
};

enum sgBool {
    sgFalse = 0,
    sgTrue = 1
};

enum sgMultiTonePhase {
    sgParabolic = 0,
    sgRandom = 1,
    sgRandomFixedSeed = 2
};

enum sgShape {
    sgShapeSine = 0,
    sgShapeTriangle = 1,
    sgShapeSquare = 2,
	sgShapeRamp = 3
};

enum sgFilterType {
    sgRaisedCosine = 0,
    sgRootRaisedCosine = 1,
	sgGaussian = 2,
	sgNone = 3
};

enum sgModulationType {
    sgModBPSK = 0,
    sgModDBPSK = 1,
    sgModQPSK = 2,
    sgModDQPSK = 3,
    sgModOQPSK = 4,
    sgModPI4DQPSK = 5,
	sgMod8PSK = 6,
    sgModD8PSK = 7,
	sgMod16PSK = 8,
	sgMod16QAM = 9,
	sgMod64QAM = 10, 
	sgMod256QAM = 11
};

enum sgStatus {
    sgInvalidParameter = -6,
    sgMaxDevicesOpen = -5,
    sgUnableToFindDevice = -4,
    sgInvalidDeviceHandle = -3,
    sgNullPtrErr = -1, 
    sgNoError = 0,
    sgSettingClamped = 1
};

#ifdef	__cplusplus
extern "C" {
#endif

SG_API sgStatus sgGetDeviceList(int deviceList[16], int *length);
SG_API sgStatus sgOpenDevice(int *device);
SG_API sgStatus sgOpenDeviceBySerial(int *device, int serialNumber);
SG_API sgStatus sgCloseDevice(int device);

SG_API sgStatus sgGetSerialNumber(int device, int *serialNumber);

SG_API sgStatus sgSetFrequencyAmplitude(int device, double frequency, double amplitude);
SG_API sgStatus sgRFOff(int device);
SG_API sgStatus sgSetCW(int device);
SG_API sgStatus sgSetAM(int device, double frequency, double depth, sgShape shape);
SG_API sgStatus sgSetFM(int device, double frequency, double deviation, sgShape shape);
SG_API sgStatus sgSetPulse(int device, double period, double width);
SG_API sgStatus sgSetSweep(int device, double time, double span);
SG_API sgStatus sgSetMultitone(int device, int count, double spacing, 
                               double notchWidth, sgMultiTonePhase phase);
SG_API sgStatus sgSetASK(int device, double symbolRate, sgFilterType filterType,
                         double filterAlpha, double depth, int *symbols, int symbolCount);
SG_API sgStatus sgSetFSK(int device, double symbolRate, sgFilterType filterType,
                         double filterAlpha, double modulationIndex, int *symbols, int symbolCount);
SG_API sgStatus sgSetPSK(int device, double symbolRate, sgModulationType modType,
                         sgFilterType filterType, double filterAlpha, int *symbols, int symbolCount);
SG_API sgStatus sgSetMultitone(int device, int count, double spacing, double notchWidth, sgMultiTonePhase phase);

SG_API sgStatus sgSetCustomIQ(int device, double clockRate, double *IVals, 
                              double *QVals, int length, int period);

SG_API sgStatus sgQueryPulse(int device, double *period, double *width);
SG_API sgStatus sgQuerySymbolClockRate(int device, double *clock);
SG_API sgStatus sgQueryClockError(int device, double *error);

SG_API const char* sgGetStatusString(sgStatus status);
SG_API const char* sgGetAPIVersion();

SG_API sgStatus sgSetIQNullValue(int device, int Icount, int Qcount); // Sets an I/Q value to null the LO feed-thru. This may belong in SG_API_internal

#ifdef	__cplusplus
} // extern "C"
#endif

#endif