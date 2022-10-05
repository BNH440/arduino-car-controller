// // #include "socket.ino"

// #include "config.h"

// // enum motorType
// // {
// //     left,
// //     right
// // };

// // void spinMotor(motorType motor, int speed)
// // {
// //     if (speed > 0)
// //     {
// //         if (motor == left)
// //         {
// //             digitalWrite(AIN1, HIGH);
// //             digitalWrite(AIN2, LOW);
// //             analogWrite(PWMA, speed);
// //         }
// //         else
// //         {
// //             digitalWrite(BIN1, HIGH);
// //             digitalWrite(BIN2, LOW);
// //             analogWrite(PWMB, speed);
// //         }
// //     }
// //     else if (speed < 0)
// //     {
// //         if (motor == left)
// //         {
// //             digitalWrite(AIN1, LOW);
// //             digitalWrite(AIN2, HIGH);
// //             analogWrite(PWMA, -speed);
// //         }
// //         else
// //         {
// //             digitalWrite(BIN1, LOW);
// //             digitalWrite(BIN2, HIGH);
// //             analogWrite(PWMB, -speed);
// //         }
// //     }
// //     else
// //     {
// //         if (motor == left)
// //         {
// //             digitalWrite(AIN1, LOW);
// //             digitalWrite(AIN2, LOW);
// //             analogWrite(PWMA, 0);
// //         }
// //         else
// //         {
// //             digitalWrite(BIN1, LOW);
// //             digitalWrite(BIN2, LOW);
// //             analogWrite(PWMB, 0);
// //         }
// //     }
// // }

// // void setup()
// // {
// //     pinMode(AIN1, OUTPUT);
// //     pinMode(AIN2, OUTPUT);
// //     pinMode(PWMA, OUTPUT);

// //     pinMode(BIN1, OUTPUT);
// //     pinMode(BIN2, OUTPUT);
// //     pinMode(PWMB, OUTPUT);

// //     Serial.begin(9600);
// // }

// // void loop()
// // {
// // }

// void setup()
// {
//     pinMode(photoPin, INPUT);
//     pinMode(headlightPin, OUTPUT);

//     Serial.begin(9600);
// }

// void loop()
// {
//     int photoValue = analogRead(photoPin);
//     if (Serial.available() > 0)
//     {
//         Serial.println(Serial.readString());
//     }
//     // Serial.println(analogRead(photoPin));
// }