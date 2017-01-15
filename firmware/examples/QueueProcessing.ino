/*
  NAME:
  Basic usage of QueueProcessing library

  DESCRIPTION:
  This sketch demonstrates the basic usage of the library.
  * As an event value the sketch employs the RSSI (Received signal
    strength indication) of the wifi radio in negative integers by default
    from -1 to -127 dBm, but with positive integers at errors; however, this
    sketch does not recognizes them.
  * For demonstration purposes the used measuring period is long and each
    measured data point is displayed.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/

#include "queue-processing/queue-processing.h"
#define SKETCH "QUEUEPROCESSING 1.0.0"

const unsigned int PERIOD_PUBLISH_PARTICLE = 10000;   // Milliseconds
QueueProcessing particleQueue;

void setup()
{
    // Put sketch and libraries identification to the queue
    particleQueue.enqueue("Sketch", String(SKETCH));
    particleQueue.enqueue("Library", String(QUEUEPROCESSING_VERSION));
}

void loop()
{
  measure();                // Prepare events to the queue
  particleQueue.publish();  // Publish events to the Particle Cloud
}

void measure()
{
    static unsigned long timeStamp;
    if (millis() - timeStamp >= PERIOD_PUBLISH_PARTICLE || timeStamp == 0)
    {
        timeStamp = millis();
        // Put regular event to the queue
        particleQueue.enqueue("RSSI", String(WiFi.RSSI()));
    }
}
