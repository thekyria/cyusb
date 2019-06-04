
// Properties -> Configuration properties -> C/C++ -> General
// Additional Include Directories: $(SolutionDir)\CyUSB.prj\;%(AdditionalIncludeDirectories)
#include "cyusb.h"

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <stdint.h>

#ifndef VERBOSE
#define VERBOSE 1
#endif // VERBOSE

// Forward declerations
int write_test(CyUSB* pusb, int devId,
               unsigned int starting_cypress_address,
               const vector<uint32_t>& data );
int read_test( CyUSB* pusb, int devId,
               const unsigned int& starting_cypress_address,
               const unsigned int& words_to_read,
               vector<uint32_t>& read_buffer );

int main(int argc, char *argv[]){
  string line;
  cout << "CyUSB library test module" << endl;
  getline(cin, line);

  CyUSB* pusb = ::GetCyUSB();

  if(!pusb){
    cout << "CyUSB* pusb = ::GetCyUSB() failed ... " << endl;
  }
  cout << "CyUSB* pusb = ::GetCyUSB() successful ... " << endl;

  int ans;
  ans = pusb->init();
  cout << "init() ";
  cout << "(ans = " << ans << ")" << endl;

  int deviceCount;
  ans = pusb->deviceCount( &deviceCount );
  cout << "deviceCount() = " << deviceCount << " ";
  cout << "(ans = " << ans << ")" << endl;

  cout << endl;

  // Allocate char arrays
  char* deviceName;
  deviceName = new char[256];
  char* friendlyName;
  friendlyName = new char[256];
  wchar_t* manufacturer;
  manufacturer = new wchar_t[256];
  wchar_t* product;
  product = new wchar_t[256];
  wchar_t* serialNumber;
  serialNumber = new wchar_t[256];

  // Iterate through all devices
  for ( unsigned int k = 0 ; k != deviceCount ; ++k ){
    cout << "------------- For device #" << k << endl;

    int USBVersion = 0;
    ans = pusb->USBVersion(k, &USBVersion);
    cout << "USBVersion() = " << USBVersion << " ";
    cout << "(ans = " << ans << ")" << endl;

    int deviceClass = 0;
    ans = pusb->deviceClass(k, &deviceClass);
    cout << "deviceClass() = " << deviceClass << " ";
    cout << "(ans = " << ans << ")" << endl;
      
    ans = pusb->deviceName(k, deviceName);
    cout << "deviceName() = " << deviceName << " ";
    cout << "(ans = " << ans << ")" << endl;

    int deviceProtocol = 0;
    ans = pusb->deviceProtocol(k, &deviceProtocol);
    cout << "deviceProtocol() = " << deviceProtocol << " ";
    cout << "(ans = " << ans << ")" << endl;

    ans = pusb->friendlyName(k, friendlyName);
    cout << "friendlyName() = " << friendlyName << " ";
    cout << "(ans = " << ans << ")" << endl;

    bool isOpen = false;
    ans = pusb->isOpen(k, &isOpen);
    cout << "isOpen() = " << isOpen << " ";
    cout << "(ans = " << ans << ")" << endl;

    ans = pusb->manufacturer(k, manufacturer);
    cout << "manufacturer() = " << manufacturer << " ";
    cout << "(ans = " << ans << ")" << endl;

    int maxPower = 0;
    ans = pusb->maxPower(k, &maxPower);
    cout << "maxPower() = " << maxPower << " ";
    cout << "(ans = " << ans << ")" << endl;

    int powerState = 0;
    ans = pusb->powerState(k, &powerState);
    cout << "powerState() = " << powerState << " ";
    cout << "(ans = " << ans << ")" << endl;

    ans = pusb->product(k, product);
    cout << "product() = " << product << " ";
    cout << "(ans = " << ans << ")" << endl;

    int productId = 0;
    ans = pusb->productId(k, &productId);
    cout << "productId() = " << productId << " ";
    cout << "(ans = " << ans << ")" << endl;

    ans = pusb->serialNumber(k, serialNumber);
    cout << "serialNumber() = " << serialNumber << " ";
    cout << "(ans = " << ans << ")" << endl;

    int languageId = 0;
    ans = pusb->languageId(k, &languageId);
    cout << "languageId() = " << languageId << " ";
    cout << "(ans = " << ans << ")" << endl;

    int USBAddress = 0;
    ans = pusb->USBAddress(k, &USBAddress);
    cout << "USBAddress() = " << USBAddress << " ";
    cout << "(ans = " << ans << ")" << endl;

    unsigned long USBControlerDriver = 0;
    ans = pusb->USBControlerDriver(k, &USBControlerDriver);
    cout << "USBControlerDriver() = " << USBControlerDriver << " ";
    cout << "(ans = " << ans << ")" << endl;

    int vendorId = 0;
    ans = pusb->vendorId(k, &vendorId);
    cout << "vendorId() = " << vendorId << " " ;
    cout << "(ans = " << ans << ")" << endl;

    int endpointCount = 0;
    ans = pusb->endpointCount(k, &endpointCount);
    cout << "endpointCount() = " << endpointCount << " ";
    cout << "(ans = " << ans << ")" << endl;

    // Iterate through all endpoint of the device k
    for ( unsigned int m = 0 ; m != endpointCount ; ++m ){
      cout << " For endpoint #" << m << endl;

      int ep_address = 0;
      ans = pusb->endpoint_address(k,m, &ep_address);
      cout << "  endpoint_address() = " << ep_address << " ";
      cout << "(ans = " << ans << ")" << endl;

      int ep_attributes = 0;
      ans = pusb->endpoint_attributes(k,m, &ep_attributes);
      cout << "  endpoint_attributes() = " << ep_attributes << " ";
      cout << "(ans = " << ans << ")" << endl;

      bool ep_isIn = false;
      ans = pusb->endpoint_isIn(k,m, &ep_isIn);
      cout << "  endpoint_isIn() = " << ep_isIn << " ";
      cout << "(ans = " << ans << ")" << endl;

      int ep_maxPacketSize = 0;
      ans = pusb->endpoint_maxPacketSize(k,m, &ep_maxPacketSize);
      cout << "  endpoint_maxPacketSize() = " << ep_maxPacketSize << " ";
      cout << "(ans = " << ans << ")" << endl;

      unsigned long ep_lastErrorCode = 0;
      ans = pusb->endpoint_lastErrorCode(k,m, &ep_lastErrorCode);
      cout << "  endpoint_lastErrorCode() = " << ep_lastErrorCode << " ";
      cout << "(ans = " << ans << ")" << endl;

      unsigned long ep_timeout = 1000;
      ans = pusb->set_endpoint_timeout(k,m, ep_timeout);
      cout << "  set_endpoint_timeout() ";
      cout << "(ans = " << ans << ")" << endl;
      ep_timeout = 0;
      ans = pusb->endpoint_timeout(k,m, &ep_timeout);
      cout << "  endpoint_timeout() = " << ep_timeout << " ";
      cout << "(ans = " << ans << ")" << endl;
    }

    // Test write
    cout << "--- Testing write command ---" << endl;
    int st_address = 2;
    vector<uint32_t> wr_data;
    wr_data.push_back(23); wr_data.push_back(23); wr_data.push_back(23); wr_data.push_back(23); 
    write_test(pusb, k, st_address, wr_data);

    // Test read
    cout << "--- Testing read command ---" << endl;
    vector<uint32_t> rd_data;
    int words_to_read = 10;
    read_test(pusb, k, st_address, words_to_read, rd_data);

    cout << "----------------------------" << endl;
  }

  // Test init functionality
  // Give the user some time to modify the devices (connect/disconnect)
  cout << "Press enter key to continue ..." << endl;
  getline(cin, line);
  cout << endl;

  ans = pusb->init();
  cout << "init() ";
  cout << "(ans = " << ans << ")" << endl;

  ans = pusb->deviceCount( &deviceCount );
  cout << "deviceCount() = " << deviceCount << " ";
  cout << "(ans = " << ans << ")" << endl;

  // Deallocate memory for char arrays
  delete [] deviceName;
  delete [] friendlyName;
  delete [] manufacturer;
  delete [] product;
  delete [] serialNumber;
  pusb->destroy();

  cout << "Press enter key to exit ..." << endl;
  getline(cin, line);

  return 0;
}

