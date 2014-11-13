/*
 accepted Returned after a valid and successful parameter write
  rejected Returned when the parameter could not be read or written
  notsupported Returned if the parameter is unknown
  error Returned when the message format is incorrect or the checksum does not correspond
 */

char p_buffer[25]; 
#define P(str) (strcpy_P(p_buffer, PSTR(str)), p_buffer)

void settingscommandinit(void) {

  //Settings command list
  SCmd.addCommand(P("Version?"), cmdVersion); //Hardware and firmware version of the peripheral controller
  SCmd.addCommand(P("ChangeState"), cmdChangeState); //Request to change the peripheral controller state
  SCmd.addCommand(P("State?"), cmdState); //Read peripheral component status and error flags
  SCmd.addCommand(P("Reset"), cmdReset); //Reset the peripheral application
  SCmd.addCommand(P("setLed"), cmdsetLed); //Sets the LED properties for a spe-cific operational mode
  SCmd.addCommand(P("setInstallation"), cmdsetInstallation); //Sets number of connected phases and maximum current per phase for installation
  SCmd.addCommand(P("setCurrentCheck"), cmdsetCurrentCheck); //Sets the check margin for current checking
  SCmd.addCommand(P("setVoltageCheck"), cmdsetVoltageCheck); //Sets the expected phase voltage to neutral (RMS), check margin and the sample period
  SCmd.addCommand(P("setHeartbeatCheck"), cmdsetHeartbeatCheck); //Parameters for heartbeat between PIC and Linux
  SCmd.addCommand(P("setTemperatureLimit"), cmdsetTemperatureLimit); //Sets limits for the temperature check
  SCmd.addCommand(P("setVentilationPermission"), cmdsetVentilationPermission); //Enables ventilated charge sessions
  SCmd.addCommand(P("setTagScanPeriod"), cmdsetTagScanPeriod); //Sets the time period in which addi-tional scans for the same tag ID are rejected
  SCmd.addCommand(P("setBuzzer"), cmdSetBuzzer); //Autonomous buzzer when tag is read (set time to 0 for no buzzer)

  SCmd.addCommand(P("accepted"), cmdAccepted); //Hardware and firmware version of the peripheral controller
  //SCmd.addCommand(P("error"), cmdError); //Hardware and firmware version of the peripheral controller

  SCmd.addDefaultHandler(rejected);
}

