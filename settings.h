//Default settings//
#define fwVersion "2.82.20141103"
#define debug 0
#define RAMWarning 100 //How mutch RAM left to give warning, 0 to disable
#define AbsoluteCurrentLimit 32 //This systems absolute current limit
#define DefaultsetInstallationCurrent 6
#define AbsoluteMinCurrent 6

#define DefaultsetTemperatureLimitLimitLow -20
#define DefaultsetTemperatureLimitLimitHigh 60
#define DefaultsetTemperatureLimitWarnLow -10
#define DefaultsetTemperatureLimitWarnHigh 50

#define DefaultVoltageRMS 230 //default 230
#define DefaultVoltageMarginHi 6 //Default 6%
#define DefaultVoltageMarginLo 10 //Default 10%
#define DefaultCurrentCheckMargin 10 //default 10%
#define DefaultCurrentCheckGraceperiod 1000 //default 1000 ms
#define DefaultCurrentCheckPeriod 1 //default 1 ms

#define DefaultHeartbeatCheckPeriod 1000 //default 1000 ms
#define DefaultHeartbeatCheckMisses 3 //default 3 misses

#define tagtimeout 5000
#define linethreshold 500 //default 500
#define linecount 100 //default 200
#define sep ","
#define beattimeout 1000
#define ldrth 100 //Intrusion Detection threshold

//The default settings above are set below to be used at other places in the code
int SmartCurrentMax = AbsoluteCurrentLimit;  
int MaxCurrent = AbsoluteCurrentLimit;
int setInstallationPhases = 3;
int setInstallationCurrent = DefaultsetInstallationCurrent;
int setTemperatureLimitLimitLow = DefaultsetTemperatureLimitLimitLow;
int setTemperatureLimitLimitHigh = DefaultsetTemperatureLimitLimitHigh;
int setTemperatureLimitWarnLow = DefaultsetTemperatureLimitWarnLow;
int setTemperatureLimitWarnHigh = DefaultsetTemperatureLimitWarnHigh;
int VoltageRMS = DefaultVoltageRMS;
int VoltageMarginHi = (DefaultVoltageRMS * (100+DefaultVoltageMarginHi)) / 100;
int VoltageMarginLo = (DefaultVoltageRMS * (100-DefaultVoltageMarginLo)) / 100;
int VoltageSampleperiode = 1;
int CurrentCheckMargin = DefaultCurrentCheckMargin; 
int CurrentCheckGraceperiod = DefaultCurrentCheckGraceperiod; 
int CurrentCheckPeriod = DefaultCurrentCheckPeriod; 
int TagScanPeriod = tagtimeout;
int TagBlinkPeriod = 5000;
int setHeartbeatCheckPeriod = DefaultHeartbeatCheckPeriod; 
int setHeartbeatCheckMisses = DefaultHeartbeatCheckMisses; 
boolean VentilationPermission = false; //false is prohibited (default), true is allowed
int LedIntensity = 255;

//Defauld LED modes
int setLedIndex = 0; //to request the LED scene
int LedIndex[21] =         {100, 101, 102, 103, 104, 105, 106, 107, 108, 199, 201, 202, 203, 204, 299, 301, 302, 303, 304, 305, 399};
int LedFunctionArray[21] = {  0,   3,   1,   1,   1,   3,   2,   1,   1,   0,   1,   2, 000, 000, 000,   1,   1,   1,   1,   2, 000}; //function is integer “off”=0, “on”=1, “flashing”=2, “breathing”=3 (off is only used for disabling override)
int LedRArray[21]=         {000, 000, 100, 100, 000, 000, 100, 000, 000,   0, 100,   0, 000, 000, 000, 100, 100,   0, 100, 100, 000};
int LedGArray[21]=         {000, 100, 000,  50, 000, 000,  50, 000, 000,   0, 100, 100, 000, 000, 000, 100, 100, 100,   0, 100, 000};
int LedBArray[21]=         {000,   0, 000, 000, 100, 100, 000, 000, 000,   0,   0,   0, 000, 000, 000,   0,   0,   0,   0,   0, 000}; 

