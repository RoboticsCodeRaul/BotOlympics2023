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

void segueLinha();
void desviaParede();
bool check();

// Variável para determinar o valor a partir do qual deteta linha
int lightSense = 500;

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
  front = Lidar.getLidarFrontDistance();
  one.lcd1(front);
  // segueLinha();
  /* unsigned long currentMillis = millis(); // store the current time

  if (currentMillis - previousMillis >= 100)
  {                                 // check if 1000ms passed
    previousMillis = currentMillis; // save the last time you blinked the LED
    front = Lidar.getLidarFrontDistance();
  } */

  // front = Lidar.getLidarFrontDistance();

  // left = Lidar.getLidarLeftDistance();

  // front = Lidar.getLidarFrontDistance();
  // delay(10);

  if (front < 110)
  {
    one.brake(100, 100);
    delay(200);
    one.move(-40, 40);
    delay(270);
    while (!check())
    {
      desviaParede();
    }
    one.brake(100, 100);
    delay(200);
    one.move(-40, 40);
    delay(300);
  }
  else
  {
    segueLinha();
  }
}

bool check()
{

  for (int i = 0; i < 8; i++)
  {
    if (one.readAdc(i) >= lightSense)
    {
      return true;
    }
  }

  return false;
}

void desviaParede()
{
  right = Lidar.getLidarRightDistance();
  if (right > 300)
  {
    one.move(70, 5);
  }
  else if (right < 100)
  {
    one.move(20, 30);
  }
  else if (right >= 100 && right <= 200)
  {
    one.move(30, 30);
  }
  else if (right > 200 && right <= 300)
  {
    one.move(30, 20);
  }
  delay(30);
}

void segueLinha()
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

  if (sensor0 >= lightSense)
  {
    one.move(-10, 20);
    // one.lcd1("Sensor 0");
  }
  else if (sensor7 >= lightSense)
  {
    one.move(20, -10);
    // one.lcd1("Sensor 7");
  }
  else if (sensor1 >= lightSense)
  {
    one.move(5, 16);
    // one.lcd1("Sensor 1");
  }
  else if (sensor6 >= lightSense)
  {
    one.move(16, 5);
    // one.lcd1("Sensor 6");
  }
  else if (sensor2 >= lightSense)
  {
    one.move(10, 17);
    // one.lcd1("Sensor 2");
  }
  else if (sensor5 >= lightSense)
  {
    one.move(17, 10);
    // one.lcd1("Sensor 5");
  }
  else if (sensor3 >= lightSense)
  {
    one.move(20, 20);
    // one.lcd1("Sensor 3");
  }
  else if (sensor4 >= lightSense)
  {
    one.move(20, 20);
    // one.lcd1("Sensor 4");
  }
  delay(30);
}