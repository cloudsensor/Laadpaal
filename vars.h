#define StateInitialize 1
#define StateIdle 2
#define StateSession 3
#define StateError 4
#define StateReserved 5
#define StatePending 6

#define hwVersionPin 5
#define hwDevide 10 //devide value by this number

#define ldrPin 7

#define lockA 0  //v1: 2
#define lockB 1
#define lockStatePin 4  //v1: A3
#define cs_rfid 13 // v1: 7
#define rst_rfid 19
#define pilotPin 12// v1: 9
#define relayPin 2
#define measPin 1
#define proxPin 2  // v1: A2
#define PINTILT 18
#define tempPin 0
#define buzzerPin 4
#define linePin 3
#define NeoPixelPIN 15
#define NeoPixelpix 20
#define NeoPixelcard 3
#define NeoPixelcable 6

#define TxEnablePin 20  //v5

#define timeout 10
#define polling 200 // the scan rate
#define retry_count 10

#define LinuxResetPin 21
#define powerLostPin 6


char answer = ' ';

char* msg;

boolean ErrorFlagPowerFail = false;
boolean ErrorFlagOverHeat = false;
boolean ErrorFlagOutVoltageRange = false;
boolean ErrorFlagOverCurrent = false;
boolean ErrorFlagIntrusionDetected = false;
boolean ErrorFlagTilt = false;
boolean ErrorFlagMainsACDown = false;
boolean ErrorFlagInterfaceError = false;
boolean ErrorFlagLineTestCheck = false;

boolean powerOK = true;
boolean light = false;
boolean UnsafeSituation = false;
boolean RequestCharge = false;
boolean ActuallyCharging = false;
boolean VehicleStatusChanged = true;
boolean LedBlinkCard = false;
unsigned long tagblinkms = 0;
unsigned long msLock = 0;

int atmegastatus = 0;  //State: 0 - “bootloader”
//int settingsState = 0;
char VehicleStatus = 'F'; // “F” – Unknown/Error
char oldVehicleStatus = 'F'; // “F” – Unknown/Error

int buzzerfreq = 5000; //hz, no default given
int buzzerms = 250; // ms no default given

int ledofftest = 0;
int delayloop1 = 0;
int delayloop2 = 50;
char oldchargestate = 'U';    // State 'U': No tag read
char chargestate = 'U';
int error = 0;
int relaystate = 0;
int oldcablecurrent = 0;
int proxcablecurrent = 0;
int state2 = 0;
int temp = 0;
int oldtemp = 0;
boolean linestate = false;
boolean chargemodeActive = false;
int hwVersion = 0;
int vars[4]; //maximum amount of arguments [4] in any command 
unsigned long tagms = 0;
unsigned long statems = 0;
int phigh;
int plow;
float eastronvoltage = 0;
int oldfreeRam = 0;
int tilt=0;
String strMOD ="abcdefghijklmnopqrstuvwxyz";

//measure and controll from KwH
int frequency = 0;
int VoltageL1 = 0;
int VoltageL2 = 0;
int VoltageL3 = 0;
boolean MainsConnected = false;
int CurrentTotal = 0;
int CurrentL1 = 0; 
int CurrentL2 = 0;
int CurrentL3 = 0;

unsigned long ms2=0;

//modbus inputRegs
unsigned int inputRegs0[20]; //van 0 tot 20
unsigned int inputRegs20[20]; //van 20 tot 40
unsigned int inputRegs40[4]; //van 40 tot 44
unsigned int inputRegs47[4]; //van 47 tot 50
unsigned int inputRegs52[2]; //van 53 tot 55
unsigned int inputRegs60[4]; //van 60 tot 64
unsigned int inputRegs66[2]; //van 66 tot 68
unsigned int inputRegs70[20]; //van 70 tot 88
unsigned int holdRegs40[20];

