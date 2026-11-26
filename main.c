/* Goutham C P
 * 26001B_059
 * ============================================================================
 *  Project : Car Black Box
 *  Platform: Embedded System (PIC18F4580)
 *  Language: Embedded C
 *  IDE     : MPLAB X IDE
 *  Compiler: MPLAB XC8
 *
 *  Description:
 *  ------------
 *  This project implements a Car Black Box system using the PIC18F4580
 *  microcontroller.
 *
 *  The system continuously monitors vehicle-related parameters such as
 *  speed and gear position, displays real-time information on a Character
 *  LCD, and records driving events along with their corresponding
 *  timestamps in external EEPROM.
 *
 *  The recorded data can be viewed, cleared, and downloaded through
 *  the menu-driven interface.
 *
 *  Features:
 *  ---------
 *  - Real-time vehicle information monitoring.
 *  - Dashboard displaying:
 *          ? Real-Time Clock (HH:MM:SS)
 *          ? Gear Position
 *          ? Vehicle Speed
 *  - Gear position detection using Matrix Keypad.
 *  - Speed measurement using ADC.
 *  - Real-Time Clock implementation using DS1307.
 *  - External EEPROM interfacing using 24C02.
 *  - I2C communication for RTC and external EEPROM operations.
 *  - Stores a maximum of 10 driving log entries.
 *  - Each log entry contains:
 *          ? Time (8 bytes)
 *          ? Event (2 bytes)
 *          ? Speed (2 bytes)
 *  - View Log functionality to retrieve stored records.
 *  - Clear Log functionality to erase stored records.
 *  - Download Log functionality using UART communication.
 *  - Set Time functionality to update the RTC.
 *  - Short-press and long-press switch detection.
 *  - Menu-driven navigation using Matrix Keypad.
 *  - LCD-based user interface using 16x2 Character LCD.
 *  - Persistent log metadata management.
 *  - Handles EEPROM read and write operations.
 *
 *  Hardware Peripherals Used:
 *  --------------------------
 *  - PIC18F4580 Microcontroller
 *  - 24C02 External EEPROM
 *  - DS1307 Real-Time Clock
 *  - 16x2 Character LCD
 *  - ADC
 *  - Matrix Keypad
 *  - UART Communication Interface
 *  - I2C Communication Interface
 *
 *  Technical Concepts Used:
 *  ------------------------
 *  - Embedded C Programming
 *  - Microcontroller Interfacing
 *  - ADC Programming
 *  - I2C Protocol
 *  - UART Communication
 *  - External EEPROM Interfacing
 *  - Real-Time Clock Interfacing
 *  - LCD Interfacing
 *  - Matrix Keypad Scanning
 *  - Switch Debouncing and Press Detection
 *  - State Machine Implementation
 *  - Persistent Data Storage
 *  - Modular Embedded System Design
 *
 *  Execution:
 *  ----------
 *  1. Build the project using MPLAB X IDE and XC8 Compiler.
 *  2. Generate the production HEX file.
 *  3. Program the PIC18F4580 microcontroller using the supported
 *     programming method.
 *  4. Power ON the system to start the Car Black Box application.
 *
 *  Operation:
 *  ----------
 *  - Dashboard:
 *        Displays current time, gear position and speed.
 *
 *  - View Log:
 *        Displays previously recorded driving events.
 *
 *  - Clear Log:
 *        Clears the stored driving records.
 *
 *  - Download Log:
 *        Transfers recorded log information through UART.
 *
 *  - Set Time:
 *        Allows the user to modify the RTC time.
 *
 *  Remarks:
 *  --------
 *  The project uses I2C communication to interface with the DS1307 RTC
 *  and 24C02 external EEPROM.
 *
 *  A maximum of 10 log entries are maintained, with each entry occupying
 *  12 bytes of external EEPROM storage. circular buffer concept
 *
 *  The system provides a menu-driven interface for monitoring vehicle
 *  information and managing recorded driving events.

 The matrix keypad consists of five switches (MK1?MK5), each assigned specific functions in the Car Black Box system.

1. Dashboard State

MK1: Increases the gear/event selection.
MK2: Decreases the gear/event selection.
MK3: Detects a collision.
MK4 (Long Press): Enters the Main Menu.

2. Main Menu State

MK4 (Short Press): Scrolls up through the menu options.
MK5 (Short Press): Scrolls down through the menu options.
MK4 (Long Press): Selects the currently highlighted menu option.
MK5 (Long Press): Returns to the Dashboard (Home Screen) from the Main Menu or an active menu operation.

3. View Log State

On entering View Log, the latest recorded log is displayed first.
MK4 (Short Press): Scrolls up to view newer logs.
MK5 (Short Press): Scrolls down to view older logs, from latest to oldest.
MK5 (Long Press): Returns to the Main Menu.

4. Download Log State

Downloads the stored log records through UART communication.
Logs are displayed in the Tera Term terminal in chronological order, from oldest to latest.

5. Set Time State

MK4 (Long Press): Navigates between the Hour, Minute, and Second fields.
MK4 (Short Press): Increments the selected field.
MK5 (Short Press): Decrements the selected field.
The selected field blinks to indicate that it is currently active and can be modified.
Hour Range: 0?23
Minute Range: 0?59
Second Range: 0?59

The RTC uses a 24-hour time format.
MK5 (Long Press): Returns to the Main Menu after setting the time.

6. Clear Log State

Clears all previously stored log records from the external EEPROM.
Resets the log count and associated log metadata to allow fresh log recording.

7. Short Press and Long Press Detection

Short press and long press operations are implemented using switch detection and counter-based logic.
Short press performs the assigned navigation or operation.
Long press performs menu selection or state transitions.
The long-press threshold is configured as 300 loop iterations.
 * ============================================================================
 */



