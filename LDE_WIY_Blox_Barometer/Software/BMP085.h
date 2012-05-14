/*********************************************************************************/
/*                                                                               */
/*                             LAWN DART ELECTRONICS                             */
/*                                                                               */
/*                          WWW.LAWNDARTELECTRONICS.COM                          */
/*                                                                               */
/* Notices     : Copyright (C) 2012 Lawn Dart Electronics. This file, as well as */
/*               the software and information within are free. It may be         */
/*               redistributed and/or modified, so long as the information       */
/*               within this title block remains.                                */
/*                                                                               */
/*               For purposes of this notice, the term "We" shall refer to and   */
/*               include Lawn Dart Electronics and its: parent company,          */
/*               employees, managers, partners, business affiliates, and         */
/*               subsidiaires. This file, as well as the software and information*/
/*               contained within it are entirely provided "AS-IS", ans We make  */
/*               no express or implied warranties whatsoever with respect to its */
/*               funtionality, operability, or use, including without limitation,*/
/*               any implied warranties of merchantability, fitness for a        */
/*               particular purpose, or infringement. We expressly disclaim any  */
/*               liability whatsoever for any direct, indirect, consequential,   */
/*               incidental or special damages, including without limitation,    */
/*               lost revenues, lost profits, losses resulting from business     */
/*               interruption or loss of data, regardless of the form of action  */
/*               or legal theory under which the liability may be asserted, even */
/*               if advised of the possibility or likelihood of such damages.    */
/*                                                                               */
/*               This file, the software, and the information contained in it    */
/*               may change at any time and without notice.                      */
/*                                                                               */
/*               Support for this file and the software is provided by the user  */
/*               community.                                                      */
/*                                                                               */
/* Filename    : BMP085.h                                                        */
/*                                                                               */
/* Description : H include file for use with Lawn Dart Electronics software      */
/*               programs which utilize the Bosch BMP085.                        */
/*                                                                               */
/*               The H file is derived from BMP085 datasheet Rev 1.2             */
/*               15 October 2009.                                                */
/*                                                                               */
/* Target      : BMP085 & BMP085 masters                                         */
/*                                                                               */
/* Dependencies: LDE_WIY_Blox.h                                                  */
/*                                                                               */
/*                                REVISION HISTORY                               */
/*                                                                               */
/* Revision    Date    Initials                      Comments                    */
/* --------  --------  --------  ----------------------------------------------- */
/*   1.0     05/11/12    CAB     Initial release.                                */
/*                                                                               */
/*********************************************************************************/


/*********************************************************************************/
/*                                                                               */
/* I2C clock cycles for various transactions. Presumes a start, stop, re-start,  */
/* and slave ack are each  1 clock cycle.                                        */
/*                                                                               */
/* Take measurement: (S+DAw+A)+(RO+A)+(DAT+A+P) = 29 clocks                      */
/*                                                                               */
/* Read temperature: (S+DAw+A)+(RO+A)+(R+DAr+A)+(DAT+A)+(DAT+A+P) = 48 clocks    */
/*                                                                               */
/* Read pressure (16-bit): (same as read temperature) = 48 clocks                */
/*                                                                               */
/* Read pressure (19-bit): (same as read temperature)+(DAT+A) = 57 clocks        */
/*                                                                               */
/*********************************************************************************/

/************/
/* INCLUDES */
/************/

  #include "LDE_WIY_Blox.h"                         // LDE WIY-Blox H file will only be
                                                    // included if not already included

/*******************/
/* START OF H FILE */
/*******************/

#ifdef USES_BMP085                                  // Only use H file if BMP085 is being used

#ifndef BMP085_h
  #define BMP085_h                                  // Prevent multiple copies of H file
 
/*************************/
/* DEVICE SELECT ADDRESS */
/*************************/

  #define BMP085_device_select               0x77   // Single possible device select


