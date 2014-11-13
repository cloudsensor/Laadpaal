void update_leds(void){ //update leds function is called every loop
  // NO DELAYS IN THIS FUNCTION ALLOWED

  strip.setBrightness(LedIntensity); //set LedIntensity

    if(LedFunctionArray[GetLedIndex(199)]){ //override 
    SetLedTo(199);
  }
  else if(UnsafeSituation){
    SetLedTo(102);
  }
  else if(LedBlinkCard){
    leds_off();
    SetLedTo(202); // flash green when tag detected
    if((millis() > tagblinkms + TagBlinkPeriod) || (millis() < tagblinkms - TagBlinkPeriod)) { 
      LedBlinkCard = false;
    }
  }
  else if(AtmegaState() == StatePending)
  {
    leds_off();
    if(RequestCharge){
      //here the card led normally turns green, according to LED document, this is done by linuxoverride. 
    }
    else{
      SetLedTo(201);
    }
    
    /*if(CableConnected()&& locked()=="LOCKED" ){
      SetLedTo(303);

      //SetLedTo(302); //to be programmed
      // SetLedTo(304); //cable invalid to be programmed
    }*/
    if(!RequestCharge && CableConnected()&& locked()=="UNLOCKED"){
      SetLedTo(303);
    }
    else if(RequestCharge && CableConnected()&& locked()=="UNLOCKED"){
      SetLedTo(305);
    }
    else{
      SetLedTo(301);

    }
  }
  else if(AtmegaState() == StateSession){ //StateSession //RequestCharge && CableConnected()
    if(ActuallyCharging){
      SetLedTo(105);
    }
    else{
      SetLedTo(104); //session idle
    }
  }
  else if(AtmegaState() == StateReserved){
    SetLedTo(103);
  }
  else if(AtmegaState() == StateIdle){
    SetLedTo(101);
  }
  else{
    leds_off();
  }
  if(LedFunctionArray[GetLedIndex(299)]){ //override 
    SetLedTo(299);
  }
  if(LedFunctionArray[GetLedIndex(399)]){ //override 
    SetLedTo(399);
  }
  strip.show(); //Output the new settings to the leds!
}  

void SetLedTo(int mode){ //setLed = mode, function, R, G, B

  int TheLedIndex = GetLedIndex(mode);
  int function = LedFunctionArray[TheLedIndex];
  int r = LedRArray[TheLedIndex] * 2.55;
  int g = LedGArray[TheLedIndex] * 2.55;
  int b = LedBArray[TheLedIndex] * 2.55;
  uint32_t TheColor = strip.Color(r, g, b);
  int vtc = 0; 
  if(mode >= 200 && mode < 300){
    vtc = 1;
  }
  else if(mode >= 300 && mode < 400){
    vtc = 2;
  }
  if(function == 3){
    led_breath(r,g,b,vtc);
  }
  else if(function == 2){
    led_blink(TheColor,vtc);
  }
  else if(function == 1){
    led_color(TheColor,vtc);
  }
}
int GetLedIndex(int mode){
  int TheLedIndex = 0;
  for (int i=0; i < 22; i++){
    if(LedIndex[i]==mode){
      TheLedIndex = i;
    }
  }
  return TheLedIndex;
}

void leds_off(void) {
  for(int i = 0; i < NeoPixelpix; i++)
    strip.setPixelColor(i, 000000);
}


void led_color(uint32_t color, int area) {

  if(area == 0) {
    for(int i = 0; i < NeoPixelpix; i++)
      strip.setPixelColor(i, color);
  }
  if(area == 1) {
    for(int i = NeoPixelcard; i < NeoPixelcable; i++) {
      strip.setPixelColor(i, color);
      strip.setPixelColor(NeoPixelpix -1 - i, color);
    }
  }
  if(area == 2) {
    for(int i = NeoPixelcable; i < NeoPixelpix - NeoPixelcable; i++)
      strip.setPixelColor(i, color);
  }

}

void led_breath(int r, int g, int b, int area) {
  float val = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
  r = val/255*r;
  g = val/255*g;
  b = val/255*b;
  if(area == 0) {
    for(int i = 0; i < NeoPixelpix; i++)
      strip.setPixelColor(i, r, g, b);
  }
  if(area == 1) {
    for(int i = NeoPixelcard; i < NeoPixelcable; i++) {
      strip.setPixelColor(i, r, g, b);
      strip.setPixelColor(NeoPixelpix -1 - i, r, g, b);
    }
  }
  if(area == 2) {
    for(int i = NeoPixelcable; i < NeoPixelpix - NeoPixelcable; i++)
      strip.setPixelColor(i, r, g, b);
  }

}

void led_blink(uint32_t color, int area) { 
  int duration = 500;
  if(millis()%(2*duration) < duration){
    led_color(color, area);
  }
  else{
    led_color(ledsoff, area);
  }

}

void all_flash(uint32_t color, unsigned long duration) {
  if(millis()%(2*duration) < duration)
    for(int i = 0; i < NeoPixelpix; i++)
      strip.setPixelColor(i, color); 
  else
    for(int i = 0; i < NeoPixelpix; i++)
      strip.setPixelColor(i, 000000);
}














