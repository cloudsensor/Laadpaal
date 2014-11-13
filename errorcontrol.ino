void ErrorControl(void){
  /**********************************Set and Reset the control center booleans for new testcycle******************************/
  //  booleans for problemsituations go here
  boolean PowerFail = false;
  boolean OverTemperature = false;
  boolean UnderTemperature = false;
  boolean IntrusionDetected = false;   
  boolean UnderVoltage = false;
  boolean OverVoltage = false;
  //boolean OutFrequencyRange = false;
  boolean NewErrorFlag = false;
  boolean OverCurrent = false;
  boolean TiltDetected = false;
  boolean LineTestCheckError = false;
  boolean MeterFailure = false;

  //Actions to take go here
  boolean DoShutDown = false;
  
  //Check all the importend values en deside if there is a problem

  if(analogRead(powerLostPin) < 345){
    PowerFail = true;
  }

  if(temp > setTemperatureLimitLimitHigh){ //check temp
    OverTemperature = true;
  }
  
  if(temp < setTemperatureLimitLimitLow){ //check temp
    UnderTemperature = true;
  }
  
  if(MainsConnected){ //prevend sending errors on startup, 
    if(VoltageL1 >= VoltageMarginHi){
      measuredVoltage = VoltageL1;
      OverVoltage = true;
    }
    if(VoltageL2 >= VoltageMarginHi){
      measuredVoltage = VoltageL2;
      OverVoltage = true;
    }
    if(VoltageL3 >= VoltageMarginHi){
      measuredVoltage = VoltageL3;
      OverVoltage = true;
    }

    if(VoltageL1 <= VoltageMarginLo && VoltageL1 != 0){
      measuredVoltage = VoltageL1;
      UnderVoltage = true;
    }
    if(VoltageL2 <= VoltageMarginLo && VoltageL2 != 0){
      measuredVoltage = VoltageL2;
      UnderVoltage = true;
    }
    if(VoltageL3 <= VoltageMarginLo && VoltageL3 != 0){
      measuredVoltage = VoltageL3;
      UnderVoltage = true;
    }
  }

  if(analogRead(ldrPin) > ldrth){
    IntrusionDetected = true;
  }

  //current check
  if(max(CurrentL1,(max(CurrentL2,CurrentL3))) > MaxCurrent){
    OverCurrent = true;
  }
  //Tilt check
  if(tilt>0){
    TiltDetected = true;
    tilt = 0;
  }

  //Line test check
  //monitor phase lines. if the phase lines are diffrent that expected (compare to actual state of relays
  // ErrorReport=relayFailure,det=closed,exp=open
  linestate = linetest();
  if(linestate ==0 && relaystate==1){ //relays vs linestate?
    LineTestCheckError=true;
  }
  else if(linestate==1 && relaystate==0){
    LineTestCheckError=true;
  }

  if(packet1->requests > 0){
    if(packet1->failed_requests >= 1 && packet2->failed_requests >= 1 && packet3->failed_requests >= 1){ 
      MeterFailure = true;

      /* //error
       Serial.println(F("Show Modbus Stats"));
       Serial.println(F("Packet1: Request, Successful, Failed, ExceptionErrors"));
       Serial.println(packet1->requests);
       Serial.println(packet1->successful_requests);
       Serial.println(packet1->failed_requests);
       Serial.println(packet1->exception_errors);
       */
    }
  }

  //****The 'rules' go here, set ErrorFlags, one time actions****************************************
  //if a problem is detected above, in this section we deside the one time action and set the actual ErrorFlags
  //And in this section the Error messages are defined

  if(PowerFail){
    if(!ErrorFlagPowerFail){
      ErrorFlagPowerFail = true;
      sendMes(PSTR("ErrorReport=PowerFail")); //“ErrorReport=overtemperatureFailure,det=60.2,lim=60.0(cs)”
      //No further action on PowerFail
    } 
  }
  else{
    if(ErrorFlagPowerFail){
      ErrorFlagPowerFail = false;

    }
  }

  if(OverTemperature){
    if(!ErrorFlagOverHeat){
      ErrorFlagOverHeat = true;
      //Serial.println(F("error - TemperatureLimit exceeded"));
      strMOD = ",det=";
      strMOD += temp;
      strMOD += ",lim=";
      strMOD += setTemperatureLimitLimitHigh;
      
      sendMes(PSTR("ErrorReport=overtemperatureFailure"), strMOD); //“ErrorReport=overtemperatureFailure,det=60.2,lim=60.0(cs)”
      //“ErrorReport=undertemperatureFailure,det=-40.1,lim=-40.0(cs)”
      beep(250);
      //No further action on overheat
    } 
  }
  else{
    if(ErrorFlagOverHeat){
      ErrorFlagOverHeat = false;
      //sendMes(PSTR("warning - ErrorFlagOverHeat = off")); //Send a one time message if situation returns to normal
    }
  }
  
  if(UnderTemperature){
    if(!ErrorFlagUnderHeat){
      ErrorFlagUnderHeat = true;
      //Serial.println(F("error - TemperatureLimit exceeded"));
      strMOD = ",det=";
      strMOD += temp;
      strMOD += ",lim=";
      strMOD += setTemperatureLimitLimitLow;
      
      sendMes(PSTR("ErrorReport=undertemperatureFailure"), strMOD); //“ErrorReport=undertemperatureFailure,det=-40.1,lim=-40.0(cs)”
      beep(250);
    } 
  }
  else{
    if(ErrorFlagUnderHeat){
      ErrorFlagUnderHeat = false;
      //sendMes(PSTR("warning - ErrorFlagOverHeat = off")); //Send a one time message if situation returns to normal
    }
  }

  if(IntrusionDetected){
    if(!ErrorFlagIntrusionDetected){
      ErrorFlagIntrusionDetected = true;
      sendMes(PSTR("ErrorReport=intrusiondetect")); 
      //No further action on IntrusionDetected
    } 
  }
  else{
    if(ErrorFlagIntrusionDetected){
      ErrorFlagIntrusionDetected = false;
      //sendMes(PSTR("warning - ErrorFlagIntrusionDetected = off")); //Send a one time message if situation returns to normal
    }
  }

  if(OverVoltage){
    if(!ErrorFlagOutOverVoltage){
      ErrorFlagOutOverVoltage = true;
      strMOD = ",det=";
      strMOD += measuredVoltage;
      strMOD += ",lim=";
      strMOD += VoltageRMS;
      strMOD += "+";
      strMOD += VoltagePercHi;
      strMOD += "%";
      
      sendMes(PSTR("ErrorReport=overvoltage"), strMOD);  //“ErrorReport=undervoltage,det=150,lim=230-10%(cs)” //“ErrorReport=overvoltage,det=300,lim=230+6%(cs)”
      beep(250);
    } 
  }
  else{
    if(ErrorFlagOutOverVoltage){
      ErrorFlagOutOverVoltage = false;
    }
  }

  if(UnderVoltage){
    if(!ErrorFlagOutUnderVoltage){
      ErrorFlagOutUnderVoltage = true;
      strMOD = ",det=";
      strMOD += measuredVoltage;
      strMOD += ",lim=";
      strMOD += VoltageRMS;
      strMOD += "-";
      strMOD += VoltagePercHi;
      strMOD += "%";
      
      sendMes(PSTR("ErrorReport=undervoltage"), strMOD);  //“ErrorReport=undervoltage,det=150,lim=230-10%(cs)” //“ErrorReport=overvoltage,det=300,lim=230+6%(cs)”
      beep(250);
    } 
  }
  else{
    if(ErrorFlagOutUnderVoltage){
      ErrorFlagOutUnderVoltage = false;
    }
  }

  if(OverCurrent){
    if(!ErrorFlagOverCurrent){
      ErrorFlagOverCurrent = true;
      
      strMOD = ",meas=";
      strMOD += max(CurrentL1,(max(CurrentL2,CurrentL3)));
      strMOD += ",lim=";
      strMOD += MaxCurrent;
      sendMes(PSTR("ErrorReport=overcurrent"), strMOD); //“ErrorReport=overcurrent,meas=16.0,lim=10.0(cs)”;
      beep(250);
      //No further one time action on out of voltage range

    } 
  }
  else{
    if(ErrorFlagOverCurrent){
      ErrorFlagOverCurrent = false;
      //sendMes(PSTR("warning - ErrorFlagOverCurrent = off")); //Send a one time message if situation returns to normal
    }
  }


  if(TiltDetected){
    if(!ErrorFlagTilt){
      ErrorFlagTilt = true;
      sendMes(PSTR("ErrorReport=tiltdetect")); //“ErrorReport=tiltdetect(cs)”
      //beep(250);
      //No further one time action on out of voltage range

    } 
  }
  else{
    if(ErrorFlagTilt){
      ErrorFlagTilt = false;
      //sendMes(PSTR("warning - ErrorFlagTilt = off")); //Send a one time message if situation returns to normal
    }
  }

  if(LineTestCheckError){
    if(!ErrorFlagLineTestCheck){
      ErrorFlagLineTestCheck=true;
      if(linestate ==0 && relaystate==1){ //relays vs linestate?
        sendMes(PSTR("ErrorReport=relayFailure,det=closed,exp=open")); //ErrorReport=relayFailure,det=closed,exp=open

      }
      else if(linestate==1 && relaystate==0){
        sendMes(PSTR("ErrorReport=relayFailure,det=open,exp=closed")); //ErrorReport=relayFailure,det=closed,exp=open
      }
      else{
        sendMes(PSTR("ErrorReport=relayFailure"));
      }
    }
  }
  else{
    if(ErrorFlagLineTestCheck){
      ErrorFlagLineTestCheck = false;
    }
  }

  if(MeterFailure){  //“ErrorReport=meterFailure,internal(cs)"
    if(!ErrorFlagMeterFailure){
      ErrorFlagMeterFailure=true;
      sendMes(PSTR("ErrorReport=meterFailure,internal")); 
    }
  }
  else{
    if(ErrorFlagMeterFailure){
      ErrorFlagMeterFailure = false;
    }
  }

  //“ErrorReport=Mode3Error(cs)”
  //“ErrorReport=peFailure,ev(cs)”
  //“ErrorReport=connectorlockFailure,det=locked,exp=unlocked(cs)”

  //“ErrorReport=readerFailure(cs)”

  /********************* Excecute the final decisions on flagstatus ******************************/
  //In this section we deside what the 'long term' action is for a set errorflag.
  UnsafeSituation = false;   //  Reset decision
  // The set booleans in this section 'control' the rest of the programm
  if(ErrorFlagPowerFail){
    UnsafeSituation = true;
  }
  if(ErrorFlagOverHeat){
    UnsafeSituation = true;
  }
  if(ErrorFlagUnderHeat){
    UnsafeSituation = true;
  }
  if(ErrorFlagOutOverVoltage){
    UnsafeSituation = true;
  }
  if(ErrorFlagOutUnderVoltage){
    UnsafeSituation = true;
  }
  if(ErrorFlagOverCurrent){
    UnsafeSituation = true;
  }
  if(ErrorFlagIntrusionDetected){
    UnsafeSituation = true;
  }
  if(ErrorFlagTilt){
    UnsafeSituation = true;
  }
  if(ErrorFlagMainsACDown){
    //UnsafeSituation = true;
  }
  if(ErrorFlagInterfaceError){
    //UnsafeSituation = true;
  }
  if(ErrorFlagLineTestCheck){
    UnsafeSituation = true;
  }
  if(ErrorFlagMeterFailure){
    UnsafeSituation = true;
  }
}












