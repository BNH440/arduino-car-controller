#include <ArduinoBLE.h>
#include "config.h"

enum motorType
{
    left,
    right
};

void spinMotor(motorType motor, int speed)
{
    if (speed > 0)
    {
        if (motor == left)
        {
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
            analogWrite(PWMA, speed);
        }
        else
        {
            digitalWrite(BIN1, HIGH);
            digitalWrite(BIN2, LOW);
            analogWrite(PWMB, speed);
        }
    }
    else if (speed < 0)
    {
        if (motor == left)
        {
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, HIGH);
            analogWrite(PWMA, -speed);
        }
        else
        {
            digitalWrite(BIN1, LOW);
            digitalWrite(BIN2, HIGH);
            analogWrite(PWMB, -speed);
        }
    }
    else
    {
        if (motor == left)
        {
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, LOW);
            analogWrite(PWMA, 0);
        }
        else
        {
            digitalWrite(BIN1, LOW);
            digitalWrite(BIN2, LOW);
            analogWrite(PWMB, 0);
        }
    }
}

void forward(int speed)
{
    spinMotor(left, speed);
    spinMotor(right, speed);
}

void reverse(int speed)
{
    spinMotor(left, -speed);
    spinMotor(right, -speed);
}

void turnLeft(int speed)
{
    spinMotor(left, -speed);
    spinMotor(right, speed);
}

void turnRight(int speed)
{
    spinMotor(left, speed);
    spinMotor(right, -speed);
}

void stop()
{
    spinMotor(left, 0);
    spinMotor(right, 0);
}

void honk()
{
    tone(hornPin, 1000, 300);
}

BLEService carService("19B10010-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic headlightCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic photoCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEByteCharacteristic hornCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic motorCharacteristic("19B10014-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    pinMode(headlightPin, OUTPUT);
    pinMode(hornPin, OUTPUT);

    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);

    // begin initialization
    if (!BLE.begin())
    {
        Serial.println("starting Bluetooth Low Energy module failed!");

        while (1)
            ;
    }

    BLE.setLocalName("RemoteCar");
    BLE.setAdvertisedService(carService);

    carService.addCharacteristic(headlightCharacteristic);
    carService.addCharacteristic(photoCharacteristic);
    carService.addCharacteristic(hornCharacteristic);
    carService.addCharacteristic(motorCharacteristic);

    BLE.addService(carService);

    headlightCharacteristic.writeValue(0);
    photoCharacteristic.writeValue(0);
    hornCharacteristic.writeValue(0);
    motorCharacteristic.writeValue(0);

    BLE.advertise();

    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop()
{
    BLE.poll();

    char photoVal = analogRead(photoPin);

    // has the value changed since the last read
    bool photoChanged = (photoCharacteristic.value() != photoVal);

    if (photoChanged)
    {
        photoCharacteristic.writeValue(photoVal);
    }

    if (headlightCharacteristic.written())
    {
        if (headlightCharacteristic.value())
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

    switch (motorCharacteristic.value())
    {
    case 0:
        stop();
        break;
    case 1:
        forward(255);
        break;
    case 2:
        reverse(255);
        break;
    case 3:
        turnLeft(255);
        break;
    case 4:
        turnRight(255);
        break;
    default:
        stop();
        break;
    }
}