int write_test(CyUSB* pusb, int devId,
               unsigned int starting_cypress_address,
               const vector<uint32_t>& data ){

  unsigned short __rddata[4096];     // Internal read buffer
  unsigned short __wrdata[4096];     // Internal write buffer

  unsigned short words_to_write = 2 * data.size();

  if (VERBOSE){
    cout << "Data to write: " << endl;
    cout << data.size() << " 32-bit words OR" << words_to_write << " 16-bit words" << endl;
    for (unsigned int k = 0 ; k != data.size() ; ++k )
      cout << "Data " << k << ": " << data[k] << endl;
  }

  // fill in write buffer
  // [      32b       ]
  // [  16b  ][  16b  ]
  // --------------
  // [ MSB:0 ][ LSB:1 ] 0
  // [ MSB:2 ][ LSB:3 ] 1
  uint32_t mask16 = static_cast<uint32_t> (0xFFFF);
  unsigned short LSB = 0;
  unsigned short MSB = 0;
  for (unsigned int k = 0 ; k != data.size() ; ++k ){
    MSB = static_cast<unsigned short>( (data[k]>>16) & mask16 );
    LSB = static_cast<unsigned short>( data[k] & mask16 );
    __wrdata[2*k + 0] = MSB;
    __wrdata[2*k + 1] = LSB;
  }
  if (VERBOSE)
    for (unsigned int k = 0 ; k != 2*data.size() ; ++k)
      cout << "Internal write buffer data " << k << ": "
            << __wrdata[k] << endl;

  // Build write packet
  int addToWrBufResult;
  unsigned short start_address = static_cast<unsigned short>(2*starting_cypress_address);
  addToWrBufResult = pusb->addWriteCommand( devId, words_to_write, start_address, __wrdata );
  if (VERBOSE)
    cout << "Building write packet ... " << addToWrBufResult << endl;

  // Send write packet
  int sendDataResult = pusb->sendData( devId, __rddata, 4096 );
  if (VERBOSE)
    cout << "Sending write packet ..." << sendDataResult << endl;

  return 0;
}

