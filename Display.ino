/**
 * Listens and interprets commands sent by the display
 * 
 * Intreprets the received commands and handles them accordingly, calling the corresponding function
 */
//handle input eg. button press or value entered
void listenInterpret() {
  String msg = myNextion.listen(); //check for message

  if(msg != ""){
    //received message not empty, check for content

    if (msg.equals(btn_start)) {
      //start measurement
      SerialUSB.println("btn_start");
      startMeasurement();
      SerialUSB.print("Measurement running: ");
      SerialUSB.println(measurementRunning);
    }
    else if (msg.equals(btn_pause)) {
      //pause measurement
      SerialUSB.println("btn_pause");
      if (measurementRunning == false) resumeMeasurement();
      else pauseMeasurement();
      SerialUSB.print("Measurement running: ");
      SerialUSB.println(measurementRunning);
    }
    else if (msg.equals(btn_stop)) {
      //stop measurement
      SerialUSB.println("btn_stop");
      pauseMeasurement();
      SerialUSB.print("Measurement running: ");
      SerialUSB.println(measurementRunning);
    }
    else if (msg.equals(btn_SRparameters)) {
      //download measurement parameters and exit parameters menu
      SerialUSB.println("btn_SRparameters");
      delay(nextionPageChangeDelay); //delay to wait till Nextion has finished page change and doesn't send false information
      getLoraParameters();

      #ifdef DEBUG_ENABLE
        debugPrintArrays();
      #endif 
    }
    else if (msg.equals(btn_SRsettings)) {
      //download settings and exit settings menu
      SerialUSB.println("btn_SRsettings");
      delay(nextionPageChangeDelay); //delay to wait till Nextion has finished page change and doesnt send false information
      getGeneralSettings();

      #ifdef DEBUG_ENABLE
        debugPrintLora();
      #endif      
    }
    else if (msg.equals(btn_SRinfo)) {
      //exit info menu
      SerialUSB.println("btn_SRinfo");
      delay(nextionPageChangeDelay); //delay to wait till Nextion has finished page change and doesnt send false information
    }
    else {
      SerialUSB.print("Listen: ");
      SerialUSB.println(msg);
    }
  }
}


/**
 * Download parameter settings
 *
 * Downloads data via serial from Nextion display
 */
void getLoraParameters() {
  getSFpacketSettings(packetAmount1, PACKETSPOWER1);
    delay(100);
    getSFpacketSettings(packetAmount2, PACKETSPOWER2);
    delay(100);
    getSFdelaySettings(packetDelay1, DELAYPOWER1);
    delay(100);
    getSFdelaySettings(packetDelay2, DELAYPOWER2);
    delay(100);
    getSFenabledSettings(SFenabled1, SFENABLEDPOWER1);
    delay(100);
    getSFenabledSettings(SFenabled2, SFENABLEDPOWER2);
    getSFpower();
}


/**
 * Sets the current values retrieved from the GPS to the corresponding display component
 * Updated Values: Date and Time
 * Uploads data via serial to Nextion display
 */
void updateStatusbarDT() {
  myNextion.setComponentText("tDate", getGPSDate());
  myNextion.setComponentText("tTime", getGPSTime());
}

/**
 * Sets the current values retrieved from the GPS to the corresponding display component
 * Updated Values: Latitude, Longitude, Satellite Count
 * Uploads data via serial to Nextion display
 */
void updateStatusbarGPS() {
  myNextion.setComponentValue("nSat", getGPSSat());
  myNextion.setComponentText("tLat", getGPSLat(6));
  myNextion.setComponentText("tLng", getGPSLng(6));
}


/**
 * Download general settings (LoRaWAN Access details for eg. TTN or TTS)
 *
 * Downloads data via serial from Nextion display
 */
void getGeneralSettings() {
  myNextion.getComponentText("page2.tNwkSKey").toCharArray(NwkSKey, sizeof(NwkSKey)); 
  myNextion.getComponentText("page2.tAppSKey").toCharArray(AppSKey, sizeof(AppSKey));
  myNextion.getComponentText("page2.tAppKey").toCharArray(AppKey, sizeof(AppKey));
  myNextion.getComponentText("page2.tDevAddr").toCharArray(DevAddr, sizeof(DevAddr));
  myNextion.getComponentText("page2.tDevEUI").toCharArray(DevEUI, sizeof(DevEUI));
  myNextion.getComponentText("page2.tAppEUI").toCharArray(AppEUI, sizeof(AppEUI));
  authMode = myNextion.getComponentValue("page2.rOTAA"); //radio Buttons linked on display
  
}