void commandinit(void) {
  //Status command list



  SCmd.addCommand(P("LockStatus?"), cmdLockStatus); //Charge cable lock in socket status and control
  SCmd.addCommand(P("VehicleStatus?"), cmdVehicleStatus); //Read the connection status of the electric vehicle
  SCmd.addCommand(P("RelayStatus?"), cmdRelayState); //Main power relay status and control
  SCmd.addCommand(P("TemperatureStatus?"), cmdTemperatureStatus); //Charge point internal temperature
  SCmd.addCommand(P("CableCurrentMax?"), cmdCableCurrentMax); //Maximum current strength allowed to be transferred by the cable
  SCmd.addCommand(P("EnclosureStatus?"), cmdEnclosureStatus); //Intrusion detect sensor status

    //Commands command list

  SCmd.addCommand(P("EmergencyStop"), cmdEmergencyStop); //Command peripheral controller to go to a safe state immediatelywhen an unsafe situation is detected by Linux application,
  SCmd.addCommand(P("Unlock"), cmdUnlock); //Charge cable explicit unlock control
  SCmd.addCommand(P("ChargeStart"), cmdChargeStart); //Enable charging of the verhicle
  SCmd.addCommand(P("ChargeStop"), cmdChargeStop); //Disable charging of the verhicle
  SCmd.addCommand(P("Buzzer"), cmdBuzzer); //Activate buzzer
  SCmd.addCommand(P("LedOverride"), cmdLedOverride); //RGB LED override control
  SCmd.addCommand(P("LedIntensity"), cmdLedIntensity); //RGB LED override control
  SCmd.addCommand(P("SmartCurrentMax"), cmdSmartCurrentMax); //Set the (dynamical) maximum allowable current for smart charging
  //SCmd.addCommand(P("ExternalSwitch"), fniy); //Control the state of potential free contacts

  //Metering command list
  SCmd.addCommand(P("EnergyActiveExport?"), cmdEnergyActiveExport); //Energy exported by EV (To-tal,L1,L2,L3)//INFORMATION NOT COMPLETELY AVAILABLE.
  SCmd.addCommand(P("EnergyActiveImport?"), cmdEnergyActiveImport); //Energy imported by EV (To-tal,L1,L2,L3) //INFORMATION NOT COMPLETELY AVAILABLE.
  SCmd.addCommand(P("EnergyReactiveExport?"), cmdEnergyReactiveExport); //Reactive energy exported by EV (Total,L1,L2,L3)//INFORMATION NOT COMPLETELY AVAILABLE.
  SCmd.addCommand(P("EnergyReactiveImport?"), cmdEnergyReactiveImport); //Reactive energy imported by EV (Total,L1,L2,L3)//INFORMATION NOT COMPLETELY AVAILABLE.
  SCmd.addCommand(P("PowerActiveExport?"), fniy); //Instantaneous active power export-ed by EV (Total,L1,L2,L3)//INFORMATION NOT AVAILABLE.
  SCmd.addCommand(P("PowerActiveImport?"), cmdPowerActiveImport); //Instantaneous active power imported by EV (Total,L1,L2,L3)
  SCmd.addCommand(P("PowerReactiveExport?"), fniy); //Instantaneous reactive power exported by EV (Total,L1,L2,L3)//INFORMATION NOT AVAILABLE.
  SCmd.addCommand(P("PowerReactiveImport?"), cmdPowerReactiveImport); //Instantaneous reactive power imported by EV (Total,L1,L2,L3)
  SCmd.addCommand(P("CurrentExport?"), cmdCurrentExport); //Instantaneous current flow to EV (Total,L1,L2,L3)
  SCmd.addCommand(P("CurrentImport?"), cmdCurrentImport); //Instantaneous current flow from EV (Total,L1,L2,L3) //INFORMATION NOT AVAILABLE.
  SCmd.addCommand(P("Voltage?"), cmdVoltage); //AC RMS supply voltage (L1,L2,L3)
  SCmd.addCommand(P("PowerFactor?"), cmdPowerFactor); //Power factor (Total, L1,L2,L3)
  SCmd.addCommand(P("PhaseAngle?"), cmdPhaseAngle); //Phase angle (L1,L2,L3)
  SCmd.addCommand(P("Frequency?"), cmdFrequency); //Frequency of the supply voltage
  SCmd.addCommand(P("SerialNumber?"), cmdSerialNumber); //Frequency of the supply voltage

  SCmd.addDefaultHandler(unrecognized);
  //Metering External command list is not implemented

}

void fniy(void) {
  sendMes(PSTR("notsupported)")); 
}

void cmdAccepted(void) {
  answer = 'a';
}

/*void cmdError(void) {
 answer = 'e';
 }*/
/////////////////////////////////Status functions//////////////////////////////////////////////////////
void cmdVersion(void) {
  strMOD = "=";
  strMOD += hwVersion;
  strMOD += sep;
  strMOD += fwVersion;
  //sendMes(PSTR("Version"), strMOD);
  sendMes(PSTR("Version"), strMOD);
}

void cmdState(void) {
  sendMes(PSTR("State"), GetState());
}

void cmdLockStatus(void) {
  if(locked() == "LOCKED")
    sendMes(PSTR("LockStatus=locked")); 
  else
    sendMes(PSTR("LockStatus=unlocked"));  
}

void cmdVehicleStatus(void) {
  strMOD = "=";
  strMOD += VehicleStatus;
  sendMes(PSTR("VehicleStatus"), strMOD);
}

