#include <xc.h>

/* Enum for maintaining the app state */
typedef enum {
    e_dashboard, e_main_menu, e_view_log, e_set_time, e_download_log, e_clear_log
} State_t;


extern State_t state; // App state
extern char *event[9];
extern char ev[3];
extern char speed[3];
extern char time[10];
unsigned char clock_reg[3];
unsigned char ind;
extern unsigned char clock_reg[3];
extern int logindex;
extern char menu_type;
extern unsigned char longPress;
extern unsigned int delayUp;
extern char scrollcount;
extern char *menu[4];
extern char start;
extern char end;
extern unsigned char LongPress;
extern unsigned char ShortPress;
extern unsigned int delayDown;
extern int logcount;
extern unsigned char logViewcount;
extern char view_log_begin;
extern unsigned char temp;
extern unsigned char field;
extern unsigned char SetTimeFlag;
extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;
extern unsigned char prevHour;
extern unsigned char prevMin;
extern unsigned char prevSec;
extern unsigned char downloadend;
extern unsigned char temp;
extern char f;
extern unsigned char clearend;
extern unsigned char isNull;
extern unsigned char isUp;
extern char tempdashboard[17];

extern char tempstr[16];
//Function declarations

//Dashboard function declaration
void view_dashboard(void);

//Storing events function declaration
void event_store(void);

//Password function declaration
void password(void);

//main menu function declaration
void display_main_menu(void);

//View log function declaration
void view_log(void);

//Reading events function declaration
void event_reader(void);

//Change password function declaration
void change_password(void);

//Set time function declaration
void set_time(void);

//Download log function _decleration
void download_log(void);

//Clear log function declaration
void clear_log(void);

// Gear pos
unsigned char get_gear_pos();