/* GPS.ino
 * Includes alls functions to communicate with the gps
 * mounted on the seeeduino
 *
 *
 */

void pullGPSdata() {
  char c;

  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }
}

String getGPSTime() {
  if (gps.time.isValid())
  {
    String str;
    if (gps.time.hour() < 10) str += "0";
    str += gps.time.hour();
    str += ":";
    if (gps.time.minute() < 10) str += "0";
    str += gps.time.minute();
    str += ":";
    if (gps.time.second() < 10) str += "0";
    str += gps.time.second();
    return str;
  }
  else
  {
    return "TIME INVALID";
  }    
}

String getGPSDate() {
  if (gps.time.isValid())
  {
    String str;
    str += gps.date.day();
    str += "/";
    str += gps.date.month();
    str += "/";
    str += gps.date.year();
    return str;
  }
  else
  {
    return "DATE INVALID";
  }
}

String getGPSLat(int decPlaces) {
  if (gps.location.isValid())
  {
    String str = String(gps.location.lat(), decPlaces);
    return str;
  }
  else
  {
    return "N/A";
  }
}

String getGPSLng(int decPlaces) {
  if (gps.location.isValid())
  {
    String str = String(gps.location.lng(), decPlaces);
    return str;
  }
  else
  {
    return "N/A";
  }
}

int getGPSSat() {
  return gps.satellites.value();
}

void printGPSAll() {
  Serial.print("GPS: ");
  Serial.print(getGPSLat(6));
  Serial.print(", ");
  Serial.print(getGPSLng(6));
  Serial.print("    ");
  Serial.print(getGPSDate());
  Serial.print(" ");
  Serial.println(getGPSTime());
}