
// put function declarations here:
//int myFunction(int, int);

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
//#include <Wire.h>
#include <Arduino.h>

// Definisci i pin per il sensore HC-SR04
const int triggerPin = 5;
const int echoPin = 18;
const float sound_vel = 0.034;

//BLE server name
#define bleServerName "ESP32 BLE Distance"

bool deviceConnected = false;
bool readvise = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"
//"ca73b3ba-39f6-4ab3-91ae-186dc9577d99" uuid de inserire nella caratteristica della distanza

#define _UART_UUID = Uuid.parse("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
#define _UART_RX   = Uuid.parse("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
#define _UART_TX   = Uuid.parse("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

static BLEUUID serviceUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID charUUID_RX("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");   // RX Characteristic
static BLEUUID charUUID_TX("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");   // TX Characteristic


// Humidity Characteristic and Descriptor
BLECharacteristic distCharacteristics(charUUID_TX, BLECharacteristic::PROPERTY_NOTIFY);  //"ca73b3ba-39f6-4ab3-91ae-186dc9577d99"
BLEDescriptor distDescriptor(BLEUUID((uint16_t)0x2903));
BLEServer *pServer1;

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    readvise = false;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    readvise = true;
  }
};

void advertising(BLEServer *pServer) {
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceUUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

}




void setup() {
  
  // Start serial communication 
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("Starting BLE");

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  pServer1 = BLEDevice::createServer();
  pServer1->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *distService = pServer1->createService(serviceUUID);

  // Distance
  distService->addCharacteristic(&distCharacteristics);
  distDescriptor.setValue("HC-SR04 Sensor Distance");
  distCharacteristics.addDescriptor(new BLE2902());
  
  // Start the service
  distService->start();

  // Start advertising
  advertising(pServer1);

}

void loop() {

  if (deviceConnected) {
    
    // Genera impulso di trigger per il sensore HC-SR04
    digitalWrite(triggerPin, LOW);  //invia un impulso di 10microsec sul trigger      
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(triggerPin, LOW);

    // Misura il tempo di eco per calcolare la distanza e visualizza la distanza sul serial monitor
    long durata = pulseIn(echoPin, HIGH); //legge l'echopin e calcola la durata (in microsecondi) del tempo di volo dell'onda inviata
    double distance = durata * sound_vel / 2.0; //calcolo della distanza in cm
	  Serial.printf("Distanza in cm: ");
    static char distanceF[6];
    dtostrf(distance, 6, 2, distanceF);
    Serial.println(distanceF);
    std::string dist = distanceF;

    //test con invio di un json
    std::string jsonData = "{\"distance\": " + dist + "}";

    //Notify humidity reading from BME
    //static char humidityTemp[6];
    //dtostrf converte un double in un array di char di dimensione 6 con 2 valori dopo la virgola
    //dtostrf(hum, 6, 2, humidityTemp);
    //Set humidity Characteristic value and notify connected client
    //distCharacteristics.setValue(distanceF);
    distCharacteristics.setValue(jsonData);
    distCharacteristics.notify();   

    Serial.print(" - Humidity: ");
    // Serial.print(hum);
    Serial.println(" %");
  }

  if (readvise) {
    advertising(pServer1);
  }

//da prevedere funzionalità tali per cui una volta che un dispositivo si disconnette, allora venga 
//riavviata la ricerca BLE in modo da consentire ad altri di collegarsi. 

//da rivedere anche il formato dei dati da inviare, provare con formato json.

  delay(500);
}