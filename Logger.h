#ifndef Logger

#include <iostream> // For input and ouput
#include <fstream> // For file handling
#include <string> // For string data type

using namespace std;

// Define a Logger class
class Logger {
public:
  // Constructor used to initialise the Logger based on given file name
  explicit Logger(const string& filename) {
    // Open the file stream with the given file name
    ofs.open(filename);
    // Write the header line in the file
    ofs << "ln_num" << "," << "value" << endl;
    
  }

  // Function to log a given index and value in the file
  void log(int idx, float value) {
    // Write the index and value in the file
    ofs << idx << "," << value << endl;
  }
  
  // Constructor to initialize the logger with a given file name and header
  Logger(const string& filename, const string& header) {
    // Open the file with the given file name
    ofs.open(filename);
    // Write to the header file
    ofs << header << endl;
  }

  // Function to close the open file
  void close()
  {
    ofs.close();
  }

private:
  // The output file used for logging
  ofstream ofs;

};

#endif