void cmdRelayState(void) {
  if(linestate) //This supposed to be relaystate
    sendMes(PSTR("RelayStatus=close"));  
  else
    sendMes(PSTR("RelayStatus=open"));  
}

void cmdTemperatureStatus(void) {
  strMOD = "=";
  strMOD += temp;
  sendMes(PSTR("TemperatureStatus"), strMOD);
}

void cmdCableCurrentMax(void) { //Maximum current strength allowed to be transferred by the cable
  strMOD = "=";
  if(UnsafeSituation)
    strMOD += 0;
  else
    strMOD += MaxCurrent;
  sendMes(PSTR("CableCurrentMax"), strMOD);  
}

void cmdEnclosureStatus(void) {
  if(ErrorFlagIntrusionDetected)
    sendMes(PSTR("EnclosureStatus=open"));  
  else
    sendMes(PSTR("EnclosureStatus=closed")); 
}

////////////////////////////////////////////////////////////Commands functions///////////////////////////////////////////////

void cmdChangeState(void) {
  int newState = atoi(SCmd.next());

  if(newState == 2 || newState == 5 ) { //2 is idle, 5 is reserved
    if(atmegastatus == 1 || atmegastatus == 2 ||  atmegastatus == 4 || atmegastatus == 5){ //only possible if in INIT, IDLE, ERROR or RESERVED
      AtmegaState(newState);
      sendMes(PSTR("accepted"));
    }
    else{
      sendMes(PSTR("rejected"));
    }
  }
  else{
    sendMes(PSTR("invalid"));
  }
}

void cmdEmergencyStop(void) {
  sendMes(PSTR("accepted"));  
  RequestCharge = false;
}

void cmdUnlock(void) {
  RequestCharge = false;
  sendMes(PSTR("accepted"));  
  //SafeUnlockCable();
}

void cmdReset(void) {
  sendMes(PSTR("accepted")); 
  delay(100); 
  asm volatile("jmp 0");
}

void cmdChargeStart(void) {
  sendMes(PSTR("accepted"));  
  RequestCharge = true;
}

void cmdChargeStop(void) {
  sendMes(PSTR("accepted"));  
  RequestCharge = false;
}

void cmdBuzzer(void){ //Activate buzzer
  char *arg;  
  arg = SCmd.next();
  if (arg != NULL) {
    char * readString = arg;
    parseNumbers(arg, sep, 2, vars);

    if(vars[1] >=1 && vars[1] <= 10000){ 
      sendMes(PSTR("accepted"));
      beep(vars[1]);
    }
    else{
      sendMes(PSTR("invalid"));
    }
  }
  else{
    sendMes(PSTR("invalid"));
  }
}

void cmdLedOverride(void){ //RGB LED override control
  //
  char *arg;  
  arg = SCmd.next();
  if (arg != NULL) {
    char * readString = arg;
    parseNumbers(arg, sep, 5, vars); //vars.h vars needs to be edited to more space in array?! in case of error, needs testing.

    if((vars[0]==100 ||vars[0]==200 || vars[0]==300) && vars[1]>=0 && vars[1]<4 && vars[2]>=0 && vars[2]<=100 && vars[3]>=0 && vars[3]<=100 && vars[4]>=0 && vars[4]<=100){ //test the range
      if(vars[0] == 100)
        setLedIndex = GetLedIndex(199);
      else if(vars[0] == 200)
        setLedIndex = GetLedIndex(299);
      else if(vars[0] == 300)
        setLedIndex = GetLedIndex(399);
     
      LedFunctionArray[setLedIndex] = vars[1];
      LedRArray[setLedIndex] = vars[2];
      LedGArray[setLedIndex] = vars[3];
      LedBArray[setLedIndex] = vars[4];
      sendMes(PSTR("accepted"));

    }
    else{
      sendMes(PSTR("invalid"));
    }
  }
  else{
    sendMes(PSTR("invalid"));
  }
}

