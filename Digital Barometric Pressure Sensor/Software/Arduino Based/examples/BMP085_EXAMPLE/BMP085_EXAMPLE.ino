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
/* Filename    : BMP085_EXAMPLE.ino                                              */
/*                                                                               */
/* Description : This file provides an Arduino compatible example for using the  */
/*               BMP085 function: BMP085_SENSOR().                               */
/*                                                                               */
/* Target      : BMP085 & BMP085 masters                                         */
/*                                                                               */
/* Dependencies: LDE_WIY_Blox.h                                                  */
/*               Arduino.h                                                       */
/*               Wire.h                                                          */
/*               BMP085.h                                                        */
/*               BMP085_SENSOR.cpp                                               */
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
/* INCLUDES */
/************/

  #include <Arduino.h>                                      // Arduino h file will only be included
                                                            // if not already included
                                                            
  #include <Wire.h>                                         // Wire h file will only be included 
                                                            // if not already included

  #include <LDE_WIY_Blox.h>                                 // LDE WIY-Blox H file will only be
                                                            // included if not already included

                                                    
#ifdef USES_BMP085
  #include <BMP085.h>                                       // BMP085.h file will only be included
#endif                                                      // if not already included and BMP085 is used

/***************************/
/* TESTER UART DEFINITIONS */
/***************************/

  #define UART_baud             9600                        // UART baud rate


/***********************/
/* EXAMPLE DEFINITIONS */
/***********************/

  #define RESET_INTERVAL        30000
  #define TEMP_INTERVAL         4000
  #define PRESS_INTERVAL        1000

  #define LOOP_REPEAT_COUNT     1000                        // Count whereby loop repeats

  #define SEALEVEL_P0           101670                      // Current sea-level barometric pressure

//#define BMP085_OS             BMP085_1x_OS
//#define BMP085_OS             BMP085_2x_OS
//#define BMP085_OS             BMP085_4x_OS
  #define BMP085_OS             BMP085_8x_OS
  
  /***********/
  /* Globals */
  /***********/

  int pass_count = 0;
  long reset_timer = 0;

#ifdef USES_BMP085  
  barom_struct BMP085 = {
                         0,
                         0,
                         0,
                         BMP085_temp_per_default,
                         BMP085_press_per_default,
                         BMP085_clear_all_errors,
                         BMP085_1x_OS,
                         BMP085_SW_reset,
                         BMP085_reset_state,
                         BMP085_clear_all_status,
                         0,
                        #ifdef DO_BAROM_COMPENSATION                         
                         BMP085_press_default,
                         BMP085_temp_default,
                        #endif
                         BMP085_press_default,
                         BMP085_temp_default,
                      //#ifdef USES_BMP085
                         BMP085_AC1_coeff_default,
                         BMP085_AC2_coeff_default,
                         BMP085_AC3_coeff_default,
                         BMP085_AC4_coeff_default,
                         BMP085_AC5_coeff_default,
                         BMP085_AC6_coeff_default,
                         BMP085_B1_coeff_default,
                         BMP085_B2_coeff_default,
                         BMP085_MB_coeff_default,
                         BMP085_MC_coeff_default,
                         BMP085_MD_coeff_default,
                      //#endif
                        #ifdef DO_BAROM_ALTITUDE
                         BMP085_P0_default,
                         BMP085_alti_default
                        #endif
                        };
