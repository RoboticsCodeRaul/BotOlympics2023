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
uint16_t left, front, right;

void followline();
void segueDireita();
bool check();

// Variável para determinar o valor a partir do qual deteta linha
int lightSense = 420;

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
    float battery = one.readBattery(); // read battery voltage
    one.lcd2("Battery V: ", battery);  // print data on LCD line 2
    // stuff to do while waiting for button PB1 to be pressed

    // example to get lidars values and display to Serial monitor
  }
}

//  The loop function runs over and over again forever
void loop()
{

  // para ler os valores de cada sensor do Segue Linha
  sensor0 = one.readAdc(0);
  sensor1 = one.readAdc(1);
  sensor2 = one.readAdc(2);
  sensor3 = one.readAdc(3);
  sensor4 = one.readAdc(4);
  sensor5 = one.readAdc(5);
  sensor6 = one.readAdc(6);
  sensor7 = one.readAdc(7);

  followline();
}

// Função Segue Linha
void followline()
{
  left = Lidar.getLidarLeftDistance();
  front = Lidar.getLidarFrontDistance();
  right = Lidar.getLidarRightDistance();

  if (front <= 150)
  {
    one.brake(100, 100);
    delay(200);
    one.lcd2(" Esquerda ");
    one.move(-40, 40);
    delay(400);
    while (!check())
    {
      one.lcd2("Entrei");
      segueDireita();
    }
  }
  else
  {

    if (sensor0 > lightSense)
    {
      one.move(-19, 41);
      one.lcd1("Sensor 0");
    }
    else if (sensor1 > lightSense)
    {
      one.move(11, 32);
      one.lcd1("Sensor 1");
    }
    else if (sensor2 > lightSense)
    {
      one.move(21, 35);
      one.lcd1("Sensor 2");
    }
    else if (sensor3 > lightSense)
    {
      one.move(41, 41);
      one.lcd1("Sensor 3");
    }
    else if (sensor4 > lightSense)
    {
      one.move(41, 41);
      one.lcd1("Sensor 4");
    }
    else if (sensor5 > lightSense)
    {
      one.move(39, 21);
      one.lcd1("Sensor 5");
    }
    else if (sensor6 > lightSense)
    {
      one.move(39, 7);
      one.lcd1("Sensor 6");
    }
    else if (sensor7 > lightSense)
    {
      one.move(41, -19);
      one.lcd1("Sensor 7");
    }
  }
}

void segueDireita()
{

  if (right < 100)
  {
    one.lcd2(" Afastar ");
    one.lcd2(right);
    one.move(20, 40);
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
    one.move(40, 20);
  }
  else if (right >= 210)
  {
    one.lcd2(" Curva ");
    one.lcd2(right);
    one.move(75, 1);
  }
}

bool check()
{

  for (int i = 0; i < 8; i++)
  {
    if (one.readAdc(i) > lightSense)
    {
      return true;
    }
  }

  return false;
}