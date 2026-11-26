/*
 * File:   black_box.c
 * Author: shrit
 *
 * Created on 17 November, 2026, 11:36 AM
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



void view_dashboard(){
    if(isUp==1){
        clcd_print("TIME     EV   SP",LINE1(0));
        clcd_print(tempdashboard,LINE2(0));
    }else{
    int read = read_adc(CHANNEL4);
    int sp = (read/10.23);
    
    speed[0] = (sp /10) + '0';
    speed[1] = (sp %10) + '0';
    speed[2] = '\0';
    
    get_time();
    get_gear_pos();
    
 
    clcd_print("TIME     EV   SP",LINE1(0));
    clcd_print(time,LINE2(0));
    strcpy(tempdashboard,time);
    tempdashboard[8] = ' ';
    tempdashboard[9] = '\0';
    clcd_print(event[ind],LINE2(9));
    strcat(tempdashboard,event[ind]);
    clcd_print(speed,LINE2(14));
    tempdashboard[11] = ' ';
    tempdashboard[12] = ' ';
    tempdashboard[13] = ' ';
    tempdashboard[14] = '\0';
    strcat(tempdashboard,speed);
    tempdashboard[16] = '\0';
    }
}

void event_store(void){
    
}

//Password function declaration
void password(void){
    
}

//main menu function declaration
void display_main_menu(void){
    if(menu_type==start){
        clcd_putch('>',LINE1(0));
    }else{
        clcd_putch(' ',LINE1(0));
    }
    clcd_print(menu[start],LINE1(1));
    if(menu_type==end){
        clcd_putch('>',LINE2(0));
    }else{
        clcd_putch(' ',LINE2(0));
    }
    clcd_print(menu[end],LINE2(1));
}

//View log function declaration
void view_log(void){
    
    if(logcount == 0){
        clcd_print("View Log",LINE1(0));
        clcd_print("No Log exists",LINE2(0));
    }else{
        
        if(f==1){
            clcd_print(tempstr,LINE1(0));
            clcd_putch((logViewcount/10) +'0',LINE2(0));
            clcd_putch((logViewcount%10) +'0',LINE2(1));
            
        }else{
    int addr = (logViewcount - 1)*12;
    
    char View[16];
    for(int i = 0; i < 8; i++)
    {
        View[i] = read_external_eeprom( addr++);
    }
    int i = 8;
    View[i] = ' ';
    i++;
    
   View[i++] = read_external_eeprom(addr++);
   View[i++] = read_external_eeprom(addr++);
   
   
   View[i++] = ' ';
   
   
   
   View[i++] = read_external_eeprom(addr++);
   View[i++] = read_external_eeprom(addr++);
   
   View[i] = '\0';
   strcpy(tempstr,View);
  // clcd_write(0x01,0);
   clcd_print(View,LINE1(0));
   clcd_putch((logViewcount/10) +'0',LINE2(0));
   clcd_putch((logViewcount%10) +'0',LINE2(1));
    }
}
}

//Reading events function declaration
void event_reader(void){
    
}

//Change password function declaration
void change_password(void){
    
}

//Set time function declaration
void set_time(void){
    clcd_print("SET TIME    ",LINE1(0));
    
    if(SetTimeFlag==0){
        hour = read_ds1307(HOUR_ADDR);
        if(hour & 0x40){
            hour = (((hour>>4)&0x01)*10) + (hour & 0x0F);
            
        }else{
            hour = (((hour>>4)&0x03)*10)+(hour & 0x0F);
        }
       
      
        min = read_ds1307(MIN_ADDR);
        min = (((min>>4)&0x0F)*10)+ (min & 0x0F);
        
        sec = read_ds1307(SEC_ADDR);
        sec = (((sec>>4)&0x0F)*10)+ (sec & 0x0F);
        prevHour=hour;
        prevMin=min;
        prevSec=sec;
         SetTimeFlag=1;
    }
    
    display_time(hour,min,sec,field);
}

//Download log function _decleration
void download_log(void){
    
    if(logcount==0){
        clcd_print("Download Log",LINE1(0));
        clcd_print("Log not exists",LINE2(0));
        
    }else if(logcount!=0 && downloadend==0){
    
    putch(0x1B);
    putch('[');
    putch('2');
    putch('J');

    putch(0x1B);
    putch('[');
    putch('H');
    
    
    unsigned char tempaddr = (logViewcount-1)*12;
    unsigned char t = logViewcount;
    
    if(logcount<10){
        
        for(int i=1;i<=logcount;i++){
            
            tempaddr = (i-1)*12;
            for(int j=0;j<12;j++){
                
                unsigned char get = read_external_eeprom(tempaddr+j);    
                putch(get);
                if(j==7){
                    putch(' ');
                }
                if(j==9){
                    putch(' ');
                } 
            }
            if(i<logcount){
            putch('\r');
            putch('\n');
            }
                     
        }
        
        
    }else if(logcount>=10){
    
    for(int i=0;i<10;i++){
        
        
      for(int j=0;j<12;j++){
          
         unsigned char get = read_external_eeprom(tempaddr+j);    
         putch(get);
         if(j==7){
              putch(' ');
         }
         if(j==9){
              putch(' ');
         } 
                    
      }  
      t = (t%10)+1;
      tempaddr = (t-1)*12;
      if(i<9){
            putch('\r');
            putch('\n');
            }
        
    }
    
}
    clcd_print("Download Log",LINE1(0));
    clcd_print("Success !",LINE2(0));
    downloadend=1;
    }else if(downloadend==1){
     clcd_print("Download Log",LINE1(0));
     clcd_print("Success !",LINE2(0));   
    }
    
}

//Clear log function declaration
void clear_log(void){
    if(logcount==0){
        clcd_print("Clear Log",LINE1(0));
        clcd_print("No Log exists",LINE2(0));
        isNull = 1;
       
    }else{
        clcd_print("Clear Log",LINE1(0));
        clcd_print("Log cleared",LINE2(0));
        logcount = 0;
        isNull = 0;
    }
    
    
}