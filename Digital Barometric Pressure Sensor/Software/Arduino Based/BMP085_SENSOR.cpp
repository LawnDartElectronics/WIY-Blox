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
/* Filename    : BMP085_SENSOR.cpp                                               */
/*                                                                               */
/* Description : This file includes the primary function utilized to control     */
/*               the BMP085 sensor. The function is SW state machine based.      */
/*               There is provision for some parts of the function to be         */
/*               conditionally compiled and used. The conditions are located in  */
/*               the LDE_WIY_Blox.h file.                                        */
/*                                                                               */
/*               This file does not set up the master I/O pins, I2C port, UART   */
/*               port, etc, or anything related to initialization of the master  */
/*               after a HW reset.                                               */
/*                                                                               */
/*               This file is derived from BMP085 datasheet Rev 1.2              */
/*               15 October 2009.                                                */
/*                                                                               */
/* Target      : BMP085 & BMP085 masters                                         */
/*                                                                               */
/* Dependencies: LDE_WIY_Blox.h                                                  */
/*               Arduino.h                                                       */
/*               Wire.h                                                          */
/*               BMP085.h                                                        */
/*               BMP085_DEBUG_DUMPS.cpp                                          */
/*                                                                               */
/*                                REVISION HISTORY                               */
/*                                                                               */
/* Revision    Date    Initials                      Comments                    */
/* --------  --------  --------  ----------------------------------------------- */
/*   1.0     05/11/12    CAB     Initial release.                                */
/*                                                                               */
/*                               This first version relies on directly using the */
/*                               Wire library I2C functions within this SW state */
/*                               machine. So there is some performance hit.      */
/*                                                                               */
/*********************************************************************************/

/************/
/* Includes */
/************/

  #include <Arduino.h>                                                                                  // Arduino h file will only be included
                                                                                                        // if not already included
                                                             
  #include <Wire.h>                                                                                     // Wire h file will only be included 
                                                                                                        // if not already included

  #include <LDE_WIY_Blox.h>                                                                             // LDE WIY-Blox h file will only be
                                                                                                        // included if not already included

 #ifdef USES_BMP085
  #include "BMP085.h"                                                                                   // BMP085.h file will only be included
 #endif                                                                                                 // if not already included and the BMP085 is used


/*****************************/
/* FUNCTION: BMP085_SENSOR() */
/* RETURNS : null            */
/*****************************/