void cmdLedIntensity(void){ //Sets LedIntensity 0 till 100 %
  int newLedIntensity = atoi(SCmd.next());

  if(newLedIntensity >=0 && newLedIntensity <= 100){ 
    sendMes(PSTR("accepted"));
    LedIntensity = newLedIntensity * 2,55;
  }
  else{
    sendMes(PSTR("invalid"));
  }
}

void cmdSmartCurrentMax(void) { //Set the (dynamical) maximum allowable current for smart charging
  int oldSmartCurrentMax = SmartCurrentMax;
  int newSmartCurrentMax = atoi(SCmd.next());

  if((newSmartCurrentMax >= AbsoluteMinCurrent) && (newSmartCurrentMax <= AbsoluteCurrentLimit) && (newSmartCurrentMax <= setInstallationCurrent)){ //Check value from 6 till 32 Amps
    SmartCurrentMax = newSmartCurrentMax;
    sendMes(PSTR("accepted"));
  }
  else{
    sendMes(PSTR("invalid"));
  }

}

//Functie cmdExternalSwitch komt hier //Control the state of potential free contacts

//////////////////////////////////////////////////////Metering functions list/////////////////////////////////////////////////////////

void fl2str(float fl, String str = "") {
  char buffer[11];
  dtostrf(fl, 2, 2, buffer);
  strMOD += buffer;
  strMOD += str;
}

void cmdEnergyActiveExport(void) { //Energy exported by EV (To-tal,L1,L2,L3) kwh
  strMOD = "";
  fl2str(IEEE(inputRegs70, 4));
  sendMes(PSTR("EnergyActiveExport="), strMOD);
}

void cmdEnergyActiveImport(void) { ///Energy imported by EV (To-tal,L1,L2,L3)
  strMOD = "";
  fl2str(IEEE(inputRegs70, 2));
  sendMes(PSTR("EnergyActiveImport="), strMOD);
}

void cmdEnergyReactiveExport(void) { //Reactive energy exported by EV (Total,L1,L2,L3)
  strMOD = "";
  fl2str(IEEE(inputRegs70, 8));
  sendMes(PSTR("EnergyReactiveExport="), strMOD);
}

void cmdEnergyReactiveImport(void) { //Reactive energy imported by EV (Total,L1,L2,L3)kwhr
  strMOD = "";
  fl2str(IEEE(inputRegs70, 6));
  sendMes(PSTR("EnergyReactiveImport="), strMOD);
}

//functie cmdPowerActiveExport komt hier //Instantaneous active power export-ed by EV (Total,L1,L2,L3) W
// deze waarde zit niet standaard in de Eastron, zelf berekenen timers complex?

void cmdPowerActiveImport(void) { //Instantaneous active power imported by EV (Total,L1,L2,L3) W
  strMOD = "";
  fl2str(IEEE(inputRegs52, 0), sep);
  fl2str(IEEE(inputRegs0, 12), sep);
  fl2str(IEEE(inputRegs0, 14), sep);
  fl2str(IEEE(inputRegs0, 16));
  sendMes(PSTR("PowerActiveImport="), strMOD);
}

//functie cmdPowerReactiveExport komt hier //Instantaneous reactive power exported by EV (Total,L1,L2,L3) VAr

void cmdPowerReactiveImport(void) { ///Instantaneous reactive power imported by EV (Total,L1,L2,L3) VAr
  strMOD = "";
  fl2str(IEEE(inputRegs60, 0), sep);
  fl2str(IEEE(inputRegs20, 4), sep);
  fl2str(IEEE(inputRegs20, 6), sep);
  fl2str(IEEE(inputRegs20, 8));
  sendMes(PSTR("PowerReactiveImport="), strMOD);
}

