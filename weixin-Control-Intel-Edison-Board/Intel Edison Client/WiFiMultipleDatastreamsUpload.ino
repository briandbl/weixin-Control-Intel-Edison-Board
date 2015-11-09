
#include <HttpClient.h>
#include <SPI.h>
#include <WiFi.h>
#include <Xively.h>
#include <Servo.h>
//#include <ChainableLED.h>
//ChainableLED leds(7, 8, 1); // connect to pin7 and pin8 , one led
//char ssid[] = "dwg155f-ap03-2.4G"; //  your network SSID (name) 
//char pass[] = "test1234";    // your network password (use for WPA, or use as key for WEP)
char ssid[] = "your hotspot name"; //  your network SSID (name) 
char pass[] = "your password";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
 int pos = 0;
int status = WL_IDLE_STATUS;

// Your Xively key to let you upload data
char xivelyKey[] = "YOUR Xively Key";

// Analog pin which we're monitoring (0 and 1 are used by the Ethernet shield)
int sensorPin = 2;

// Define the strings for our datastream IDs
char sensorId[] = "air_reading";
char bufferId[] = "info_message";
String stringId("random_string");
String controlLedId("control_led");
String controlFanId("control_fan");
const int bufferSize = 140;
char bufferValue[bufferSize]; // enough space to store the string we're going to send

Servo servo;
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
  XivelyDatastream(bufferId, strlen(bufferId), DATASTREAM_BUFFER, bufferValue, bufferSize),
  XivelyDatastream(stringId, DATASTREAM_STRING)
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(248508715, datastreams, 3 /* number of datastreams */);

// Define the string for our datastream ID

XivelyDatastream datastreams1[] = {
   // XivelyDatastream(bufferId, strlen(bufferId), DATASTREAM_BUFFER, bufferValue, bufferSize),
   XivelyDatastream(controlLedId, DATASTREAM_STRING),
   XivelyDatastream(controlFanId, DATASTREAM_STRING),
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed1(YOUR FEEDID, datastreams1, 2 /* number of datastreams */);

WiFiClient client;
XivelyClient xivelyclient(client);

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(3); //pin 11
  pinMode(13, OUTPUT);
  Serial.println("Starting multiple datastream upload to Xively...");
  Serial.println();

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {
 // leds.setColorRGB(0,255, 255, 255);
  int ret = xivelyclient.get(feed1, xivelyKey);
  Serial.print("xivelyclient.get returned ");
  Serial.println(ret);

  if (ret > 0)
  {
    Serial.println("Datastream is...");
    Serial.println(feed1[0]);

    Serial.print("feed1[0]: ");
    Serial.println(feed1[0].getString());
     Serial.print("feed1[1]: ");
    Serial.println(feed1[1].getString());
 if(feed1[0].getString().equals("on")){
     Serial.print("led on ");
     digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
//     leds.setColorRGB(0,255, 255, 255);
  }else if(feed1[0].getString().equals("off")){
    Serial.print("led off ");
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  }
  if(feed1[1].getString().equals("on")){
    
     Serial.print("Fan on ");
    if(servo.attached()==false){
       servo.attach(3); //pin 11
    }
     for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
     servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(1);                 // waits 15ms for the servo to reach the position
    }
   for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
           delay(1);              // waits 15ms for the servo to reach the position
    } 
  }else if(feed1[1].getString().equals("off")){
    Serial.print("Fan off ");
    servo.detach();
  }
  
  } 
  int sensorValue = analogRead(sensorPin);
  datastreams[0].setFloat(sensorValue);

  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());

  datastreams[1].setBuffer("a message to upload");
  Serial.print("Setting buffer value to:\n    ");
  // Pick a random number to send up in a string
  String stringValue(random(100));
  stringValue += " is a random number";
  Serial.println(datastreams[1].getBuffer());

  datastreams[2].setString(stringValue);
  Serial.print("Setting string value to:\n    ");
  Serial.println(datastreams[2].getString());

 // Serial.println("Uploading it to Xively");
 // int ret1 = xivelyclient.put(feed, xivelyKey);
 // Serial.print("xivelyclient.put returned ");
 // Serial.println(ret1);

//  Serial.println();
 // delay(15000);
}
