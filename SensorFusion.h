#ifndef SensorFusion

#include <iostream> // For input and output
#include <fstream> // for file handling
#include <string> // for string handling
#include <cmath> // for math functions.

// Define a sensorFusion class
class SensorFusion {

public:
  // Class constructor
  SensorFusion() {
    sensor2value_previous = -1;
  }

  // Public variables of the class
  double sensor1value;
  double sensor2value;
  double sensor2value_previous;
  float sensor2value_Current;

  double sensor1_scaledValue;
  double sensor2_scaledValue;
  double sensor3_scaledValue;

  // constants defined in the class
  const float sensor1gain = 2.7f;
  const float sensor1offset = 1.0f;
  const float sensor2gain = 0.7f;
  const float sensor2offset = -0.5f;
  const float sensor3gain = 1.0f;
  const float sensor3offset = 0.2f;

  // public  variables
  float sensorFusion;

  double motorStates;
};

#endif