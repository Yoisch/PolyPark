
//Polypark Beta Arduino Code !!!!

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

/******************** ETHERNET SETTINGS ********************/

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x85, 0xD9 };   //physical mac address
byte ip[] = { 192, 168, 0, 112 };                   // ip in lan can change
byte subnet[] = { 255, 255, 255, 0 };              //subnet mask
byte gateway[] = { 192, 168, 0, 1 };              // default gateway
EthernetServer server(80);                       //server port
const int trigPin = 3;
const int echoPin = 4;
long duration;// For real spot
int distance;
String status; // Status of the spot OPEN or CLOSED
void setup()
{
Ethernet.begin(mac,ip,gateway,subnet);     // initialize Ethernet device
server.begin();                                // start to listen for clients
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);                            // input pin for switch
Serial.begin(9600);
}



void loop()
{

EthernetClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connnection: close");
client.println();
// Clears the trigPin

// All for sensing distance//

digitalWrite(trigPin, LOW);
delayMicroseconds(2);


// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);


// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance = duration*0.034/2;


// Prints the distance on the Serial Monitor
// Debug
Serial.print("Distance: ");
Serial.println(distance);

//check to export, can change to function if needed  
  
if (distance < 50 ){
  status = "CLOSED";
}
else{status= "OPEN";} 

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


client.println("<meta http-equiv=\"refresh\" content=\"10\">"); // Refresh data ever 10 sec can cahnge later if needed.



client.stop(); // Stop the connection so it wont keep trying to connect to the server.
}

String randomnum(){ // Dummy creator
 int randomnum = random(1,3);
  if(randomnum == 2){return "CLOSED";}
  else{return "OPEN";}
  }
