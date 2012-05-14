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
/* Filename    : BMP085_DEBUG_DUMPS.cpp                                          */
/*                                                                               */
/* Description : This file includes a function which is used to dump debug       */
/*               information for the BMP085 sensor and sensor support            */
/*               function(s). The dumps may take place at the BMP085 sensor      */
/*               function level or the next higher level (eg. example file)      */
/*               depending.                                                      */
/*                                                                               */
/* Target      : BMP085 & BMP085 masters                                         */
/*                                                                               */
/* Dependencies: LDE_WIY_Blox.h                                                  */
/*               Arduino.h                                                       */
/*               BMP085.h                                                        */
/*                                                                               */
/*                                REVISION HISTORY                               */
/*                                                                               */
/* Revision    Date    Initials                      Comments                    */
/* --------  --------  --------  ----------------------------------------------- */
/*   1.0     05/11/12    CAB     Initial release.                                */
/*                                                                               */
/*********************************************************************************/

/************/
/* Includes */
/************/

  #include <Arduino.h>                                                    // Arduino h file will only be included
                                                                          // if not already included
                                                             
  #include <LDE_WIY_Blox.h>                                               // LDE WIY-Blox h file will only be
                                                                          // included if not already included

 #ifdef USES_BMP085
  #include "BMP085.h"                                                     // BMP085.h file will only be included
                                                                          // if not already included and the BMP085 is used

/*****************************************************************/
/* FUNCTION: BMP085_DEBUG_DUMPS(char)                            */
/*                                                               */
/* char values:                                                  */
/*    0 - BMP085 coefficients dump                               */
/*    1 - BMP085 function condition on entry dump                */
/*    2 - BMP085 function condition on exit dump                 */
/*    3 - BMP085 read raw data dump                              */
/*    4 - BMP085 compensated data and (optionally) altitude dump */
/*    5 - BMP085 timestamp of pressure reading                   */
/*    6 - BMP085 timestamp of temperature reading                */
/*                                                               */
/* RETURNS : null                                                */
/*****************************************************************/

  void BMP085_DEBUG_DUMPS(char dump_select) {
          switch (dump_select) {
             case 0  : 
                      Serial.print(F("AC1, 0x"));                         // BMP085 coefficients dump
                      Serial.println(BMP085.ac1,HEX);
                      Serial.print(F("AC2, 0x"));
                      Serial.println(BMP085.ac2,HEX);
                      Serial.print(F("AC3, 0x"));
                      Serial.println(BMP085.ac3,HEX);
                      Serial.print(F("AC4, 0x"));
                      Serial.println(BMP085.ac4,HEX);
                      Serial.print(F("AC5, 0x"));
                      Serial.println(BMP085.ac5,HEX);
                      Serial.print(F("AC6, 0x"));
                      Serial.println(BMP085.ac6,HEX);
                      Serial.print(F(" B1, 0x"));
                      Serial.println(BMP085.b1,HEX);
                      Serial.print(F(" B2, 0x"));
                      Serial.println(BMP085.b2,HEX);
                      Serial.print(F(" MB, 0x"));
                      Serial.println(BMP085.mb,HEX);
                      Serial.print(F(" MC, 0x"));
                      Serial.println(BMP085.mc,HEX);
                      Serial.print(F(" MD, 0x"));
                      Serial.println(BMP085.md,HEX);
                      Serial.println();                                                    
                      break;
             case 1  :
                      Serial.print(F("Time: "));                          // Dump of BMP085 function condition
                      Serial.print(millis(),DEC);                         // as it enters function
                      Serial.print(F(" BMP085 Function Entry  State: 0x"));
                      Serial.print(BMP085.state,HEX);
                      Serial.print(F(" Control: 0x"));
                      Serial.print(BMP085.control,HEX);
                      Serial.print(F(" Status: 0x"));
                      Serial.print(BMP085.stat,HEX);
                      Serial.print(F(" Error(s): 0x"));
                      Serial.println(BMP085.errors,HEX);
                      break;

             case 2  :
                      Serial.print(F("Time: "));                          // Dump of BMP085 function condition as it
                      Serial.print(millis(),DEC);                         // as it exits function
                      Serial.print(F(" BMP085 Function Exit   State: 0x"));
                      Serial.print(BMP085.state,HEX);
                      Serial.print(F(" Control: 0x"));
                      Serial.print(BMP085.control,HEX);
                      Serial.print(F(" Status: 0x"));
                      Serial.print(BMP085.stat,HEX);
                      Serial.print(F(" Error(s): 0x"));
                      Serial.println(BMP085.errors,HEX);
                      break;
   
             case 3  :
                      Serial.println();                                   // Dump of read BMP085 raw data
                      Serial.print(F("             Timestamp: "));
                      Serial.print(BMP085.pres_timestamp,DEC);
                      Serial.print(F("  Temperature(raw): "));
                      Serial.print(BMP085.temperature_raw,DEC);                                                 
                      Serial.print(F("  Pressure(raw): "));
                      Serial.print(BMP085.pressure_raw,DEC);
                      Serial.println();
                      break;
        
             case 4  :
                     #ifdef DO_BAROM_COMPENSATION
                      Serial.println();                                   // Dump of BMP085 compensated data
                      Serial.print(F("             Timestamp: "));        // and (optionally) altitude
                      Serial.print(BMP085.pres_timestamp,DEC);
                      Serial.print(F("  Temperature: "));
                      Serial.print(BMP085.temperature,DEC);                                                 
                      Serial.print(F("  Pressure: "));
                      Serial.print(BMP085.pressure,DEC);
                     #ifdef DO_BAROM_ALTITUDE
                      Serial.print(F("  Altitude: "));
                      Serial.println(BMP085.altitude,DEC);
                     #endif
                      Serial.println();
                     #endif
                      break;
     
             case 5  :
                      Serial.print(F("Time: "));                          // Dump of point where BMP085 takes
                      Serial.print(millis(),DEC);                         // pressure reading
                      Serial.println(F("    PRESSURE READING"));
                      break;
        
             case 6  :
                      Serial.print(F("Time: "));                          // Dump of point where BMP085 takes
                      Serial.print(millis(),DEC);                         // temperature reading
                      Serial.println(F("                      TEMPERATURE READING"));
                      break;
        
             default :                                                    // no dump 
                      break;
          }
       }
/*******************/
/* END OF FUNCTION */
/*******************/

 #endif                  
/*******************/
/* END OF CPP FILE */
/*******************/