/*******************************************/
/* EEPROM COEFFICIENT REGISTER DEFINITIONS */
/*               (Read-Only)               */
/*******************************************/

  #define BMP085_AC1_coeff_UB_reg            0xAA   // EEPROM AC1 calibration coefficient upper byte register offset
  #define BMP085_AC1_coeff_LB_reg            0xAB   // EEPROM AC1 calibration coefficient lower byte register offset

  #define BMP085_AC2_coeff_UB_reg            0xAC   // EEPROM AC2 calibration coefficient upper byte register offset
  #define BMP085_AC2_coeff_LB_reg            0xAD   // EEPROM AC2 calibration coefficient lower byte register offset

  #define BMP085_AC3_coeff_UB_reg            0xAE   // EEPROM AC3 calibration coefficient upper byte register offset
  #define BMP085_AC3_coeff_LB_reg            0xAF   // EEPROM AC3 calibration coefficient lower byte register offset

  #define BMP085_AC4_coeff_UB_reg            0xB0   // EEPROM AC4 calibration coefficient upper byte register offset
  #define BMP085_AC4_coeff_LB_reg            0xB1   // EEPROM AC4 calibration coefficient lower byte register offset

  #define BMP085_AC5_coeff_UB_reg            0xB2   // EEPROM AC5 calibration coefficient upper byte register offset
  #define BMP085_AC5_coeff_LB_reg            0xB3   // EEPROM AC5 calibration coefficient lower byte register offset

  #define BMP085_AC6_coeff_UB_reg            0xB4   // EEPROM AC6 calibration coefficient upper byte register offset
  #define BMP085_AC6_coeff_LB_reg            0xB5   // EEPROM AC6 calibration coefficient lower byte register offset

  #define BMP085_B1_coeff_UB_reg             0xB6   // EEPROM B1 calibration coefficient upper byte register offset
  #define BMP085_B1_coeff_LB_reg             0xB7   // EEPROM B1 calibration coefficient lower byte register offset

  #define BMP085_B2_coeff_UB_reg             0xB8   // EEPROM B2 calibration coefficient upper byte register offset
  #define BMP085_B2_coeff_LB_reg             0xB9   // EEPROM B2 calibration coefficient lower byte register offset

  #define BMP085_MB_coeff_UB_reg             0xBA   // EEPROM MB calibration coefficient upper byte register offset
  #define BMP085_MB_coeff_LB_reg             0xBB   // EEPROM MB calibration coefficient lower byte register offset

  #define BMP085_MC_coeff_UB_reg             0xBC   // EEPROM MC calibration coefficient upper byte register offset
  #define BMP085_MC_coeff_LB_reg             0xBD   // EEPROM MC calibration coefficient lower byte register offset

  #define BMP085_MD_coeff_UB_reg             0xBE   // EEPROM MD calibration coefficient upper byte register offset
  #define BMP085_MD_coeff_LB_reg             0xBF   // EEPROM MD calibration coefficient lower byte register offset

  #define BMP085_coeff_all_zeroes_error      0x0000 // Calibration coefficient all zeroes error
  #define BMP085_coeff_all_ones_error        0xFFFF // Calibration coefficient all ones error

  #define BMP085_AC1_coeff_default           0x0000 // Default A1 coefficient value
  #define BMP085_AC2_coeff_default           0x0000 // Default A2 coefficient value
  #define BMP085_AC3_coeff_default           0x0000 // Default A3 coefficient value
  #define BMP085_AC4_coeff_default           0x0000 // Default A4 coefficient value
  #define BMP085_AC5_coeff_default           0x0000 // Default A5 coefficient value
  #define BMP085_AC6_coeff_default           0x0000 // Default A6 coefficient value
  #define BMP085_B1_coeff_default            0x0000 // Default B1 coefficient value
  #define BMP085_B2_coeff_default            0x0000 // Default B2 coefficient value
  #define BMP085_MB_coeff_default            0x0000 // Default MB coefficient value
  #define BMP085_MC_coeff_default            0x0000 // Default MC coefficient value
  #define BMP085_MD_coeff_default            0x0000 // Default MD coefficient value
  
  
/********************************/
/* CONTROL REGISTER DEFINITIONS */
/*        (Write-Only)          */
/********************************/

  #define BMP085_control_reg                 0xF4   // Control register offset

  #define BMP085_get_temp_mode               0x2E   // Initiate temperature measurement
  
  #define BMP085_get_press_mode              0x34   // Initiate pressure measurement (base value)
     
/*****************************/
/* DATA REGISTER DEFINITIONS */
/*        (Read-Only)        */
/*****************************/

  #define BMP085_temp_data_UB_reg            0xF6   // Updated temperature data upper byte register offset
  #define BMP085_temp_data_LB_reg            0xF7   // Updated temperature data lower byte register offset

  #define BMP085_press_data_UB_reg           0xF6   // Updated pressure data upper byte register offset
  #define BMP085_press_data_LB_reg           0xF7   // Updated pressure data lower byte register offset
  #define BMP085_press_data_XB_reg           0xF8   // Updated pressure data extra lower byte register offset


/***************************************************/
/* ALTITUDE, PRESSURE, AND TEMPERATURE DEFINITIONS */
/***************************************************/

  #define BMP085_temp_default                0x0000 // Default temperature measurement value

  #define BMP085_press_default               0x0000 // Default pressure measurement value

 #ifdef DO_BAROM_ALTITUDE
  #define BMP085_alti_default                0x0000 // Default altitude measurement value
  #define BMP085_P0_default                  101100 // Default sea-level pressure value
 #endif