#include<xc.h>
#include"black_box.h"
#include"ds1307.h"
#include"clcd.h"
#include"i2c.h"
#include"adc.h"
#include"matrix_keypad.h"
#include"eeprom.h"
#include<string.h>
#include"uart.h"

char menu_type=0;
unsigned char longPress = 0;
unsigned char ShortPress = 0;
unsigned int delayUp= 0;
unsigned int delayDown = 0;
unsigned char logViewcount;
unsigned char downloadend=0;
char start= 0;
char end = 1;
unsigned char temp = 0;
char view_log_begin = 0;
unsigned char LongPress = 0;
unsigned char field = 0;
unsigned char SetTimeFlag = 0;
unsigned char setincrement;
unsigned char hour;
unsigned char min;
unsigned char sec;
unsigned char prevHour;
unsigned char prevMin;
unsigned char prevSec;
unsigned char clearend = 0;
unsigned char isNull=0;
unsigned char isUp=0;

char f = 0;

char scrollcount = 0;
char *menu[4] = {"  View Log","  Clear Log","  DownLoad Log","  SetTime"};


unsigned char ind = 0;
int logindex = 0;
int logcount = 0;
State_t state;
char speed[3];
char time[10];
char *event[9] = {"ON" , "GN" , "G1", "G2", "G3", "G4", "G5", "GR", "CO"};
char ev[3];
unsigned char clock_reg[3];
char tempstr[16];
char tempdashboard[17];

void init_config()
{   init_uart();
    init_i2c();
    init_adc();
    init_clcd();
    init_ds1307();
    init_matrix_keypad();
    state = e_dashboard;
}

