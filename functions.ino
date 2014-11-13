void PowerFailCheck(void){
  if(analogRead(powerLostPin) < 345) { // Default is 365 
    powerOK = false;     
    if(powerOK == true){  // send only once
      sendMes(PSTR("ErrorReport=powerFail"));  
      //Do safe shutdown etc..? Unlock Cable?
    }
  }
  else {
    powerOK = true;
  }
}

void AtmegaState(int NewAtmegaState){
  int oldatmegastatus = atmegastatus;
  atmegastatus = NewAtmegaState;
  if(atmegastatus != oldatmegastatus && atmegastatus != StateInitialize){
    addQueue(PSTR("State"), GetState());
  }
}

int AtmegaState(void){
  return atmegastatus;
}

void initBoard(void) {
  //strMOD.reserve(50);
  uint8_t tmp = 1<<JTD; // Disable JTAG  
  MCUCR = tmp; // Disable JTAG
  MCUCR = tmp; // Disable JTAG  
  pinMode(LinuxResetPin, INPUT);
  pinMode(hwVersionPin, INPUT);

  pinMode(pilotPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  relayOff();

  pinMode(lockA, OUTPUT);
  pinMode(lockB, OUTPUT);
  solenoid("UNLOCK"); 
  solenoid("OFF"); 

  pinMode(ldrPin, INPUT);
  digitalWrite(ldrPin, HIGH);  

  pinMode(PINTILT, INPUT); 
  digitalWrite(PINTILT, HIGH);
  PCintPort::attachInterrupt(PINTILT, &pintiltfunc, CHANGE);  //interrupt

  SPI.begin();
  mfrc522.PCD_Init();  

  Timer1.initialize(1000);
  delay(150);
  analogWrite(pilotPin, 255);
  hwVersion = analogRead(hwVersionPin)/hwDevide;
  
  addQueue(PSTR("Version"));
}

void initNeoPixel(void){ //Setup NeoPixel
  strip.begin();
  led_color(red, 0);
  strip.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
  strip3.begin();
  strip3.show(); // Initialize all pixels to 'off'
}

void HoldUntilStateChage(void ){
  while(atmegastatus == StateInitialize) { //wait for settings and state change from Linux application
    SCmd.readSerial(ms2);
    updateQueue();
    PowerFailCheck();
    delay(10);
  }
}

void Functions(void){
  if(delayloop1 > 25){ //The functions that execute every x loops
    modbus_update();
    readID();
    readProx();
    readPilot();
    UpdateVehicleStatus();
    CheckKwHValues();

    delayloop1 = 0;
  }
  else
    delayloop1++;

  //the non fast functions**************************************************
  if(delayloop2 > 100){ ////The functions that execute every x loops
    checkfreeRam();
    readTemp();
    readLdr();

    if((millis() - ms2 > 5000) || (millis() < ms2 && ms2 > 5000)) { //old -> millis() - ms2 > 5000
      sendMes(PSTR("ErrorReport=Timeout")); 
      ms2 = millis();
    }
    delayloop2 = 0;
  }
  else
    delayloop2++;
}

void UpdateVehicleStatus(void){ //“A” – Not connected, “B” – EV connected (ready), “C” – EV Charge, “D” – EV Charge – ventilation, “E” – Error, “F” – Unknown/Error "U" - (Unknown)not defined yet after boot 
  if(CableConnected()){
    if(phigh > 900)
      VehicleStatus = 'A';
    if(phigh > 800 && phigh < 900)
      VehicleStatus = 'B';
    if(phigh > 700 && phigh < 800)
      VehicleStatus = 'C';
    if(phigh > 600 && phigh < 700)
      VehicleStatus = 'D';
    if(phigh < 100)
      VehicleStatus = 'E';
    if(plow > 200 && plow < 600)
      VehicleStatus = 'Q';    // Diode check NOT ok
  }
  else if(CableConnected()==0){
    VehicleStatus = 'A';// not connected

  }
  else{ //Should never happen?
    VehicleStatus = 'F';
  }
  if(VehicleStatus != oldVehicleStatus){
    String str = "";
    str += VehicleStatus;
    addQueue(PSTR("VehicleStatus="), str);
    oldVehicleStatus=VehicleStatus;
    VehicleStatusChanged = true;
  }
}

void pintiltfunc() { //Tilt interrupt function
  tilt++;
}

void CheckKwHValues(void){
  //how to check if the connection is alive? At dead connection old 'outdated' values stay in the inputRegs

  frequency = IEEE(inputRegs70, 0);
  VoltageL1 = IEEE(inputRegs0, 0);
  VoltageL2 = IEEE(inputRegs0, 2);
  VoltageL3 = IEEE(inputRegs0, 4);

  CurrentTotal = IEEE(inputRegs47, 1); //REG49
  CurrentL1 = IEEE(inputRegs0, 6); //REG7
  CurrentL2 = IEEE(inputRegs0, 8); //REG9
  CurrentL3 = IEEE(inputRegs0, 10); //REG11

  //KWHser = IEEE(holdingRegs, 10);

  MainsConnected = false;
  if(frequency>=40 && frequency <= 70){
    if(VoltageL1 >= 100 || VoltageL2 >= 100 || VoltageL3 >= 100){
      MainsConnected = 1;
    }
  }
}

void readID(void) {
  String ScannedIDTag;
  ScannedIDTag.reserve(11);

  if(!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if(!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    ScannedIDTag +=(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    ScannedIDTag += String(mfrc522.uid.uidByte[i], HEX);
  }

  addQueue(PSTR("Tag="), ScannedIDTag);
  beep(buzzerms);
  LedBlinkCard = true;
  tagblinkms = millis();
  mfrc522.PICC_HaltA();
}

boolean CableConnected(void) {
  if(proxcablecurrent != 0)
    return 1;
  return 0;
}

void StopCharge(void){
  analogWrite(pilotPin, 255);
  //delay(200); 
  relayOff();
  chargemodeActive = false;
}

boolean TryLockCable(void){
  if(CableConnected()){ //Als cabel gevonden en geschikt
    if(VehicleStatus == 'B' || VehicleStatus == 'C' || VehicleStatus == 'D') {
      if(!UnsafeSituation){
        if((millis() - msLock > 2500 || (millis() < msLock && msLock > 2500))) { // //millis() - msLock > 2500
          if(DoLock())
            return true;
          msLock = millis();
        }
      }
    }
  }
  return false;
}

boolean DoLock(void){
  delay(10);
  solenoid("LOCK");
  delay(100);
  if(locked() == "LOCKED"){
    addQueue(PSTR("LockStatus=locked"));
    return true; 
  }
  else
    return false;
}

boolean SafeUnlockCable(void){
  if(linetest()==0){
    delay(10);
    solenoid("UNLOCK"); 
    delay(200);
    if(locked() == "UNLOCKED"){
      addQueue(PSTR("LockStatus=unlocked"));
      return true; 
    }
  }
  return false;
}

void readProx(void) {
  proximity();
  MaxCurrent = min(SmartCurrentMax, min(proxcablecurrent, setInstallationCurrent));
}


void proximity(void) { //Proximity Detection - Prevents movement of the car while connected to the charger.
  unsigned long prox = 0;
  for(int i = 0; i < 20; i++)
    prox += analogRead(proxPin);
  prox = prox/20;
  //Serial.println(prox);
  if(prox > 800)
    proxcablecurrent = 0;
  if(prox > 600 && prox < 800)
    proxcablecurrent = 13;
  else if(prox > 394 && prox < 450)
    proxcablecurrent = 20;
  else if(prox > 175 && prox < 193)
    proxcablecurrent = 32;
  else if(prox > 88 && prox < 98)
    proxcablecurrent = 63;
}

String locked(void) { // this function is used in cmdLockStatus
  int lockval = analogRead(lockStatePin);
  if(lockval > 100)
    return("UNLOCKED");
  return("LOCKED");
}

void solenoid(String state) {
  int ms = 300;
  delay(500);
  if(state == "OFF") {
    digitalWrite(lockA, HIGH);
    digitalWrite(lockB, HIGH);
  }
  if(state == "LOCK") {
    digitalWrite(lockA, HIGH);
    digitalWrite(lockB, LOW);
    delay(ms);
    digitalWrite(lockA, HIGH);
    digitalWrite(lockB, HIGH);
  }
  if(state == "UNLOCK") {
    digitalWrite(lockA, LOW);
    digitalWrite(lockB, HIGH);
    delay(ms);
    digitalWrite(lockA, HIGH);
    digitalWrite(lockB, HIGH);
  }
}

void relayOn() {
  addQueue(PSTR("RelayStatus=close"));
  digitalWrite(relayPin, HIGH);
  delay(100);
  linestate = linetest();

  relaystate = 1;
}

void relayOff() {
  digitalWrite(relayPin, LOW);
  if (atmegastatus != StateInitialize){
    addQueue(PSTR("RelayStatus=open"));
  }
  delay(100);
  linestate = linetest();
  relaystate = 0;
}

void readPilot(void) { //Control Pilot - Communication line used to coordinate charging level between the car and the charger as well as other information.
  int reading;
  plow = 1023;
  phigh = 0;

  for (int i = 0; i < 100; i++) {
    reading = analogRead(measPin);  // measures pilot voltage
    if (reading > phigh) {
      phigh = reading;
    }
    if (reading < plow) {
      plow = reading;
    }
  }

}


void setPWM(int current) {
  int dc = current*10/6;
  Timer1.pwm(pilotPin, dc*10.24);
}

void readTemp(void) {
  float tempfl;
  tempfl = analogRead(tempPin)*5/1024.0;
  tempfl = tempfl - 0.5;
  temp = tempfl / 0.01;

  //  if(temp != oldtemp){
  //    String str = "";
  //    str += temp;
  //    sendMes(PSTR("TemperatureStatus="), str);
  //    oldtemp=temp;
  //
  //  }
}

void readLdr(void){
  if(analogRead(ldrPin) < ldrth)
    light = true; // or closed, to be tested
  else
    light = false;
}

void beep(int delayms){
  analogWrite(buzzerPin, 20);
  delay(delayms); //holds entire code on beep
  analogWrite(buzzerPin, 0);  
}  

boolean linetest(void) {
  int line = 0;
  int reading;
  for (int i = 0; i < linecount; i++) {
    reading = analogRead(linePin);  // measures pilot voltage
    if (reading > line) {
      line = reading;
      delay(1);
    }
  }
  if(line > linethreshold)
    return 0;
  return 1;
}

typedef union {
  byte array[4];
  float value;
} 
ByteToFloat;

float IEEE(unsigned int reg[], int pos) {
  byte array[4];
  array[0] = (byte)((reg[pos] >> 8) & 0xff);
  array[1] = (byte)((reg[pos] >> 0) & 0xff);
  array[2] = (byte)((reg[pos+1] >> 8) & 0xff);
  array[3] = (byte)((reg[pos+1] >> 0) & 0xff);

  ByteToFloat converter;
  for (byte i = 0; i < 4; i++){
    converter.array[3-i] = array[i]; //or converter.array[i] = data[i]; depending on endianness 
  }
  return converter.value;
}

void linuxReset(int ms) { // Carambola (3V3) is direct aan ATMEGA (5V) aangesloten; input te maken is ie hoogohmig, door output te maken  is ie active low
  digitalWrite(LinuxResetPin, LOW);
  pinMode(LinuxResetPin, OUTPUT);
  delay(ms);
  pinMode(LinuxResetPin, INPUT);
}

String GetState(void){
  strMOD = "=";
  int i =0;
  strMOD += atmegastatus;
  strMOD += sep;      
  if(ErrorFlagOutVoltageRange){
    i += 1;
  }
  if(ErrorFlagOverCurrent){
    i += 2;
  }
  if(ErrorFlagOverHeat){
    i += 4;
  }
  if(ErrorFlagIntrusionDetected){
    i += 8;
  }
  if(ErrorFlagTilt){
    i += 16;
  }
  if(ErrorFlagMainsACDown){
    i += 32;
  }
  if(ErrorFlagInterfaceError){
    i += 64;
  }
  strMOD += i;
  return strMOD;
}

//this is a regular used function, please leave between // in code.
void checkfreeRam(){ 
  int RAM = freeRam();
  if(RAM < RAMWarning){
    if(oldfreeRam != RAM){
      String str = "=";
      str += RAM;
      addQueue(PSTR("Warning - RAM left: "), str);
      // Serial.print(F("=> Warning! RAM left: "));
      //Serial.println(RAM);
      oldfreeRam = RAM;
    }
  }
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
