void cmdCurrentExport(void) { //Instantaneous current flow to EV (Total,L1,L2,L3)
  //Could alsow be 'DemandOnly' REG85, REG259, REG261, REG263*/ ?
  strMOD = "";
  fl2str(IEEE(inputRegs47, 1), sep);
  fl2str(IEEE(inputRegs0, 6), sep);
  fl2str(IEEE(inputRegs0, 8), sep);
  fl2str(IEEE(inputRegs0, 10));
  sendMes(PSTR("CurrentExport="), strMOD);
}

void cmdCurrentImport(void) { //Instantaneous current flow from EV (Total,L1,L2,L3)
  //  NOTICE! THIS FUNCTION REPLIES THE ACTUAL CURRENT FLOW, NOT THE FLOW 'FROM or TO' the EV
  //Could alsow be 'DemandOnly' REG85, REG259, REG261, REG263*/
  strMOD = "";
  fl2str(IEEE(inputRegs47, 1), sep);
  fl2str(IEEE(inputRegs0, 6), sep);
  fl2str(IEEE(inputRegs0, 8), sep);
  fl2str(IEEE(inputRegs0, 10));
  sendMes(PSTR("CurrentImport="), strMOD);
}

void cmdVoltage(void) { //AC RMS supply voltage (L1,L2,L3)
  strMOD = "";
  fl2str(IEEE(inputRegs0, 0), sep);
  fl2str(IEEE(inputRegs0, 2), sep);
  fl2str(IEEE(inputRegs20, 4));
  sendMes(PSTR("Voltage="), strMOD);
}

void cmdPowerFactor(void) {  //Power factor (Total, L1,L2,L3)
  strMOD = "";
  fl2str(IEEE(inputRegs60, 2), sep);
  fl2str(IEEE(inputRegs20, 10), sep);
  fl2str(IEEE(inputRegs20, 12), sep);
  fl2str(IEEE(inputRegs20, 14));
  sendMes(PSTR("PowerFactor="), strMOD);
}

void cmdPhaseAngle(void) {  //Phase angle (L1,L2,L3)
  strMOD = "";
  fl2str(IEEE(inputRegs66, 0), sep);
  fl2str(IEEE(inputRegs20, 16), sep);
  fl2str(IEEE(inputRegs20, 18), sep);
  fl2str(IEEE(inputRegs40, 0));
  sendMes(PSTR("PhaseAngle="), strMOD);
}

void cmdFrequency(void) { //Frequency of the supply voltage
  strMOD = "";
  fl2str(IEEE(inputRegs70, 0));
  sendMes(PSTR("Frequency="), strMOD);
}

void cmdSerialNumber(void) { //Frequency of the supply voltage
  strMOD = "";
  strMOD += holdRegs40[2];
  strMOD += holdRegs40[3];
  sendMes(PSTR("SerialNumber="), strMOD);
}

/*
void cmdTestModbus(void) {  //Power factor (Total, L1,L2,L3)
 Serial.println(F("Show Modbus Stats"));
 Serial.println(F("Packet1: Request, Successful, Failed, ExceptionErrors"));
 Serial.println(packet1->requests);
 Serial.println(packet1->successful_requests);
 Serial.println(packet1->failed_requests);
 Serial.println(packet1->exception_errors);
 Serial.println(F("Packet2: Request, Successful, Failed, ExceptionErrors"));
 Serial.println(packet2->requests);
 Serial.println(packet2->successful_requests);
 Serial.println(packet2->failed_requests);
 Serial.println(packet2->exception_errors);
 Serial.println(F("Packet3: Request, Successful, Failed, ExceptionErrors"));
 Serial.println(packet3->requests);
 Serial.println(packet3->successful_requests);
 Serial.println(packet3->failed_requests);
 Serial.println(packet3->exception_errors);
 //This Function is tested correctly  
 }*/

//////////////////////////////////////////////////////Settings function list//////////////////////////////////////////////////////////

