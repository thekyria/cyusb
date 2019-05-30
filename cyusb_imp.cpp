
#include "cyusb_imp.h"
//#include <algorithm>
//using std::copy;

void CYUSBAPI CyUSB_impl::destroy(){

  unsigned int k; //counter

  // Close all open devices
  for ( k = 0 ; k != _dev.size() ; ++k ){
    //_dev.at(k)->Close();
    delete _dev[k];
  }
  _dev.clear();

  // Clear all other memebers
  _inEndpoint.clear();
  _outEndpoint.clear();
  _rdComBuf.clear();
  _wrComBuf.clear();
  _toSendData.clear();
  _GPIFCount.clear();

  delete this;

  return;
}

int CYUSBAPI CyUSB_impl::init(){

  int ans;
  ans = _checkChange();
  if ( ans == 0 )
    // No change detected so init can return successfully
    return 0;
  // _checkChange() returning non-zero exit code signifies change detected
  // so the devices have to be reset

  // Reset status of CyUSB
  for ( unsigned int k = 0 ; k != _dev.size() ; ++k ){
    //_dev.at(k)->Close();
    delete _dev[k];
  }
  _dev.clear();
  _dev.push_back( new CCyUSBDevice() );

  _inEndpoint.clear();
  _outEndpoint.clear();
  _rdComBuf.clear();
  _wrComBuf.clear();
  _toSendData.clear();
  _GPIFCount.clear();

  UCHAR deviceCount = _dev.at(0)->DeviceCount();
  // Resize auxiliary members according to deviceCount
  _rdComBuf.resize( deviceCount );
  _wrComBuf.resize( deviceCount );
  _toSendData.resize( deviceCount, vector<unsigned short>(SIZETOSENDDATA) );
  _GPIFCount.resize( deviceCount, 0 );

  for ( UCHAR k = 0 ; k < deviceCount ; ++k ){
    // Create CCyUSBDevice for each one of the devices
    if ( k > 0 )
      // as _dev.at(0) already exists!
      _dev.push_back( new CCyUSBDevice() );

    // Open device k
    if ( !_dev.at(k)->Open( k ) )
      // Device k could not be opened properly
      return 1;
    
    if ( _dev.at(k)->EndPointCount() != 5 ){
      return 2;
    }
    _inEndpoint.push_back( (CCyUSBEndPoint *) _dev.at(k)->EndPoints[4] );
    _outEndpoint.push_back( (CCyUSBEndPoint *) _dev.at(k)->EndPoints[3] );
  }
  return 0;
}

int CYUSBAPI CyUSB_impl::deviceCount(int* ans){

  if ( _dev.size() == 0 )
    return 1;

  *ans = static_cast<int>( _dev.at(0)->DeviceCount() );
  
  return 0;
}

int CYUSBAPI CyUSB_impl::isHighSpeed(int devId, bool* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = _dev.at(devId)->bHighSpeed;

  return 0;
}

int CYUSBAPI CyUSB_impl::USBVersion(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->BcdUSB );

  return 0;
}

int CYUSBAPI CyUSB_impl::deviceClass(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->DevClass );

  return 0;
}

int CYUSBAPI CyUSB_impl::deviceName(int devId, char* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
  
  char* temp = (char*) _dev.at(devId)->DeviceName;
  strcpy(ans, temp); // safer MSVC alternative strcpy_s

  return 0;
}

int CYUSBAPI CyUSB_impl::deviceProtocol(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->DevProtocol );

  return 0;
}

//int CYUSBAPI CyUSB_impl::driverGUID(int devId, GUID* ans){return 0;}

int CYUSBAPI CyUSB_impl::friendlyName(int devId, char* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
    
  char* temp = (char*) _dev.at(devId)->FriendlyName;
  strcpy(ans, temp); // safer MSVC alternative strcpy_s

  return 0;
}

//int CYUSBAPI CyUSB_impl::deviceDescriptor(int devId, PUSB_DEVICE_DESCRIPTOR ans){return 0;}

int CYUSBAPI CyUSB_impl::isOpen(int devId, bool* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;

  *ans =  _dev.at(devId)->IsOpen();

  return 0;
}

int CYUSBAPI CyUSB_impl::manufacturer(int devId, wchar_t* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  wchar_t* temp = (wchar_t*) _dev[devId]->Manufacturer;
  wcscpy(ans, temp); // copy wide-character string

  return 0;
}

