#pragma once

//#define USE_USB 1

#if defined(USE_USB)

#include "LinkedList.h"
#include "Numbers.h"

#pragma warning( push )
#pragma warning( disable : 4200 4482 )
#include <libusb.h>
#pragma warning( pop )

class USBDevice : public ListItem {
public:
 libusb_device *device;
 libusb_device_handle *handle;
 libusb_device_descriptor descriptor;
 libusb_config_descriptor *active,*config;
 Integers claims;
 Zint bus,port,address,speed;
 Zint channel,result;
 Zbool busy,oom;
 Zstring manufacturer,product;
 USBDevice( libusb_device *d ) : ListItem() {
  device=d;
  handle=NULL;
  active=NULL;
  config=NULL;
  Poll();
 }
 USBDevice() : ListItem() {
  device=NULL;
  handle=NULL;
  active=NULL;
  config=NULL;
 }
 string GetError( int err ) {
  switch ( err ) {
              case LIBUSB_SUCCESS: return string("");
             case LIBUSB_ERROR_IO: return string("I/O Error");
  case LIBUSB_ERROR_INVALID_PARAM: return string("Invalid Parameter");
         case LIBUSB_ERROR_ACCESS: return string("Access denied (insufficient permissions)");
      case LIBUSB_ERROR_NO_DEVICE: return string("No such device (it may have been disconnected)");
      case LIBUSB_ERROR_NOT_FOUND: return string("Entity not found");
           case LIBUSB_ERROR_BUSY: return string("Resource busy");
        case LIBUSB_ERROR_TIMEOUT: return string("Timed out");
       case LIBUSB_ERROR_OVERFLOW: return string("Overflow");
           case LIBUSB_ERROR_PIPE: return string("Pipe error");
    case LIBUSB_ERROR_INTERRUPTED: return string("System call interrupted (perhaps due to signal)");
         case LIBUSB_ERROR_NO_MEM: return string("Insufficient memory");
  case LIBUSB_ERROR_NOT_SUPPORTED: return string("Operation not supported or unimplemnted on this platform");
          case LIBUSB_ERROR_OTHER: return string("Other error");
                          default: return string("Unknown error");
  }
 }
 string GetSpeedName() {
  switch ( speed.value ) {
  case LIBUSB_SPEED_UNKNOWN: return string("Unknown");
  case LIBUSB_SPEED_LOW:     return string("Low");
  case LIBUSB_SPEED_FULL:    return string("Full");
  case LIBUSB_SPEED_HIGH:    return string("High");
  case LIBUSB_SPEED_SUPER:   return string("Super");
                    default: return string("Not known");
  }
 }
 bool GetDeviceDescriptor() {
  result = libusb_get_device_descriptor(device,&descriptor);
  if ( result < 0 ) {
   OUTPUT("USBDevice:Open(%d): Could not get device descriptor: %s\n", channel, GetError(result).c_str() );
   return false;
  }
  return true;
 }
 bool GetDeviceConfiguration( int index=0 ) {
  result = libusb_get_config_descriptor(device,(uint8_t)index,&config);
  if ( result < 0 ) {
   OUTPUT("USBDevice:GetDeviceConfiguration: Error: %s\n", GetError(result).c_str() );
   return false;
  }
  return true;
 }
 bool GetDeviceActiveConfiguration() {
  result = libusb_get_active_config_descriptor(device,&active);
  if ( result < 0 ) {
   OUTPUT("USBDevice:GetDeviceActiveConfiguration: Error: %s\n", GetError(result).c_str() );
   return false;
  }
  return true;
 }
 bool SetDeviceConfiguration( int configuration ) {
  if ( !handle ) {
   OUTPUT("USBDevice:GetDeviceActiveConfiguration: No handle\n" );
   return false;
  }
  result=libusb_set_configuration(handle,configuration);
  if ( result < 0 ) {
   OUTPUT("USBDevice:GetDeviceActiveConfiguration: Error: %s\n", GetError(result).c_str() );
   return false;
  }
  return true;
 }
 bool Claim( int channel=0 ) {
  if ( !handle ) {
   OUTPUT("USBDevice:Claim(%d): No handle\n", channel );
   return false;
  }
  if ( claims.find(channel) ) {
   OUTPUT("USBDevice:Claim(%d): Already claimed\n", channel );
   return false;
  }
  result=libusb_claim_interface(handle,channel);
  if ( result < 0 ) {
   OUTPUT("USBDevice:Claim(%d): Error: %s\n", channel, GetError(result).c_str() );
   return false;
  }
  claims.Add(channel);
  return true;
 }
 bool Release( int channel=0 ) {
  if ( !handle ) {
   OUTPUT("USBDevice:Release(%d): No handle\n", channel );
   return false;
  }
  Integer *claim=claims.find(channel);
  if ( !claim ) {
   OUTPUT("USBDevice:Release(%d): Not claimed\n", channel );
   return false;
  }
  result=libusb_release_interface(handle,channel);
  claims.Remove(claim);
  delete claim;
  if ( result < 0 ) {
   OUTPUT("USBDevice:Release(%d): Error: %s\n", channel, GetError(result).c_str() );
   return false;
  }
  OUTPUT("USBDevice:Release(%d): Success\n", channel );
  return true;
 }
 bool Open() {
  result = libusb_open(device, &handle);
  if ( handle == NULL ) {
   OUTPUT("USBDevice:Open: Could not open device: %s, NULL handle\n", GetError(result).c_str() );
   return false;
  }
  return true;
 }
 void Close() {
  EACHN(claims.first,Integer,claim,{
   Release(claim->i);
  });
  if ( handle ) {
   OUTPUT("Closing USB.\n");
   libusb_close(handle);
  }
  handle=NULL;
 }
 /*
 If the reset fails, the descriptors change, or the previous state cannot be restored,
 the device will appear to be disconnected and reconnected. This means that the device
 handle is no longer valid (you should close it) and rediscover the device. A return code
 of LIBUSB_ERROR_NOT_FOUND indicates when this is the case.
 This is a blocking function which usually incurs a noticeable delay.
 */
 bool Reset() {
  if ( !handle ) {
   OUTPUT("USBDevice:Reset: No handle\n" );
   return false;
  }
  result=libusb_reset_device(handle);
  if ( result < 0 ) {
   OUTPUT("USBDevice:Reset: Error: %s\n", GetError(result).c_str() );
   return false;
  }
  return true;
 }
 bool Alternate( int alternate, int channel=0 ) {
  if ( !handle ) {
   OUTPUT("USBDevice:Alternate(%d,%d): No handle\n", alternate, channel );
   return false;
  }
  result=libusb_set_interface_alt_setting(handle,channel,alternate);
  if ( result < 0 ) {
   OUTPUT("USBDevice:Alternate(%d,%d): Error: %s\n", alternate, channel, GetError(result).c_str() );
   return false;
  }
  return true;
 }
 bool Attach( int channel=0 ) {
  if ( !handle ) {
   OUTPUT("USBDevice:Attach(%d): No handle\n", channel );
   return false;
  }
  result=libusb_attach_kernel_driver(handle,channel);
  if ( result < 0 ) {
   OUTPUT("USBDevice:Attach(%d): Error: %s\n", channel, GetError(result).c_str() );
   return false;
  }
  return true;
 }
 bool Detach( int channel=0 ) {
  if ( !handle ) {
   OUTPUT("USBDevice:Detach(%d): No handle\n", channel );
   return false;
  }
  result=libusb_detach_kernel_driver(handle,channel);
  if ( result < 0 ) {
   OUTPUT("USBDevice:Detach(%d): Error: %s\n", channel, GetError(result).c_str() );
   return false;
  }
  return true;
 }
 void Poll() {
//  char buf[1024];
  if ( GetDeviceDescriptor() ) {
   DebugInfo();
  }
  if ( Open() ) {
   bus=(int) libusb_get_bus_number(device);
   speed=(int) libusb_get_device_speed(device);
   address=(int) libusb_get_device_address(device);
   OUTPUT("Bus: %d  Speed: %s  Address: %d\n", (int) bus, GetSpeedName().c_str(), (int) address );
   Close();
  }
 }
 void DebugInfo() {
  OUTPUT(
   "USB Device: -- Vendor #: %d -- Product #: %d / %d\n"
   "Serial #%d -- Manufacturer: %d -- cdDevice: %d -- cdUSB: %d\n"
   "Type: %d Class: %d Protocol: %d SubClass: %d Length: %d\n"
   "Max Packet Size (0): %d bytes, Configurations: %d\n"
   "----\n",
   (int) descriptor.idVendor,
   (int) descriptor.idProduct,
   (int) descriptor.iProduct,
   (int) descriptor.iSerialNumber,
   (int) descriptor.iManufacturer,
   (int) descriptor.bcdDevice,
   (int) descriptor.bcdUSB,
   (int) descriptor.bDescriptorType,
   (int) descriptor.bDeviceClass,
   (int) descriptor.bDeviceProtocol,
   (int) descriptor.bDeviceSubClass,
   (int) descriptor.bLength,
   (int) descriptor.bMaxPacketSize0,
   (int) descriptor.bNumConfigurations
  );
 }
 ~USBDevice() {
  Close();
 }
};

