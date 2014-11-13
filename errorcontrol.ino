void ErrorControl(void){
  /**********************************Set and Reset the control center booleans for new testcycle******************************/
  //  booleans for problemsituations go here
  boolean PowerFail = false;
  boolean OverTemperature = false;
  boolean UnderTemperature = false;
  boolean IntrusionDetected = false;   
  boolean OutVoltageRange = false;
  //boolean OutFrequencyRange = false;
  boolean NewErrorFlag = false;
  boolean OverCurrent = false;
  boolean TiltDetected = false;
  boolean LineTestCheckError = false;

  //Actions to take go here
  boolean DoShutDown = false;




  //Check all the importend values en deside if there is a problem

  if(analogRead(powerLostPin) < 345){
    PowerFail = true;
  }

  if(temp > setTemperatureLimitLimitHigh){ //check temp
    OverTemperature = true;

  }
  if(MainsConnected){ //prevend sending errors on startup, 
    if(VoltageL1 >= VoltageMarginHi || VoltageL1 <= VoltageMarginLo && VoltageL1 != 0){
      OutVoltageRange = true;
    }
    if(VoltageL2 >= VoltageMarginHi || VoltageL2 <= VoltageMarginLo && VoltageL2 != 0){
      OutVoltageRange = true;
    }
    if(VoltageL3 >= VoltageMarginHi || VoltageL3 <= VoltageMarginLo && VoltageL3 != 0){
      OutVoltageRange = true;
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
      sendMes(PSTR("ErrorReport=overtemperatureFailure")); //“ErrorReport=overtemperatureFailure,det=60.2,lim=60.0(cs)”
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
  if(OutVoltageRange){
    if(!ErrorFlagOutVoltageRange){
      ErrorFlagOutVoltageRange = true;
      sendMes(PSTR("ErrorReport=Voltageoutofrange"));  //“ErrorReport=undervoltage,det=150,lim=230-10%(cs)” //“ErrorReport=overvoltage,det=300,lim=230+6%(cs)”
      beep(250);
      //No further one time action on out of voltage range

    } 
  }
  else{
    if(ErrorFlagOutVoltageRange){
      ErrorFlagOutVoltageRange = false;

      //sendMes(PSTR("warning - ErrorFlagOutVoltageRange = off")); //Send a one time message if situation returns to normal
    }
  }

  if(OverCurrent){
    if(!ErrorFlagOverCurrent){
      ErrorFlagOverCurrent = true;
      sendMes(PSTR("ErrorReport=overcurrent")); //“ErrorReport=overcurrent,meas=16.0,lim=10.0(cs)”
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
     

      // ErrorReport=relayFailure,det=closed,exp=open

      if(linestate ==0 && relaystate==1){ //relays vs linestate?
        sendMes(PSTR("ErrorReport=relayFailure,det=closed,exp=open")); //ErrorReport=relayFailure,det=closed,exp=open

      }
      else if(linestate==1 && relaystate==0){
       sendMes(PSTR("ErrorReport=relayFailure,det=open,exp=closed")); //ErrorReport=relayFailure,det=closed,exp=open
      }



    }

  }
  else{
    if(ErrorFlagLineTestCheck){
      ErrorFlagLineTestCheck = false;
    }
  }

  //“ErrorReport=relayFailure,det=closed,exp=open(cs)”
  //“ErrorReport=Mode3Error(cs)”
  //“ErrorReport=peFailure,ev(cs)”
  //“ErrorReport=connectorlockFailure,det=locked,exp=unlocked(cs)”
  //“ErrorReport=meterFailure,internal(cs)”
  //“ErrorReport=readerFailure(cs)”

  /********************* Excecute the final decisions on flagstatus ******************************/
  //In this section we deside what the 'long term' action is for a set errorflag.
  // The set booleans in this section 'control' the rest of the programm


  //  Reset decisions
  UnsafeSituation = false;

  if(ErrorFlagPowerFail){
    UnsafeSituation = true;
  }
  if(ErrorFlagOverHeat){
    UnsafeSituation = true;
  }
  if(ErrorFlagOutVoltageRange){
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






  // The set booleans above control the rest of the programm

}







