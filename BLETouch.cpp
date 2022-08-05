/* project k0uchpad. Q30 Keyboard with nRF51822
   Coded by TinLethax 2021/08/05 +7
*/

#include "BLETouch.h"
#include <stdint.h>

uint8_t HIDpacket[16] = {0};

static const PROGMEM unsigned char HID_Touch[] = {
  0x05,  0x0D,        // Usage page Digitizer
  0x09,  0x04,        // Usage Touch screen
  0xA1,  0x01,        // Collection Application
  0x85,  0x01 ,     // Report ID 0x01
  0x05,  0x0D,      // Usage page Digitizer
  0x09,  0x22 ,     // Usage Finger
  0xA1,  0x02  ,    // Collection Logical
  0x09,  0x42  ,  // Usage Tip switch (Finger)
  0x15,  0x00  ,  // Logical minimum 0
  0x25,  0x01  ,  // Logical maximum 1
  0x35,  0x00  ,  // Physical minimum 0
  0x45,  0x01  ,  // Physical maximum 1
  0x65,  0x00  ,  // Unit system none
  0x55,  0x00  ,  // Unit exponent none
  0x75,  0x01  ,  // Report size 1 bit
  0x95,  0x01  ,  // Report count 1
  0x81,  0x02  ,  // Input Data Var Abs
  0x95,  0x07  ,  // Report count 7 (Padding)
  0x81,  0x03  ,  // Input Const Var Abs

  0x09,  0x51  ,  // Usage Contact Identifier
  0x25,  0xFF  ,  // Logical Maximum 255
  0x75,  0x08  ,  // Report size 8 bit
  0x95,  0x01  ,  // Report count 1
  0x81,  0x02  ,  // Input Data Var Abs
  0x09,  0x30  ,  // Usage Tip Pressure
  0x81,  0x02  ,  // Input Data Var Abs

  0x05,  0x01  ,  // Usage page Generic Desktop
  0x09,  0x30  ,  // Usage X
  0x26,  0xA0  , 0x0C, // Logical maximum 3232
  //0x46,  0xA0  ,0x0C, // Physical Maximum 3232
  //0x65,  0x11  ,  // Unit SI linear cm (iirc)
  //0x55,  0x0E  ,  // Unit exponent -4 (iirc)
  0x75,  0x10  ,  // Report size 16 bit
  0x95,  0x01  ,  // Report count 1
  0x81,  0x02  ,  // Input Data Var Abs
  0x09,  0x31  ,  // Usage Y
  0x26,  0x8A  , 0x03, // Logical maximum 906
  //0x46,  0x8A  ,0x03, // Physical maximum 906
  0x81,  0x02  ,  // Input Data Var Abs

  0xC0 ,// End collection (Logical)

  0x05,  0x0D,      // Usage page Digitizer
  0x09,  0x22 ,     // Usage Finger
  0xA1,  0x02  ,    // Collection Logical
  0x09,  0x42  ,  // Usage Tip switch (Finger)
  0x15,  0x00  ,  // Logical minimum 0
  0x25,  0x01  ,  // Logical maximum 1
  0x35,  0x00  ,  // Physical minimum 0
  0x45,  0x01  ,  // Physical maximum 1
  0x65,  0x00  ,  // Unit system none
  0x55,  0x00  ,  // Unit exponent none
  0x75,  0x01  ,  // Report size 1 bit
  0x95,  0x01  ,  // Report count 1
  0x81,  0x02  ,  // Input Data Var Abs
  0x95,  0x07  ,  // Report count 7 (Padding)
  0x81,  0x03  ,  // Input Const Var Abs

  0x09,  0x51  ,  // Usage Contact Identifier
  0x25,  0xFF  ,  // Logical Maximum 255
  0x75,  0x08  ,  // Report size 8 bit
  0x95,  0x01  ,  // Report count 1
  0x81,  0x02  ,  // Input Data Var Abs
  0x09,  0x30  ,  // Usage Tip Pressure
  0x81,  0x02  ,  // Input Data Var Abs

  0x05,  0x01  ,  // Usage page Generic Desktop
  0x09,  0x30  ,  // Usage X
  0x26,  0xA0  , 0x0C, // Logical maximum 3232
  //0x46,  0xA0  ,0x0C, // Physical Maximum 3232
  //0x65,  0x11  ,  // Unit SI linear cm (iirc)
  //0x55,  0x0E  ,  // Unit exponent -4 (iirc)
  0x75,  0x10  ,  // Report size 16 bit
  0x95,  0x01  ,  // Report count 1
  0x81,  0x02  ,  // Input Data Var Abs
  0x09,  0x31  ,  // Usage Y
  0x26,  0x8A  , 0x03, // Logical maximum 906
  //0x46,  0x8A  ,0x03, // Physical maximum 906
  0x81,  0x02  ,  // Input Data Var Abs

  0xC0 ,// End collection (Logical)

  0x05,  0x0D  ,  // Usage page Digitizer
  0x09,  0x54  ,  // Usage contact count
  0x25,  0x0A  ,  // Logical Maximum 10
  0x75,  0x08  ,  // Report size 8 bit
  0x95,  0x01  ,  // Report count 1
  0x81,  0x02  ,  // Input Data Var Abs

  // Necessary Feature report that Windows OS required.
  // necessary to respond to CUSTOM_HID_REQ_GET_REPORT.
  // and return the Maximum contact (finger) number supported.
  0x85,  0x02  ,  // Report ID 0x02
  0x09,  0x55  ,  // Usage Contact Count Maximum
  0x25,  0x0A  ,  // Logical Maximum 10
  0x65,  0x00  ,  // Unit system none
  0x55,  0x00  ,  // Unit exponent none
  0x75,  0x08  ,  // report size 8 bit
  0x95,  0x01  ,  // report count 1
  0xB1,  0x02  ,  // Feature Data Var Abs

  // Necessary Feature report that Windows OS required.
  // But Not necessary to send the actual blob data.
  0x85, 0x44,                         //   REPORT_ID (Feature)
  0x06, 0x00, 0xff,                   //   USAGE_PAGE (Vendor Defined)
  0x09, 0xC5,                         //   USAGE (Vendor Usage 0xC5)
  0x15, 0x00,                         //   LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x00,                   //   LOGICAL_MAXIMUM (0xff)
  0x75, 0x08,                         //   REPORT_SIZE (8)
  0x96, 0x00,  0x01,                  //   REPORT_COUNT (0x100 (256))
  0xb1, 0x02,                         //   FEATURE (Data,Var,Abs)
  0xC0 // End collection (Application)
};