#ifdef USES_BMP085
  void BMP085_SENSOR() {
        //char any_error = 0;                                                                           // local variable for I2C and coefficient errors
          unsigned int BMP085_coeff = 0;                                                                // local variable for coefficient reading
          unsigned int index = 0;                                                                       // local variable for indexing
          char I2C_reg = 0;                                                                             // local variable for I2C register selection
          long interval_time = 0;                                                                       // local variable for interval time selection
         #ifdef DO_BAROM_COMPENSATION
          long data_raw = 0;                                                                            // local variable for raw temperature/pressure
          long BMP085_x1 = 0;                                                                           // local variable for temp/press compensation
          long BMP085_x2 = 0;                                                                           // local variable for temp/press compensation
          long BMP085_x3 = 0;                                                                           // local variable for temp/press compensation          
          long BMP085_b3 = 0;                                                                           // local variable for temp/press compensation
          unsigned long BMP085_b4 = 0;                                                                  // local variable for temp/press compensation
          long BMP085_b5 = 0;                                                                           // local variable for temp/press compensation
          long BMP085_b6 = 0;                                                                           // local variable for temp/press compensation
          unsigned long BMP085_b7 = 0;                                                                  // local variable for temp/press compensation
         #endif
         
         #ifdef BMP085_DEBUG_MSGS
          BMP085_DEBUG_DUMPS(1);                                                                        // Print out BMP085 function condition as it
         #endif                                                                                         // enters function
          
          if (BMP085.control & BMP085_SW_reset) {                                                       // Detect master BMP085 SW reset
                                                                                                        // SW reset has priority over all other controls
             BMP085.wait_next_time = millis();                                                          // Time in mS for next wait action
             BMP085.temp_next_time = millis() + BMP085_temp_per_default;                                // Time in mS for next temperature read action
             BMP085.pres_next_time = millis() + BMP085_press_per_default;                               // Time in mS for next pressure read action
             BMP085.temp_interval = BMP085_temp_per_default;                                            // Set temperature measurement interval to 0
             BMP085.press_interval = BMP085_press_per_default;                                          // Set pressure measurement interval to 0
             BMP085.errors = BMP085_clear_all_errors;                                                   // Clear all errors
             BMP085.config = BMP085_1x_OS;                                                              // Set pressure measurement to 1x oversampling
             BMP085.control = BMP085_SW_reset;                                                          // maintain master SW reset until master clears
             BMP085.stat = BMP085_clear_all_status;                                                     // Clear all statuses
             BMP085.state = BMP085_reset_state;                                                         // Goto SW reset state
             BMP085.pres_timestamp = 0;                                                                 // Set pressure measurement taken Timestamp to 0
            #ifdef DO_BAROM_COMPENSATION
             BMP085.pressure = BMP085_press_default;                                                    // Set pressure to default value
             BMP085.temperature = BMP085_temp_default;                                                  // Set temperature to default value
            #endif
             BMP085.pressure_raw = BMP085_press_default;                                                // Set uncompensated pressure to default value
             BMP085.temperature_raw = BMP085_temp_default;                                              // Set uncompensated temperature to default value
            #ifdef USES_BMP085
             BMP085.ac1 = BMP085_AC1_coeff_default;                                                     // Set AC1 coefficient to default value
             BMP085.ac2 = BMP085_AC2_coeff_default;                                                     // Set AC2 coefficient to default value
             BMP085.ac3 = BMP085_AC3_coeff_default;                                                     // Set AC3 coefficient to default value
             BMP085.ac4 = BMP085_AC4_coeff_default;                                                     // Set AC4 coefficient to default value
             BMP085.ac5 = BMP085_AC5_coeff_default;                                                     // Set AC5 coefficient to default value
             BMP085.ac6 = BMP085_AC6_coeff_default;                                                     // Set AC6 coefficient to default value
             BMP085.b1 = BMP085_B1_coeff_default;                                                       // Set B1 coefficient to default value
             BMP085.b2 = BMP085_B2_coeff_default;                                                       // Set B2 coefficient to default value
             BMP085.mb = BMP085_MB_coeff_default;                                                       // Set MB coefficient to default value
             BMP085.mc = BMP085_MC_coeff_default;                                                       // Set MC coefficient to default value
             BMP085.md = BMP085_MD_coeff_default;                                                       // Set MD coefficient to default value                                                 
            #endif
            #ifdef DO_BAROM_ALTITUDE 
             BMP085.p0 = BMP085_P0_default;                                                             // Set sea-level pressure to default value
             BMP085.altitude = BMP085_alti_default;                                                     // Set altitude to default value
            #endif                                                
          }
          else {
             if (BMP085.control & BMP085_data_ready_ack) {                                              // Detect data ready acknowledge
                BMP085.stat &= ~BMP085_data_ready;                                                      // Clear the data ready
                BMP085.control &= ~BMP085_data_ready_ack;                                               // Clear the data ready acknowledge
             }
             switch (BMP085.state) {                                                                    // SW state machine
                case BMP085_reset_state         :/*********************************/
                                                 /* Only goes to the detect state */
                                                 /*********************************/
                                                 BMP085.state = BMP085_detect_state;                    // Goto detect state
                                                 break;

                case BMP085_detect_state        :/***********************************************/
                                                 /* Can only be entered from the SW reset state */
                                                 /***********************************************/
                                                 Wire.beginTransmission(BMP085_device_select);          // Call BMP085
                                                 Wire.write(BMP085_AC1_coeff_UB_reg);                   // Set BMP085 register offset to first coefficient
                                                 if (Wire.endTransmission()) {                          // Detect no acknowledge
                                                    BMP085.errors |= BMP085_not_present;                // Set BMP085 is not present error
                                                    BMP085.stat |= BMP085_errors;                       // Set BMP085 error status
                                                    BMP085.state = BMP085_error_state;                  // Go to error state
                                                 }
                                                 else {                                                 // Detect acknowledge to BMP085 call
                                                    BMP085.state = BMP085_coeffs_state;                 // Go to download coefficients state
                                                 }
                                                 break;

                case BMP085_coeffs_state        :/*********************************************/
                                                 /* Can only be entered from the detect state */
                                                 /*********************************************/
                                                 Wire.beginTransmission(BMP085_device_select);          // Call BMP085
                                                 Wire.write(BMP085_AC1_coeff_UB_reg);                   // Set BMP085 register offset to first coefficient
                                                 if (Wire.endTransmission()) {                          // Detect I2C error
                                                    BMP085.errors |= BMP085_I2C_error;                  // Set BMP085 I2C error
                                                    BMP085.stat |= BMP085_errors;                       // Set BMP085 error status
                                                 }
                                                 else {
                                                    Wire.requestFrom(BMP085_device_select,22);          // Get all 11 coefficients (2-bytes per coefficient)
                                                    while (Wire.available() < 22) {                     // Wait for data
                                                    }
                                                    while (index < 11) {                                // Detect coefficient read count
                                                       BMP085_coeff = (Wire.read() << 8) + Wire.read(); // Read coefficient
                                                       if ((BMP085_coeff == BMP085_coeff_all_zeroes_error) || // Check for all zeroes or
                                                           (BMP085_coeff == BMP085_coeff_all_ones_error)) {   // all ones error
                                                          BMP085.errors |= BMP085_bad_coeffs;           // Set BMP085 bad coefficients error
                                                          BMP085.stat |= BMP085_errors;                 // Set BMP085 error status
                                                       }
                                                       switch(index) {                                  // Store coefficients
                                                          case 0  : BMP085.ac1 = BMP085_coeff;
                                                                    break;
                                                          case 1  : BMP085.ac2 = BMP085_coeff;
                                                                    break;
                                                          case 2  : BMP085.ac3 = BMP085_coeff;
                                                                    break;
                                                          case 3  : BMP085.ac4 = BMP085_coeff;
                                                                    break;
                                                          case 4  : BMP085.ac5 = BMP085_coeff;
                                                                    break;
                                                          case 5  : BMP085.ac6 = BMP085_coeff;
                                                                    break;
                                                          case 6  : BMP085.b1 = BMP085_coeff;
                                                                    break;
                                                          case 7  : BMP085.b2 = BMP085_coeff;
                                                                    break;
                                                          case 8  : BMP085.mb = BMP085_coeff;
                                                                    break;
                                                          case 9  : BMP085.mc = BMP085_coeff;
                                                                    break;
                                                          case 10 : BMP085.md = BMP085_coeff;
                                                                    break;
                                                       }
                                                       index ++;
                                                    }
                                                   #ifdef BMP085_DEBUG_MSGS
                                                    BMP085_DEBUG_DUMPS(0);                              // Print out all coefficients
                                                   #endif
                                                 }
                                                 if (BMP085.stat & BMP085_errors) {                     // Detect errors
                                                    BMP085.state = BMP085_error_state;                  // Go to error state
                                                 }
                                                 else {
                                                    BMP085.state = BMP085_take_meas_state;              // Go to take measurement state
                                                    BMP085.stat |= BMP085_temp_in_prog;                 // Initiate first temperature measurement
                                                 }
                                                 break;

                case BMP085_idle_state          :
                                                 switch(BMP085.control & (BMP085_take_mult | BMP085_take_single)) {
                                                   case BMP085_take_mult   : 
                                                                            if (((long)millis() - BMP085.temp_next_time) >= 0) {
                                                                               BMP085.stat |= BMP085_temp_in_prog;
                                                                               BMP085.temp_next_time = (long)millis() + BMP085.temp_interval;
                                                                               BMP085.state = BMP085_take_meas_state; // Go to take temperature state
                                                                            }
                                                                            else if (((long)millis() - BMP085.pres_next_time) >=0) {
                                                                               BMP085.stat |= BMP085_pres_in_prog;
                                                                               BMP085.pres_next_time = (long)millis() + BMP085.press_interval;
                                                                               BMP085.state = BMP085_take_meas_state; // Go to take pressure state
                                                                            }
                                                                            else {
                                                                               BMP085.state = BMP085_idle_state;  // Remain in idle state
                                                                            }
                                                                            BMP085.stat &= ~BMP085_ready;           // Indicate BMP085 is no longer ready
                                                                            break;
                                                   case BMP085_take_single :
                                                                            if (!(BMP085.stat & (BMP085_pres_in_prog | BMP085_temp_in_prog))) {
                                                                               BMP085.stat |= BMP085_temp_in_prog;  // Select temperature as first measurement
                                                                            }
                                                                            BMP085.state = BMP085_take_meas_state; // Take single measurement
                                                                            BMP085.stat &= ~BMP085_ready;           // Indicate BMP085 is no longer ready
                                                                            break;
                                                   default                 :
                                                                            BMP085.state = BMP085_idle_state;      // Remain in idle state
                                                                            BMP085.stat |= BMP085_ready;           // Indicate BMP085 is ready
                                                                            break;
                                                 }
                                                 break;

                case BMP085_take_meas_state     :/*****************************************************************/
                                                 /* Can only be entered from the idle state or coefficients state */
                                                 /*****************************************************************/
                                                 if (BMP085.stat & BMP085_temp_in_prog) {
                                                    I2C_reg = BMP085_get_temp_mode;                     // Set register and interval
                                                    interval_time = BMP085_temp_conv_time;              // for temperature measurement
                                                 }
                                                 else {                                                 // Set register and interval
                                                    I2C_reg = (BMP085_get_press_mode + (BMP085.config << 6));
                                                    interval_time = BMP085_press_conv_time + (BMP085.config * 3) + (12 >> ((3-BMP085.config)*2));
                                                 }                                                      // If the above calculation is correct it
                                                                                                        // should produce the following:
                                                                                                        // 1x oversampling = 5 mS conversion time
                                                                                                        // 2x oversampling = 8 mS conversion time
                                                                                                        // 4x oversampling = 14 mS conversion time
                                                                                                        // 8x oversampling = 26 mS conversion time
                                                 Wire.beginTransmission(BMP085_device_select);          // Call BMP085
                                                 Wire.write(BMP085_control_reg);                        // Set BMP085 register offset to control register
                                                 Wire.write(I2C_reg);
                                                 switch (Wire.endTransmission()) {
                                                    case 0  :
                                                             if(BMP085.stat & BMP085_pres_in_prog) {
                                                               BMP085.pres_timestamp = millis();        // Set pressure measurement taken timestamp
                                                             }
                                                             BMP085.state = BMP085_meas_wait_state;     // Go to wait for temperature measurement completion
                                                             BMP085.wait_next_time = (long)millis() + interval_time; // Load time when temperature measurement will be ready
                                                            #ifdef BMP085_DEBUG_MSGS
                                                             if (BMP085.stat & BMP085_pres_in_prog) {
                                                                BMP085_DEBUG_DUMPS(5);                  // Print out time stamp for pressure measurement taken
                                                             }
                                                             else {
                                                                BMP085_DEBUG_DUMPS(6);                  // Print out time stamp for temperature measurement taken                                                               
                                                             }
                                                            #endif          
                                                             break;
                                                    default :
                                                             BMP085.control &= ~(BMP085_take_single | BMP085_take_mult);
                                                             BMP085.errors |= BMP085_I2C_error;         // Clear take single/multi measurements
                                                             BMP085.stat |= BMP085_errors;              // Set BMP085 error status
                                                             BMP085.state = BMP085_error_state;         // Set BMP085 I2C error status
                                                             break;                                     // Go to error state
                                                 }
                                                 break;

                case BMP085_meas_wait_state     :/*******************************************************/
                                                 /* Can only be entered from the take measurement state */
                                                 /*******************************************************/
                                                #ifdef USE_BMP085_EOC
                                                 if (digitalRead(BAROMETER_INT_pin)) {                  // Detect End Of Conversion
                                                    BMP085.state = BMP085_read_meas_state;              // Go to read measurement state
                                                 }
                                                 else if ((long)(millis() - BMP085.wait_next_time) >=0) {
                                                    BMP085.errors |= BMP085_EOC_timeout;                // Detect timeout and set timeout error
                                                    BMP085.stat |= BMP085_errors;                       // Set BMP085 error status
                                                    BMP085.state = BMP085_error_state;                  // Go to error state
                                                 }
                                                 else {
                                                    BMP085.state = BMP085_meas_wait_state;              // Remain in wait for temperature measurement state
                                                 }
                                                #endif
                                                #ifndef USE_BMP085_EOC
                                                 if ((long)(millis() - BMP085.wait_next_time) >=0) {
                                                    BMP085.state = BMP085_read_meas_state;              // Go to read measurement state
                                                 }
                                                 else {
                                                    BMP085.state = BMP085_meas_wait_state;              // Remain in wait for temperature measurement state
                                                 }
                                                #endif
                                                 break;

                case BMP085_read_meas_state     :/*******************************************************/
                                                 /* Can only be entered from the measurement wait state */
                                                 /*******************************************************/
                                                 if (BMP085.stat & BMP085_temp_in_prog) {
                                                   I2C_reg = BMP085_temp_data_UB_reg;                   // Set register offset to temperature data UB
                                                   index = 2;                                           // Set index to 2 for 2 byte read
                                                 }
                                                 else {
                                                    I2C_reg = BMP085_press_data_UB_reg;                 // Set register offset to pressure data UB
                                                    index = 3;                                          // Set index to 3 for 3 byte read
                                                 }
                                                 Wire.beginTransmission(BMP085_device_select);          // Call BMP085
                                                 Wire.write(I2C_reg);                                   // Set register offset per above selection
                                                 if (Wire.endTransmission()) {                          // Detect I2C error
                                                    BMP085.control &= ~(BMP085_take_single | BMP085_take_mult); 
                                                    BMP085.errors |= BMP085_I2C_error;                  // Clear take single/multi measurements
                                                    BMP085.stat |= BMP085_errors;                       // Set BMP085 error status
                                                    BMP085.state = BMP085_error_state;                  // Set BMP085 I2C error
                                                 }                                                      // Go to error state
                                                 else {                                                    
                                                    Wire.requestFrom(BMP085_device_select,index);       // Get the temperature
                                                    while (Wire.available() < (int)index){              // Wait for the bytes                                                       
                                                    }
                                                    if (!(BMP085.control & BMP085_data_overwrite) &&    // Detect overwrite condition
                                                        (BMP085.stat & BMP085_data_ready)) {
                                                       Wire.read();                                     // Flush I2C buffer
                                                       Wire.read();
                                                       if (index == 3) {
                                                          Wire.read();
                                                       }
                                                       BMP085.control &= ~(BMP085_take_single | BMP085_take_mult);
                                                       BMP085.errors |= BMP085_data_overrun;            // Clear take single/multi measurements
                                                       BMP085.stat |= BMP085_errors;                    // Set BMP085 error status
                                                       BMP085.state = BMP085_error_state;               // Set BMP085 data overrun error
                                                    }                                                   // Go to error state
                                                    else {
                                                          if (BMP085.stat & BMP085_temp_in_prog) {
                                                             data_raw = Wire.read();
                                                             data_raw <<= 8;
                                                             data_raw += Wire.read();
                                                             BMP085.temperature_raw = data_raw;
                                                             BMP085.stat &= ~BMP085_temp_in_prog;
                                                             BMP085.stat |= BMP085_pres_in_prog;
                                                             BMP085.state = BMP085_idle_state;          // Go to idle state
                                                          }
                                                          else {
                                                             data_raw = Wire.read();
                                                             data_raw <<= 8;
                                                             data_raw += Wire.read();
                                                             data_raw <<=  8;
                                                             data_raw += Wire.read();
                                                             data_raw >>= (8-BMP085.config);
                                                             BMP085.pressure_raw = data_raw;
                                                             BMP085.stat &= ~BMP085_pres_in_prog;
                                                            #ifdef DO_BAROM_COMPENSATION
                                                             BMP085.state = BMP085_calc_state;          // Go to calc state
                                                            #endif
                                                            #ifndef DO_BAROM_COMPENSATION
                                                             BMP085.control &= ~BMP085_take_single;     // Clear take single measurement
                                                             BMP085.stat |= BMP085_data_ready;          // Set data ready status
                                                             BMP085.state = BMP085_idle_state;          // Go to idle state      
                                                            #ifdef BMP085_DEBUG_MSGS
                                                             BMP085_BEBUG_DUMPS(3);                     // Print out uncompensated raw measurements
                                                            #endif  
                                                            #endif
                                                          }
                                                    }
                                                 }
                                                 break;
               #ifdef DO_BAROM_COMPENSATION                                 
                case BMP085_calc_state          :/*******************************************************/
                                                 /* Can only be entered from the measurement read state */
                                                 /* and the measurement being read is pressure          */
                                                 /*******************************************************/
                                               
                                                 data_raw = BMP085.temperature_raw;
                                                 BMP085_x1 = ((data_raw - (long)BMP085.ac6) * (long)BMP085.ac5) >> 15;
                                                 BMP085_x2 =  ((long)BMP085.mc << 11) / (BMP085_x1 + (long)BMP085.md);
                                                 BMP085_b5 = BMP085_x1 + BMP085_x2;
                                                 BMP085.temperature = (BMP085_b5 + 8) >> 4;
                                            
                                                 data_raw = BMP085.pressure_raw;
                                                 BMP085_b6 = BMP085_b5 - 4000;
                                                 BMP085_x1 = ((long)BMP085.b2 * ((BMP085_b6 * BMP085_b6) >> 12)) >> 11;
                                                 BMP085_x2 = ((long)BMP085.ac2 * BMP085_b6) >> 11;
                                                 BMP085_x3 = BMP085_x1 + BMP085_x2;
                                                 BMP085_b3 = (((((long)BMP085.ac1 << 2) + BMP085_x3) << BMP085.config) + 2) >> 2;
                                                 BMP085_x1 = ((long)BMP085.ac3 * BMP085_b6) >> 13;
                                                 BMP085_x2 =((long)BMP085.b1 * ((BMP085_b6 * BMP085_b6) >> 12)) >> 16;
                                                 BMP085_x3 = ((BMP085_x1 + BMP085_x2) + 2) >> 2;
                                                 BMP085_b4 = ((long)BMP085.ac4 * (unsigned long)(BMP085_x3 + 32768)) >> 15;
                                                 BMP085_b7 = ((unsigned long)data_raw - BMP085_b3) * (50000 >> BMP085.config);
                                                 if (BMP085_b7 < 0x80000000) {
                                                    data_raw = (BMP085_b7 << 1) / BMP085_b4;
                                                 }
                                                 else {
                                                    data_raw = (BMP085_b7 / BMP085_b4) << 1;
                                                 }
                                                 BMP085_x1 = data_raw >> 8;
                                                 BMP085_x1 *= BMP085_x1;
                                                 BMP085_x1 = (BMP085_x1 * 3038) >> 16;
                                                 BMP085_x2 = (-7357 * data_raw) >> 16;
                                                 BMP085.pressure = data_raw + ((BMP085_x1 + BMP085_x2 + 3791) >> 4);
                                                #ifdef DO_BAROM_ALTITUDE
                                                 BMP085.altitude = (float)BMP085.pressure / BMP085.p0;
                                                 BMP085.altitude = 1 - pow(BMP085.altitude,1/5.255);
                                                 BMP085.altitude *= 44330;
                                                #ifdef BMP085_DEBUG_MSGS
                                                 BMP085_DEBUG_DUMPS(4);                                 // Print out compensated measurements and altitude
                                                #endif
                                                #endif
                                                 BMP085.control &= ~BMP085_take_single;                 // Clear take single measurement
                                                 BMP085.stat |= BMP085_data_ready;                      // Set data ready status
                                                 BMP085.state = BMP085_idle_state;                      // Go to idle state
                                                 break;
               #endif
                case BMP085_error_state         :/*********************************************************/
                                                 /* Remain in this state until master performs a SW reset */
                                                 /*********************************************************/
                                                 
                                                 BMP085.state = BMP085_error_state;                     // Remain in error state until
                                                 break;                                                 // master initiates SW reset

                default                         :
                                                 BMP085.stat = BMP085_clear_all_status;                 // Clear all statuses
                                                 BMP085.control = BMP085_SW_reset;                      // Clear all controls but reset state
                                                 BMP085.state= BMP085_reset_state;                      // Return to SW reset state
                                                 break;
             }
          }
         #ifdef BMP085_DEBUG_MSGS
          BMP085_DEBUG_DUMPS(1);                                                                        // Print out BMP085 function condition as it
         #endif                                                                                         // exits function
       }
/*******************/
/* END OF FUNCTION */
/*******************/

#endif                  
/*******************/
/* END OF CPP FILE */
/*******************/