#endif
  
  /*********************/
  /* FUNCTION: setup() */
  /* RETURNS : null    */
  /*********************/

  void setup() {                                            // Arduino set up function
          /*************************/
          /* Set Up Megaduino Pins */
          /*************************/

          pinMode(LED_pin,OUTPUT);                          // Setup LED driver pin as an output
          digitalWrite(LED_pin,LOW);                        // Initialize to logic '0'

          pinMode(RST_pin,OUTPUT);                          // Setup Remote Host Port reset pin as an output
          digitalWrite(RST_pin,LOW);                        // Initialize to logic '0' (active)

         #ifdef USES_BMP085
          pinMode(BAROMETER_INT_pin,INPUT);                 // Setup Barometer interrupt pin as an input
          digitalWrite(BAROMETER_INT_pin,HIGH);             // Activate pull-up on Barometer interrupt pin
         #endif

          /********************/
          /* Set Up UART Port */
          /********************/
          Serial.begin(9600);                               // Open serial port and 
                                                            // set serial baud rate to 9600
         #ifdef WIY_Blox_DEBUG_MSGS
          Serial.println();
          Serial.println(F("Serial Port Opened, 9600 Baud"));// Send message out UART port
          Serial.println();                                 // that UART is open and up
         #endif

          /*******************/
          /* Set Up I2C Port */
          /*******************/
          Wire.begin();                                     // Open I2C-bus port and 
                                                            // connect as a master
         #ifdef WIY_Blox_DEBUG_MSGS
          Serial.println(F("I2C Port Set Up As Master"));   // Send message out UART port
          Serial.println();                                 // that I2C port is set up as master
         #endif
         
          digitalWrite(RST_pin,HIGH);                       // Set to logic '1' (inactive)
                                                            // take WIY-Blox out of reset
          delay(1000);                                      // 1S delay before enterng loop function
       }
  /*******************/
  /* END OF FUNCTION */
  /*******************/


  /********************/
  /* FUNCTION: loop() */
  /* RETURNS : null   */
  /********************/

  void loop() {                                             // Arduino main loop function
         #ifdef WIY_Blox_DEBUG_MSGS
          Serial.print(F("Loop pass #:"));                  // Send message on current loop count
          Serial.println(pass_count,DEC);
         #endif

         #ifdef USES_BMP085
          /********************/
          /* SW RESET HANDLER */
          /********************/
          if (((long)millis() - reset_timer) >= 0) {        // Detect SW reset interval
             BMP085_DEBUG_DUMPS(1);                         // Message snapshot of everything just prior to reset
             BMP085.control = BMP085_SW_reset;              // Perform SW reset
             Serial.println(F("SW RESET INITIATED"));       // Message SW reset initiated
             reset_timer = (long)millis() + RESET_INTERVAL; // Set next reset interval
          }
          if ((BMP085.control & BMP085_SW_reset) && (BMP085.state == BMP085_reset_state)) {
             BMP085.control &= ~BMP085_SW_reset;            // Take out of SW reset once in SW reset
             Serial.println(F("SW RESET CLEARED"));         // Message SW reset is cleared
          }

          /*************************/
          /* CONFIGURATION HANDLER */
          /*************************/
          if (BMP085.stat & BMP085_ready) {
             BMP085_DEBUG_DUMPS(0);                         // Message coefficients
             BMP085.temp_interval = TEMP_INTERVAL;          // Set temperature measurement interval to
                                                            // 4 seconds when multi-meaurement is used
             BMP085.press_interval =  PRESS_INTERVAL;       // Set pressure measurement interval to
                                                            // 1 second when multi-measurement is used
             BMP085.p0 = SEALEVEL_P0;                       // Set sea-level barometric pressure

             BMP085.config = BMP085_OS;                     // Set pressure measurement oversampling

             BMP085.control |= BMP085_take_mult;            // Set for continuous multiple measurement mode

             Serial.println(F("BMP085 CONFIGURED"));        // Message BMP085 configured
          }

          /**********************/
          /* DATA READY HANDLER */
          /**********************/
          if (BMP085.stat & BMP085_data_ready) {            // Detect data ready
            #ifdef DO_BAROM_COMPENSATION
             BMP085_DEBUG_DUMPS(4);                         // Message compensated measurements and altitude
            #endif
            #ifndef DO_BAROM_COMPENSATION
             BMP085_BEBUG_DUMPS(3);                         // Message uncompensated raw measurements
            #endif  
             BMP085.control |= BMP085_data_ready_ack;       // Acknowledge data ready
          }
         #endif

          BMP085_SENSOR();                                  // Call BMP085 sensor handler
          pass_count ++;                                    // Increment pass count
         #ifdef WIY_Blox_DEBUG_MSGS
          Serial.println();                                 // Message line space
         #endif
       }
  /*******************/
  /* END OF FUNCTION */
  /*******************/
   
/*******************/  
/* END OF INO FILE */
/*******************/


