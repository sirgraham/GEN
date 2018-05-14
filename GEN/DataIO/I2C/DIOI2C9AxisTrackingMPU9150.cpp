/*------------------------------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150.CPP
//
//  Data IO I2C 9 Axis Motion Tracking MPU9150  class (InvenSense - MPU9150)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/05/2014 16:21:57
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"
#include "XSleep.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"

#include "DIOI2C9AxisTrackingMPU9150.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::DIOI2C9AXISTRACKINGMPU9150
*/
/**
//
//  Class Constructor DIOI2C9AXISTRACKINGMPU9150
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:37:54
//


//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOI2C9AXISTRACKINGMPU9150::DIOI2C9AXISTRACKINGMPU9150() : DIODEVICEI2C()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::~DIOI2C9AXISTRACKINGMPU9150
*/
/**
//
//   Class Destructor DIOI2C9AXISTRACKINGMPU9150
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:25:15
//
*/
/*-----------------------------------------------------------------*/
DIOI2C9AXISTRACKINGMPU9150::~DIOI2C9AXISTRACKINGMPU9150()
{
  End();

  Clean();
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:29:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::End()
{
  if(!DIODEVICE::End()) return false;

  if(diostream)
    {
      diostream->Close();
      diofactory->DeleteStreamIO(diostream);
      diostream = NULL;
    }

  if(diostreamcfg)
    {
      delete diostreamcfg;
      diostreamcfg = NULL;
    }

  if(xtimerout)
    {
      xfactory->DeleteTimer(xtimerout);
      xtimerout = NULL;
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::IniDevice
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 16:24:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::IniDevice()
{
  diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO( diostreamcfg);
  if(diostream)
    {
      if(diostream->Open())
        {
          if(diostream->WaitToConnected(timeout))
            {


              return DIODEVICE::Ini();
            }
        }
    }

  return false;
}





/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::Initialize
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:37:26
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::Initialize()
{
  SetClockSource(DIOI2CMPU9150_CLOCK_PLL_XGYRO);
  SetFullScaleGyroRange(DIOI2CMPU9150_GYRO_FS_250);
  SetFullScaleAccelRange(DIOI2CMPU9150_ACCEL_FS_2);
  SetSleepEnabled(false); // thanks to Jack Elston for pointing this one out!
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::TestConnection
*/
/**
//  Verify the I2C connection.
//  Make sure the device is connected and responds as expected.
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:38:25
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::TestConnection()
{
  return (GetDeviceID() == 0x34)?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAuxVDDIOLevel
*/
/**
//  Get the auxiliary I2C supply voltage level. AUX_VDDIO register (InvenSense demo code calls this RA_*G_OFFS_TC)
//  When set to 1, the auxiliary I2C bus high logic level is VDD. When cleared to 0, the auxiliary I2C
//  bus high logic level is VLOGIC. This does not apply to the MPU-6000, which does not have a VLOGIC pin.
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:39:57
//
//  @return       XBYTE : I2C supply voltage level (0=VLOGIC, 1=VDD)
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetAuxVDDIOLevel()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_TC, DIOI2CMPU9150_TC_PWR_MODE_BIT, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetAuxVDDIOLevel
*/
/**
//  Set the auxiliary I2C supply voltage level.
//  When set to 1, the auxiliary I2C bus high logic level is VDD. When cleared to 0,
//  the auxiliary I2C bus high logic level is VLOGIC. This does not apply to the MPU-6000,
//  which does not have a VLOGIC pin.
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:42:03
//
//  @param        level : I2C supply voltage level (0=VLOGIC, 1=VDD)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAuxVDDIOLevel(XBYTE level)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_TC, DIOI2CMPU9150_TC_PWR_MODE_BIT, level);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetRate
*/
/**
//  Get gyroscope output rate divider.
//  The sensor register output, FIFO output, DMP sampling, Motion detection, Zero
//  Motion detection, and Free Fall detection are all based on the Sample Rate.
//  The Sample Rate is generated by dividing the gyroscope output rate by
//  SMPLRT_DIV:
//
//  Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
//
//  where Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF_CFG = 0 or
//  7), and 1kHz when the DLPF is enabled (see Register 26).
//
//  Note: The accelerometer output rate is 1kHz. This means that for a Sample
//  Rate greater than 1kHz, the same accelerometer sample may be output to the
//  FIFO, DMP, and sensor registers more than once.
//
//  For a diagram of the gyroscope and accelerometer signal paths, see Section 8
//  of the MPU-6000/MPU-6050 Product Specification document.
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:44:34
//
//  @return       XBYTE :  Current sample rate  (see DIOI2CMPU9150_RA_SMPLRT_DIV)
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetRate()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_SMPLRT_DIV, buffer);
  return buffer[0];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetRate
*/
/**
//  Set gyroscope sample rate divider.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:46:06
//
//  @param        rate : New sample rate divider (see DIOI2CMPU9150_RA_SMPLRT_DIV)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetRate(XBYTE rate)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_SMPLRT_DIV, rate);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::CheckMagStatus
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:46:47
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::CheckMagStatus()
{
   ReadByte(DIOI2CMPU9150_RA_MAG_ADDRESS, 0x02, buffer);
   return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetExternalFrameSync
*/
/**
//  Get external FSYNC configuration.
//  Configures the external Frame Synchronization (FSYNC) pin sampling. An
//  external signal connected to the FSYNC pin can be sampled by configuring
//  EXT_SYNC_SET. Signal changes to the FSYNC pin are latched so that short
//  strobes may be captured. The latched FSYNC signal will be sampled at the
//  Sampling Rate, as defined in register 25. After sampling, the latch will
//  reset to the current FSYNC signal state.
//
//  The sampled value will be reported in place of the least significant bit in
//  a sensor data register determined by the value of EXT_SYNC_SET according to
//  the following table.
//
//  EXT_SYNC_SET | FSYNC Bit Location
//  -------------+-------------------
//  0            | Input disabled
//  1            | TEMP_OUT_L[0]
//  2            | GYRO_XOUT_L[0]
//  3            | GYRO_YOUT_L[0]
//  4            | GYRO_ZOUT_L[0]
//  5            | ACCEL_XOUT_L[0]
//  6            | ACCEL_YOUT_L[0]
//  7            | ACCEL_ZOUT_L[0]
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:47:09
//
//  @return       XBYTE : FSYNC configuration value
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetExternalFrameSync()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_CONFIG, DIOI2CMPU9150_CFG_EXT_SYNC_SET_BIT, DIOI2CMPU9150_CFG_EXT_SYNC_SET_LENGTH, buffer);
  return buffer[0];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetExternalFrameSync
*/
/**
//  Set external FSYNC configuration.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:49:34
//
//  @param        sync : New FSYNC configuration value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetExternalFrameSync(XBYTE sync)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_CONFIG, DIOI2CMPU9150_CFG_EXT_SYNC_SET_BIT, DIOI2CMPU9150_CFG_EXT_SYNC_SET_LENGTH, sync);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDLPFMode
*/
/**
//  Get digital low-pass filter configuration.
//  The DLPF_CFG parameter sets the digital low pass filter configuration. It
//  also determines the internal sampling rate used by the device as shown in
//  the table below.
//
//  Note: The accelerometer output rate is 1kHz. This means that for a Sample
//  Rate greater than 1kHz, the same accelerometer sample may be output to the
//  FIFO, DMP, and sensor registers more than once.
//
//           |   ACCELEROMETER    |           GYROSCOPE
//  DLPF_CFG | Bandwidth | Delay  | Bandwidth | Delay  | Sample Rate
//  ---------+-----------+--------+-----------+--------+-------------
//  0        | 260Hz     | 0ms    | 256Hz     | 0.98ms | 8kHz
//  1        | 184Hz     | 2.0ms  | 188Hz     | 1.9ms  | 1kHz
//  2        | 94Hz      | 3.0ms  | 98Hz      | 2.8ms  | 1kHz
//  3        | 44Hz      | 4.9ms  | 42Hz      | 4.8ms  | 1kHz
//  4        | 21Hz      | 8.5ms  | 20Hz      | 8.3ms  | 1kHz
//  5        | 10Hz      | 13.8ms | 10Hz      | 13.4ms | 1kHz
//  6        | 5Hz       | 19.0ms | 5Hz       | 18.6ms | 1kHz
//  7        |   -- Reserved --   |   -- Reserved --   | Reserved
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:50:09
//
//  @return       XBYTE :  DLFP configuration
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDLPFMode()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_CONFIG, DIOI2CMPU9150_CFG_DLPF_CFG_BIT, DIOI2CMPU9150_CFG_DLPF_CFG_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetDLPFMode
*/
/**
//  Set digital low-pass filter configuration.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:52:09
//
//  @param        mode : New DLFP configuration setting
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDLPFMode(XBYTE mode)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_CONFIG, DIOI2CMPU9150_CFG_DLPF_CFG_BIT, DIOI2CMPU9150_CFG_DLPF_CFG_LENGTH, mode);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFullScaleGyroRange
*/
/**
//  Get full-scale gyroscope range.
//  The FS_SEL parameter allows setting the full-scale range of the gyro sensors,
//  as described in the table below.
//
//  0 = +/- 250 degrees/sec
//  1 = +/- 500 degrees/sec
//  2 = +/- 1000 degrees/sec
//  3 = +/- 2000 degrees/sec
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:53:41
//
//  @return       XBYTE : Current full-scale gyroscope range setting
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFullScaleGyroRange()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_GYRO_CONFIG, DIOI2CMPU9150_GCONFIG_FS_SEL_BIT, DIOI2CMPU9150_GCONFIG_FS_SEL_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFullScaleGyroRange
*/
/**
//  Set full-scale gyroscope range.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:55:18
//
//  @param        range : New full-scale gyroscope range value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFullScaleGyroRange(XBYTE range)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_GYRO_CONFIG, DIOI2CMPU9150_GCONFIG_FS_SEL_BIT, DIOI2CMPU9150_GCONFIG_FS_SEL_LENGTH, range);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAccelXSelfTest
*/
/**
//  Get self-test enabled setting for accelerometer X axis.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:56:08
//
//  @return       bool : Self-test enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetAccelXSelfTest()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_XA_ST_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetAccelXSelfTest
*/
/**
//  Get self-test enabled setting for accelerometer X axis.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:57:00
//
//  @param        enabled : Self-test enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelXSelfTest(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_XA_ST_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAccelYSelfTest
*/
/**
//  Get self-test enabled value for accelerometer Y axis.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:57:42
//
//  @return       bool : Self-test enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetAccelYSelfTest()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_YA_ST_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetAccelYSelfTest
*/
/**
//  Get self-test enabled value for accelerometer Y axis.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:58:14
//
//  @param        enabled : Self-test enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelYSelfTest(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_YA_ST_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAccelZSelfTest
*/
/**
//  Get self-test enabled value for accelerometer Z axis.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:59:04
//
//  @return       bool : Self-test enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetAccelZSelfTest()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_ZA_ST_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetAccelZSelfTest
*/
/**
//  Set self-test enabled value for accelerometer Z axis.
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2016 23:59:37
//
//  @param        enabled : Self-test enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelZSelfTest(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_ZA_ST_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::getFullScaleAccelRange
*/
/**
//  Get full-scale accelerometer range.
//  The FS_SEL parameter allows setting the full-scale range of the accelerometer
//  sensors, as described in the table below.
//
//  0 = +/- 2g
//  1 = +/- 4g
//  2 = +/- 8g
//  3 = +/- 16g
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:01:03
//
//  @return       XBYTE : Current full-scale accelerometer range setting
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFullScaleAccelRange()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_AFS_SEL_BIT, DIOI2CMPU9150_ACONFIG_AFS_SEL_LENGTH, buffer);
  return buffer[0];
}





/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFullScaleAccelRange
*/
/**
//  Set full-scale accelerometer range.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:22:39
//
//  @param        range : New full-scale accelerometer range setting
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFullScaleAccelRange(XBYTE range)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_AFS_SEL_BIT, DIOI2CMPU9150_ACONFIG_AFS_SEL_LENGTH, range);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDHPFMode
*/
/**
//  Get the high-pass filter configuration.
//  The DHPF is a filter module in the path leading to motion detectors (Free
//  Fall, Motion threshold, and Zero Motion). The high pass filter output is not
//  available to the data registers (see Figure in Section 8 of the MPU-6000/
//  MPU-6050 Product Specification document).
//
//  The high pass filter has three modes:
//
//    Reset: The filter output settles to zero within one sample. This
//           effectively disables the high pass filter. This mode may be toggled
//           to quickly settle the filter.
//
//    On:    The high pass filter will pass signals above the cut off frequency.
//
//    Hold:  When triggered, the filter holds the present sample. The filter
//           output will be the difference between the input sample and the held
//           sample.
//
//  ACCEL_HPF | Filter Mode | Cut-off Frequency
//  ----------+-------------+------------------
//  0         | Reset       | None
//  1         | On          | 5Hz
//  2         | On          | 2.5Hz
//  3         | On          | 1.25Hz
//  4         | On          | 0.63Hz
//  7         | Hold        | None
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:23:12
//
//  @return       XBYTE : Current high-pass filter configuration
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDHPFMode()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_ACCEL_HPF_BIT, DIOI2CMPU9150_ACONFIG_ACCEL_HPF_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::setDHPFMode
*/
/**
//  Set the high-pass filter configuration.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:25:32
//
//  @param        bandwidth : New high-pass filter configuration
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDHPFMode(XBYTE bandwidth)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_ACCEL_HPF_BIT, DIOI2CMPU9150_ACONFIG_ACCEL_HPF_LENGTH, bandwidth);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionThreshold
*/
/**
// Get free-fall event acceleration threshold.
// This register configures the detection threshold for Free Fall event
// detection. The unit of FF_THR is 1LSB = 2mg. Free Fall is detected when the
// absolute value of the accelerometer measurements for the three axes are each
// less than the detection threshold. This condition increments the Free Fall
// duration counter (Register 30). The Free Fall interrupt is triggered when the
// Free Fall duration counter reaches the time specified in FF_DUR.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:26:24
//
//  @return       XBYTE : Current free-fall acceleration threshold value (LSB = 2mg)
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionThreshold()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_FF_THR, buffer);
  return buffer[0];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionThreshold
*/
/**
//  Set free-fall event acceleration threshold.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:28:22
//
//  @param        threshold : New free-fall acceleration threshold value (LSB = 2mg)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionThreshold(XBYTE threshold)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_FF_THR, threshold);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionDuration
*/
/**
//  Get free-fall event duration threshold.
//  this register configures the duration counter threshold for Free Fall event
//  detection. The duration counter ticks at 1kHz, therefore FF_DUR has a unit
//  of 1 LSB = 1 ms.

//  The Free Fall duration counter increments while the absolute value of the
//  accelerometer measurements are each less than the detection threshold
//  (Register 29). The Free Fall interrupt is triggered when the Free Fall
// duration counter reaches the time specified in this register.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:30:26
//
//  @return       XBYTE : Current free-fall duration threshold value (LSB = 1ms)
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionDuration()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_FF_DUR, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionDuration
*/
/**
//  Get free-fall event duration threshold.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:32:24
//
//  @param        duration : New free-fall duration threshold value (LSB = 1ms)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionDuration(XBYTE duration)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_FF_DUR, duration);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionThreshold
*/
/**
//  Get motion detection event acceleration threshold.
//  This register configures the detection threshold for Motion interrupt
//  generation. The unit of MOT_THR is 1LSB = 2mg. Motion is detected when the
//  absolute value of any of the accelerometer measurements exceeds this Motion
//  detection threshold. This condition increments the Motion detection duration
//  counter (Register 32). The Motion detection interrupt is triggered when the
//  Motion Detection counter reaches the time count specified in MOT_DUR
//  (Register 32).
//
//  The Motion interrupt will indicate the axis and polarity of detected motion
//  in MOT_DETECT_STATUS (Register 97).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:33:07
//
//  @return       XBYTE : Current motion detection acceleration threshold value (LSB = 2mg)
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionThreshold()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_MOT_THR, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionThreshold
*/
/**
//  Set free-fall event acceleration threshold.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:36:01
//
//  @param        threshold : New motion detection acceleration threshold value (LSB = 2mg)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionThreshold(XBYTE threshold)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_MOT_THR, threshold);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionDuration
*/
/**
//  Get motion detection event duration threshold.
//  This register configures the duration counter threshold for Motion interrupt
//  generation. The duration counter ticks at 1 kHz, therefore MOT_DUR has a unit
//  of 1LSB = 1ms. The Motion detection duration counter increments when the
//  absolute value of any of the accelerometer measurements exceeds the Motion
//  detection threshold (Register 31). The Motion detection interrupt is
//  triggered when the Motion detection counter reaches the time count specified
//  in this register.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:36:45
//
//  @return       XBYTE : Current motion detection duration threshold value (LSB = 1ms)
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionDuration()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_MOT_DUR, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionDuration
*/
/**
//  Set motion detection event duration threshold.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:42:44
//
//  @param        duration : New motion detection duration threshold value (LSB = 1ms)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionDuration(XBYTE duration)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_MOT_DUR, duration);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetectionThreshold
*/
/**
//  Get zero motion detection event acceleration threshold.
//  This register configures the detection threshold for Zero Motion interrupt
//  generation. The unit of ZRMOT_THR is 1LSB = 2mg. Zero Motion is detected when
//  the absolute value of the accelerometer measurements for the 3 axes are each
//  less than the detection threshold. This condition increments the Zero Motion
//  duration counter (Register 34). The Zero Motion interrupt is triggered when
//  the Zero Motion duration counter reaches the time count specified in
//  ZRMOT_DUR (Register 34).
//
//  Unlike Free Fall or Motion detection, Zero Motion detection triggers an
//  interrupt both when Zero Motion is first detected and when Zero Motion is no
//  longer detected.
//
//  When a zero motion event is detected, a Zero Motion Status will be indicated
//  in the MOT_DETECT_STATUS register (Register 97). When a motion-to-zero-motion
//  condition is detected, the status bit is set to 1. When a zero-motion-to-
//  motion condition is detected, the status bit is set to 0.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:43:39
//
//  @return       XBYTE : Current zero motion detection acceleration threshold value (LSB = 2mg)
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetectionThreshold()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_ZRMOT_THR, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetZeroMotionDetectionThreshold
*/
/**
//  Set zero motion detection event acceleration threshold.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:45:21
//
//  @param        threshold : New zero motion detection acceleration threshold value (LSB = 2mg)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZeroMotionDetectionThreshold(XBYTE threshold)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_ZRMOT_THR, threshold);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetectionDuration
*/
/**
//  Get zero motion detection event duration threshold.
//  This register configures the duration counter threshold for Zero Motion
//  interrupt generation. The duration counter ticks at 16 Hz, therefore
//  ZRMOT_DUR has a unit of 1 LSB = 64 ms. The Zero Motion duration counter
//  increments while the absolute value of the accelerometer measurements are
//  each less than the detection threshold (Register 33). The Zero Motion
//  interrupt is triggered when the Zero Motion duration counter reaches the time
//  count specified in this register.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:45:57
//
//  @return       XBYTE : Current zero motion detection duration threshold value (LSB = 64ms)
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetectionDuration()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_ZRMOT_DUR, buffer);
  return buffer[0];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetZeroMotionDetectionDuration
*/
/**
//  Set zero motion detection event duration threshold.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:47:50
//
//  @param        duration : New zero motion detection duration threshold value (LSB = 1ms)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZeroMotionDetectionDuration(XBYTE duration)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_ZRMOT_DUR, duration);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetTempFIFOEnabled
*/
/**
//  Get temperature FIFO enabled value.
//  When set to 1, this bit enables TEMP_OUT_H and TEMP_OUT_L (Registers 65 and
//  66) to be written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:48:31
//
//  @return       bool : Current temperature FIFO enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetTempFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_TEMP_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::setTempFIFOEnabled
*/
/**
//  Set temperature FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:49:18
//
//  @param        enabled :New temperature FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetTempFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_TEMP_FIFO_EN_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetXGyroFIFOEnabled
*/
/**
//  Get gyroscope X-axis FIFO enabled value.
//  When set to 1, this bit enables GYRO_XOUT_H and GYRO_XOUT_L (Registers 67 and
//  68) to be written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:50:14
//
//  @return       bool : Current gyroscope X-axis FIFO enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetXGyroFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_XG_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}





/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetXGyroFIFOEnabled
*/
/**
//  Set gyroscope X-axis FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:50:59
//
//  @param        enabled : New gyroscope X-axis FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXGyroFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_XG_FIFO_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetYGyroFIFOEnabled
*/
/**
//  Get gyroscope Y-axis FIFO enabled value.
//  When set to 1, this bit enables GYRO_YOUT_H and GYRO_YOUT_L (Registers 69 and
//  70) to be written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:51:34
//
//  @return       bool : Current gyroscope Y-axis FIFO enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetYGyroFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_YG_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetYGyroFIFOEnabled
*/
/**
//  Set gyroscope Y-axis FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:52:16
//
//  @param        enabled :New gyroscope Y-axis FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetYGyroFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_YG_FIFO_EN_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZGyroFIFOEnabled
*/
/**
//  Get gyroscope Z-axis FIFO enabled value.
//  When set to 1, this bit enables GYRO_ZOUT_H and GYRO_ZOUT_L (Registers 71 and
//  72) to be written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:52:58
//
//  @return       bool : Current gyroscope Z-axis FIFO enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetZGyroFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_ZG_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetZGyroFIFOEnabled
*/
/**
//  Set gyroscope Z-axis FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:53:43
//
//  @param        enabled : New gyroscope Z-axis FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZGyroFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_ZG_FIFO_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAccelFIFOEnabled
*/
/**
//  Get accelerometer FIFO enabled value.
//  When set to 1, this bit enables ACCEL_XOUT_H, ACCEL_XOUT_L, ACCEL_YOUT_H,
//  ACCEL_YOUT_L, ACCEL_ZOUT_H, and ACCEL_ZOUT_L (Registers 59 to 64) to be
//  written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:54:26
//
//  @return       bool : Current accelerometer FIFO enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetAccelFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_ACCEL_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetAccelFIFOEnabled
*/
/**
//  Set accelerometer FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:55:20
//
//  @param        enabled : New accelerometer FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_ACCEL_FIFO_EN_BIT, enabled);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave2FIFOEnabled
*/
/**
//  Get Slave 2 FIFO enabled value.
//  When set to 1, this bit enables EXT_SENS_DATA registers (Registers 73 to 96)
//  associated with Slave 2 to be written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:55:53
//
//  @return       bool : Current Slave 2 FIFO enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave2FIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV2_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave2FIFOEnabled
*/
/**
//  Set Slave 2 FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:56:34
//
//  @param        enabled : New Slave 2 FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave2FIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV2_FIFO_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave1FIFOEnabled
*/
/**
//  Get Slave 1 FIFO enabled value.
//  When set to 1, this bit enables EXT_SENS_DATA registers (Registers 73 to 96)
//  associated with Slave 1 to be written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:57:14
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave1FIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV1_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave1FIFOEnabled
*/
/**
//  Set Slave 1 FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:57:57
//
//  @param        enabled : New Slave 1 FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave1FIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV1_FIFO_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave0FIFOEnabled
*/
/**
//  Get Slave 0 FIFO enabled value.
//  When set to 1, this bit enables EXT_SENS_DATA registers (Registers 73 to 96)
//  associated with Slave 0 to be written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:58:30
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave0FIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV0_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave0FIFOEnabled
*/
/**
//  Set Slave 0 FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:59:16
//
//  @param        enabled : New Slave 0 FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave0FIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV0_FIFO_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetMultiMasterEnabled
*/
/**
//  Get multi-master enabled value.
//  Multi-master capability allows multiple I2C masters to operate on the same
//  bus. In circuits where multi-master capability is required, set MULT_MST_EN
//  to 1. This will increase current drawn by approximately 30uA.
//
//  In circuits where multi-master capability is required, the state of the I2C
//  bus must always be monitored by each separate I2C Master. Before an I2C
//  Master can assume arbitration of the bus, it must first confirm that no other
//  I2C Master has arbitration of the bus. When MULT_MST_EN is set to 1, the
//  MPU-60X0's bus arbitration detection logic is turned on, enabling it to
//  detect when the bus is available.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 0:59:50
//
//  @return       bool : Current multi-master enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetMultiMasterEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_MULT_MST_EN_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetMultiMasterEnabled
*/
/**
//  Set multi-master enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:01:28
//
//  @param        enabled : New multi-master enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMultiMasterEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_MULT_MST_EN_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetWaitForExternalSensorEnabled
*/
/**
//  Get wait-for-external-sensor-data enabled value.
//  When the WAIT_FOR_ES bit is set to 1, the Data Ready interrupt will be
//  delayed until External Sensor data from the Slave Devices are loaded into the
//  EXT_SENS_DATA registers. This is used to ensure that both the internal sensor
//  data (i.e. from gyro and accel) and external sensor data have been loaded to
//  their respective data registers (i.e. the data is synced) when the Data Ready
//  interrupt is triggered.

//  @author       Abraham J. Velez
//  @version      04/07/2016 1:01:58
//
//  @return       bool : Current wait-for-external-sensor-data enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetWaitForExternalSensorEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_WAIT_FOR_ES_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetWaitForExternalSensorEnabled
*/
/**
//  Set wait-for-external-sensor-data enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:03:00
//
//  @param        enabled : New wait-for-external-sensor-data enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetWaitForExternalSensorEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_WAIT_FOR_ES_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave3FIFOEnabled
*/
/**
//  Get Slave 3 FIFO enabled value.
//  When set to 1, this bit enables EXT_SENS_DATA registers (Registers 73 to 96)
//  associated with Slave 3 to be written into the FIFO buffer.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:03:41
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave3FIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_SLV_3_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave3FIFOEnabled
*/
/**
//  Set Slave 3 FIFO enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:04:19
//
//  @param        enabled : New Slave 3 FIFO enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave3FIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_SLV_3_FIFO_EN_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlaveReadWriteTransitionEnabled
*/
/**
//  Get slave read/write transition enabled value.
//  The I2C_MST_P_NSR bit configures the I2C Master's transition from one slave
//  read to the next slave read. If the bit equals 0, there will be a restart
//  between reads. If the bit equals 1, there will be a stop followed by a start
//  of the following read. When a write transaction follows a read transaction,
//  the stop followed by a start of the successive write will be always used.

//  @author       Abraham J. Velez
//  @version      04/07/2016 1:05:14
//
//  @return       bool : Current slave read/write transition enabled value
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveReadWriteTransitionEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_I2C_MST_P_NSR_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveReadWriteTransitionEnabled
*/
/**
//  Set slave read/write transition enabled value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:08:02
//
//  @param        enabled : New slave read/write transition enabled value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveReadWriteTransitionEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_I2C_MST_P_NSR_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetMasterClockSpeed
*/
/**
//  Get I2C master clock speed.
//  I2C_MST_CLK is a 4 bit unsigned value which configures a divider on the
//  MPU-60X0 internal 8MHz clock. It sets the I2C master clock speed according to
//  the following table:
//
//  I2C_MST_CLK | I2C Master Clock Speed | 8MHz Clock Divider
//  ------------+------------------------+-------------------
//  0           | 348kHz                 | 23
//  1           | 333kHz                 | 24
//  2           | 320kHz                 | 25
//  3           | 308kHz                 | 26
//  4           | 296kHz                 | 27
//  5           | 286kHz                 | 28
//  6           | 276kHz                 | 29
//  7           | 267kHz                 | 30
//  8           | 258kHz                 | 31
//  9           | 500kHz                 | 16
//  10          | 471kHz                 | 17
//  11          | 444kHz                 | 18
//  12          | 421kHz                 | 19
//  13          | 400kHz                 | 20
//  14          | 381kHz                 | 21
//  15          | 364kHz                 | 22
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:08:33
//
//  @return       XBYTE : Current I2C master clock speed
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMasterClockSpeed()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_I2C_MST_CLK_BIT, DIOI2CMPU9150_I2C_MST_CLK_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetMasterClockSpeed
*/
/**
//  Set I2C master clock speed.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:11:04
//
//  @param        speed : Current I2C master clock speed
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMasterClockSpeed(XBYTE speed)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_I2C_MST_CLK_BIT, DIOI2CMPU9150_I2C_MST_CLK_LENGTH, speed);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlaveAddress
/*
//  Get the I2C address of the specified slave (0-3).
//  Note that Bit 7 (MSB) controls read/write mode. If Bit 7 is set, it's a read
//  operation, and if it is cleared, then it's a write operation. The remaining
//  bits (6-0) are the 7-bit device address of the slave device.
//
//  In read mode, the result of the read is placed in the lowest available
//  EXT_SENS_DATA register. For further information regarding the allocation of
//  read results, please refer to the EXT_SENS_DATA register description
//  (Registers 73 - 96).
//
//  The MPU-6050 supports a total of five slaves, but Slave 4 has unique
//  characteristics, and so it has its own functions (getSlave4* and setSlave4*).
//
//  I2C data transactions are performed at the Sample Rate, as defined in
//  Register 25. The user is responsible for ensuring that I2C data transactions
//  to and from each enabled Slave can be completed within a single period of the
//  Sample Rate.
//
//  The I2C slave access rate can be reduced relative to the Sample Rate. This
//  reduced access rate is determined by I2C_MST_DLY (Register 52). Whether a
//  slave's access rate is reduced relative to the Sample Rate is determined by
//  I2C_MST_DELAY_CTRL (Register 103).
//
//  The processing order for the slaves is fixed. The sequence followed for
//  processing the slaves is Slave 0, Slave 1, Slave 2, Slave 3 and Slave 4. If a
//  particular Slave is disabled it will be skipped.
//
//  Each slave can either be accessed at the sample rate or at a reduced sample
//  rate. In a case where some slaves are accessed at the Sample Rate and some
//  slaves are accessed at the reduced rate, the sequence of accessing the slaves
//  (Slave 0 to Slave 4) is still followed. However, the reduced rate slaves will
//  be skipped if their access rate dictates that they should not be accessed
//  during that particular cycle. For further information regarding the reduced
//  access rate, please refer to Register 52. Whether a slave is accessed at the
//  Sample Rate or at the reduced rate is determined by the Delay Enable bits in
//  Register 103.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:11:52
//
//  @return       XBYTE : Current address for specified slave
//
//  @param        num :  Slave number (0-3)
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlaveAddress(XBYTE num)
{
  if(num > 3) return 0;

  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_ADDR + num*3, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveAddress
*/
/**
//  Set the I2C address of the specified slave (0-3).
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:16:04
//
//  @param        num :  Slave number (0-3)
//  @param        address : New address for specified slave
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveAddress(XBYTE num, XBYTE address)
{
   if(num > 3) return;

   WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_ADDR + num*3, address);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::getSlaveRegister
*/
/**
//  Get the active internal register for the specified slave (0-3).
//  Read/write operations for this slave will be done to whatever internal
//  register address is stored in this MPU register.
//
//  The MPU-6050 supports a total of five slaves, but Slave 4 has unique
//  characteristics, and so it has its own functions.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:17:04
//
//  @return       XBYTE : Current active register for specified slave
//
//  @param        num : Slave number (0-3)
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlaveRegister(XBYTE num)
{
  if(num > 3) return 0;

  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_REG + num*3, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveRegister
*/
/**
//  Set the active internal register for the specified slave (0-3).
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:18:33
//
//  @param        num : Slave number (0-3)
//  @param        reg : New active register for specified slave
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveRegister(XBYTE num, XBYTE reg)
{
  if(num > 3) return;

  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_REG + num*3, reg);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlaveEnabled
*/
/**
//  Get the enabled value for the specified slave (0-3).
//  When set to 1, this bit enables Slave 0 for data transfer operations. When
//  cleared to 0, this bit disables Slave 0 from data transfer operations.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:19:11
//
//  @return       bool : Current enabled value for specified slave
//
//  @param        num : Slave number (0-3)
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveEnabled(XBYTE num)
{
  if(num > 3) return false;

  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::setSlaveEnabled
*/
/**
//  Set the enabled value for the specified slave (0-3).
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:20:07
//
//  @param        num : Slave number (0-3)
//  @param        enabled : New enabled value for specified slave
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveEnabled(XBYTE num, bool enabled)
{
  if(num > 3) return;

  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlaveWordByteSwap
*/
/**
//  Get word pair byte-swapping enabled for the specified slave (0-3).
//  When set to 1, this bit enables byte swapping. When byte swapping is enabled,
//  the high and low bytes of a word pair are swapped. Please refer to
//  I2C_SLV0_GRP for the pairing convention of the word pairs. When cleared to 0,
//  bytes transferred to and from Slave 0 will be written to EXT_SENS_DATA
//  registers in the order they were transferred.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:20:52
//
//  @return       bool : Current word pair byte-swapping enabled value for specified slave
//
//  @param        num : Slave number (0-3)
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWordByteSwap(XBYTE num)
{
  if(num > 3) return false;

  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_BYTE_SW_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveWordByteSwap
*/
/**
//  Set word pair byte-swapping enabled for the specified slave (0-3).
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:23:16
//
//  @param        num : Slave number (0-3)
//  @param        enabled : New word pair byte-swapping enabled value for specified slave
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWordByteSwap(XBYTE num, bool enabled)
{
  if(num > 3) return;

  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_BYTE_SW_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlaveWriteMode
*/
/**
//  Get write mode for the specified slave (0-3).
//  When set to 1, the transaction will read or write data only. When cleared to
//  0, the transaction will write a register address prior to reading or writing
//  data. This should equal 0 when specifying the register address within the
//  Slave device to/from which the ensuing data transaction will take place.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:24:01
//
//  @return       bool : Current write mode for specified slave (0 = register address + data, 1 = data only)
//
//  @param        num : Slave number (0-3)
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWriteMode(XBYTE num)
{
  if(num > 3) return false;

  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_REG_DIS_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveWriteMode
*/
/**
//  Set write mode for the specified slave (0-3).
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:25:33
//
//  @param        num : Slave number (0-3)
//  @param        mode : New write mode for specified slave (0 = register address + data, 1 = data only)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWriteMode(XBYTE num, bool mode)
{
  if(num > 3) return;

  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_REG_DIS_BIT, mode);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlaveWordGroupOffset
*/
/**
//  Get word pair grouping order offset for the specified slave (0-3).
//  This sets specifies the grouping order of word pairs received from registers.
//  When cleared to 0, bytes from register addresses 0 and 1, 2 and 3, etc (even,
//  then odd register addresses) are paired to form a word. When set to 1, bytes
//  from register addresses are paired 1 and 2, 3 and 4, etc. (odd, then even
//  register addresses) are paired to form a word.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:26:15
//
//  @return       bool : Current word pair grouping order offset for specified slave
//
//  @param        num : Slave number (0-3)
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWordGroupOffset(XBYTE num)
{
  if(num > 3) return false;

  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_GRP_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveWordGroupOffset
*/
/**
//  Set word pair grouping order offset for the specified slave (0-3).
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:27:29
//
//  @param        num : Slave number (0-3)
//  @param        enabled : New word pair grouping order offset for specified slave
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWordGroupOffset(XBYTE num, bool enabled)
{
  if(num > 3) return;

  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_GRP_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlaveDataLength
*/
/**
//  Get number of bytes to read for the specified slave (0-3).
//  Specifies the number of bytes transferred to and from Slave 0. Clearing this
//  bit to 0 is equivalent to disabling the register by writing 0 to I2C_SLV0_EN.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:28:14
//
//  @return       XBYTE :  Number of bytes to read for specified slave
//
//  @param        num : Slave number (0-3)
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlaveDataLength(XBYTE num)
{
  if(num > 3) return 0;
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_LEN_BIT, DIOI2CMPU9150_I2C_SLV_LEN_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveDataLength
*/
/**
//  Set number of bytes to read for the specified slave (0-3).
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:29:22
//
//  @param        num : Slave number (0-3)
//  @param        length : length Number of bytes to read for specified slave
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveDataLength(XBYTE num, XBYTE length)
{
  if(num > 3) return;
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_LEN_BIT, DIOI2CMPU9150_I2C_SLV_LEN_LENGTH, length);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4Address
*/
/**
//  Get the I2C address of Slave 4.
//  Note that Bit 7 (MSB) controls read/write mode. If Bit 7 is set, it's a read
//  operation, and if it is cleared, then it's a write operation. The remaining
//  bits (6-0) are the 7-bit device address of the slave device.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:30:00
//
//  @return       XBYTE : Current address for Slave 4
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4Address()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_ADDR, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave4Address
*/
/**
//  Set the I2C address of Slave 4.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:30:58
//
//  @param        address : New address for Slave 4
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Address(XBYTE address)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_ADDR, address);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4Register
*/
/**
//  Get the active internal register for the Slave 4.
//  Read/write operations for this slave will be done to whatever internal
//  register address is stored in this MPU register.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:31:29
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4Register()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_REG, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave4Register
*/
/**
//  Set the active internal register for Slave 4.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:32:03
//
//  @param        reg : New active register for Slave 4
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Register(XBYTE reg)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_REG, reg);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave4OutputByte
*/
/**
//  Set new byte to write to Slave 4.
//  This register stores the data to be written into the Slave 4. If I2C_SLV4_RW
//  is set 1 (set to read), this register has no effect.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:32:33
//
//  @param        data : New byte to write to Slave 4
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4OutputByte(XBYTE data)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_DO, data);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4Enabled
*/
/**
//  Get the enabled value for the Slave 4.
//  When set to 1, this bit enables Slave 4 for data transfer operations. When
//  cleared to 0, this bit disables Slave 4 from data transfer operations.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:33:23
//
//  @return       bool : Current enabled value for Slave 4
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4Enabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave4Enabled
*/
/**
//  Set the enabled value for Slave 4.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:34:12
//
//  @param        enabled : New enabled value for Slave 4
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Enabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_EN_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4InterruptEnabled
*/
/**
//  Get the enabled value for Slave 4 transaction interrupts.
//  When set to 1, this bit enables the generation of an interrupt signal upon
//  completion of a Slave 4 transaction. When cleared to 0, this bit disables the
//  generation of an interrupt signal upon completion of a Slave 4 transaction.
//  The interrupt status can be observed in Register 54.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:34:44
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4InterruptEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_INT_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave4InterruptEnabled
*/
/**
//  Set the enabled value for Slave 4 transaction interrupts.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:35:51
//
//  @param        enabled : New enabled value for Slave 4 transaction interrupts.
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4InterruptEnabled(bool enabled)
{
   WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_INT_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4WriteMode
*/
/**
//  Get write mode for Slave 4.
//  When set to 1, the transaction will read or write data only. When cleared to
//  0, the transaction will write a register address prior to reading or writing
//  data. This should equal 0 when specifying the register address within the
//  Slave device to/from which the ensuing data transaction will take place.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:36:26
//
//  @return       bool : Current write mode for Slave 4 (0 = register address + data, 1 = data only)
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4WriteMode()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_REG_DIS_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlave4WriteMode
*/
/**
//  Set write mode for the Slave 4.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 1:37:45
//
//  @param        mode : New write mode for Slave 4 (0 = register address + data, 1 = data only)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4WriteMode(bool mode)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_REG_DIS_BIT, mode);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4MasterDelay
*/
/**
//  Get Slave 4 master delay value.
//  This configures the reduced access rate of I2C slaves relative to the Sample
//  Rate. When a slave's access rate is decreased relative to the Sample Rate,
//  the slave is accessed every:
//
//     1 / (1 + I2C_MST_DLY) samples
//
//  This base Sample Rate in turn is determined by SMPLRT_DIV (register 25) and
//  DLPF_CFG (register 26). Whether a slave's access rate is reduced relative to
//  the Sample Rate is determined by I2C_MST_DELAY_CTRL (register 103). For
//  further information regarding the Sample Rate, please refer to register 25.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:12:42
//
//  @return       XBYTE : Current Slave 4 master delay value
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4MasterDelay()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_MST_DLY_BIT, DIOI2CMPU9150_I2C_SLV4_MST_DLY_LENGTH, buffer);
  return buffer[0];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4MasterDelay
*/
/**
//  Set Slave 4 master delay value.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:14:09
//
//  @param        delay : New Slave 4 master delay value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetSlave4MasterDelay(XBYTE delay)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_MST_DLY_BIT, DIOI2CMPU9150_I2C_SLV4_MST_DLY_LENGTH, delay);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlate4InputByte
*/
/**
//  Get last available byte read from Slave 4.
//  This register stores the data read from Slave 4. This field is populated
//  after a read transaction.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:14:39
//
//  @return       XBYTE : Last available byte read from to Slave 4
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlate4InputByte()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_DI, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetPassthroughStatus
*/
/**
//  Get FSYNC interrupt status.
//  This bit reflects the status of the FSYNC interrupt from an external device
//  into the MPU-60X0. This is used as a way to pass an external interrupt
//  through the MPU-60X0 to the host application processor. When set to 1, this
//  bit will cause an interrupt if FSYNC_INT_EN is asserted in INT_PIN_CFG
//  (Register 55).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:15:41
//
//  @return       bool : FSYNC interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetPassthroughStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_PASS_THROUGH_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4IsDone
*/
/**
//  Get Slave 4 transaction done status.
//  Automatically sets to 1 when a Slave 4 transaction has completed. This
//  triggers an interrupt if the I2C_MST_INT_EN bit in the INT_ENABLE register
//  (Register 56) is asserted and if the SLV_4_DONE_INT bit is asserted in the
//  I2C_SLV4_CTRL register (Register 52).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:16:48
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4IsDone()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV4_DONE_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetLostArbitration
*/
/**
//  Get master arbitration lost status.
//  This bit automatically sets to 1 when the I2C Master has lost arbitration of
//  the auxiliary I2C bus (an error condition). This triggers an interrupt if the
//  I2C_MST_INT_EN bit in the INT_ENABLE register (Register 56) is asserted.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:17:58
//
//  @return       bool : Master arbitration lost status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetLostArbitration()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_LOST_ARB_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave4Nack
*/
/**
//  Get Slave 4 NACK status.
//  This bit automatically sets to 1 when the I2C Master receives a NACK in a
//  transaction with Slave 4. This triggers an interrupt if the I2C_MST_INT_EN
//  bit in the INT_ENABLE register (Register 56) is asserted.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:18:44
//
//  @return       bool : Slave 4 NACK interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV4_NACK_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave3Nack
*/
/**
//  Get Slave 3 NACK status.
//  This bit automatically sets to 1 when the I2C Master receives a NACK in a
//  transaction with Slave 3. This triggers an interrupt if the I2C_MST_INT_EN
//  bit in the INT_ENABLE register (Register 56) is asserted.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:20:06
//
//  @return       bool : Slave 3 NACK interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave3Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV3_NACK_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave2Nack
*/
/**
//  Get Slave 2 NACK status.
//  This bit automatically sets to 1 when the I2C Master receives a NACK in a
//  transaction with Slave 2. This triggers an interrupt if the I2C_MST_INT_EN
//  bit in the INT_ENABLE register (Register 56) is asserted.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:20:55
//
//  @return       bool : Slave 2 NACK interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave2Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV2_NACK_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave1Nack
*/
/**
//  Get Slave 1 NACK status.
//  This bit automatically sets to 1 when the I2C Master receives a NACK in a
//  transaction with Slave 1. This triggers an interrupt if the I2C_MST_INT_EN
//  bit in the INT_ENABLE register (Register 56) is asserted.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:21:48
//
//  @return       bool : Slave 1 NACK interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave1Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV1_NACK_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlave0Nack
*/
/**
//  Get Slave 0 NACK status.
//  This bit automatically sets to 1 when the I2C Master receives a NACK in a
//  transaction with Slave 0. This triggers an interrupt if the I2C_MST_INT_EN
//  bit in the INT_ENABLE register (Register 56) is asserted.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:22:36
//
//  @return       bool : Slave 0 NACK interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave0Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV0_NACK_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetInterruptMode
*/
/**
//  Get interrupt logic level mode.
//  Will be set 0 for active-high, 1 for active-low.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:23:39
//
//  @return       bool : Current interrupt mode (0=active-high, 1=active-low)
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptMode()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_LEVEL_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetInterruptMode
*/
/**
//  Set interrupt logic level mode.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:24:19
//
//  @param        mode : New interrupt mode (0=active-high, 1=active-low)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetInterruptMode(bool mode)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_LEVEL_BIT, mode);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetInterruptDrive
*/
/**
//  Get interrupt drive mode.
//  Will be set 0 for push-pull, 1 for open-drain.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:24:53
//
//  @return       bool : Current interrupt drive mode (0=push-pull, 1=open-drain)
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptDrive()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_OPEN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetInterruptDrive
*/
/**
//  Set interrupt drive mode.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:25:30
//
//  @param        drive : New interrupt drive mode (0=push-pull, 1=open-drain)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetInterruptDrive(bool drive)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_OPEN_BIT, drive);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetInterruptLatch
*/
/**
//  Get interrupt latch mode.
//  Will be set 0 for 50us-pulse, 1 for latch-until-int-cleared.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:26:14
//
//  @return       bool : Current latch mode (0=50us-pulse, 1=latch-until-int-cleared)
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptLatch()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_LATCH_INT_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetInterruptLatch
*/
/**
//  Set interrupt latch mode.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:27:15
//
//  @param        latch : New latch mode (0=50us-pulse, 1=latch-until-int-cleared)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetInterruptLatch(bool latch)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_LATCH_INT_EN_BIT, latch);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetInterruptLatchClear
*/
/**
//  Get interrupt latch clear mode.
//  Will be set 0 for status-read-only, 1 for any-register-read.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:27:51
//
//  @return       bool : Current latch clear mode (0=status-read-only, 1=any-register-read)
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptLatchClear()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_RD_CLEAR_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetInterruptLatchClear
*/
/**
//  Set interrupt latch clear mode.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:28:47
//
//  @param        clear : New latch clear mode (0=status-read-only, 1=any-register-read)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetInterruptLatchClear(bool clear)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_RD_CLEAR_BIT, clear);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFSyncInterruptLevel
*/
/**
//  Get FSYNC interrupt logic level mode.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:29:25
//
//  @return       bool : Current FSYNC interrupt mode (0=active-high, 1=active-low)
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetFSyncInterruptLevel()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_FSYNC_INT_LEVEL_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFSyncInterruptLevel
*/
/**
//  Set FSYNC interrupt logic level mode.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:31:03
//
//  @param        level : New FSYNC interrupt mode (0=active-high, 1=active-low)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFSyncInterruptLevel(bool level)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_FSYNC_INT_LEVEL_BIT, level);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFSyncInterruptEnabled
*/
/**
//  Get FSYNC pin interrupt enabled setting.
//  Will be set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:31:43
//
//  @return       bool : Current interrupt enabled setting
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetFSyncInterruptEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_FSYNC_INT_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFSyncInterruptEnabled
*/
/**
//  Set FSYNC pin interrupt enabled setting.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:32:17
//
//  @param        enabled : New FSYNC pin interrupt enabled setting
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFSyncInterruptEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_FSYNC_INT_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetI2CBypassEnabled
*/
/**
//  Get I2C bypass enabled status.
//  When this bit is equal to 1 and I2C_MST_EN (Register 106 bit[5]) is equal to
//  0, the host application processor will be able to directly access the
//  auxiliary I2C bus of the MPU-60X0. When this bit is equal to 0, the host
//  application processor will not be able to directly access the auxiliary I2C
//  bus of the MPU-60X0 regardless of the state of I2C_MST_EN (Register 106
//  bit[5]).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:32:50
//
//  @return       bool : Current I2C bypass enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetI2CBypassEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_I2C_BYPASS_EN_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetI2CBypassEnabled
*/
/**
//  Set I2C bypass enabled status.
//  When this bit is equal to 1 and I2C_MST_EN (Register 106 bit[5]) is equal to
//  0, the host application processor will be able to directly access the
//  auxiliary I2C bus of the MPU-60X0. When this bit is equal to 0, the host
//  application processor will not be able to directly access the auxiliary I2C
//  bus of the MPU-60X0 regardless of the state of I2C_MST_EN (Register 106
//  bit[5]).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 17:34:19
//
//  @param        enabled :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetI2CBypassEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetClockOutputEnabled
*/
/**
//  Get reference clock output enabled status.
//  When this bit is equal to 1, a reference clock output is provided at the
//  CLKOUT pin. When this bit is equal to 0, the clock output is disabled. For
//  further information regarding CLKOUT, please refer to the MPU-60X0 Product
//  Specification document.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:07:01
//
//  @return       bool : Current reference clock output enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetClockOutputEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_CLKOUT_EN_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetClockOutputEnabled
*/
/**
//  Set reference clock output enabled status.
//  When this bit is equal to 1, a reference clock output is provided at the
//  CLKOUT pin. When this bit is equal to 0, the clock output is disabled. For
//  further information regarding CLKOUT, please refer to the MPU-60X0 Product
//  Specification document.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:09:23
//
//  @param        enabled :  New reference clock output enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetClockOutputEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_CLKOUT_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntEnabled
*/
/**
//  Get full interrupt enabled status.
//  Full register byte for all interrupts, for quick reading. Each bit will be
//  set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:11:11
//
//  @return       XBYTE : Current interrupt enabled status
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetIntEnabled()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntEnabled
*/
/**
//  Set full interrupt enabled status.
//  Full register byte for all interrupts, for quick reading. Each bit should be
//  set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:13:14
//
//  @param        enabled : New interrupt enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntEnabled(XBYTE enabled)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntFreefallEnabled
*/
/**
//  Get Free Fall interrupt enabled status.
//  Will be set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:13:56
//
//  @return       bool : Current interrupt enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntFreefallEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_FF_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntFreefallEnabled
*/
/**
//  Set Free Fall interrupt enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:14:50
//
//  @param        enabled : New interrupt enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntFreefallEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_FF_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntMotionEnabled
*/
/**
//  Get Motion Detection interrupt enabled status.
//  Will be set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:15:18
//
//  @return       bool : Current interrupt enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntMotionEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_MOT_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntMotionEnabled
*/
/**
//  Set Motion Detection interrupt enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:16:42
//
//  @param        enabled : New interrupt enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntMotionEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_MOT_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntZeroMotionEnabled
*/
/**
//  Get Zero Motion Detection interrupt enabled status.
//  Will be set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:17:18
//
//  @return       bool : Current interrupt enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntZeroMotionEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_ZMOT_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntZeroMotionEnabled
*/
/**
//  Set Zero Motion Detection interrupt enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:17:53
//
//  @param        enabled : New interrupt enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntZeroMotionEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_ZMOT_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntFIFOBufferOverflowEnabled
*/
/**
//  Get FIFO Buffer Overflow interrupt enabled status.
//  Will be set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:18:18
//
//  @return       bool : Current interrupt enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntFIFOBufferOverflowEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_FIFO_OFLOW_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntFIFOBufferOverflowEnabled
*/
/**
//  Set FIFO Buffer Overflow interrupt enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:18:49
//
//  @param        enabled : New interrupt enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntFIFOBufferOverflowEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_FIFO_OFLOW_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntI2CMasterEnabled
*/
/**
//  Get I2C Master interrupt enabled status.
//  This enables any of the I2C Master interrupt sources to generate an
//  interrupt. Will be set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:19:17
//
//  @return       bool : Current interrupt enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntI2CMasterEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_I2C_MST_INT_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntI2CMasterEnabled
*/
/**
//  Set I2C Master interrupt enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:19:52
//
//  @param        enabled : New interrupt enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntI2CMasterEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_I2C_MST_INT_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntDataReadyEnabled
*/
/**
//  Get Data Ready interrupt enabled setting.
//  This event occurs each time a write operation to all of the sensor registers
//  has been completed. Will be set 0 for disabled, 1 for enabled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:20:17
//
//  @return       bool : Current interrupt enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntDataReadyEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_DATA_RDY_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntDataReadyEnabled
*/
/**
//  Set Data Ready interrupt enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:20:54
//
//  @param        enabled : New interrupt enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntDataReadyEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_DATA_RDY_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntStatus
*/
/**
//  Get full set of interrupt status bits.
//  These bits clear to 0 after the register has been read. Very useful
//  for getting multiple INT statuses, since each single bit read clears
//  all of them because it has to read the whole byte.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:21:20
//
//  @return       XBYTE : Current interrupt status
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetIntStatus()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntFreefallStatus
*/
/**
//  Get Free Fall interrupt status.
//  This bit automatically sets to 1 when a Free Fall interrupt has been
//  generated. The bit clears to 0 after the register has been read.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:21:53
//
//  @return       bool : Current interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntFreefallStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_FF_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntMotionStatus
*/
/**
//  Get Motion Detection interrupt status.
//  This bit automatically sets to 1 when a Motion Detection interrupt has been
//  generated. The bit clears to 0 after the register has been read.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:22:32
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntMotionStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_MOT_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntZeroMotionStatus
*/
/**
//  Get Zero Motion Detection interrupt status.
//  This bit automatically sets to 1 when a Zero Motion Detection interrupt has
//  been generated. The bit clears to 0 after the register has been read.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:23:21
//
//  @return       bool : Current interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntZeroMotionStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_ZMOT_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntFIFOBufferOverflowStatus
*/
/**
//  Get FIFO Buffer Overflow interrupt status.
//  This bit automatically sets to 1 when a Free Fall interrupt has been
//  generated. The bit clears to 0 after the register has been read.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:23:49
//
//  @return       bool : Current interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntFIFOBufferOverflowStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_FIFO_OFLOW_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntI2CMasterStatus
*/
/**
//  Get I2C Master interrupt status.
//  This bit automatically sets to 1 when an I2C Master interrupt has been
//  generated. For a list of I2C Master interrupts, please refer to Register 54.
//  The bit clears to 0 after the register has been read.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:24:35
//
//  @return       bool : Current interrupt status
//
*/
/*-------------- ---------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntI2CMasterStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_I2C_MST_INT_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntDataReadyStatus
*/
/**
//  Get Data Ready interrupt status.
//  This bit automatically sets to 1 when a Data Ready interrupt has been
//  generated. The bit clears to 0 after the register has been read.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:25:29
//
//  @return       bool : Current interrupt status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntDataReadyStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_DATA_RDY_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetMotion9
*/
/**
//  Get raw 9-axis motion sensor readings (accel/gyro/compass).
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:26:05
//
//  @param        ax : 16-bit signed integer container for accelerometer X-axis value
//  @param        ay : 16-bit signed integer container for accelerometer Y-axis value
//  @param        az : 16-bit signed integer container for accelerometer Z-axis value
//  @param        gx : 16-bit signed integer container for gyroscope X-axis value
//  @param        gy : 16-bit signed integer container for gyroscope Y-axis value
//  @param        gz : 16-bit signed integer container for gyroscope Z-axis value
//  @param        mx : 16-bit signed integer container for magnetometer X-axis value
//  @param        my : 16-bit signed integer container for magnetometer Y-axis value
//  @param        mz : 16-bit signed integer container for magnetometer Z-axis value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetMotion9(XWORDSIG* ax, XWORDSIG* ay, XWORDSIG* az, XWORDSIG* gx, XWORDSIG* gy, XWORDSIG* gz, XWORDSIG* mx, XWORDSIG* my, XWORDSIG* mz)
{
  //get accel and gyro
  GetMotion6(ax, ay, az, gx, gy, gz);

  //read mag
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, 0x02); //set i2c bypass enable pin to true to access magnetometer
  xsleep->MilliSeconds(10);

  WriteByte(DIOI2CMPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
  xsleep->MilliSeconds(10);

  ReadBytes(DIOI2CMPU9150_RA_MAG_ADDRESS, DIOI2CMPU9150_RA_MAG_XOUT_L, 6, buffer);
  *mx = (((XWORDSIG)buffer[1]) << 8) | buffer[0];
  *my = (((XWORDSIG)buffer[3]) << 8) | buffer[2];
  *mz = (((XWORDSIG)buffer[5]) << 8) | buffer[4];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetMag
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:27:48
//
//  @param        mx :
//  @param        my :
//  @param        mz :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetMag(XWORDSIG* mx, XWORDSIG* my, XWORDSIG* mz)
{
  //read mag
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, 0x02); //set i2c bypass enable pin to true to access magnetometer
  xsleep->MilliSeconds(10);

  WriteByte(DIOI2CMPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
  xsleep->MilliSeconds(10);

  ReadBytes(DIOI2CMPU9150_RA_MAG_ADDRESS, DIOI2CMPU9150_RA_MAG_XOUT_L, 6, buffer);
  *mx = (((XWORDSIG)buffer[1]) << 8) | buffer[0];
  *my = (((XWORDSIG)buffer[3]) << 8) | buffer[2];
  *mz = (((XWORDSIG)buffer[5]) << 8) | buffer[4];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetMotion6
*/
/**
//  Get raw 6-axis motion sensor readings (accel/gyro).
//  Retrieves all currently available motion sensor values.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:28:21
//
//  @param        ax : 16-bit signed integer container for accelerometer X-axis value
//  @param        ay : 16-bit signed integer container for accelerometer Y-axis value
//  @param        az : 16-bit signed integer container for accelerometer Z-axis value
//  @param        gx : 16-bit signed integer container for gyroscope X-axis value
//  @param        gy : 16-bit signed integer container for gyroscope Y-axis value
//  @param        gz : 16-bit signed integer container for gyroscope Z-axis value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetMotion6(XWORDSIG* ax, XWORDSIG* ay, XWORDSIG* az, XWORDSIG* gx, XWORDSIG* gy, XWORDSIG* gz)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_XOUT_H, 14, buffer);
  *ax = (((XWORDSIG)buffer[0])  << 8) | buffer[1];
  *ay = (((XWORDSIG)buffer[2])  << 8) | buffer[3];
  *az = (((XWORDSIG)buffer[4])  << 8) | buffer[5];
  *gx = (((XWORDSIG)buffer[8])  << 8) | buffer[9];
  *gy = (((XWORDSIG)buffer[10]) << 8) | buffer[11];
  *gz = (((XWORDSIG)buffer[12]) << 8) | buffer[13];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAcceleration
*/
/**
//  Get 3-axis accelerometer readings.
//  These registers store the most recent accelerometer measurements.
//  Accelerometer measurements are written to these registers at the Sample Rate
//  as defined in Register 25.
//
//  The accelerometer measurement registers, along with the temperature
//  measurement registers, gyroscope measurement registers, and external sensor
//  data registers, are composed of two sets of registers: an internal register
//  set and a user-facing read register set.
//
//  The data within the accelerometer sensors' internal register set is always
//  updated at the Sample Rate. Meanwhile, the user-facing read register set
//  duplicates the internal register set's data values whenever the serial
//  interface is idle. This guarantees that a burst read of sensor registers will
//  read measurements from the same sampling instant. Note that if burst reads
//  are not used, the user is responsible for ensuring a set of single byte reads
//  correspond to a single sampling instant by checking the Data Ready interrupt.
//
//  Each 16-bit accelerometer measurement has a full scale defined in ACCEL_FS
//  (Register 28). For each full scale setting, the accelerometers' sensitivity
//  per LSB in ACCEL_xOUT is shown in the table below:
//
//  AFS_SEL | Full Scale Range | LSB Sensitivity
//  --------+------------------+----------------
//  0       | +/- 2g           | 8192 LSB/mg
//  1       | +/- 4g           | 4096 LSB/mg
//  2       | +/- 8g           | 2048 LSB/mg
//  3       | +/- 16g          | 1024 LSB/mg
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:29:15
//
//  @param        x : 16-bit signed integer container for X-axis acceleration
//  @param        y : 16-bit signed integer container for Y-axis acceleration
//  @param        z : 16-bit signed integer container for Z-axis acceleration
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetAcceleration(XWORDSIG* x, XWORDSIG* y, XWORDSIG* z)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_XOUT_H, 6, buffer);
  *x = (((XWORDSIG)buffer[0]) << 8) | buffer[1];
  *y = (((XWORDSIG)buffer[2]) << 8) | buffer[3];
  *z = (((XWORDSIG)buffer[4]) << 8) | buffer[5];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAccelerationX
*/
/**
//  Get X-axis accelerometer reading.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:31:35
//
//  @return       XWORDSIG : X-axis acceleration measurement in 16-bit 2's complement format
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationX()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_XOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAccelerationY
*/
/**
//  Get Y-axis accelerometer reading.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:31:57
//
//  @return       XWORDSIG : Y-axis acceleration measurement in 16-bit 2's complement format
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationY()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_YOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAccelerationZ
*/
/**
//  Get Z-axis accelerometer reading.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:32:27
//
//  @return       XWORDSIG : Z-axis acceleration measurement in 16-bit 2's complement format
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationZ()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_ZOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetTemperature
*/
/**
//  Get current internal temperature.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:32:52
//
//  @return       XWORDSIG : Temperature reading in 16-bit 2's complement format
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetTemperature()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_TEMP_OUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetRotation
*/
/**
// Get 3-axis gyroscope readings.
// These gyroscope measurement registers, along with the accelerometer
// measurement registers, temperature measurement registers, and external sensor
// data registers, are composed of two sets of registers: an internal register
// set and a user-facing read register set.
// The data within the gyroscope sensors' internal register set is always
// updated at the Sample Rate. Meanwhile, the user-facing read register set
// duplicates the internal register set's data values whenever the serial
// interface is idle. This guarantees that a burst read of sensor registers will
// read measurements from the same sampling instant. Note that if burst reads
// are not used, the user is responsible for ensuring a set of single byte reads
// correspond to a single sampling instant by checking the Data Ready interrupt.
//
// Each 16-bit gyroscope measurement has a full scale defined in FS_SEL
// (Register 27). For each full scale setting, the gyroscopes' sensitivity per
// LSB in GYRO_xOUT is shown in the table below:
//
// FS_SEL | Full Scale Range   | LSB Sensitivity
// -------+--------------------+----------------
// 0      | +/- 250 degrees/s  | 131 LSB/deg/s
// 1      | +/- 500 degrees/s  | 65.5 LSB/deg/s
// 2      | +/- 1000 degrees/s | 32.8 LSB/deg/s
// 3      | +/- 2000 degrees/s | 16.4 LSB/deg/s
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:33:12
//
//  @param        x : x 16-bit signed integer container for X-axis rotation
//  @param        y : y 16-bit signed integer container for Y-axis rotation
//  @param        z : z 16-bit signed integer container for Z-axis rotation
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetRotation(XWORDSIG* x, XWORDSIG* y, XWORDSIG* z)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_GYRO_XOUT_H, 6, buffer);
  *x = (((XWORDSIG)buffer[0]) << 8) | buffer[1];
  *y = (((XWORDSIG)buffer[2]) << 8) | buffer[3];
  *z = (((XWORDSIG)buffer[4]) << 8) | buffer[5];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetRotationX
*/
/**
//  Get X-axis gyroscope reading.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:35:35
//
//  @return       XWORDSIG : X-axis rotation measurement in 16-bit 2's complement format
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationX()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_GYRO_XOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetRotationY
*/
/**
//  Get Y-axis gyroscope reading.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:36:06
//
//  @return       XWORDSIG : Y-axis rotation measurement in 16-bit 2's complement format
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationY()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_GYRO_YOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetRotationZ
*/
/**
//  Get Z-axis gyroscope reading.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:36:33
//
//  @return       XWORDSIG : Z-axis rotation measurement in 16-bit 2's complement format
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationZ()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_GYRO_ZOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}






/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorByte
*/
/**
// Read single byte from external sensor data register.
// These registers store data read from external sensors by the Slave 0, 1, 2,
// and 3 on the auxiliary I2C interface. Data read by Slave 4 is stored in
// I2C_SLV4_DI (Register 53).
//
// External sensor data is written to these registers at the Sample Rate as
// defined in Register 25. This access rate can be reduced by using the Slave
// Delay Enable registers (Register 103).
//
// External sensor data registers, along with the gyroscope measurement
// registers, accelerometer measurement registers, and temperature measurement
// registers, are composed of two sets of registers: an internal register set
// and a user-facing read register set.
//
// The data within the external sensors' internal register set is always updated
// at the Sample Rate (or the reduced access rate) whenever the serial interface
// is idle. This guarantees that a burst read of sensor registers will read
// measurements from the same sampling instant. Note that if burst reads are not
// used, the user is responsible for ensuring a set of single byte reads
// correspond to a single sampling instant by checking the Data Ready interrupt.
//
// Data is placed in these external sensor data registers according to
// I2C_SLV0_CTRL, I2C_SLV1_CTRL, I2C_SLV2_CTRL, and I2C_SLV3_CTRL (Registers 39,
// 42, 45, and 48). When more than zero bytes are read (I2C_SLVx_LEN > 0) from
// an enabled slave (I2C_SLVx_EN = 1), the slave is read at the Sample Rate (as
// defined in Register 25) or delayed rate (if specified in Register 52 and
// 103). During each Sample cycle, slave reads are performed in order of Slave
// number. If all slaves are enabled with more than zero bytes to be read, the
// order will be Slave 0, followed by Slave 1, Slave 2, and Slave 3.
//
// Each enabled slave will have EXT_SENS_DATA registers associated with it by
// number of bytes read (I2C_SLVx_LEN) in order of slave number, starting from
// EXT_SENS_DATA_00. Note that this means enabling or disabling a slave may
// change the higher numbered slaves' associated registers. Furthermore, if
// fewer total bytes are being read from the external sensors as a result of
// such a change, then the data remaining in the registers which no longer have
// an associated slave device (i.e. high numbered registers) will remain in
// these previously allocated registers unless reset.
//
// If the sum of the read lengths of all SLVx transactions exceed the number of
// available EXT_SENS_DATA registers, the excess bytes will be dropped. There
// are 24 EXT_SENS_DATA registers and hence the total read lengths between all
// the slaves cannot be greater than 24 or some bytes will be lost.
//
// Note: Slave 4's behavior is distinct from that of Slaves 0-3. For further
// information regarding the characteristics of Slave 4, please refer to
// Registers 49 to 53.
//
// EXAMPLE:
// Suppose that Slave 0 is enabled with 4 bytes to be read (I2C_SLV0_EN = 1 and
// I2C_SLV0_LEN = 4) while Slave 1 is enabled with 2 bytes to be read so that
// I2C_SLV1_EN = 1 and I2C_SLV1_LEN = 2. In such a situation, EXT_SENS_DATA _00
// through _03 will be associated with Slave 0, while EXT_SENS_DATA _04 and 05
// will be associated with Slave 1. If Slave 2 is enabled as well, registers
// starting from EXT_SENS_DATA_06 will be allocated to Slave 2.
//
// If Slave 2 is disabled while Slave 3 is enabled in this same situation, then
// registers starting from EXT_SENS_DATA_06 will be allocated to Slave 3
// instead.
//
// REGISTER ALLOCATION FOR DYNAMIC DISABLE VS. NORMAL DISABLE:
// If a slave is disabled at any time, the space initially allocated to the
// slave in the EXT_SENS_DATA register, will remain associated with that slave.
// This is to avoid dynamic adjustment of the register allocation.
//
// The allocation of the EXT_SENS_DATA registers is recomputed only when (1) all
// slaves are disabled, or (2) the I2C_MST_RST bit is set (Register 106).
//
// This above is also true if one of the slaves gets NACKed and stops
// functioning.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:37:03
//
//  @return       XBYTE : read from register
//
//  @param        position : Starting position (0-23)
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorByte(int position)
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_EXT_SENS_DATA_00 + position, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorWord
*/
/**
//  Read word (2 bytes) from external sensor data registers.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:41:47
//
//  @return       XWORDSIG : Word read from register
//
//  @param        position : Starting position (0-21)
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorWord(int position)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_EXT_SENS_DATA_00 + position, 2, buffer);
  return (XWORDSIG)(((XWORDSIG)buffer[0]) << 8) | buffer[1];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorDWord
*/
/**
//  Read double word (4 bytes) from external sensor data registers.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:42:57
//
//  @return       XDWORD : Double word read from registers
//
//  @param        position : Starting position (0-20)
*/
/*-----------------------------------------------------------------*/
XDWORD DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorDWord(int position)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_EXT_SENS_DATA_00 + position, 4, buffer);
  return (((XDWORD)buffer[0]) << 24) | (((XDWORD)buffer[1]) << 16) | (((XWORDSIG)buffer[2]) << 8) | buffer[3];
}





/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetXNegMotionDetected
*/
/**
//  Get X-axis negative motion detection interrupt status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:43:32
//
//  @return       bool : Motion detection status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetXNegMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_XNEG_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetXPosMotionDetected
*/
/**
//  Get X-axis positive motion detection interrupt status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:43:59
//
//  @return       bool : Motion detection status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetXPosMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_XPOS_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetYNegMotionDetected
*/
/**
//  Get Y-axis negative motion detection interrupt status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:44:21
//
//  @return       bool : Motion detection status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetYNegMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_YNEG_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetYPosMotionDetected
*/
/**
//  Get Y-axis positive motion detection interrupt status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:44:42
//
//  @return       bool : Motion detection status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetYPosMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_YPOS_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZNegMotionDetected
*/
/**
//  Get Z-axis negative motion detection interrupt status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:45:00
//
//  @return       bool : Motion detection status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetZNegMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_ZNEG_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZPosMotionDetected
*/
/**
//  Get Z-axis positive motion detection interrupt status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:45:30
//
//  @return       bool : Motion detection status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetZPosMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_ZPOS_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetected
*/
/**
//  Get zero motion detection interrupt status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:46:03
//
//  @return       bool : Motion detection status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_ZRMOT_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveOutputByte
*/
/**
//  Write byte to Data Output container for specified slave.
//  This register holds the output data written into Slave when Slave is set to
//  write mode. For further information regarding Slave control, please
//  refer to Registers 37 to 39 and immediately following.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:46:24
//
//  @param        num : Slave number (0-3)
//  @param        data : Byte to write
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveOutputByte(XBYTE num, XBYTE data)
{
  if(num > 3) return;

  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_DO + num, data);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetExternalShadowDelayEnabled
*/
/**
//  Get external data shadow delay enabled status.
//  This register is used to specify the timing of external sensor data
//  shadowing. When DELAY_ES_SHADOW is set to 1, shadowing of external
//  sensor data is delayed until all data has been received.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:47:12
//
//  @return       bool : Current external data shadow delay enabled status.
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetExternalShadowDelayEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL, DIOI2CMPU9150_DELAYCTRL_DELAY_ES_SHADOW_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetExternalShadowDelayEnabled
*/
/**
//  Set external data shadow delay enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:48:14
//
//  @param        enabled : New external data shadow delay enabled status.
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetExternalShadowDelayEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL, DIOI2CMPU9150_DELAYCTRL_DELAY_ES_SHADOW_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSlaveDelayEnabled
*/
/**
// Get slave delay enabled status.
// When a particular slave delay is enabled, the rate of access for the that
// slave device is reduced. When a slave's access rate is decreased relative to
// the Sample Rate, the slave is accessed every:
//
//  1 / (1 + I2C_MST_DLY) Samples
//
// This base Sample Rate in turn is determined by SMPLRT_DIV (register  * 25)
// and DLPF_CFG (register 26).
//
// For further information regarding I2C_MST_DLY, please refer to register 52.
// For further information regarding the Sample Rate, please refer to register 25.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:48:39
//
//  @return       bool : Current slave delay enabled status.
//
//  @param        num : Slave number (0-4)
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveDelayEnabled(XBYTE num)
{
  // DIOI2CMPU9150_DELAYCTRL_I2C_SLV4_DLY_EN_BIT is 4, SLV3 is 3, etc.
  if(num > 4) return false;
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL, num, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSlaveDelayEnabled
*/
/**
//  Set slave delay enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:50:14
//
//  @param        num : Slave number (0-4)
//  @param        enabled : New slave delay enabled status.
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveDelayEnabled(XBYTE num, bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL, num, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ResetGyroscopePath
*/
/**
//  Reset gyroscope signal path.
//  The reset will revert the signal path analog to digital converters and
//  filters to their power up configurations.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:57:20
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetGyroscopePath()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_SIGNAL_PATH_RESET, DIOI2CMPU9150_PATHRESET_GYRO_RESET_BIT, true);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ResetAccelerometerPath
*/
/**
//  Reset accelerometer signal path.
//  The reset will revert the signal path analog to digital converters and
//  filters to their power up configurations.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:58:04
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetAccelerometerPath()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_SIGNAL_PATH_RESET, DIOI2CMPU9150_PATHRESET_ACCEL_RESET_BIT, true);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ResetTemperaturePath
*/
/**
//  Reset temperature sensor signal path.
//  The reset will revert the signal path analog to digital converters and
//  filters to their power up configurations.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:58:43
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetTemperaturePath()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_SIGNAL_PATH_RESET, DIOI2CMPU9150_PATHRESET_TEMP_RESET_BIT, true);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetAccelerometerPowerOnDelay
*/
/**
//  Get accelerometer power-on delay.
//  The accelerometer data path provides samples to the sensor registers, Motion
//  detection, Zero Motion detection, and Free Fall detection modules. The
//  signal path contains filters which must be flushed on wake-up with new
//  samples before the detection modules begin operations. The default wake-up
//  delay, of 4ms can be lengthened by up to 3ms. This additional delay is
//  specified in ACCEL_ON_DELAY in units of 1 LSB = 1 ms. The user may select
//  any value above zero unless instructed otherwise by InvenSense. Please refer
//  to Section 8 of the MPU-6000/MPU-6050 Product Specification document for
//  further information regarding the detection modules.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 18:59:18
//
//  @return       XBYTE : Current accelerometer power-on delay
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetAccelerometerPowerOnDelay()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_BIT, DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_LENGTH, buffer);
  return buffer[0];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetAccelerometerPowerOnDelay
*/
/**
//  Set accelerometer power-on delay.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:00:36
//
//  @param        delay : New accelerometer power-on delay (0-3)
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelerometerPowerOnDelay(XBYTE delay)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_BIT, DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_LENGTH, delay);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionCounterDecrement
*/
/**
//  Get Free Fall detection counter decrement configuration.
//  Detection is registered by the Free Fall detection module after accelerometer
//  measurements meet their respective threshold conditions over a specified
//  number of samples. When the threshold conditions are met, the corresponding
//  detection counter increments by 1. The user may control the rate at which the
//  detection counter decrements when the threshold condition is not met by
//  configuring FF_COUNT. The decrement rate can be set according to the
//  following table:
//
//  FF_COUNT | Counter Decrement
//  ---------+------------------
//  0        | Reset
//  1        | 1
//  2        | 2
//  3        | 4

//  When FF_COUNT is configured to 0 (reset), any non-qualifying sample will
//  reset the counter to 0. For further information on Free Fall detection,
//  please refer to Registers 29 to 32.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:01:04
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionCounterDecrement()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_FF_COUNT_BIT, DIOI2CMPU9150_DETECT_FF_COUNT_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionCounterDecrement
*/
/**
//  Set Free Fall detection counter decrement configuration.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:02:43
//
//  @param        decrement : New decrement configuration value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionCounterDecrement(XBYTE decrement)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_FF_COUNT_BIT, DIOI2CMPU9150_DETECT_FF_COUNT_LENGTH, decrement);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionCounterDecrement
*/
/**
//  Get Motion detection counter decrement configuration.
//  Detection is registered by the Motion detection module after accelerometer
//  measurements meet their respective threshold conditions over a specified
//  number of samples. When the threshold conditions are met, the corresponding
//  detection counter increments by 1. The user may control the rate at which the
//  detection counter decrements when the threshold condition is not met by
//  configuring MOT_COUNT. The decrement rate can be set according to the
//  following table:
//
//  MOT_COUNT | Counter Decrement
//  ----------+------------------
//  0         | Reset
//  1         | 1
//  2         | 2
//  3         | 4
//
//  When MOT_COUNT is configured to 0 (reset), any non-qualifying sample will
//  reset the counter to 0. For further information on Motion detection,
//  please refer to Registers 29 to 32.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:05:20
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionCounterDecrement()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_MOT_COUNT_BIT, DIOI2CMPU9150_DETECT_MOT_COUNT_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionCounterDecrement
*/
/**
//  Set Motion detection counter decrement configuration.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:05:43
//
//  @param        decrement : New decrement configuration value
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionCounterDecrement(XBYTE decrement)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_MOT_COUNT_BIT, DIOI2CMPU9150_DETECT_MOT_COUNT_LENGTH, decrement);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFIFOEnabled
*/
/**
//  Get FIFO enabled status.
//  When this bit is set to 0, the FIFO buffer is disabled. The FIFO buffer
//  cannot be written to or read from while disabled. The FIFO buffer's state
//  does not change unless the MPU-60X0 is power cycled.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:06:04
//
//  @return       bool : Current FIFO enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFIFOEnabled
*/
/**
//  Set FIFO enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:06:40
//
//  @param        enabled : New FIFO enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_FIFO_EN_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetI2CMasterModeEnabled
*/
/**
//  Get I2C Master Mode enabled status.
//  When this mode is enabled, the MPU-60X0 acts as the I2C Master to the
//  external sensor slave devices on the auxiliary I2C bus. When this bit is
//  cleared to 0, the auxiliary I2C bus lines (AUX_DA and AUX_CL) are logically
//  driven by the primary I2C bus (SDA and SCL). This is a precondition to
//  enabling Bypass Mode. For further information regarding Bypass Mode, please
//  refer to Register 55.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:07:03
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetI2CMasterModeEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_I2C_MST_EN_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetI2CMasterModeEnabled
*/
/**
//  Set I2C Master Mode enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:07:40
//
//  @param        enabled : New I2C Master Mode enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetI2CMasterModeEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_I2C_MST_EN_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SwitchSPIEnabled
*/
/**
//  Switch from I2C to SPI mode (MPU-6000 only)
//  If this is set, the primary SPI interface will be enabled in place of the
//  disabled primary I2C interface.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:08:04
//
//  @param        enabled :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SwitchSPIEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_I2C_IF_DIS_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ResetFIFO
*/
/**
//  Reset the FIFO.
//  This bit resets the FIFO buffer when set to 1 while FIFO_EN equals 0. This
//  bit automatically clears to 0 after the reset has been triggered.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:08:34
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetFIFO()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_FIFO_RESET_BIT, true);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ResetI2CMaster
*/
/**
//  Reset the I2C Master.
//  This bit resets the I2C Master when set to 1 while I2C_MST_EN equals 0.
//  This bit automatically clears to 0 after the reset has been triggered.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:08:57
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetI2CMaster()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_I2C_MST_RESET_BIT, true);
}



/**
 *
 *
 * When resetting only the signal path (and not the sensor registers), please
 * use Register 104, SIGNAL_PATH_RESET.
 *
 * @see DIOI2CMPU9150_RA_USER_CTRL
 * @see DIOI2CMPU9150_USERCTRL_SIG_COND_RESET_BIT
 */


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ResetSensors
*/
/**
//  Reset all sensor registers and signal paths.
//  When set to 1, this bit resets the signal paths for all sensors (gyroscopes,
//  accelerometers, and temperature sensor). This operation will also clear the
//  sensor registers. This bit automatically clears to 0 after the reset has been
//  triggered.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:09:26
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetSensors()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_SIG_COND_RESET_BIT, true);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::Reset
*/
/**
//  Trigger a full device reset.
//  A small delay of ~50ms may be desirable after triggering a reset.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:10:30
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::Reset()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_DEVICE_RESET_BIT, true);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetSleepEnabled
*/
/**
//  Get sleep mode status.
//  Setting the SLEEP bit in the register puts the device into very low power
//  sleep mode. In this mode, only the serial interface and internal registers
//  remain active, allowing for a very low standby current. Clearing this bit
//  puts the device back into normal mode. To save power, the individual standby
//  selections for each of the gyros should be used if any gyro axis is not used
//  by the application.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:11:03
//
//  @return       bool : Current sleep mode enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSleepEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_SLEEP_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetSleepEnabled
*/
/**
//  Set sleep mode status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:11:54
//
//  @param        enabled : New sleep mode enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSleepEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_SLEEP_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetWakeCycleEnabled
*/
/**
//  Get wake cycle enabled status.
//  When this bit is set to 1 and SLEEP is disabled, the MPU-60X0 will cycle
//  between sleep mode and waking up to take a single sample of data from active
//  sensors at a rate determined by LP_WAKE_CTRL (register 108).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:12:23
//
//  @return       bool : Current sleep mode enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetWakeCycleEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_CYCLE_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetWakeCycleEnabled
*/
/**
//  Set wake cycle enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:13:00
//
//  @param        enabled : New sleep mode enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetWakeCycleEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_CYCLE_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetTempSensorEnabled
*/
/**
//  Get temperature sensor enabled status.
//  Control the usage of the internal temperature sensor.
//
//  Note: this register stores the *disabled* value, but for consistency with the
//  rest of the code, the function is named and used with standard true/false
//  values to indicate whether the sensor is enabled or disabled, respectively.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:13:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetTempSensorEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_TEMP_DIS_BIT, buffer);
  return (buffer[0] == 0)?true:false; // 1 is actually disabled here
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetTempSensorEnabled
*/
/**
//  Set temperature sensor enabled status.
//  Note: this register stores the *disabled* value, but for consistency with the
//  rest of the code, the function is named and used with standard true/false
//  values to indicate whether the sensor is enabled or disabled, respectively.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:13:59
//
//  @param        enabled : New temperature sensor enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetTempSensorEnabled(bool enabled)
{
  // 1 is actually disabled here
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_TEMP_DIS_BIT, !enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetClockSource
*/
/**
//  Get clock source setting.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:14:31
//
//  @return       XBYTE :  Current clock source setting
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetClockSource()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_CLKSEL_BIT, DIOI2CMPU9150_PWR1_CLKSEL_LENGTH, buffer);
  return buffer[0];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetClockSource
*/
/**
// Set clock source setting.
// An internal 8MHz oscillator, gyroscope based clock, or external sources can
// be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
// or an external source is chosen as the clock source, the MPU-60X0 can operate
// in low power modes with the gyroscopes disabled.
//
// Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
// However, it is highly recommended that the device be configured to use one of
// the gyroscopes (or an external clock source) as the clock reference for
// improved stability. The clock source can be selected according to the following table:
//
// CLK_SEL | Clock Source
// --------+--------------------------------------
// 0       | Internal oscillator
// 1       | PLL with X Gyro reference
// 2       | PLL with Y Gyro reference
// 3       | PLL with Z Gyro reference
// 4       | PLL with external 32.768kHz reference
// 5       | PLL with external 19.2MHz reference
// 6       | Reserved
// 7       | Stops the clock and keeps the timing generator in reset
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:14:55
//
//  @param        source : New clock source setting
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetClockSource(XBYTE source)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_CLKSEL_BIT, DIOI2CMPU9150_PWR1_CLKSEL_LENGTH, source);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetWakeFrequency
*/
/**
//  Get wake frequency in Accel-Only Low Power Mode.
//  The MPU-60X0 can be put into Accerlerometer Only Low Power Mode by setting
//  PWRSEL to 1 in the Power Management 1 register (Register 107). In this mode,
//  the device will power off all devices except for the primary I2C interface,
//  waking only the accelerometer at fixed intervals to take a single
//  measurement. The frequency of wake-ups can be configured with LP_WAKE_CTRL
//  as shown below:
//
//  LP_WAKE_CTRL | Wake-up Frequency
//  -------------+------------------
//  0            | 1.25 Hz
//  1            | 2.5 Hz
//  2            | 5 Hz
//  3            | 10 Hz
//
//  For further information regarding the MPU-60X0's power modes, please refer to
//  Register 107.
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:17:00
//
//  @return       XBYTE : Current wake frequency
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetWakeFrequency()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_BIT, DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetWakeFrequency
*/
/**
//  Set wake frequency in Accel-Only Low Power Mode.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:18:33
//
//  @param        frequency : New wake frequency
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetWakeFrequency(XBYTE frequency)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_BIT, DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_LENGTH, frequency);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetStandbyXAccelEnabled
*/
/**
//  Get X-axis accelerometer standby enabled status.
//  If enabled, the X-axis will not gather or report data (or use power).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:18:51
//
//  @return       bool : Current X-axis standby enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyXAccelEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_XA_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetStandbyXAccelEnabled
*/
/**
//  Set X-axis accelerometer standby enabled status
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:19:13
//
//  @param        enabled : New X-axis standby enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyXAccelEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_XA_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetStandbyYAccelEnabled
*/
/**
//  Get Y-axis accelerometer standby enabled status.
//  If enabled, the Y-axis will not gather or report data (or use power).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:19:43
//
//  @return       bool : Current Y-axis standby enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyYAccelEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_YA_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetStandbyYAccelEnabled
*/
/**
//  Set Y-axis accelerometer standby enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:20:13
//
//  @param        enabled : New Y-axis standby enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyYAccelEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_YA_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetStandbyZAccelEnabled
*/
/**
//  Get Z-axis accelerometer standby enabled status.
//  If enabled, the Z-axis will not gather or report data (or use power).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:20:36
//
//  @return       bool : Current Z-axis standby enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyZAccelEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_ZA_BIT, buffer);
  return buffer[0]?true:false;
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetStandbyZAccelEnabled
*/
/**
//  Set Z-axis accelerometer standby enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:21:11
//
//  @param        enabled : New Z-axis standby enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyZAccelEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_ZA_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetStandbyXGyroEnabled
*/
/**
//  Get X-axis gyroscope standby enabled status
//  If enabled, the X-axis will not gather or report data (or use power).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:21:39
//
//  @return       bool : Current X-axis standby enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyXGyroEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_XG_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetStandbyXGyroEnabled
*/
/**
//  Set X-axis gyroscope standby enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:22:05
//
//  @param        enabled : New X-axis standby enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyXGyroEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_XG_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetStandbyYGyroEnabled
*/
/**
//  Get Y-axis gyroscope standby enabled status.
//  If enabled, the Y-axis will not gather or report data (or use power).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:22:27
//
//  @return       bool : Current Y-axis standby enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyYGyroEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_YG_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetStandbyYGyroEnabled
*/
/**
//  Set Y-axis gyroscope standby enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 16:24:33
//
//  @param        enabled : New Y-axis standby enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyYGyroEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_YG_BIT, enabled);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetStandbyZGyroEnabled
*/
/**
//  Get Z-axis gyroscope standby enabled status.
//   If enabled, the Z-axis will not gather or report data (or use power).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:22:50
//
//  @return       bool : Current Z-axis standby enabled status
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyZGyroEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_ZG_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetStandbyZGyroEnabled
*/
/**
//  Set Z-axis gyroscope standby enabled status.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:23:25
//
//  @param        enabled : New Z-axis standby enabled status
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyZGyroEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_ZG_BIT, enabled);
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFIFOCount
*/
/**
//  Get current FIFO buffer size.
//  This value indicates the number of bytes stored in the FIFO buffer. This
//  number is in turn the number of bytes that can be read from the FIFO buffer
//  and it is directly proportional to the number of samples available given the
//  set of sensor data bound to be stored in the FIFO (register 35 and 36).
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:23:43
//
//  @return       XWORDSIG :
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetFIFOCount()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_FIFO_COUNTH, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}




/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFIFOByte
*/
/**
//  Get byte from FIFO buffer.
//  This register is used to read and write data from the FIFO buffer. Data is
//  written to the FIFO in order of register number (from lowest to highest). If
//  all the FIFO enable flags (see below) are enabled and all External Sensor
//  Data registers (Registers 73 to 96) are associated with a Slave device, the
//  contents of registers 59 through 96 will be written in order at the Sample
//  Rate.
//
//  The contents of the sensor data registers (Registers 59 to 96) are written
//  into the FIFO buffer when their corresponding FIFO enable flags are set to 1
//  in FIFO_EN (Register 35). An additional flag for the sensor data registers
//  associated with I2C Slave 3 can be found in I2C_MST_CTRL (Register 36).
//
//  if the FIFO buffer has overflowed, the status bit FIFO_OFLOW_INT is
//  automatically set to 1. This bit is located in INT_STATUS (Register 58).
//  When the FIFO buffer has overflowed, the oldest data will be lost and new
//  data will be written to the FIFO.
//
//  If the FIFO buffer is empty, reading this register will return the last byte
//  that was previously read from the FIFO until new data is available. The user
//  should check FIFO_COUNT to ensure that the FIFO buffer is not read when
//  empty.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:24:17
//
//  @return       XBYTE : Byte from FIFO buffer
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFIFOByte()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_FIFO_R_W, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetFIFOBytes
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:25:30
//
//  @param        *data :
//  @param        length :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetFIFOBytes(XBYTE* data, XBYTE length)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_FIFO_R_W, length, data);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetFIFOByte
*/
/**
//  Write byte to FIFO buffer.
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:25:37
//
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFIFOByte(XBYTE data)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_FIFO_R_W, data);
}





/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDeviceID
*/
/**
//  Get Device ID.
//  This register is used to verify the identity of the device (0b110100, 0x34)
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:25:55
//
//  @return       XBYTE : Device ID (6 bits only! should be 0x34)
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDeviceID()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_WHO_AM_I, DIOI2CMPU9150_WHO_AM_I_BIT, DIOI2CMPU9150_WHO_AM_I_LENGTH, buffer);
  return buffer[0];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetOTPBankValid
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:26:22
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetOTPBankValid()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_TC, DIOI2CMPU9150_TC_OTP_BNK_VLD_BIT, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetOTPBankValid
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:26:29
//
//  @param        enabled :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetOTPBankValid(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_TC, DIOI2CMPU9150_TC_OTP_BNK_VLD_BIT, enabled);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetXGyroOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:26:44
//
//  @return       XBYTESIG :
//
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetXGyroOffset()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetXGyroOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:29:56
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXGyroOffset(XBYTESIG offset)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, offset);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetYGyroOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:03
//
//  @return       XBYTESIG :
//
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetYGyroOffset()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetYGyroOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:09
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetYGyroOffset(XBYTESIG offset)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, offset);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZGyroOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:15
//
//  @return       XBYTESIG :
//
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetZGyroOffset()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_ZG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetZGyroOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:19
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZGyroOffset(XBYTESIG offset)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_ZG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, offset);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetXFineGain
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:24
//
//  @return       XBYTESIG :
//
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetXFineGain()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_X_FINE_GAIN, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetXFineGain
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:33
//
//  @param        gain :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXFineGain(XBYTESIG gain)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_X_FINE_GAIN, gain);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetYFineGain
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:36
//
//  @return       XBYTESIG :
//
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetYFineGain()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_Y_FINE_GAIN, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetYFineGain
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:42
//
//  @param        gain :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetYFineGain(XBYTESIG gain)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_Y_FINE_GAIN, gain);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZFineGain
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:45
//
//  @return       XBYTESIG :
//
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetZFineGain()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_Z_FINE_GAIN, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetZFineGain
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:51
//
//  @param        gain :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZFineGain(XBYTESIG gain)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_Z_FINE_GAIN, gain);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetXAccelOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:30:55
//
//  @return       XWORDSIG :
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetXAccelOffset()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_XA_OFFS_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetXAccelOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:01
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXAccelOffset(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_XA_OFFS_H, offset);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetYAccelOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:04
//
//  @return       XWORDSIG :
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetYAccelOffset()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_YA_OFFS_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetYAccelOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:09
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetYAccelOffset(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_YA_OFFS_H, offset);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZAccelOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:13
//
//  @return       XWORDSIG :
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetZAccelOffset()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ZA_OFFS_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetZAccelOffset
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:17
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZAccelOffset(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_ZA_OFFS_H, offset);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetXGyroOffsetUser
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:22
//
//  @return       XWORDSIG :
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetXGyroOffsetUser()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_USRH, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetXGyroOffsetUser
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:27
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXGyroOffsetUser(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_USRH, offset);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetYGyroOffsetUser
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:30
//
//  @return       XWORDSIG :
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetYGyroOffsetUser()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_USRH, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetYGyroOffsetUser
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:35
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetYGyroOffsetUser(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_USRH, offset);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetZGyroOffsetUser
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:39
//
//  @return       XWORDSIG :
//
*/
/*-----------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetZGyroOffsetUser()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ZG_OFFS_USRH, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetZGyroOffsetUser
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:43
//
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZGyroOffsetUser(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_ZG_OFFS_USRH, offset);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntPLLReadyEnabled
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:47
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntPLLReadyEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_PLL_RDY_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntPLLReadyEnabled
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:51
//
//  @param        enabled :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntPLLReadyEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_PLL_RDY_INT_BIT, enabled);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntDMPEnabled
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:31:55
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntDMPEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_DMP_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetIntDMPEnabled
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:00
//
//  @param        enabled :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntDMPEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_DMP_INT_BIT, enabled);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPInt5Status
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:04
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt5Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_5_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPInt4Status
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:10
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt4Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_4_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPInt3Status
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:14
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt3Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_3_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPInt2Status
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:18
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt2Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_2_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPInt1Status
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:22
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt1Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_1_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPInt0Status
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:26
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt0Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_0_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntPLLReadyStatus
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:30
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntPLLReadyStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_PLL_RDY_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetIntDMPStatus
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:34
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntDMPStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_DMP_INT_BIT, buffer);
  return buffer[0]?true:false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPEnabled
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:38
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_DMP_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetDMPEnabled
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:42
//
//  @param        enabled :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDMPEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_DMP_EN_BIT, enabled);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ResetDMP
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:46
//
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetDMP()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_DMP_RESET_BIT, true);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetMemoryBank
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:50
//
//  @param        bank :
//  @param        prefetchEnabled :
//  @param        userBank :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMemoryBank(XBYTE bank, bool prefetchEnabled, bool userBank)
{
  bank &= 0x1F;

  if(userBank)        bank |= 0x20;
  if(prefetchEnabled) bank |= 0x40;

  WriteByte(deviceaddr, DIOI2CMPU9150_RA_BANK_SEL, bank);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetMemoryStartAddress
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:54
//
//  @param        address :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMemoryStartAddress(XBYTE address)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_MEM_START_ADDR, address);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadMemoryByte
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:32:58
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::ReadMemoryByte()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_MEM_R_W, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteMemoryByte
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:33:31
//
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::WriteMemoryByte(XBYTE data)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_MEM_R_W, data);
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPConfig1
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:34:01
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDMPConfig1()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_DMP_CFG_1, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetDMPConfig1
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:34:05
//
//  @param        config :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDMPConfig1(XBYTE config)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_DMP_CFG_1, config);
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::GetDMPConfig2
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:34:10
//
//  @return       XBYTE :
//
*/
/*-----------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDMPConfig2()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_DMP_CFG_2, buffer);
  return buffer[0];
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::SetDMPConfig2
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2016 19:34:13
//
//  @param        config :
*/
/*-----------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDMPConfig2(XBYTE config)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_DMP_CFG_2, config);
}





/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadBit
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:29:42
//
//  @return       XBYTESIG :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        bitnum :
//  @param        data :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBit(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XBYTE* data, XWORD timeout)
{
  return 0;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadBitW
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:29:46
//
//  @return       XBYTESIG :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        bitnum :
//  @param        data :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBitW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XWORD* data, XWORD timeout)
{
  return 0;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadBits
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:29:50
//
//  @return       XBYTESIG :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        bitstart :
//  @param        length :
//  @param        data :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBits(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XBYTE* data, XWORD timeout)
{
  return 0;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadBitsW
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:29:56
//
//  @return       XBYTESIG :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        bitstart :
//  @param        length :
//  @param        data :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBitsW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XWORD* data, XWORD timeout)
{
  return 0;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadByte
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:03
//
//  @return       XBYTESIG :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        data :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadByte(XBYTE deviceaddr, XBYTE regaddr, XBYTE* data, XWORD timeout)
{
  return 0;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadWord
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:08
//
//  @return       XBYTESIG :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        data :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadWord(XBYTE deviceaddr, XBYTE regaddr, XWORD* data, XWORD timeout)
{
  return 0;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadBytes
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:12
//
//  @return       XBYTESIG :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        length :
//  @param        data :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBytes(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XBYTE* data, XWORD timeout)
{
  return 0;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::ReadWords
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:18
//
//  @return       XBYTESIG :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        length :
//  @param        data :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadWords(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XWORD* data, XWORD timeout)
{
  return 0;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteBit
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:29
//
//  @return       bool :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        bitnum :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBit(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XBYTE data)
{
  return false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteBitW
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:38
//
//  @return       bool :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        bitnum :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBitW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XWORD data)
{
  return false;
}



/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteBits
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:49
//
//  @return       bool :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        bitstart :
//  @param        length :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBits(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XBYTE data)
{
  return false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteBitsW
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:53
//
//  @return       bool :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        bitstart :
//  @param        length :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBitsW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XWORD data)
{
  return false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteByte
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:30:58
//
//  @return       bool :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteByte(XBYTE deviceaddr, XBYTE regaddr, XBYTE data)
{
  return false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteWord
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:31:03
//
//  @return       bool :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteWord(XBYTE deviceaddr, XBYTE regaddr, XWORD data)
{
  return false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteBytes
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:31:08
//
//  @return       bool :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        length :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBytes(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XBYTE* data)
{
  return false;
}


/*-------------------------------------------------------------------
//  DIOI2C9AXISTRACKINGMPU9150::WriteWords
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2016 17:31:13
//
//  @return       bool :
//
//  @param        deviceaddr :
//  @param        regaddr :
//  @param        length :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteWords(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XWORD* data)
{
  return false;
}
