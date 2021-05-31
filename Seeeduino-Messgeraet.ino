#include "Seeeduino-Messgeraet.h"

int tDT = 0;
int tGPS = 0;

void setup() {
  //setup USB debug output
  SerialUSB.begin(115200);
  while (!SerialUSB) {} // wait for serial port to connect. Needed for Native USB only

  pinMode(pin_battery_status, INPUT);

  //init nextion Object
  myNextion.init();

  //init Lora & GPS Chip
  lora.init();
  lora.setDeviceReset();
  Serial2.begin(9600);     // open the GPS serial

  getLoraParameters();
  getGeneralSettings();

  SerialUSB.println("Setup completed!");
}

void loop() {
  listenInterpret();

  pullGPSdata();

  //refresh onscreen GPS data
  if (millis() - tDT >= INTV_DT) {
    updateStatusbarDT();
    tDT = millis();
  }

  //refresh onscreen GPS data
  if (millis() - tGPS >= INTV_GPS) {
    updateStatusbarDT();
    tGPS = millis();
  }

  

  if (measurementRunning == true) {

    if (currentPWR == PWR1) {

      if (millis() - tLastPacket >= packetDelay1[currentSF]) {

        if (currentSF == _SF7 && packetSent1[currentSF] == 0 && packetAmount1[currentSF] > 0) { //initial setup of LoRa
          SerialUSB.println("Initial Lora Setup");
          LoRaSetup(currentSF, SFPower1, TIMEOUT);
        }

        else if (packetSent1[currentSF] == packetAmount1[currentSF]) { //amounts of packets at specific SF reached, selecting next enabled SF
          if (currentSF < _SF12) {
            while (currentSF < _SF12 && SFenabled1[currentSF] != true) {  //select next enabled SF at power 1
              currentSF++;
            }
            if (currentSF == _SF12 && packetAmount1[currentSF] == 0) {  //no more SF enabled at power 1, switch to power 2 (skips unneeded setup for SF 12 at power 1)
              currentPWR = PWR2;
            }
          }
          else { //all SF at first power used, switch to second power and select first enabled SF
            currentPWR = PWR2;
            currentSF = _SF7;
            while (currentSF < _SF12 && SFenabled2[currentSF] != true) {  //select first enabled SF at power 2
              currentSF++;
            }
            if (currentSF == _SF12 && packetAmount2[currentSF] == 0) {  //no SF enabled at power 2, end measurement
              measurementRunning = false;
            }
          }

          //setup LoRa chip with selected SF and Power only if measurement is still running
          if (measurementRunning == true) {
            if (currentPWR = PWR1) {  //setup for power 1
              LoRaSetup(currentSF, SFPower1, TIMEOUT);
            }
            else if (currentPWR = PWR2) { //setup for power 2
              LoRaSetup(currentSF, SFPower2, TIMEOUT);
            }
          }
        }
      }

      SerialUSB.println("Transmit Message:");
      transferResult = lora.transferPacket(payloadData, 15, TIMEOUT);
      packetSent1[currentSF]++;
      packetSent1[_SFSUM]++;
      tLastPacket = millis();
    }

    else if (currentPWR == PWR2) {

      if (millis() - tLastPacket >= packetDelay2[currentSF]) {

        if (packetSent2[currentSF] == packetAmount2[currentSF]) { //amounts of packets at specific SF reached, selecting next enabled SF
          if (currentSF < _SF12) {
            while (currentSF < _SF12 && SFenabled2[currentSF] != true) {  //select next enabled SF at power 1
              currentSF++;
            }
            if (currentSF == _SF12 && packetAmount2[currentSF] == 0) {  //no more SF enabled at power 2, end measurement
              measurementRunning = false;
            }
          }
        }

        //setup LoRa chip with selected SF and Power only if measurement is still running
        if (measurementRunning == true) {
          //setup for power 2
          LoRaSetup(currentSF, SFPower2, TIMEOUT);
        }

        transferResult = lora.transferPacket(payloadData, 15, TIMEOUT);
        packetSent2[currentSF]++;
        packetSent2[_SFSUM]++;
        tLastPacket = millis();

      }
      
    }    
  }



}
