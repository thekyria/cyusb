
#ifndef CYUSB_H
#define CYUSB_H

#ifndef CYUSBAPI
#define CYUSBAPI_OPTION 1
#define CYUSBAPI __stdcall
#endif // CYUSBAPI
// alternative(s): CYUSBAPI_OPTION 2, CYUSBAPI __cdecl

#ifndef CYDLL
#ifdef CYDLL_EXPORT
#define CYDLL __declspec(dllexport)
#else
#define CYDLL __declspec(dllimport)
#endif // CYDLL_EXPORT
#endif // CYDLL

class CyUSB{

 public:

  // Creation/destruction
  virtual void CYUSBAPI destroy() = 0;
  virtual int CYUSBAPI init() = 0;

  // Information
  virtual int CYUSBAPI deviceCount(int* ans) = 0;
  // --- Device specific ---
  virtual int CYUSBAPI isHighSpeed(int devId, bool* ans) = 0;
  virtual int CYUSBAPI USBVersion(int devId, int* ans) = 0;
  virtual int CYUSBAPI deviceClass(int devId, int* ans) = 0;
  virtual int CYUSBAPI deviceName(int devId, char* ans) = 0;
  virtual int CYUSBAPI deviceProtocol(int devId, int* ans) = 0;
  //virtual int CYUSBAPI driverGUID(int devId, GUID* ans) = 0;
  virtual int CYUSBAPI friendlyName(int devId, char* ans) = 0;
  //virtual int CYUSBAPI deviceDescriptor(int devId, PUSB_DEVICE_DESCRIPTOR ans) = 0;
  virtual int CYUSBAPI isOpen(int devId, bool* ans) = 0;
  virtual int CYUSBAPI manufacturer(int devId, wchar_t* ans) = 0;
  virtual int CYUSBAPI maxPower(int devId, int* ans) = 0;
  virtual int CYUSBAPI powerState(int devId, int* ans) = 0;
  virtual int CYUSBAPI product(int devId, wchar_t* ans) = 0;
  virtual int CYUSBAPI productId(int devId, int* ans) = 0;
  virtual int CYUSBAPI serialNumber(int devId, wchar_t* ans) = 0;
  virtual int CYUSBAPI languageId(int devId, int* ans) = 0;
  virtual int CYUSBAPI USBAddress(int devId, int* ans) = 0;
  virtual int CYUSBAPI USBControlerDriver(int devId, unsigned long* ans) = 0;
  virtual int CYUSBAPI vendorId(int devId, int* ans) = 0;
  // --- Endpoint specific ---
  virtual int CYUSBAPI endpointCount(int devId, int* ans) = 0;
  virtual int CYUSBAPI endpoint_address(int devId, int epId, int* ans) = 0;
  virtual int CYUSBAPI endpoint_attributes(int devId, int epId, int* ans) = 0;
  virtual int CYUSBAPI endpoint_isIn(int devId, int epId, bool* ans) = 0;
  virtual int CYUSBAPI endpoint_maxPacketSize(int devId, int epId, int* ans) = 0;
  virtual int CYUSBAPI endpoint_lastErrorCode(int devId, int epId, unsigned long* ans) = 0;
  virtual int CYUSBAPI endpoint_timeout(int devId, int epId, unsigned long* ans) = 0;
  virtual int CYUSBAPI set_endpoint_timeout(int devId, int epId, unsigned long val) = 0;

  // Read/write
  virtual int CYUSBAPI addWriteCommand(int devId, unsigned short word_count, unsigned short start_address, unsigned short* p_data) = 0;
  virtual int CYUSBAPI addReadCommand(int devId, unsigned short word_count, unsigned short start_address) = 0;
  virtual int CYUSBAPI sendData(int devId, unsigned short* p_readData, unsigned short readData_dim1) = 0;

};

extern "C" CYDLL CyUSB* CYUSBAPI GetCyUSB(void);

#endif // CYUSB_H