void main(void)
{
    init_config();


    while(1)
    {
        // Detect key press

         unsigned char val = read_switches(LEVEL_CHANGE);

               if(val==MK_SW4 ){      // if(val==MK_SW4 )

                 /*   while(read_switches(LEVEL_CHANGE) == MK_SW4 && delayUp<300)
                   {
                     delayUp++;
                   __delay_ms(1);
                   }  */  
                   if(delayUp==0){
                       isUp = 1;
                   }
                 delayUp++;

                   
                 if(delayUp>=300){       
                     if(state==e_dashboard){
                 clcd_write(0x01,0);
                 state = e_main_menu;
                 while(read_switches(LEVEL_CHANGE)==MK_SW4);
                 start = 0;
                 end = 1;
                 menu_type = 0;
                 delayUp=0;
                 LongPress = 0;
                 
                     }else if(state==e_main_menu){
                    if(menu_type == 0){
               
                state = e_view_log;
                
                if(logcount<10){
                logViewcount = logcount;
                }else if(logcount>=10){
              
                    if(logindex==0){
                        logViewcount = 10;
                        logindex = 10;
                    }else{
                    logViewcount =logindex;
                    }
                    if(logindex==0){
                     logViewcount= 10;  
                    }
                    view_log_begin = 1;
                }
                    
                }else if(menu_type == 1){
                state = e_clear_log;                
                clcd_write(0x01,0);

            }else if(menu_type == 2){
                state = e_download_log;
                if(logcount<10){
                logViewcount = 1;
                }else if(logcount>=10){
              
                    logViewcount =(logindex % 10) + 1;
                  /*  if(logindex==0){
                        logViewcount = 1;
                        
                    }else{
                    logViewcount =logindex+1;
                    }*/                                        
                }
            }     else if(menu_type == 3){
                state = e_set_time;
               }
             
            clcd_write(0x01,0);
            while(read_switches(LEVEL_CHANGE)==MK_SW4);
            
            delayUp = 0;
            LongPress = 0;
               }else if(state==e_set_time){                   
                   field = (field +1)%3;
                   while(read_switches(LEVEL_CHANGE)==MK_SW4);
                   delayUp = 0;
                                       
               }
                     
                }
           //   clcd_putch(logcount+'0',LINE2(14));
           }else if(delayUp>0 && delayUp<300){

               if(state==e_main_menu){
                 if(menu_type>0){
               menu_type-=1;
             }
             if(menu_type<0){
                 menu_type = 0;
             }else if(menu_type>=0){
                 if(menu_type<start){
                     start = start-1;
                     end = end-1;
                     clcd_write(0x01,0);
                 }else if(menu_type>=start && menu_type<=end){
                     start = start;
                     end = end;
                 }
             }
            delayUp = 0;
            ShortPress = 0;
           }else if(state==e_view_log){
               
               if(logcount<10){
                   
                   if(logViewcount<logcount){
                       logViewcount++;
                   }
                   
                   
               }else if(logcount==10){
                   
                   if(logindex<10){
                     if((logViewcount-logindex)!=0 && view_log_begin!=1){
                         
                     logViewcount = (logViewcount%10)+1;
                     }
                     if(view_log_begin==1){
                         view_log_begin=0;
                     }
                     }else if(logindex==10){
                         
                         if(logViewcount!=10){
                             temp = 0;
                             logViewcount = (logViewcount%10)+1;
                         }
                     }
                                      
               }
               
               delayUp = 0;
           }else if(state==e_set_time){
               
               if(field==0){
                   if(hour!=23){
                   hour = (hour+1)%24;
                   }                   
               }else if(field==1){
                   if(min!=59){
                   min = (min + 1)%60;
                   }
               }else if(field==2){
                   if(sec!=59){
                   sec = (sec + 1)%60;
                   }
               }
               delayUp = 0;
           }
              
               delayUp = 0;
               isUp = 0;
           }
           else{
               
            delayUp = 0;
            isUp = 0;

           }
         
         
         

           if(val==MK_SW5){
               
       /*        while(read_switches(LEVEL_CHANGE) == MK_SW5 && delayDown<300)
    {
        delayDown++;

        __delay_ms(1);
    }*/
               if(delayDown==0){
                   f = 1;
               }
              delayDown++;
               if(delayDown>=300){
                   
                   if(state==e_view_log){
                       
                       state = e_main_menu;
                       f = 0;
                       if(logindex==10){
                           logindex = 0;
                       }  
                       clcd_write(0x01,0);               
                       while(read_switches(LEVEL_CHANGE)==MK_SW5);
                       
                      
                       
                   }else if(state==e_download_log){
                        state = e_main_menu;
                        downloadend = 0;
                        clcd_write(0x01,0);
                        while(read_switches(LEVEL_CHANGE)==MK_SW5);
                       
                       if(logindex==10){
                           logindex = 0;
                       }    
                       
                   }else if(state==e_set_time){
                       
                        if((hour==prevHour) && (min==prevMin) && (sec==prevSec)){
                            SetTimeFlag = 0;
                            state = e_main_menu;
                        }else{
                            write_time(hour,min,sec);
                            SetTimeFlag = 0;
                            field = 0;
                            state = e_main_menu;
                        }
                        clcd_write(0x01,0);  
                        while(read_switches(LEVEL_CHANGE)==MK_SW5);
                             
                   }else if(state==e_main_menu){
                       
                       state = e_dashboard;
                       clcd_write(0x01,0);
                       while(read_switches(LEVEL_CHANGE)==MK_SW5);
                          
                       
                   }else if(state==e_clear_log){
                       clcd_write(0x01,0);
                       state = e_main_menu;
                       clearend = 0;
                       isNull = 0;
                       while(read_switches(LEVEL_CHANGE)==MK_SW5);
                   }else if(state==e_download_log){
                       state = e_main_menu;
                       clcd_write(0x01,0);
                       while(read_switches(LEVEL_CHANGE)==MK_SW5);
                       
                   }  
                   delayDown = 0;
                   f = 0;
               }
               
         }else  if(delayDown>0 && delayDown<300){
               
             if(state==e_main_menu){
                menu_type+=1;
             if(menu_type>3){
                 menu_type = 3;
             }else if(menu_type<=3){
                 if(menu_type>end){
                     start = end;
                     end = end+1;
                     clcd_write(0x01,0);
                 }else if(menu_type>=start && menu_type<=end){
                     start = start;
                     end = end;
                 }
             }
                delayDown = 0;
             }else if(state==e_view_log){ // down key
                 
                 if(logcount<10){
                     if(logViewcount>1){
                     logViewcount-=1;
                     }  
                     clcd_write(0x01,0);//
                 }else if(logcount>=10){
                     
                     if(logindex<10){
                     if((logViewcount-logindex)!=1 && view_log_begin!=1){
                     logViewcount = ((logViewcount+8)%10)+1;
                     }
                     if(view_log_begin==1){
                         view_log_begin=0;
                     }
                     }else if(logindex==10){
                         if(logViewcount!=1){
                             logViewcount = ((logViewcount+8)%10)+1;
                         }
                     }
                     clcd_write(0x01,0);
                 }
                 
                delayDown = 0; 
               
             }else if(state==e_set_time){
                 
                 if(field==0){
                     if(hour!=0){
                     hour = (hour+23)%24;
                     }
                 }else if(field==1){
                     if(min!=0){
                     min = (min+59)%60;
                     }
                 }else if(field==2){
                     if(sec!=0){
                     sec = (sec+59)%60;
                     }
                 }
                 
                 delayDown = 0;  
             }
             delayDown = 0;
             f = 0;
           }else{
             delayDown = 0;
             f = 0;
           }

    



        switch (state)
        {
            case e_dashboard:
                // Display dashboard
                view_dashboard();
                break;

            case e_main_menu:
                // Display dashboard  
                display_main_menu();
                break;

            case e_view_log:
                // Display dashboard
                view_log();
                break;

            case e_download_log:
                download_log();
                break;

            case e_clear_log: 
                if(clearend==0){
                clear_log();
                clearend = 1;
                }else{
                    if(isNull==1){
                  clcd_print("Clear Log",LINE1(0));
                  clcd_print("No Log exists",LINE2(0));
                    }else{
                      clcd_print("Clear Log",LINE1(0));
                      clcd_print("Log cleared",LINE2(0));
                    }
                }
                    
                break;


            case e_set_time:
                set_time();
                break;

        }

    }

}
