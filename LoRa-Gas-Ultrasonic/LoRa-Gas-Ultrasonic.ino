// Define the analog pin where the sensor is connected
const int gasSensorPin = A0;  // Analog input pin
const int distanceSensorPin = A1; 
long distanceSensorValue, cm;
const int binHeight = 500; // Height of the dustbin in cm
float fillPercentage;
// Variable to store the sensor reading
int sensorValue = 0;
#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;    // LoRa radio chip select
const int resetPin = 9;  // LoRa radio reset
const int irqPin = 2;    // change for your board; must be a hardware interrupt pin

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  // Setup LoRa transceiver module
  LoRa.setPins(csPin, resetPin, irqPin);  // Set CS, Reset, IRQ pins

  if (!LoRa.begin(867E6)) {  // Set frequency to 433 MHz (change to your region's frequency)
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  sensorValue = analogRead(gasSensorPin); // Read the analog value from the gas sensor
  float voltage = sensorValue * (3.3 / 1023.0);// Convert the analog reading to a voltage (adjusted for 3.3V)
  distanceSensorValue = analogRead(distanceSensorPin);    // Read the analog value from the distance sensor
  cm = distanceSensorValue / 2; // Display the distance value in centimeters
  if (cm > binHeight) {
    fillPercentage = 0;  // If the distance is greater than the bin height, it's empty
  } else {
    fillPercentage = ((binHeight - cm) / (float)binHeight) * 100;
  }
  Serial.print("Distance to Trash = ");
  Serial.print(cm);
  Serial.print(" cm\n");
  Serial.print("Dustbin Fill Percentage = ");
  Serial.print(fillPercentage);
  Serial.print(" %\n");
  Serial.print("Analog Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage);
  Serial.println("V");
  Serial.println("Successfully Connected");
  LoRa.beginPacket();            // Start LoRa packet
  LoRa.print("Gas:");      // Send message
  LoRa.print(sensorValue);      // Send message
  LoRa.print("Distance:");      // Send message
  LoRa.print(cm);      // Send message
  LoRa.endPacket();               // Finish LoRa packet
  delay(1000);  // Wait 2 seconds before sending again
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    