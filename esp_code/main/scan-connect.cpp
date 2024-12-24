#include "Arduino.h"
#include "WiFi.h"
#include <math.h>

// Servo motor setup
const int servo1Pin = 5;
const int servo2Pin = 6;
const int servo3Pin = 7;
const int servo4Pin = 8;
const int servoPinResolution = 14;
const float minDuty = 0.455 / 20 * (pow(2, servoPinResolution) - 1);
const float maxDuty = 2.550 / 20 * (pow(2, servoPinResolution) - 1);
const float maxValue = pow(2, servoPinResolution) - 1;

NetworkServer server(80);

void servoWriteAngle(int pin, int value) {
    float duty;
    if (value >= 0 && value <= 180) {
        duty = map(value, 0, 180, minDuty, maxDuty);
    } else {
        duty = map(value, 0, 20000, 0, maxValue);
    }
    ledcWrite(pin, (uint)duty);
    // Serial.print("Set duty to: ");
    // Serial.println((uint)duty);
}

void servoWriteDuty(int pin, float value) {
    float duty;
    duty = map(value, 0, 20000, 0, maxValue);
    ledcWrite(pin, (uint)duty);
    Serial.print("Set duty to: ");
    Serial.println((uint)duty);
}

void setup() {
    Serial.begin(115200);
    if (!ledcAttach(servo1Pin, 50, servoPinResolution))
        while (1)
            ;
    if (!ledcAttach(servo2Pin, 50, servoPinResolution))
        while (1)
            ;
    if (!ledcAttach(servo3Pin, 50, servoPinResolution))
        while (1)
            ;
    if (!ledcAttach(servo4Pin, 50, servoPinResolution))
        while (1)
            ;
    String ssid = "La_Fibre_dOrange_892F";
    String password = "g7Pvj24k";
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
        ;
    // while (!WiFi_init()) ;
    server.begin();
}

void loop() {
    NetworkClient client = server.accept(); // listen for incoming clients
    int angle1, angle2, angle3, angle4;
    if (client) {
        Serial.println("New Client connected.");
        String receivedData = ""; // Buffer for incoming data

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (c == '\n') { // End of a command
                    Serial.println("Received: " + receivedData);
                    if (sscanf(receivedData.c_str(), "%d %d %d %d", &angle1,
                               &angle2, &angle3, &angle4) == 4) {
                        servoWriteAngle(servo1Pin, angle1);
                        servoWriteAngle(servo2Pin, angle2);
                        servoWriteAngle(servo3Pin, angle3);
                        servoWriteAngle(servo4Pin, angle4);

                        Serial.printf("Servo angles set to: %d, %d, %d, %d\n",
                                      angle1, angle2, angle3, angle4);
                    } else {
                        Serial.println("Invalid command format.");
                    }
                    receivedData = ""; // Clear the buffer
                } else {
                    receivedData += c; // Append character to buffer
                }
            }
        }
        client.stop(); // Close the connection
        Serial.println("Client disconnected.");
    }
}