boolean IsInitState(void){
  if(atmegastatus != StateInitialize){
    sendMes(PSTR("notsupported"));
    return false;
  }
  else{
    return true;
  }
}


void cmdsetLed(void) { //Sets the LED properties for a spe-cific operational mode
  //setLed=101,3,0,100,100
  if(IsInitState()){
    char *arg;  
    arg = SCmd.next();
    if (arg != NULL) {
      char * readString = arg;
      parseNumbers(arg, sep, 5, vars); //vars.h vars needs to be edited to more space in array?! in case of error, needs testing.

      if(vars[0]>=100 && vars[0]<400 && vars[1]>=0 && vars[1]<4 && vars[2]>=0 && vars[2]<=100 && vars[3]>=0 && vars[3]<=100 && vars[4]>=0 && vars[4]<=100){ //test the range
        setLedIndex = GetLedIndex(vars[0]);
        if(setLedIndex == 0){
          sendMes(PSTR("invalid"));
        }
        else{
          //setLedIndex = vars[0];
          LedFunctionArray[setLedIndex] = vars[1];
          LedRArray[setLedIndex] = vars[2];
          LedGArray[setLedIndex] = vars[3];
          LedBArray[setLedIndex] = vars[4];

          sendMes(PSTR("accepted"));
        }
      }
      else{
        sendMes(PSTR("invalid"));
      }
    }
    else{
      sendMes(PSTR("invalid"));
    }
  }
}

void cmdsetInstallation(void) {//Sets number of connected phases and maximum current per phase for installation
  if(IsInitState()){
    char *arg;  
    arg = SCmd.next();
    if (arg != NULL) {
      char * readString = arg;
      parseNumbers(arg, sep, 2, vars);

      if(vars[0] >= 1 && vars[0] <= 3 && vars[1] >= 6 && vars[1] <= 32){ 
        setInstallationPhases = vars[0];
        setInstallationCurrent = vars[1];
        sendMes(PSTR("accepted"));
      }
      else{
        sendMes(PSTR("invalid"));
      }
    }
    else{
      sendMes(PSTR("invalid"));
    }
  }
}

void cmdsetCurrentCheck(void) {//Sets the check margin for current checking
  if(IsInitState()){
    char *arg;  
    arg = SCmd.next();
    if (arg != NULL) {
      char * readString = arg;
      parseNumbers(arg, sep, 3, vars);

      if(vars[0] >= 0 && vars[0] <= 100 && vars[1] >= 0 && vars[1] <= 10000 && vars[2] >= 1 && vars[2] <= 10000){ 
        CurrentCheckMargin = vars[0];
        CurrentCheckGraceperiod = vars[1];
        CurrentCheckPeriod = vars[2];
        sendMes(PSTR("accepted"));
      }
      else{
        sendMes(PSTR("invalid"));
      }
    }
    else{
      sendMes(PSTR("invalid"));
    }
  }
}

void cmdsetVoltageCheck(void) {//Sets the expected phase voltage to neutral (RMS), check margin and the sample period
  if(IsInitState()){
    char *arg;  
    arg = SCmd.next();
    if (arg != NULL) {
      char * readString = arg;
      parseNumbers(arg, sep, 4, vars);

      if(vars[0]>100 && vars[1] != 0 && vars[2] != 0 && vars[3] > 0 && vars[3] <= 10000){ 
        VoltageRMS = vars[0];
        VoltageMarginHi = (VoltageRMS * (100+vars[1])) / 100;
        VoltageMarginLo = (VoltageRMS * (100-vars[2])) / 100;
        VoltageSampleperiode = vars[3];

        sendMes(PSTR("accepted"));
      }
      else{
        sendMes(PSTR("invalid"));
      }
    }
    else{
      sendMes(PSTR("invalid"));
    }
  }
}

