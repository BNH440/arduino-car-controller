// /*
//   Battery Monitor

//   This example creates a Bluetooth® Low Energy peripheral with the standard battery service and
//   level characteristic. The A0 pin is used to calculate the battery level.

//   The circuit:
//   - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
//     Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

//   You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
//   nRF Connect (Android), to interact with the services and characteristics
//   created in this sketch.

//   This example code is in the public domain.
// */

// #include <ArduinoBLE.h>

// // Bluetooth® Low Energy Battery Service
// BLEService batteryService("180F");

// // Bluetooth® Low Energy Battery Level Characteristic
// BLEUnsignedCharCharacteristic batteryLevelChar("2A19",               // standard 16-bit characteristic UUID
//                                                BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

// int oldBatteryLevel = 0; // last battery level reading from analog input
// long previousMillis = 0; // last time the battery level was checked, in ms

// void setup()
// {
//     Serial.begin(9600); // initialize serial communication
//     while (!Serial)
//         ;

//     pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

//     // begin initialization
//     if (!BLE.begin())
//     {
//         Serial.println("starting BLE failed!");

//         while (1)
//             ;
//     }

//     /* Set a local name for the Bluetooth® Low Energy device
//        This name will appear in advertising packets
//        and can be used by remote devices to identify this Bluetooth® Low Energy device
//        The name can be changed but maybe be truncated based on space left in advertisement packet
//     */
//     BLE.setLocalName("BatteryMonitor");
//     BLE.setAdvertisedService(batteryService);           // add the service UUID
//     batteryService.addCharacteristic(batteryLevelChar); // add the battery level characteristic
//     BLE.addService(batteryService);                     // Add the battery service
//     batteryLevelChar.writeValue(oldBatteryLevel);       // set initial value for this characteristic

//     /* Start advertising Bluetooth® Low Energy.  It will start continuously transmitting Bluetooth® Low Energy
//        advertising packets and will be visible to remote Bluetooth® Low Energy central devices
//        until it receives a new connection */

//     // start advertising
//     BLE.advertise();
//     Serial.println("Bluetooth® device active, waiting for connections...     " + BLE.address());
// }

// void loop()
// {
//     // wait for a Bluetooth® Low Energy central
//     BLEDevice central = BLE.central();

//     // if a central is connected to the peripheral:
//     if (central)
//     {
//         Serial.print("Connected to central: ");
//         // print the central's BT address:
//         Serial.println(central.address());
//         // turn on the LED to indicate the connection:
//         digitalWrite(LED_BUILTIN, HIGH);

//         // check the battery level every 200ms
//         // while the central is connected:
//         while (central.connected())
//         {
//             long currentMillis = millis();
//             // if 200ms have passed, check the battery level:
//             if (currentMillis - previousMillis >= 200)
//             {
//                 previousMillis = currentMillis;
//                 updateBatteryLevel();
//             }
//         }
//         // when the central disconnects, turn off the LED:
//         digitalWrite(LED_BUILTIN, LOW);
//         Serial.print("Disconnected from central: ");
//         Serial.println(central.address());
//     }
// }

// void updateBatteryLevel()
// {
//     /* Read the current voltage level on the A0 analog input pin.
//        This is used here to simulate the charge level of a battery.
//     */
//     int battery = analogRead(A0);
//     int batteryLevel = map(battery, 0, 1023, 0, 100);

//     if (batteryLevel != oldBatteryLevel)
//     {                                             // if the battery level has changed
//         Serial.print("Battery Level % is now: "); // print it
//         Serial.println(batteryLevel);
//         batteryLevelChar.writeValue(batteryLevel); // and update the battery level characteristic
//         oldBatteryLevel = batteryLevel;            // save the level for next comparison
//     }
// }
/*
  Button LED

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED and another characteristic that
  represents the state of the button.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button connected to pin 4

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include "config.h"

void honk()
{
    tone(hornPin, 1000, 300);
}

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic hornCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic photoCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    pinMode(headlightPin, OUTPUT);
    pinMode(hornPin, OUTPUT);

    // begin initialization
    if (!BLE.begin())
    {
        Serial.println("starting Bluetooth® Low Energy module failed!");

        while (1)
            ;
    }

    // set the local name peripheral advertises
    BLE.setLocalName("RemoteCar");
    // set the UUID for the service this peripheral advertises:
    BLE.setAdvertisedService(ledService);

    // add the characteristics to the service
    ledService.addCharacteristic(ledCharacteristic);
    ledService.addCharacteristic(photoCharacteristic);
    ledService.addCharacteristic(hornCharacteristic);

    // add the service
    BLE.addService(ledService);

    ledCharacteristic.writeValue(0);
    photoCharacteristic.writeValue(0);
    hornCharacteristic.writeValue(0);

    // start advertising
    BLE.advertise();

    Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop()
{
    // poll for Bluetooth® Low Energy events
    BLE.poll();

    // read the current button pin state
    char buttonValue = analogRead(photoPin);

    // has the value changed since the last read
    bool buttonChanged = (photoCharacteristic.value() != buttonValue);

    if (buttonChanged)
    {
        // button state changed, update characteristics
        // ledCharacteristic.writeValue(buttonValue);
        photoCharacteristic.writeValue(buttonValue);
    }

    if (ledCharacteristic.written())
    {
        // update LED, either central has written to characteristic or button state has changed
        if (ledCharacteristic.value())
        {
            Serial.println("LED on");
            digitalWrite(headlightPin, HIGH);
        }
        else
        {
            Serial.println("LED off");
            digitalWrite(headlightPin, LOW);
        }
    }
    if (hornCharacteristic.written())
    {
        honk();
    }
}