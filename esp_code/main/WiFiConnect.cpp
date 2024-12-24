#include "WiFiConnect.h"
#include "Arduino.h"
#include "WiFi.h"

String readStringUntilNewLine() {
    String ret;
    int c = Serial.read();
    while (c >= 0 && (char)c != '\r') {
        ret += (char)c;
        while (!Serial.available()) {
            delay(100); // Wait for password input
        }
        c = Serial.read();
    }
    return ret;
}

bool WiFiConnect::WiFi_init() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Scan start");

    // WiFi.scanNetworks will return the number of networks found.
    int numNetworks = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (numNetworks == 0) {
        Serial.println("no networks found");

        WiFi.scanDelete();
        return false;
    } else {
        Serial.print(numNetworks);
        Serial.println(" networks found");
        Serial.println(
            "Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < numNetworks; ++i) {
            // Print SSID and RSSI for each network found
            Serial.printf("%2d", i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4ld", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2ld", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i)) {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }

    // Start connecting
    Serial.println("Enter the number of the network to connect to:");
    while (!Serial.available()) {
        delay(100); // Wait for user input
    }

    // Read the user input
    int selectedNetwork = readStringUntilNewLine().toInt();
    if (selectedNetwork < 1 || selectedNetwork > numNetworks) {
        Serial.println("Invalid selection.");
        return false;
    }

    // Connect to the selected network
    String ssid = WiFi.SSID(selectedNetwork - 1);
    String password = "";

    // Ask for password if necessary (for secured networks)
    if (WiFi.encryptionType(selectedNetwork - 1) != WIFI_AUTH_OPEN) {
        Serial.print("Enter password for network ");
        Serial.println(ssid);
        while (!Serial.available()) {
            delay(100); // Wait for password input
        }
        password = readStringUntilNewLine();
        password.trim();
    }

    // Attempt to connect to the selected Wi-Fi network
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to Wi-Fi");

    // Wait for connection
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        Serial.print(".");
        delay(500);
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected successfully!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        // Delete the scan result to free memory for code below.
        WiFi.scanDelete();
        return true;
    } else {
        Serial.println("\nFailed to connect.");
        WiFi.disconnect();
        // Delete the scan result to free memory for code below.
        WiFi.scanDelete();
        return false;
    }
}
