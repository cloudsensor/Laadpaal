void ChargeControl(void){

  //THE REAL TIME CHARGING CONTROL  
  if(RequestCharge){ //We are ordered to start the chargingsession
    if(UnsafeSituation){ //(could be a function UpdateAtmegaStatus(); And then check Flags and modes, (and add if change send message))
      AtmegaState(StateError);
    }

    ActuallyCharging = false; 

    if(CableConnected()){ //If car connected return true
      if(!chargemodeActive){
        if(TryLockCable()){ //if safe -> lock cable, if locked -> return true
          chargemodeActive = true;
        } //else cablelock failed or error?
        else{ //Lock cable faild, lock stuck or cable not pushed in correctly, UnsafeSituation, no dataconnection with car
          //possible action would be to flash cable leds red?
          //Count attempeds and give error on Serial to linux.
          chargemodeActive = false;
        }
      }
      else{ //ChargeModeActive is true
        if(VehicleStatusChanged){
          VehicleStatusChanged = false;
          if(VehicleStatus == 'A' || (VehicleStatus == 'D' && VentilationPermission == false)) {
            analogWrite(pilotPin, 255);
          }
          if(VehicleStatus == 'B' || VehicleStatus == 'C' || (VehicleStatus == 'D' && VentilationPermission == true)) {
            setPWM(MaxCurrent);
          }
        }
        if(VehicleStatus == 'C' || (VehicleStatus == 'D' && VentilationPermission == true)){
          if(!UnsafeSituation){
            if(relaystate == 0){
              relayOn();
              ActuallyCharging = true;
              AtmegaState(StateSession);
            }
            else{
              //We are actually charging!
              ActuallyCharging = true;
              AtmegaState(StateSession);
            }
          }
          else{ // we got in an unsafe situation while charging
            if(ActuallyCharging){ //Stops charging but dous not end session, (restart when session is safe again)
              analogWrite(pilotPin, 255);
              //setPWM(0); Needs to be tested. Is this needed?!
              delay(10); //
              relayOff();
            }
          }
        }
        else if((VehicleStatus == 'A' || VehicleStatus == 'B') && relaystate == 1){ // Close relays
          relayOff();
          AtmegaState(StateSession);
        }
        else if(relaystate == 1){ //this case should not happed in normal situation?
          //Needs testing, wat happens if the car switches to D or E or F ?
          relayOff();
          AtmegaState(StateError);
        }
        else{ //Session not started becouse vehicle seems not ready?
          AtmegaState(StateSession);
        }
        //end ChargeModeActive
      }
    }
    else{
      AtmegaState(StatePending);
      //There is a start request but there is no car connected
      //probably waiting for user to connect car, possible action would be for example to flash cable leds yellow (controlled in led_functions)
      if(relaystate == 1){ //this case should not happed in normal situation?
        //Needs testing, wat happens if the car switches to D or E or F ?
        analogWrite(pilotPin, 255);
        //setPWM(0); Needs to be tested. Is this needed?!
        delay(200);
        relayOff();
        //keep cable locked!
      }
    }
  }
  else{ //there is no charge request
    if(chargemodeActive || ActuallyCharging || linetest()){ //This is the normal moment of safe shutdown
      analogWrite(pilotPin, 255);//the car stops charging itselfe before relays is opened
      delay(10); 
      relayOff();
      ActuallyCharging = false;
      chargemodeActive = false;
      SafeUnlockCable(); //This should be active here according to spec item unlock 5.2.2.3 

    }
    else{ //Really IDLE
      if(UnsafeSituation){ //(could be a function UpdateAtmegaStatus(); And then check Flags and modes, (and add if change send message))
        AtmegaState(StateError);
      }
      else if(CableConnected()){
        AtmegaState(StatePending);
      }
      else if(AtmegaState() != StateReserved){
        AtmegaState(StateIdle);
      } 
      else{
        //AtmegaState(StateReserved); 
      }
    }
  }
}