BLETouch::BLETouch() :
  BLEHID(HID_Touch, sizeof(HID_Touch), 11),// Number 11 is offset to the first report ID.
  _reportCharacteristic("2a4d", BLERead | BLENotify, 16),// 16 Bytes report size of Touch.
  _reportCharacteristic2("2a4d", BLERead | BLENotify | BLEWrite, 2),// 2 Bytes  report size of Max contact count.
  _reportReferenceDescriptor(BLEHIDDescriptorTypeInput),// For HID rouch report.
  _reportReferenceDescriptor2(BLEHIDDescriptorTypeFeature)// For Feature report (Max contact count).
{
}

// Use report ID 0x02 to report the maximum contact count.
// In this case is 2 contact points (2 fingers).
void BLETouch::MaxTchCntReport() {
  HIDpacket[0] = 0x02;// Report ID 0x02
  HIDpacket[1] = 0x02;// Max touch count == 2 fingers.
  this->_reportReferenceDescriptor2.setReportId(HIDpacket[0]);
  this->sendData(this->_reportCharacteristic2, HIDpacket, 2);
}

void BLETouch::TouchReport(
  uint8_t Finger1_present,
  uint8_t Finger1_pressure,
  uint8_t Finger1_XLSB,
  uint8_t Finger1_XMSB,
  uint8_t Finger1_YLSB,
  uint8_t Finger1_YMSB,

  uint8_t Finger2_present,
  uint8_t Finger2_pressure,
  uint8_t Finger2_XLSB,
  uint8_t Finger2_XMSB,
  uint8_t Finger2_YLSB,
  uint8_t Finger2_YMSB) {

  // parse each data to report array

  HIDpacket[0] = 0x01; // Report ID 0x01 - Touch report

  // Finger 1
  HIDpacket[1] = Finger1_present ? 0x01 : 0;// Bit0 indicate finger. Set to 1 when finger is present.
  HIDpacket[2] = 0x01;// Finger 1 contact ID is fixed at 0x01.
  HIDpacket[3] = Finger1_pressure;// 0-255 pressure level.
  HIDpacket[4] = Finger1_XLSB; // Least significant byte of X
  HIDpacket[5] = Finger1_XMSB; // Most significant byte of X
  HIDpacket[6] = Finger1_YLSB; // Least significant byte of Y
  HIDpacket[7] = Finger1_YMSB; // Most significant byte of Y

  // Finger 2
  HIDpacket[8] = Finger2_present ? 0x01 : 0;// Bit0 indicate finger. Set to 1 when finger is present.
  HIDpacket[9] = 0x02;// Finger 2 contact ID is fixed at 0x02.
  HIDpacket[10] = Finger2_pressure;// 0-255 pressure level.
  HIDpacket[11] = Finger2_XLSB; // Least significant byte of X
  HIDpacket[12] = Finger2_XMSB; // Most significant byte of X
  HIDpacket[13] = Finger2_YLSB; // Least significant byte of Y
  HIDpacket[14] = Finger2_YMSB; // Most significant byte of Y

  // Report finger (contact point) count.
  HIDpacket[15] = HIDpacket[1] + HIDpacket[8];

  this->sendData(this->_reportCharacteristic, HIDpacket, 16);
}

void BLETouch::setReportId(unsigned char reportId) {
  BLEHID::setReportId(reportId);

  this->_reportReferenceDescriptor.setReportId(reportId);
}

unsigned char BLETouch::numAttributes() {
  return 4;
}

BLELocalAttribute** BLETouch::attributes() {
  static BLELocalAttribute* attributes[4];

  attributes[0] = &this->_reportCharacteristic;
  attributes[1] = &this->_reportReferenceDescriptor;
  attributes[2] = &this->_reportCharacteristic2;
  attributes[3] = &this->_reportReferenceDescriptor2;

  return attributes;
}