/***********************************************************/
/* MEASUREMENT CONVERSION TIME AND PERIODICITY DEFINITIONS */
/***********************************************************/

  #define BMP085_temp_per_default            1000   // (mS). Recommended temperature read frequency is 1000mS (or 1S)

  #define BMP085_press_per_default           250    // (mS). Recommended pressure read frequency is 1000mS (or 1S)

  #define BMP085_temp_conv_time              5      // (mS). Conversion time for temperature is > 4.5mS
//#define BMP085_temp_conv_time              4.5    // (sub-mS). Conversion time for temperature is > 4.5mS

  #define BMP085_press_conv_time             5      // (mS). Conversion time for pressure in standard mode is > 4.5mS
//#define BMP085_press_conv_time             4.5    // (sub-mS). Conversion time for pressure in standard mode is 4.5mS


/*****************************/
/* CONFIGURATION DEFINITIONS */
/*****************************/

 #define BMP085_1x_OS                        0x00   // BMP085 1x oversampling
 #define BMP085_2x_OS                        0x01   // BMP085 2x oversampling
 #define BMP085_4x_OS                        0x02   // BMP085 4x oversampling
 #define BMP085_8x_OS                        0x03   // BMP085 8x oversampling

/***********************/
/* CONTROL DEFINITIONS */
/***********************/

  #define BMP085_clear_all_controls          0x00   // Simple clear of all controls mask

  #define BMP085_SW_reset                    0x01   // Bit position for BMP085 reset state machine
  #define BMP085_data_overwrite              0x02   // Bit position for BMP085 allow (1) / don't allow (0) data overwrite
  #define BMP085_take_single                 0x04   // Bit position for BMP085 take single measurement
  #define BMP085_take_mult                   0x08   // Bit position for BMP085 take multiple measurements
//#define                                    0x10   // Reserved bit position
//#define                                    0x20   // Reserved bit position
//#define                                    0x40   // Reserved bit position
  #define BMP085_data_ready_ack              0x80   // Bit position for data ready acknowledge


/**********************/
/* STATUS DEFINITIONS */
/**********************/

  #define BMP085_clear_all_status            0x00   // Simple clear all statuses mask

  #define BMP085_errors                      0x01   // Bit position for BMP085 error(s)
  #define BMP085_ready                       0x02   // Bit position for BMP085 ready
//#define                                    0x04   // Reserved bit position
  #define BMP085_data_ready                  0x08   // Bit position for BMP085 data ready status
  #define BMP085_temp_in_prog                0x10   // Bit position for BMP085 temperature measurement in progress
  #define BMP085_pres_in_prog                0x20   // Bit position for BMP085 pressure measurement in progress
//#define                                    0x40   // Reserved bit position
//#define                                    0x80   // Reserved bit position

/*********************/
/* ERROR DEFINITIONS */
/*********************/

  #define BMP085_clear_all_errors            0x00   // Simple clear all errors mask

  #define BMP085_not_present                 0x01   // Bit position for BMP085 not present error
  #define BMP085_bad_coeffs                  0x02   // Bit position for BMP085 bad coefficients error
  #define BMP085_data_overrun                0x04   // Bit position for BMP085 data overrun error
  #define BMP085_I2C_error                   0x08   // Bit position for BMP085 I2C error
  #define BMP085_EOC_timeout                 0x10   // Bit position for BMP085 EOC timeout error

/*********************/
/* STATE DEFINITIONS */
/*********************/

  #define BMP085_reset_state                 0x00   // SW reset state
  #define BMP085_detect_state                0x01   // Detecting BMP085 state
  #define BMP085_coeffs_state                0x02   // Downloading compensation coefficients state
  #define BMP085_idle_state                  0x03   // Idle state
  #define BMP085_take_meas_state             0x04   // Initiate taking temperature or pressure measurement state
  #define BMP085_meas_wait_state             0x05   // Wait for temperature or pressure measurement completion state
  #define BMP085_read_meas_state             0x06   // Read temperature or pressure measurement state
 #ifdef DO_BAROM_COMPENSATION
  #define BMP085_calc_state                  0x07   // Calcuate temperature, pressure, altitude
 #endif
  #define BMP085_error_state                 0x08   // BMP085 error state
//#define                             0x09 - 0xFF   // Reserved states


/*********************/
/* GLOBAL STRUCTURES */
/*********************/

  extern barom_struct BMP085;                       // Global structure for the BMP085 sensor.
                                                    // Consult LDE_WIY_Blox.h for structure makeup.

/********************/
/* GLOBAL VARIABLES */
/********************/

// None.


/*************************/
/* FUNCTION DECLARATIONS */
/*************************/

  void BMP085_SENSOR();                             // BMP085 main function
  void BMP085_DEBUG_DUMPS(char);                    // Debug dumps function for BMP085

#endif
#endif
/*****************/
/* END OF H FILE */
/*****************/
