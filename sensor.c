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

unsigned char get_gear_pos()
{
    int previndex = ind;
     unsigned char val = read_switches(STATE_CHANGE);
     
     if(val==MK_SW1){
         
         if(ind>=0 && ind<7){
         ind++;

         }
      
     }else if(val==MK_SW2){
         
         if(ind==8){
             ind = 1;
             
         }
         
         if(ind>0){
         ind--;
         }
  
         
         
     }else if(val==MK_SW3){
          
         ind = 8;
     
     }
    
     if(ind !=previndex){
         
          unsigned char addr;
    unsigned char i;

    addr = logindex * 12;

    for(i = 0; i < 8; i++)
    {
        write_external_eeprom(addr++, time[i]);
    }

      
    write_external_eeprom(addr++, event[ind][0]);
    write_external_eeprom(addr++, event[ind][1]);
    write_external_eeprom(addr++, speed[0]);
    write_external_eeprom(addr++, speed[1]);
    
     if(logcount<10){
         logcount++;
         
     }

    logindex = (logindex+1)%10;
    previndex = ind;
   
}
     
    
     
         
     }
  
    
    