void cmdsetHeartbeatCheck(void){//Parameters for heartbeat between PIC and Linux
  if(IsInitState()){
    char *arg;  
    arg = SCmd.next();
    if (arg != NULL) {
      char * readString = arg;
      parseNumbers(arg, ",", 2, vars);

      if(vars[0] >=10 && vars[0] <= 10000 && vars[1] >=0 && vars[1] <= 255){ 
        setHeartbeatCheckPeriod = vars[0];
        setHeartbeatCheckMisses = vars[1];
        sendMes(PSTR("accepted"));
      }
      else
        sendMes(PSTR("invalid"));
    }
    else
      sendMes(PSTR("invalid"));
  }
}

void cmdsetTemperatureLimit(void) {//Sets limits for the temperature check
  if(IsInitState()){
    char *arg;  
    arg = SCmd.next();
    if (arg != NULL) {
      char * readString = arg;
      parseNumbers(arg, sep, 4, vars);

      if(vars[0]>=-100 && vars[0]<=100 && vars[1]>=-100 && vars[1]<=100 && vars[2]>=-100 && vars[2]<=100 && vars[3]>=-100 && vars[3]<=100){ 
        setTemperatureLimitLimitLow = vars[0];
        setTemperatureLimitLimitHigh = vars[1];
        setTemperatureLimitWarnLow = vars[2];
        setTemperatureLimitWarnHigh = vars[3];

        sendMes(PSTR("accepted"));
      }
      else{
        sendMes(PSTR("invalid"));
      }
    }
    else{
      sendMes(PSTR("invalid"));
    }
  }
}


void cmdsetVentilationPermission(void){  //Enables ventilated charge sessions
  if(IsInitState()){
    char *arg;
    arg = SCmd.next();    // Get the next argument from the SerialCommand object buffer
    if (arg != NULL)      // As long as it existed, take it
    {
      if(strcmp(arg, "prohibited") == 0){
        sendMes(PSTR("accepted"));
        VentilationPermission = false;
      }
      else if(strcmp(arg, "allowed") == 0){
        sendMes(PSTR("accepted"));
        VentilationPermission = true;
      }
      else 
        sendMes(PSTR("invalid"));
    }
    else {
      sendMes(PSTR("invalid"));
    }
  }
}

void cmdsetTagScanPeriod(void) {//Sets the time period in which addi-tional scans for the same tag ID are rejected
  if(IsInitState()){
    int oldTagScanPeriod = TagScanPeriod;
    int newTagScanPeriod = atoi(SCmd.next());

    if(newTagScanPeriod >= 0 && newTagScanPeriod <= 10){ //Do we want an absolute minimum?
      newTagScanPeriod = newTagScanPeriod * 1000;
      TagScanPeriod = newTagScanPeriod;
      sendMes(PSTR("accepted"));
    }
    else{
      sendMes(PSTR("invalid"));
    }
  }
}


void cmdSetBuzzer(void){ //Activate buzzer
  if(IsInitState()){
    char *arg;  
    arg = SCmd.next();
    if (arg != NULL) {
      char * readString = arg;
      parseNumbers(arg, ",", 2, vars);

      if(vars[0] >=100 && vars[0] <= 20000 && vars[1] >=1 && vars[1] <= 10000){ 
        sendMes(PSTR("accepted"));
        buzzerfreq = vars[0];
        buzzerms = vars[1];
      }
      else
        sendMes(PSTR("invalid"));
    }
    else
      sendMes(PSTR("rejected"));
  }
}

void rejected() {
  sendMes(PSTR("rejected"));
}

void unrecognized() {
  sendMes(PSTR("notsupported"));
}

byte parseNumbers(char *string, const char *delim, byte maxNums, int *numArray) {
  byte count = 0;
  char *tokenPtr = strtok(string, delim);
  while(tokenPtr != NULL && count < maxNums)  {
    numArray[count] = atoi(tokenPtr);  // Convert token to number.
    count++; 
    tokenPtr = strtok(NULL, delim);
  }
  return count;
}

