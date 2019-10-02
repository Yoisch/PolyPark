
//Polypark Beta Arduino Code !!!!


#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

/******************** ETHERNET SETTINGS ********************/

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x85, 0xD9 };   //physical mac address
byte ip[] = { 192, 168, 0, 112 };                   // ip in lan can change
byte subnet[] = { 255, 255, 255, 0 };              //subnet mask
byte gateway[] = { 192, 168, 0, 1 };              // default gateway
EthernetServer server(8080);                       //server port

/********************* ARDUINO SETTINGS ********************/
const int trigPin = 3; // For the ultrasonic.
const int echoPin = 4;
const int ledPinRed = 5; // Red in pin 5.
const int ledPinGreen = 6;
unsigned long previousMillis = 0;  // Error handler .
const long LedTime = 1000; // Change to change the LED setting Error collection, if not set, if errors occur, leds flash.
long duration; // Some math involved, tells distances using duration of sound.
int distance; // Ultrasonic again No need to be exact.
String status = "NULL"; // Parkling lot status, empty or full. Should never be null.
int prevDistance;


void setup()
{
Ethernet.begin(mac,ip,gateway,subnet);     // initialize Ethernet device
server.begin();                                // start to listen for clients
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);                            // input pin for switch
Serial.begin(9600);           // Debugger can remove later to save resources. 
}



void loop()
{
  // Begin server at IP
EthernetClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connnection: close");
client.println();

// ************************ ULTRASONIC CONTROLLER *******************************/
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Light controller function, Also sends Error checked status to globals.
StatusLight();


// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance = duration*0.034/2;
// Error checking to see if distance is too different from old, example random 3000 errors
distanceError();
prevDistance = distance; // Error corrector in arduino, easier than in HTML.
// Prints the distance on the Serial Monitor
// Debug
Serial.print("Distance: ");
Serial.println(distance);


// JSON Creator
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
JsonArray& array = root.createNestedArray("Spaces");
JsonObject& object = array.createNestedObject(); //Creates 1 real 3 dummy spots
object["Space1"] = status;//real spot using status as the JSON
JsonObject& object2 = array.createNestedObject();
object2["Dummy1"] = randomnum(); // Just a RNG to determine spots
JsonObject& object3 = array.createNestedObject();
object3["Dummy2"] = randomnum();
JsonObject& object4 = array.createNestedObject();
object4["Dummy3"] = randomnum();
root.printTo(client);


client.println("<meta http-equiv=\"refresh\" content=\"10\">"); // Refresh data every 10 sec can cahnge later if needed.



client.stop(); // Stop the connection so it wont keep trying to connect to the server.
}

String randomnum(){
 int randomnum = random(1,3);
  if(randomnum == 2){return "CLOSED";}
  else{return "OPEN";}
  }

void StatusLight(){
  unsigned long currentMillis = millis(); // For led controller

if (distance < 50 && currentMillis - previousMillis >= LedTime ){
  
  status = "CLOSED";
    digitalWrite(ledPinGreen, LOW);
   digitalWrite(ledPinRed, HIGH);
   previousMillis = currentMillis;

}
else if (currentMillis - previousMillis >= LedTime) {status= "OPEN";
digitalWrite(ledPinRed, LOW);
digitalWrite(ledPinGreen, HIGH);
  previousMillis = currentMillis;

} // data = status, didnt wanna reuse vars  
  
  }

void distanceError(){

for (int i=0; i < 3; i++){// We can assume if 3 errors in a row, data is actually correct    
  if (distance - 500 > prevDistance){distance = prevDistance;} // Distance is 30 - 1000 prev is 30
 
}
  }
