
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "SensorFusion.h" // include SensorFusion header file
#include "Logger.h" // include Logger header file

using namespace std;

SensorFusion mySensorFusion; // instantiate a sensorFusion object

int lineNumber = 1; // initialise a line number variable to 1


// The following code defines two C++ enumerations that are used to identify different sensors and log points in the system.
// The "sensorId" enum contains values for each sensor in the system, while the "LogPoint" enum identifies logging points.
enum sensorId{
  sensor_1,
  sensor_2,
  sensor_3
};

enum LogPoint{
  log_A, // Sensor 1 converted data
  log_B, // Sensor 2 converted data
  log_C, // Sensor 1 scaled data
  log_D, // Sensor 2 scaled data
  log_E, // Sensor 3 scaled data
  log_F, // Sensor Fusion output
};

int main() {
  // open sensor_1.csv, sensor_2.csv, sensor_3.csv files for reading
  ifstream sensor_1("sensor_1.csv"); 
  ifstream sensor_2("sensor_2.csv");
  ifstream sensor_3("sensor_3.csv");

  // utilise Logger class by creating Logger objects for each required point.
  Logger log_A("LOG_POINT_A.csv", "ln_num,S1 values");
  Logger log_B("LOG_POINT_B.csv", "ln_num,S2 values");
  Logger log_C("LOG_POINT_C.csv", "ln_num,S3 values");
  Logger log_D("LOG_POINT_D.csv", "ln_num,S1 scaled");
  Logger log_E("LOG_POINT_E.csv", "ln_num,S2 scaled");
  Logger log_F("LOG_POINT_F.csv", "ln_num,S Fusion");

  ofstream motor_output("motor_output.csv"); // open motor_output.csv file for writing

  // column headers in motor_output.csv
  motor_output << "lineNumber" << "," << "Motor A" << "," << "Motor B" << endl;

  // strings for reading lines from sensor files are initialized
  string line1, line2, line3;

  // read lines from sensor files until end of file any sensor file is reached. Avoids out-of-bounds error
  while (getline(sensor_1, line1) && getline(sensor_2, line2) && getline(sensor_3, line3)) {

    string sensor1_values, sensor2_values, sensor3_values;

    // find comma positions in each line
    size_t comma_pos1 = line1.find(",");
    size_t comma_pos2 = line2.find(",");
    size_t comma_pos3 = line3.find(",");

    // extract sensor values from the line
    if (comma_pos1 != string::npos && comma_pos2 != string::npos && comma_pos3 != string::npos)
    {
      sensor1_values = line1.substr(comma_pos1 + 1);
      sensor2_values = line2.substr(comma_pos2 + 1);
      sensor3_values = line3.substr(comma_pos3 + 1);
    }

    // if all sensor values are not empty
    if (!sensor1_values.empty() && !sensor2_values.empty() && !sensor3_values.empty())
    {
      // calculate and store sensor values after scaling
      mySensorFusion.sensor1value = 2.0 / 3.0 * sqrt(stof(sensor1_values));

      mySensorFusion.sensor2value_Current = stof(sensor2_values);
      mySensorFusion.sensor2value = mySensorFusion.sensor2value_Current - mySensorFusion.sensor2value_previous;
      mySensorFusion.sensor2value_previous = mySensorFusion.sensor2value_Current;

      // scale the sensor values based on the given gain and offest values.
      mySensorFusion.sensor1_scaledValue = mySensorFusion.sensor1gain * ((mySensorFusion.sensor1value) - mySensorFusion.sensor1offset);
      mySensorFusion.sensor2_scaledValue = mySensorFusion.sensor2gain * ((mySensorFusion.sensor2value) - mySensorFusion.sensor2offset);
      mySensorFusion.sensor3_scaledValue = mySensorFusion.sensor3gain * (stof(sensor3_values) - mySensorFusion.sensor3offset);

      if (mySensorFusion.sensor2_scaledValue == 0) {
        mySensorFusion.sensor2_scaledValue = 1e-5f;
      }

      mySensorFusion.sensorFusion = ((3.0 * mySensorFusion.sensor1_scaledValue - mySensorFusion.sensor3_scaledValue) / (mySensorFusion.sensor2_scaledValue) - 3.0);
      mySensorFusion.sensorFusion = max(min(mySensorFusion.sensorFusion, 3.0f), -3.0f);

      // log sensor data to csv files
      log_A.log(lineNumber, mySensorFusion.sensor1value);
      log_B.log(lineNumber, mySensorFusion.sensor2value);
      log_C.log(lineNumber, mySensorFusion.sensor3_scaledValue);
      log_D.log(lineNumber, mySensorFusion.sensor1_scaledValue);
      log_E.log(lineNumber, mySensorFusion.sensor2_scaledValue);
      log_F.log(lineNumber, mySensorFusion.sensorFusion);

      // calulate and write the motor output values to the motor_output.csv file
      mySensorFusion.motorStates = max(min(mySensorFusion.sensorFusion, 1.0f), -1.0f); 
      motor_output << lineNumber << "," << mySensorFusion.motorStates << "," << mySensorFusion.motorStates * -1 << endl;      
    }

    lineNumber++;
    
  }

  // close all open files
  sensor_1.close();
  sensor_2.close();
  sensor_3.close();
  log_A.close();
  log_B.close();
  log_C.close();
  log_D.close();
  log_E.close();
  log_F.close();
  motor_output.close();

  return 0;
}