int CYUSBAPI CyUSB_impl::maxPower(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->MaxPower );

  return 0;
}

int CYUSBAPI CyUSB_impl::powerState(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->PowerState() );

  return 0;
}

int CYUSBAPI CyUSB_impl::product(int devId, wchar_t* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  wchar_t* temp = (wchar_t*) _dev[devId]->Product;
  wcscpy(ans, temp); // copy wide-character string

  return 0;
}

int CYUSBAPI CyUSB_impl::productId(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->ProductID );

  return 0;
}

int CYUSBAPI CyUSB_impl::serialNumber(int devId, wchar_t* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  wchar_t* temp = (wchar_t*) _dev[devId]->SerialNumber;
  wcscpy(ans, temp); // copy wide-character string

  return 0;
}

int CYUSBAPI CyUSB_impl::languageId(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev[devId]->StrLangID );

  return 0;
}

int CYUSBAPI CyUSB_impl::USBAddress(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->USBAddress );

  return 0;
}

int CYUSBAPI CyUSB_impl::USBControlerDriver(int devId, unsigned long* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<unsigned long>( _dev[devId]->USBDIVersion );

  return 0;
}

int CYUSBAPI CyUSB_impl::vendorId(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->VendorID );

  return 0;
}

int CYUSBAPI CyUSB_impl::endpointCount(int devId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  *ans = static_cast<int>( _dev.at(devId)->EndPointCount() );

  return 0;
}

int CYUSBAPI CyUSB_impl::endpoint_address(int devId, int epId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
  if ( epId >= _dev.at(devId)->EndPointCount() )
    return 4;

  *ans = static_cast<int>( _dev[devId]->EndPoints[epId]->Address );

  return 0;
}

int CYUSBAPI CyUSB_impl::endpoint_attributes(int devId, int epId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
  if ( epId >= _dev.at(devId)->EndPointCount() )
    return 4;

  *ans = static_cast<int>( _dev[devId]->EndPoints[epId]->Attributes );

  return 0;
}

int CYUSBAPI CyUSB_impl::endpoint_isIn(int devId, int epId, bool* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
  if ( epId >= _dev.at(devId)->EndPointCount() )
    return 4;

  *ans = _dev[devId]->EndPoints[epId]->bIn;

  return 0;
}

int CYUSBAPI CyUSB_impl::endpoint_maxPacketSize(int devId, int epId, int* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
  if ( epId >= _dev.at(devId)->EndPointCount() )
    return 4;

  *ans = static_cast<int>( _dev[devId]->EndPoints[epId]->MaxPktSize );

  return 0;
}

int CYUSBAPI CyUSB_impl::endpoint_lastErrorCode(int devId, int epId, unsigned long* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
  if ( epId >= _dev.at(devId)->EndPointCount() )
    return 4;

  *ans = static_cast<unsigned long>( _dev[devId]->EndPoints[epId]->NtStatus );

  return 0;
}

int CYUSBAPI CyUSB_impl::endpoint_timeout(int devId, int epId, unsigned long* ans){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
  if ( epId >= _dev.at(devId)->EndPointCount() )
    return 4;

  *ans = static_cast<unsigned long>( _dev[devId]->EndPoints[epId]->TimeOut );

  return 0;
}

int CYUSBAPI CyUSB_impl::set_endpoint_timeout(int devId, int epId, unsigned long val){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;
  if ( epId >= _dev.at(devId)->EndPointCount() )
    return 4;

  _dev[devId]->EndPoints[epId]->TimeOut = static_cast<ULONG>(val);

  return 0;
}

int CYUSBAPI CyUSB_impl::addWriteCommand(int devId,
                                         unsigned short word_count,
                                         unsigned short start_address,
                                         unsigned short* p_data){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  // Argument validation
  if ( word_count == 0 )
    return 4;
  if ( _wrComBuf[devId].size()+(word_count+2) > SIZEBUFFERWRITE )
    return 5;

  // Add write command proper
  _wrComBuf[devId].push_back( word_count );
  _wrComBuf[devId].push_back( start_address );
  for ( unsigned int k = 0 ; k != word_count; ++k )
    _wrComBuf[devId].push_back( p_data[k] );

  return 0;
}

