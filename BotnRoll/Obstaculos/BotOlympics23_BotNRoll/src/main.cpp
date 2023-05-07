/* PRIVATE CODE BEGIN Includes */
#include <Wire.h>
#include <BnrOneA.h>
#include <Arduino.h>
#include <LiDAR.h>

/* PRIVATE CODE BEGIN DEFINE -------------------------- */
#define SSPIN 2

/* PRIVATE CODE BEGIN CLASSES -------------------------- */
BnrOneA one;
int sensor0, sensor1, sensor2, sensor3, sensor4, sensor5, sensor6, sensor7;
uint16_t left;
uint16_t front;
uint16_t right;
int LightSense = 600;

bool SegueEsq = false;
void SegueDireita();
void SegueEsquerda();

//  The setup function runs once when you press reset or power the board
void setup()
{
    /* PRIVATE CODE BEGIN Initializations -------------------------- */
    Serial.begin(9600);        // set baud rate to 9600bps for printing values at serial monitor.
    one.spiConnect(SSPIN);     // start SPI communication module
    one.stop();                // stop motors
    one.obstacleEmitters(OFF); // desactivate IR emitters
    one.minBat(10.5);          // set batery treshold
    Lidar.begin();
    /* USER CODE BEGIN Initializations */

    // place your initializations here

    /* USER CODE END Initializations */

    Lidar.scanI2C();

    while (one.readButton() != 1)
    {
        // stuff to do while waiting for button PB1 to be pressed

        // example to get lidars values and display to Serial monitor

        /*
            Serial.println("LINE SENSOR VALUES: ");
            int sensor0 = one.readAdc(0);
            int sensor1 = one.readAdc(1);
            int sensor2 = one.readAdc(2);
            int sensor3 = one.readAdc(3);
            int sensor4 = one.readAdc(4);
            int sensor5 = one.readAdc(5);
            int sensor6 = one.readAdc(6);
            int sensor7 = one.readAdc(7); */

        // Print values on Serial Monitor
        /*         Serial.print(sensor0);
                Serial.print(" ");
                Serial.print(sensor1);
                Serial.print(" ");
                Serial.print(sensor2);
                Serial.print(" ");
                Serial.print(sensor3);
                Serial.print(" ");
                Serial.print(sensor4);
                Serial.print(" ");
                Serial.print(sensor5);
                Serial.print(" ");
                Serial.print(sensor6);
                Serial.print(" ");
                Serial.print(sensor7);
                Serial.print(" ");
                Serial.println();
                delay(2000); // wait 2sec */
    }
}

//  The loop function runs over and over again forever
void loop()

{
    sensor0 = one.readAdc(0);
    sensor1 = one.readAdc(1);
    sensor2 = one.readAdc(2);
    sensor3 = one.readAdc(3);
    sensor4 = one.readAdc(4);
    sensor5 = one.readAdc(5);
    sensor6 = one.readAdc(6);
    sensor7 = one.readAdc(7);

    left = Lidar.getLidarLeftDistance();
    front = Lidar.getLidarFrontDistance();
    right = Lidar.getLidarRightDistance();

    Serial.println("DISTANCE SENSOR VALUES: ");

    Serial.println(
        "Left: " + String(left) + "mm" +
        "\nFront: " + String(front) + "mm" +
        "\nRight: " + String(right) + "mm");
    if (SegueEsq == false)
    {
        SegueDireita();
    }
    else
    {
        SegueEsquerda();
    }

    if (sensor0 >= LightSense || sensor1 >= LightSense ||
        sensor2 >= LightSense || sensor3 >= LightSense ||
        sensor4 >= LightSense || sensor5 >= LightSense ||
        sensor6 >= LightSense || sensor7 >= LightSense)
    {
        if (SegueEsq == false)
        {
            SegueEsq = true;
        }
        else
        {
            SegueEsq = false;
        }
        delay(200);
    }

    /*    if (front < 15) {
       one.lcd2(" Para ");
       one.move(0, 0);
       delay(500);
       one.move(15,-15);
       delay(1000);
       } */
}

void SegueDireita()
{
    if (front > 190)
    {

        if (right < 100)
        {
            one.lcd2(" Afastar ");
            one.lcd2(right);
            one.move(25, 40);
        }
        else if (right >= 100 && right <= 130)
        {
            one.lcd2(" Frente ");
            one.lcd2(right);
            one.move(30, 30);
        }
        else if (right >= 130 && right <= 210)
        {
            one.lcd2(" Aprox ");
            one.lcd2(right);
            one.move(40, 25);
        }
        else if (right >= 210)
        {
            one.lcd2(" Aprox ");
            one.lcd2(right);
            one.move(75, 3);
        }
    }
    else
    {
        if (right < 210 && left > 210)
        {
            one.lcd2(" Esquerda ");
            one.lcd2(left);
            one.lcd2(" ");
            one.lcd2(right);
            one.move(-20, 30);
            delay(115);
        }
        else if (left < 175 && right < 175)
        {
            one.lcd2(" 180º "); // virar 180
            one.lcd2(left);
            one.lcd2(" ");
            one.lcd2(right);
            one.move(-30, 30);
            delay(600);
        }
    }
}

void SegueEsquerda()
{
    if (front > 275)
    {

        if (left < 100)
        {
            one.lcd2(" Afastar ");
            one.lcd2(left);
            one.move(40, 25);
        }
        else if (left >= 100 && left <= 130)
        {
            one.lcd2(" Frente ");
            one.lcd2(left);
            one.move(40, 40);
        }
        else if (left >= 130 && left <= 210)
        {
            one.lcd2(" Aprox ");
            one.lcd2(left);
            one.move(25, 40);
        }
        else if (left >= 210)
        {
            one.lcd2(" Aprox ");
            one.lcd2(left);
            one.move(1, 65);
        }
    }
    else
    {
        if (left < 210 && right > 210)
        {
            one.lcd2(" Direita ");
            one.lcd2(right);
            one.lcd2(" ");
            one.lcd2(left);
            one.move(70, 1);
            /*          delay(90); */
        }
        else if (right < 175 && left < 175)
        {
            one.lcd2(" 180º "); // virar 180
            one.lcd2(right);
            one.lcd2(" ");
            one.lcd2(left);
            one.move(-30, 30);
            delay(650);
            one.move(0, 0);
        }
    }
}