class USBDevices : public LinkedList {
public:
 const struct libusb_version *version;
 libusb_context *context;
 libusb_device **devices;
 CLEARLISTRESET(USBDevice);
 USBDevices() : LinkedList() {
  context=NULL;
  devices=NULL;
  Init();
  Poll();
 }
 void Init() {
  OUTPUT("Initializing USB...");
  libusb_init(&context);
  OUTPUT("complete.\n");
#if defined(DEBUG)
  /* You are advised to set level 3. libusb is conservative with its message logging and most of the time, will only log messages that explain error conditions and other oddities. This will help you debug your software. */
  libusb_set_debug	( context, 3 );	
  /* The default level is 0, which means no messages are ever printed. */
#endif
 }
 void Poll() {  
  Clear();
  version=libusb_get_version();
  DebugInfo();
  if ( devices != NULL ) {
   libusb_free_device_list( devices, 0 );
  }
  libusb_get_device_list(context,&devices);
  OUTPUT("Got USB device list.\n");
  int i=0;
  libusb_device *dev;
 	while ((dev = devices[i++]) != NULL) {
   USBDevice *d=new USBDevice(dev);
   Append(d);   
  }
  OUTPUT("Polled %d devices.\n",i);
 }
 USBDevice *Find( int ID, int Vendor ) {
  OUTPUT("Searching for USB ID=%d Vendor=%d ...",ID,Vendor);
  FOREACH(USBDevice,d) {
   if ( d->descriptor.idProduct == ID
     && d->descriptor.idVendor == Vendor ) {
    OUTPUT("device found.\n");
    return d;
   }
  }
  OUTPUT("no such device found.\n");
  return null;
 }
 int Count( int ID, int Vendor ) {
  int i=0;
  OUTPUT("Searching for USB ID=%d Vendor=%d ",ID,Vendor);
  FOREACH(USBDevice,d) {
   if ( d->descriptor.idProduct == ID
     && d->descriptor.idVendor == Vendor ) {
    OUTPUT("...device found");
    i++;
   }
  }
  if ( i == 0 ) OUTPUT("...no such device found.\n");
  else OUTPUT("...%d devices found\n",i);
  return i;
 }
 void DebugInfo() {
  if ( version )
  OUTPUT( "USB Version: libusb-1.0 DLL: %d.%d.%d.%d -------------\n",
   (int) version->major,
   (int) version->minor,
   (int) version->micro,
   (int) version->nano
  );
 }
 ~USBDevices() {
  if ( devices != NULL ) {
   libusb_free_device_list( devices, 0 );
  }
  libusb_exit(context);
 }
};

extern USBDevices usb;

#endif