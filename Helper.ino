/* Helper.ino
 * Managing all helper functions
 * 
 *
 *
 */

//calculate battery voltage
float check_bat() {
  int a = analogRead(pin_battery_voltage);
  float v = a/1023.0*3.3*11.0;        // there's an 1M and 100k resistor divider
  return v;
}

void debugPrintArrays() {
  SerialUSB.println();
  SerialUSB.print("       Power 1: ");
  if (SFPower1 < 10) SerialUSB.print(" ");
  SerialUSB.print(SFPower1);
  SerialUSB.print("             Power 2: ");
  if (SFPower1 < 10) SerialUSB.print(" ");
  SerialUSB.println(SFPower2);
  SerialUSB.println("       Amount  Delay Enabl.   Sent    Amount  Delay Enabl.   Sent");

  for (int i = _SF7; i <= _SF12; i++) {
    SerialUSB.print("SF ");
    if (i + 7 < 10) SerialUSB.print(" ");
    SerialUSB.print(i + 7);
    SerialUSB.print(":    ");
    
    if (packetAmount1[i] < 100) SerialUSB.print(" ");
    if (packetAmount1[i] < 10) SerialUSB.print(" ");
    SerialUSB.print(packetAmount1[i]);
    SerialUSB.print("    ");

    if (packetDelay1[i] < 100) SerialUSB.print(" ");
    if (packetDelay1[i] < 10) SerialUSB.print(" ");
    SerialUSB.print(packetDelay1[i]);
    SerialUSB.print("      ");

    SerialUSB.print(SFenabled1[i]);
    SerialUSB.print("    ");

    if (packetSent1[i] < 100) SerialUSB.print(" ");
    if (packetSent1[i] < 10) SerialUSB.print(" ");
    SerialUSB.print(packetSent1[i]);
    SerialUSB.print("       ");

    if (packetAmount2[i] < 100) SerialUSB.print(" ");
    if (packetAmount2[i] < 10) SerialUSB.print(" ");
    SerialUSB.print(packetAmount2[i]);
    SerialUSB.print("    ");

    if (packetDelay2[i] < 100) SerialUSB.print(" ");
    if (packetDelay2[i] < 10) SerialUSB.print(" ");
    SerialUSB.print(packetDelay2[i]);
    SerialUSB.print("      ");

    SerialUSB.print(SFenabled2[i]);
    SerialUSB.print("    ");

    if (packetSent2[i] < 100) SerialUSB.print(" ");
    if (packetSent2[i] < 10) SerialUSB.print(" ");
    SerialUSB.print(packetSent2[i]);

    SerialUSB.println();
  }
}

void debugPrintLora() {
  SerialUSB.println();
  SerialUSB.print("NwkSKey:  ");
  SerialUSB.println(NwkSKey);
  SerialUSB.print("AppSKey:  ");
  SerialUSB.println(AppSKey);
  SerialUSB.print("AppKey:   ");
  SerialUSB.println(AppKey);
  SerialUSB.print("DevAddr:  ");
  SerialUSB.println(DevAddr);
  SerialUSB.print("DevEUI:   ");
  SerialUSB.println(DevEUI);
  SerialUSB.print("AppEUI:   ");
  SerialUSB.println(AppEUI);
  SerialUSB.print("AuthMode: ");
  if (authMode == AUTH_ABP) SerialUSB.print("ABP  ");
  if (authMode == AUTH_OTAA) SerialUSB.print("OTAA  ");
  SerialUSB.println(authMode);

}

/**
 * Used to measure the execution time of codeblocks (prints time in microseconds to SerialUSB)
 * Place tS at beginng of codeblock
 * Use tE at end of codeblock
 */
void tS() {
  timingMeasurement = micros();
}

/**
 * Used to measure the execution time of codeblocks (prints time in microseconds to SerialUSB)
 * Place tS at beginng of codeblock
 * Use tE at end of codeblock
 */
void tE() {
  SerialUSB.println(micros() - timingMeasurement);
}