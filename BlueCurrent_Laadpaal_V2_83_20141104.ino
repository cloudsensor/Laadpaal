#include "TimerOne.h"
#include <SPI.h>
#include <MFRC522.h>
#include <SerialCommandFCS2.h>
#include <Adafruit_NeoPixel.h> 
#include <PinChangeInt.h> 
#include <SimpleModbusMaster.h>
#include "vars.h"
#include "settings.h"

#define baud 9600 ////Pre-setup MODBUS

enum {
  PACKET1,
  PACKET2,
  PACKET3,
  PACKET4,
  PACKET5,
  PACKET6,
  PACKET7,
  PACKET8,
  PACKETH,
  TOTAL_NO_OF_PACKETS // leave this last entry; AUTO-UPDATED
};

Packet packets[TOTAL_NO_OF_PACKETS];
packetPointer packet1 = &packets[PACKET1];
packetPointer packet2 = &packets[PACKET2];
packetPointer packet3 = &packets[PACKET3];
packetPointer packet4 = &packets[PACKET4];
packetPointer packet5 = &packets[PACKET5];
packetPointer packet6 = &packets[PACKET6];
packetPointer packet7 = &packets[PACKET7];
packetPointer packet8 = &packets[PACKET8];
packetPointer packetH = &packets[PACKETH];

SerialCommand SCmd; //Pre-setup serialcommand
MFRC522 mfrc522(cs_rfid, rst_rfid); //Pre-setup RFID

//NeoPixel preset
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NeoPixelpix, NeoPixelPIN, NEO_GRB + NEO_KHZ800); //Pre-setup NeoPixel
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(4, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(4, 14, NEO_GRB + NEO_KHZ800);
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t ledsoff = strip.Color(0, 0, 0);

void setup() {
  Serial.begin(9600);

  AtmegaState(StateInitialize);
  settingscommandinit();
  initBoard();
  

  //Construct Modbus
  modbus_construct(packet1, 1, READ_INPUT_REGISTERS, 0, 20, inputRegs0); //packet, ID, function, adress, data, register array
  modbus_construct(packet2, 1, READ_INPUT_REGISTERS, 20, 20, inputRegs20);
  modbus_construct(packet3, 1, READ_INPUT_REGISTERS, 40, 4, inputRegs40);
  modbus_construct(packet4, 1, READ_INPUT_REGISTERS, 47, 4, inputRegs47);
  modbus_construct(packet5, 1, READ_INPUT_REGISTERS, 52, 2, inputRegs52);
  modbus_construct(packet6, 1, READ_INPUT_REGISTERS, 60, 4, inputRegs60);
  modbus_construct(packet7, 1, READ_INPUT_REGISTERS, 66, 2, inputRegs66);
  modbus_construct(packet8, 1, READ_INPUT_REGISTERS, 70, 18, inputRegs70);
  modbus_construct(packetH, 1, READ_HOLDING_REGISTERS, 40, 20, holdRegs40);
  modbus_configure(&Serial1, 9600, SERIAL_8N2, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS);

  initNeoPixel();
  HoldUntilStateChage(); //wait for settings and state change from Linux application, if not, loop for ever...
  commandinit();
}

void loop() {
  //PowerFailCheck();
  ErrorControl();
  ChargeControl();
  update_leds(); //update the LEDs
  SCmd.readSerial(ms2); //communicate with Linux
  updateQueue();
  Functions(); //The functions that execute every x loops are grouped under this function
  
  delay(1);
}