int read_test( CyUSB* pusb, int devId,
               const unsigned int& starting_cypress_address,
               const unsigned int& words_to_read,
               vector<uint32_t>& read_buffer ){

  unsigned short __rddata[4096];     // Internal read buffer

  // --- low 16-bit nibbles ---
  // Build read packet (low 16-bit nibbles)
  int addToRdBufResult;
  addToRdBufResult = pusb->addReadCommand( devId, 
        static_cast<unsigned short>(words_to_read),
        static_cast<unsigned short>(starting_cypress_address) );

  if (VERBOSE)
    cout << "Building read packet (low 16-bit nibbles) ... "
         << addToRdBufResult << endl;
  // Send read packet (low 16-bit nibbles)
  int sendDataResult = pusb->sendData( devId, __rddata, 4096 );

  if (VERBOSE){
    cout << "Size: " << words_to_read << endl;
    cout << "Result: " << sendDataResult << endl;
  }

  if (VERBOSE){
    cout << "Sending read packet (low 16-bit nibbles) ..."
         << sendDataResult << endl;
    for ( unsigned int k = 0 ; k != words_to_read ; ++k )
    cout << "Internal read buffer data (low 16-bit nibbles) " << k << ": "
        << __rddata[k] << endl;
  }

  // Fill output read_buffer with low 16-bit nibbles
  read_buffer.clear();
  for (unsigned int k = 0 ; k != words_to_read ; ++k)
    read_buffer.push_back( (uint32_t) __rddata[k] );

  // --- High 16-bit nibbles ---
  // Build read packet (high 16-bit nibbles)
  addToRdBufResult = pusb->addReadCommand( devId,
        static_cast<unsigned short>(words_to_read),
        static_cast<unsigned short>(starting_cypress_address + (1<<15)) );
  if (VERBOSE)
    cout << "Building read packet (high 16-bit nibbles) ... "
         <<addToRdBufResult << endl;
  // Send read packet (high 16-bit nibbles)
  sendDataResult = pusb->sendData( devId, __rddata, 4096);

  if (VERBOSE){
    cout << "Sending read packet (high 16-bit nibbles) ..."
         << sendDataResult << endl;
    for ( unsigned int k = 0 ; k != words_to_read ; ++k )
    cout << "Internal read buffer data (high 16-bit nibbles) " << k << ": "
         << __rddata[k] << endl;
  }

    // Fill output read_buffer with high 16-bit nibbles
  for (unsigned int k = 0 ; k != words_to_read ; ++k)
    read_buffer[k] |= (uint32_t) (__rddata[k] << 16);

  return 0;
}