int CYUSBAPI CyUSB_impl::addReadCommand(int devId, unsigned short word_count, unsigned short start_address){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  // Argument validation
  if ( word_count == 0 )
    return 4;
  if ( _rdComBuf[devId].size()+2 > SIZEBUFFERREAD )
    return 5;
  if ( _GPIFCount[devId]+word_count+2 > MAXGPIFCOUNTER )
    return 6;

  // Add read command proper
  _rdComBuf[devId].push_back( word_count );
  _rdComBuf[devId].push_back( start_address );
  _GPIFCount[devId] += word_count+2;

  return 0;
}

int CYUSBAPI CyUSB_impl::sendData(int devId,
                                  unsigned short* p_readData,
                                  unsigned short readData_dim1){

  if ( _dev.size() == 0 )
    return 1;
  if ( devId >= _dev.at(0)->DeviceCount() )
    return 2;
  if ( !_dev.at(devId)->IsOpen() )
    return 3;

  int ans = 0;
  long pos = 0; // counter
  long outlen;
  long inlen;
  for ( unsigned int k = 0 ; k != _toSendData[devId].size(); ++k )
    _toSendData[devId][k] = 0;

  if ( _rdComBuf[devId].size() > 0 ) {
    _toSendData[devId][pos++] = _rdComBuf[devId].size();
    _toSendData[devId][pos++] = 0xFFFF;

    for ( unsigned int k = 0 ; k != _rdComBuf[devId].size() ; ++k ){
      _toSendData[devId][pos++] = _rdComBuf[devId][k];
    }
  }

  for ( unsigned int k = 0 ; k != _wrComBuf[devId].size() ; ++k ){
    _toSendData[devId][pos++] = _wrComBuf[devId][k];
  }

  if( _rdComBuf[devId].size() > 0 )
    outlen = 4 + 2*_rdComBuf[devId].size();
  else
    outlen = 0;
  outlen += 2*_wrComBuf[devId].size();

  unsigned short* temp_out = new unsigned short[outlen/2];
  for ( unsigned int k = 0 ; k != outlen/2 ; ++k )
    temp_out[k] = _toSendData[devId][k];
  //copy(_toSendData[devId].begin(), _toSendData[devId].begin()+outlen, temp_out);
  _outEndpoint[devId]->XferData((UCHAR*) temp_out, outlen );
  delete temp_out;

  // If a read command has already been issued
  if( _rdComBuf[devId].size() > 0 ){
    // Each command consists of two entries in the _rdComBuf[devId], so:
    unsigned int numberOfReadCommands = _rdComBuf[devId].size()/2;
    inlen = 2*_GPIFCount[devId];
    _inEndpoint[devId]->XferData((UCHAR *) _inData,inlen);

    // apo prin SortInData ////////////
    int	CurrentPos=0;
    for ( unsigned int k = 0; k < numberOfReadCommands ; ++k ){
      int word_count = _rdComBuf[devId][2*k];
	    CurrentPos += 2;
	    for ( int m = 0; m < word_count ; ++m ) {
	      p_readData[(readData_dim1*k)+m] = _inData[CurrentPos++];
	    }
    }
    ////////////////////////////////////
    ans = 0;
  }

  _wrComBuf[devId].clear();
  _rdComBuf[devId].clear();
  _GPIFCount[devId]=0;

  return ans;
}

int CYUSBAPI CyUSB_impl::_checkChange(){

  if ( _dev.size() == 0 )
    // No CCyUSBDevices present! At least one needed to perform update()
    return 1;

  UCHAR deviceCount = _dev.at(0)->DeviceCount();
  if ( _dev.size() == deviceCount )
    // Device count and instantiated CCyUSBDevice number does not match!
    return 2;

  CCyUSBDevice* tempdev = new CCyUSBDevice();
  for ( unsigned int k = 0 ; k != _dev.size() ; ++k ){
    tempdev->Open(k);
    if ( _dev[k]->VendorID != tempdev->VendorID ){
      delete tempdev;
      return 30;
    }
    if ( _dev[k]->ProductID != tempdev->ProductID ){
      delete tempdev;
      return 31;
    }
    if ( _dev[k]->USBAddress != tempdev->USBAddress ){
      delete tempdev;
      return 32;
    }
    if ( _dev[k]->EndPointCount() != tempdev->EndPointCount() ){
      delete tempdev;
      return 33;
    }
    if ( !_dev[k]->IsOpen() ){
      delete tempdev;
      return 35;
    }
  }
  delete tempdev;

  return 0;
}
