/* LoRa.ino
 * 
 * 
 *
 *
 */

//         Extract from LoRaWan.h from seeedstudio
/*****************************************************************
Type    DataRate    Configuration   BitRate| TxPower Configuration 
EU434   0           SF12/125 kHz    250    | 0       10dBm
        1           SF11/125 kHz    440    | 1       7 dBm
        2           SF10/125 kHz    980    | 2       4 dBm
        3           SF9 /125 kHz    1760   | 3       1 dBm
        4           SF8 /125 kHz    3125   | 4       -2dBm
        5           SF7 /125 kHz    5470   | 5       -5dBm
        6           SF7 /250 kHz    11000  | 6:15    RFU
        7           FSK:50 kbps     50000  | 
        8:15        RFU                    |     
******************************************************************
Type    DataRate    Configuration   BitRate| TxPower Configuration 
EU868   0           SF12/125 kHz    250    | 0       20dBm
        1           SF11/125 kHz    440    | 1       14dBm
        2           SF10/125 kHz    980    | 2       11dBm
        3           SF9 /125 kHz    1760   | 3       8 dBm
        4           SF8 /125 kHz    3125   | 4       5 dBm
        5           SF7 /125 kHz    5470   | 5       2 dBm
        6           SF7 /250 kHz    11000  | 6:15    RFU
        7           FSK:50 kbps     50000  | 
        8:15        RFU                    | 
******************************************************************
Type    DataRate    Configuration   BitRate| TxPower Configuration
US915   0           SF10/125 kHz    980    | 0       30dBm
        1           SF9 /125 kHz    1760   | 1       28dBm
        2           SF8 /125 kHz    3125   | 2       26dBm
        3           SF7 /125 kHz    5470   | 3       24dBm
        4           SF8 /500 kHz    12500  | 4       22dBm
        5:7         RFU                    | 5       20dBm
        8           SF12/500 kHz    980    | 6       18dBm
        9           SF11/500 kHz    1760   | 7       16dBm
        10          SF10/500 kHz    3900   | 8       14dBm
        11          SF9 /500 kHz    7000   | 9       12dBm
        12          SF8 /500 kHz    12500  | 10      10dBm
        13          SF7 /500 kHz    21900  | 11:15   RFU
        14:15       RFU                    | 
*******************************************************************/


/**
 * Initialization of the LoRaWan settings/chip
 *
 *@param SF Spreading Factor to be used
 *@param power  TX power to be used (2-20dbm)
 *@param authMode Authentication mode to be used (ABP or OTAA)
 *
 */
void LoRaSetup(int SF, int power, int authMode){

  #ifdef DEBUG_ENABLE
    char buffer[256];
    memset(buffer, 0, 256);
    lora.getVersion(buffer, 256, 1);
    SerialUSB.print(buffer); 
    memset(buffer, 0, 256);
    lora.getId(buffer, 256, 1);
    SerialUSB.print(buffer);
  #endif

  //lora.setKey(char *NwkSKey, char *AppSKey, char *AppKey)
  //lora.setKey("49BBBE97129795D73F3C4F49C89AE173", "5A40ED1C0709EABA20E2A84E899E9090", NULL);
  //lora.setId(char *DevAddr, char *DevEUI, char *AppEUI)
  //lora.setId("260110AD", "00884970F34920CE", "70B3D57ED00411C5");
  
  if (strcmp(AppKey, "NULL") == 0) {
    SerialUSB.println("Key NULL");
    lora.setKey(NwkSKey, AppSKey, NULL);
  }
  else {
    lora.setKey(NwkSKey, AppSKey, AppKey);
  }
  lora.setId(DevAddr, DevEUI, AppEUI);
  
  if (authMode == AUTH_ABP) {
    lora.setDeciveMode(LWABP);
  } 
  else if (authMode == AUTH_OTAA) {
    lora.setDeciveMode(LWOTAA);
  }
  else {
    exit;
  }

  //Kanaele muessen auf drei Ports initialisiert werden

  switch (SF) {
        case 0: //SF7
            lora.setDataRate(DR5, EU868);
            lora.setChannel(0, 867.1,DR5);
            lora.setChannel(1, 867.1,DR5);
            lora.setChannel(2, 867.1,DR5);
            lora.setChannel(3, 867.1);
        case 1: //SF8
            lora.setDataRate(DR4, EU868);
            lora.setChannel(0, 867.1,DR4);
            lora.setChannel(1, 867.1,DR4);
            lora.setChannel(2, 867.1,DR4);    
            break;
        case 2: //SF9
            lora.setDataRate(DR3, EU868);
            lora.setChannel(0, 867.1,DR3);
            lora.setChannel(1, 867.1,DR3);
            lora.setChannel(2, 867.1,DR3);    
            break;    
        case 3: //SF10
            lora.setDataRate(DR2, EU868);
            lora.setChannel(0, 867.1,DR2);
            lora.setChannel(1, 867.1,DR2);
            lora.setChannel(2, 867.1,DR2);
            break;
        case 4: //SF11
            lora.setDataRate(DR1, EU868);
            lora.setChannel(0, 867.1,DR1);
            lora.setChannel(1, 867.1,DR1);
            lora.setChannel(2, 867.1,DR1);
            break; 
        case 5: //SF12
            lora.setDataRate(DR0, EU868);
            lora.setChannel(0, 867.1,DR0);
            lora.setChannel(1, 867.1,DR0);
            lora.setChannel(2, 867.1,DR0);
            break;  
            break;
        default:
            break;
    }    
  lora.setDutyCycle(false);
  lora.setJoinDutyCycle(false);
      
  lora.setPower(power);  //Set TxPower
}


/**
 * Prepare to start measurement
 *
 * Sets needed variables to beginn measurement process
 */
void startMeasurement() {
  resetMeasurement();
  measurementRunning = true;
}


/**
 * Pauses the measurement without resetting the packetSent arrays
 *
 *
 */
void pauseMeasurement() {
  measurementRunning = false;
}


/**
 * Resumes the measurement after pausing
 *
 *
 */
void resumeMeasurement() {
  measurementRunning = true;
}


/**
 * Resets the measurement values
 *
 * Resets the packetSent arrays to 0;
 */
void resetMeasurement() {
  measurementRunning = false;

  for (int i = _SF7; i <= _SFSUM; i++) {
    packetSent1[i] = 0;
    packetSent2[i] = 0;
  }

  currentSF = _SF7;
  currentPWR = PWR1;
  tLastPacket = 0;
}