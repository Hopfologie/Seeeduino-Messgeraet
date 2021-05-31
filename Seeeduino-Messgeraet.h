/* Defined from LoRaWan.h
#define SF7 7
#define SF8 8
#define SF9 9
#define SF10 10
#define SF11 11
#define SF12 12
*/

//define SF constants with _ to avoid conflict with seeeduino definition
#define _SF7 0
#define _SF8 1
#define _SF9 2
#define _SF10 3
#define _SF11 4
#define _SF12 5
#define _SFSUM 6

#define AUTH_ABP 0
#define AUTH_OTAA 1

#define DEBUG_ENABLE

#define IVR 5 //Invalid Value Repetion (how often data should be requested if the returned value is invalid: returned value = 4294967295)

// Time constants in milliseconds
#define INTV_DT 1000    ///< Refresh Intervall for onscreen Date and Time
#define INTV_GPS 20000  ///< Refresh Intervall for onscreen GPS data
#define TIMEOUT 2       ///< LoRa transfer timeout

#define PACKETSPOWER1 "_p1"
#define PACKETSPOWER2 "_p2"
#define DELAYPOWER1 "_t1"
#define DELAYPOWER2 "_t2"
#define SFENABLEDPOWER1 "_1"
#define SFENABLEDPOWER2 "_2"

#define PWR1 1
#define PWR2 2


#include <SoftwareSerial.h>
#include <Nextion.h>

#include "LoRaWan.h"
#include "TinyGPS++.h"


/*
 * Declaration and initialisation of external interfaces
 * SerialUSB:       USB Debug
 * Serial:          LoRaWan Chip
 * Serial1:         GPS Chip
 * SoftwareSerial:  Nextion Display
 */

//display setup
SoftwareSerial nextion(6, 7);     ///< Nextion TX (blue) to pin 6 and RX (yellow) to pin 7 of Seeeduino
Nextion myNextion(nextion, 9600); ///< Create a Nextion object named myNextion using the nextion serial port @ 9600bps,9600 baud proven highest stable baud rate (TX and RX)

unsigned int nextionPageChangeDelay = 500;  ///< Time after which Nextion has finished changing pages and outputs correct not corrupted data

//GPS Setup
TinyGPSPlus gps;  ///< The GPS Serial Object to communicate with


//battery status
const int pin_battery_status  = A5; ///< The pin to which the battery status is internally connected (battery available)
const int pin_battery_voltage = A4; ///< The pin at which the battery voltage can be measured

//arrays holding data 
unsigned int packetAmount1 [7];   ///< Amount of packets to be sent at power 1 per SF, extra place for overall sum of packets per Power
unsigned int packetAmount2 [7];   ///< Amount of packets to be sent at power 2 per SF, extra place for overall sum of packets per Power
unsigned int packetDelay1 [6];    ///< Time between packets at power 1 per SF
unsigned int packetDelay2 [6];    ///< Time between packets at power 2 per SF
unsigned int packetSent1 [7];     ///< Amount of packets already sent at power 1 per SF, extra place for overall sum of sent packets
unsigned int packetSent2 [7];     ///< Amount of packets already sent at power 2 per SF, extra place for overall sum of sent packets
bool SFenabled1 [6];              ///< Whether SF is enabled at specified power
bool SFenabled2 [6];              ///< Whether SF is enabled at specified power

bool payloadIncludeGPS; ///< Whether the position retrieved by the GPS should be included in the LoRa Payload

char NwkSKey[33];   ///< Network Session Key
char AppSKey[33];   ///< Application Session Key
char AppKey[33];    ///< Application Key
char DevAddr[9];    ///< Device Address
char DevEUI[17];    ///< Device EUI
char AppEUI[17];    ///< Application EUI

bool authMode;  ///< Selected authentication mode for eg. TTN / TTS

unsigned int SFPower1;  ///< LoRa TX Power 1
unsigned int SFPower2;  ///< LoRa TX Power 2

unsigned int timingMeasurement = 0; ///< Used to buffer time for measuring execution time of codeblocks

bool measurementRunning = false;  ///< Whether a measurement is in progress

//datastorage for values handling the nonblocking measurement functions
unsigned int currentSF = _SF7;  ///< Current SF, using values defined above eg. _SF7
unsigned int currentPWR = 1;    ///< Current Power, either 1 or 2
unsigned int tLastPacket = 0;   ///< The time the last packet got sent at in milliseconds

bool transferResult;
unsigned char payloadData[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/*
 * Component identifiers
 * to handle incomming messages
 * SR = Save and Return
 */
String btn_start = "65 0 7 1 ff ff ff";
String btn_pause = "65 0 41 1 ff ff ff";
String btn_stop = "65 0 1f 1 ff ff ff";
String btn_SRparameters = "65 1 19 1 ff ff ff";
String btn_SRsettings = "65 2 1 1 ff ff ff";
String btn_SRinfo = "65 3 1 1 ff ff ff";
