// Bounce.pde
// -*- mode: C++ -*-
//
// Make a single stepper bounce from one limit to another
//
// Copyright (C) 2012 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
const int dirPin = 2;
const int stepPin = 0;
#define motorInterfaceType 1

AccelStepper stepper(motorInterfaceType, stepPin, dirPin); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup()
{  
  // Change these to suit your stepper if you want
  pinMode(4,OUTPUT);
  pinMode(dirPin,OUTPUT);
  digitalWrite(dirPin,HIGH);
  digitalWrite(4,LOW);
  stepper.setMaxSpeed(30000);
  stepper.setAcceleration(500);
  stepper.setSpeed(20000);
  stepper.moveTo(50000);
}

void loop()
{
    // If at the end of travel go to the other end
    
    if (stepper.distanceToGo() == 0)
       stepper.moveTo(-stepper.currentPosition());

    stepper.run();
}