/**
 * Download settings whether GPS should be included in LoRa Payload
 *
 * Downloads data via serial from Nextion display
 */
void getPayloadIncludeGPS() {
  int rep = 0;
  do {
    payloadIncludeGPS = myNextion.getComponentValue("page1.btGPSenable");
  } while (payloadIncludeGPS == 4294967295 && rep <= IVR);
}

/**
 * Download value set for LoRa TX power from display
 *
 * Downloads data via serial from Nextion display
 */
void getSFpower() {
  int rep = 0;
  do {
    SFPower1 = myNextion.getComponentValue("page1.nPower1");
  } while (SFPower1 == 4294967295 && rep <= IVR);

  rep = 0;
  do {
    SFPower2 = myNextion.getComponentValue("page1.nPower2");
  } while (SFPower2 == 4294967295 && rep <= IVR);
}


/**
 * Download SF settings for packets at speciefied power
 *
 * Downloads data via serial from Nextion display
 *@param arrPacketAmount Array to which retrieved data should be stored
 *@param strComp From which component set (power 1 or power 2) the data should be retrieved
 */
void getSFpacketSettings(unsigned int arrPacketAmount[], String strComp) {
  packetAmount2[_SFSUM] = 0;
  for (int i = _SF7; i <= _SF12; i++) {
    int rep = 0;
    do {  //error check to prevent reading wrong value 4294967295, repeated max 5 times
      String component = "page1.nSF";
      component += 7 + i;
      component += strComp;
      arrPacketAmount[i] = myNextion.getComponentValue(component);
      arrPacketAmount[_SFSUM] += arrPacketAmount[i];
      rep++;
    } while (arrPacketAmount[i] == 4294967295 && rep <= IVR);
  }
}


/**
 * Download SF settings for delay between packets at specified power
 *
 * Downloads data via serial from Nextion display
 *@param arrPacketDelay Array to which retrieved data should be stored
 *@param strComp From which component set (power 1 or power 2) the data should be retrieved
 */
void getSFdelaySettings(unsigned int arrPacketDelay[], String strComp) { 
  for (int i = _SF7; i <= _SF12; i++) {
    int rep = 0;
    do {
      String component = "page1.nSF";
      component += 7 + i;
      component += strComp;
      arrPacketDelay[i] = myNextion.getComponentValue(component);
      rep++;
    } while (arrPacketDelay[i] == 4294967295 && rep <= IVR);
    
  }
}


/**
 * Download SF settings whether SF is enbaled at specified power
 *
 * Downloads data via serial from Nextion display
 *@param arrPacketDelay Array to which retrieved data should be stored
 *@param strComp From which component set (power 1 or power 2) the data should be retrieved
 */
void getSFenabledSettings(bool arrSFenabled[], String strComp) { 
  for (int i = _SF7; i <= _SF12; i++) {
    int rep = 0;
    do {
      String component = "page1.btSF";
      component += 7 + i;
      component += strComp;
      arrSFenabled[i] = myNextion.getComponentValue(component);
      rep++;
    } while ((arrSFenabled[i] != true || arrSFenabled[i] != false )&& rep <= IVR);
    
  }
}


/**
 * Upload SF packet amount settings to Display and update vis
 *
 * Uploads data via serial to Nextion display and stores it into components
 */
void updateSFpacketSettings(unsigned int arrPacketAmount[], unsigned int arrPacketsSent[], String strComp) {
  for (int i = _SF7; i <= _SF12; i++) {
    String component = "page0.tSF";
    component += 7 + i;
    component += strComp;
    String text = String(arrPacketsSent[i]);
    text += " / ";
    text += String(arrPacketAmount[i]);
    myNextion.setComponentText(component, text);
  }
}


/**
 * Upload SF packet delay settings to Display and update vis
 *
 * Uploads data via serial to Nextion display and stores it into components
 */
void updateSFdelaySettings(unsigned int arrPacketAmount[], unsigned int arrPacketDelay[], String strComp) {
  for (int i = _SF7; i <= _SF12; i++) {
    String component = "page0.tSF";
    component += 7 + i;
    component += strComp;
    String text = "0:00";
    text += "/";
    text += String((int) (((float) arrPacketAmount[i] * arrPacketDelay[i]) / 60));
    text += ":";
    if ((int) (arrPacketAmount[i] * arrPacketDelay[i]) % 60 < 10) {
      text += "0";  //add leading zero if seconds less than 10
    }
    text += String((int) (arrPacketAmount[i] * arrPacketDelay[i]) % 60);    
    myNextion.setComponentText(component, text);
  }
}