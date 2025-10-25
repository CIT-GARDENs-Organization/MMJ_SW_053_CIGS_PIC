//////////// Standard Header file for the PIC16F1789 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC16F1789

#nolist
//////// Program memory: 16384x14  Data RAM: 2032  Stack: 16
//////// I/O: 36   Analog Pins: 15
//////// Data EEPROM: 256
//////// C Scratch area: 77   ID Location: 8000
//////// Fuses: LP,XT,HS,RC,INTRC_IO,ECL,ECM,ECH,NOWDT,WDT_SW,WDT_NOSL,WDT
//////// Fuses: PUT,NOPUT,MCLR,NOMCLR,PROTECT,NOPROTECT,CPD,NOCPD,NOBROWNOUT
//////// Fuses: BROWNOUT_SW,BROWNOUT_NOSL,BROWNOUT,CLKOUT,NOCLKOUT,IESO
//////// Fuses: NOIESO,FCMEN,NOFCMEN,WRT,WRT_1000,WRT_200,NOWRT,VCAP,NOVCAP
//////// Fuses: PLL_SW,PLL,STVREN,NOSTVREN,BORV25,BORV19,LPBOR,NOLPBOR,DEBUG
//////// Fuses: NODEBUG,LVP,NOLVP
//////// 
#if (!defined(__PCM__)||defined(__ISNT_CCS__))
#define _bif
#define int8 char
#define int16 long
#define int32 long long
#define float32 float
#define int1 char

#endif
////////////////////////////////////////////////////////////////// I/O
// Discrete I/O Functions: SET_TRIS_x(), OUTPUT_x(), INPUT_x(),
//                         PORT_x_PULLUPS(), INPUT(),
//                         OUTPUT_LOW(), OUTPUT_HIGH(),
//                         OUTPUT_FLOAT(), OUTPUT_BIT(),
// Discrete I/O Prototypes:
_bif void set_tris_a(int8 value);  
_bif void set_tris_b(int8 value);  
_bif void set_tris_c(int8 value);  
_bif void set_tris_d(int8 value);  
_bif void set_tris_e(int8 value);  
_bif void set_input_level_a(int8 value);  
_bif void set_input_level_b(int8 value);  
_bif void set_input_level_c(int8 value);  
_bif void set_input_level_d(int8 value);  
_bif void set_input_level_e(int8 value);  
_bif void set_open_drain_a(int8 value);  
_bif void set_open_drain_b(int8 value);  
_bif void set_open_drain_c(int8 value);  
_bif void set_open_drain_d(int8 value);  
_bif void set_open_drain_e(int8 value);  
_bif int8 get_tris_a(void);  
_bif int8 get_tris_b(void);  
_bif int8 get_tris_c(void);  
_bif int8 get_tris_d(void);  
_bif int8 get_tris_e(void);  
_bif void output_a(int8 value);  
_bif void output_b(int8 value);  
_bif void output_c(int8 value);  
_bif void output_d(int8 value);  
_bif void output_e(int8 value);  
_bif int8 input_a(void);  
_bif int8 input_b(void);  
_bif int8 input_c(void);  
_bif int8 input_d(void);  
_bif int8 input_e(void);  
_bif int8 input_change_a(void);  
_bif int8 input_change_b(void);  
_bif int8 input_change_c(void);  
_bif int8 input_change_d(void);  
_bif int8 input_change_e(void);  
_bif void port_a_pullups(int8 upmask);   
_bif void port_b_pullups(int8 upmask);  
_bif void port_c_pullups(int8 upmask);   
_bif void port_d_pullups(int8 upmask);   
_bif void port_e_pullups(int8 upmask);   
_bif void set_slow_slew_a(int8 value); 
_bif void set_slow_slew_b(int8 value); 
_bif void set_slow_slew_c(int8 value); 
_bif void set_slow_slew_d(int8 value); 
_bif void set_slow_slew_e(int8 value); 
_bif int1 input(int16 pin); 
_bif int1 input_state(int16 pin); 
_bif void output_low(int16 pin); 
_bif void output_high(int16 pin); 
_bif void output_toggle(int16 pin); 
_bif void output_bit(int16 pin, int1 level); 
_bif void output_float(int16 pin); 
_bif void output_drive(int16 pin); 

// Constants used to identify pins in the above are:

#define PIN_A0  96
#define PIN_A1  97
#define PIN_A2  98
#define PIN_A3  99
#define PIN_A4  100
#define PIN_A5  101
#define PIN_A6  102
#define PIN_A7  103

#define PIN_B0  104
#define PIN_B1  105
#define PIN_B2  106
#define PIN_B3  107
#define PIN_B4  108
#define PIN_B5  109
#define PIN_B6  110
#define PIN_B7  111

#define PIN_C0  112
#define PIN_C1  113
#define PIN_C2  114
#define PIN_C3  115
#define PIN_C4  116
#define PIN_C5  117
#define PIN_C6  118
#define PIN_C7  119

#define PIN_D0  120
#define PIN_D1  121
#define PIN_D2  122
#define PIN_D3  123
#define PIN_D4  124
#define PIN_D5  125
#define PIN_D6  126
#define PIN_D7  127

#define PIN_E0  128
#define PIN_E1  129
#define PIN_E2  130
#define PIN_E3  131

////////////////////////////////////////////////////////////////// Useful defines
#define FALSE 0
#define TRUE 1

#define BYTE int8
#define BOOLEAN int1

#define getc getch
#define fgetc getch
#define getchar getch
#define putc putchar
#define fputc putchar
#define fgets gets
#define fputs puts

////////////////////////////////////////////////////////////////// Control
// Control Functions:  RESET_CPU(), SLEEP(), RESTART_CAUSE()
// Prototypes:
_bif int8 restart_cause(void);        
_bif void reset_cpu(void);
_bif void sleep(void);
// Constants returned from RESTART_CAUSE() are:
#define NORMAL_POWER_UP      0x3C        
#define MCLR_FROM_POWER_UP   0x34        
#define BROWNOUT_RESTART     0x3E        
#define MCLR_FROM_SLEEP      0x27        
#define WDT_TIMEOUT          0x1F        
#define WDT_FROM_SLEEP       0x0F        
#define INTERRUPT_FROM_SLEEP 0x2F        
#define MCLR_FROM_RUN        0x37        
#define RESET_INSTRUCTION    0x3B        
#define STACK_OVERFLOW       0xBF        
#define STACK_UNDERFLOW      0x7F        

////////////////////////////////////////////////////////////////// Timer 0
// Timer 0 (AKA RTCC)Functions: SETUP_COUNTERS() or SETUP_TIMER_0(),
//                              SET_TIMER0() or SET_RTCC(),
//                              GET_TIMER0() or GET_RTCC()
// Timer 0 Prototypes:
_bif void setup_timer_0(int8 mode);
_bif void set_timer0(int8 value);      
_bif int8 get_timer0(void);            
_bif void setup_counters(int8 mode, int8 prescaler);
_bif void set_rtcc(int8 value);        
_bif int8 get_rtcc(void);              
// Constants used for SETUP_TIMER_0() are:
#define T0_INTERNAL   0   
#define T0_EXT_L_TO_H 32
#define T0_EXT_H_TO_L 48

#define T0_DIV_1      8
#define T0_DIV_2      0
#define T0_DIV_4      1
#define T0_DIV_8      2
#define T0_DIV_16     3
#define T0_DIV_32     4
#define T0_DIV_64     5
#define T0_DIV_128    6
#define T0_DIV_256    7


#define T0_8_BIT      0     

// The following are provided for compatibility
// with older compiler versions
#define RTCC_INTERNAL   0  
#define RTCC_EXT_L_TO_H 32     
#define RTCC_EXT_H_TO_L 48
#define RTCC_DIV_1      8
#define RTCC_DIV_2      0
#define RTCC_DIV_4      1
#define RTCC_DIV_8      2
#define RTCC_DIV_16     3
#define RTCC_DIV_32     4
#define RTCC_DIV_64     5
#define RTCC_DIV_128    6
#define RTCC_DIV_256    7
#define RTCC_8_BIT      0     

// Constants used for SETUP_COUNTERS() are the above
// constants for the 1st parameter and the following for
// the 2nd parameter:

////////////////////////////////////////////////////////////////// WDT
// Watch Dog Timer Functions: SETUP_WDT(), RESTART_WDT()
// WDT base is 1ms
// WDT Prototypes:
_bif void setup_wdt(int16 mode);
_bif void restart_wdt(void);
// Constants used for SETUP_WDT() are:
#define WDT_ON      0x1000
#define WDT_OFF     0x2000

#define WDT_1MS     0x100
#define WDT_2MS     0x101
#define WDT_4MS     0x102
#define WDT_8MS     0x103
#define WDT_16MS    0x104
#define WDT_32MS    0x105
#define WDT_64MS    0x106
#define WDT_128MS   0x107
#define WDT_256MS   0x108
#define WDT_512MS   0x109
#define WDT_1S      0x10A
#define WDT_2S      0x10B
#define WDT_4S      0x10C
#define WDT_8S      0x10D
#define WDT_16S     0x10E
#define WDT_32S     0x10F
#define WDT_64S     0x110
#define WDT_128S    0x111
#define WDT_256S    0x112

////////////////////////////////////////////////////////////////// Timer 1
// Timer 1 Functions: SETUP_TIMER_1, GET_TIMER1, SET_TIMER1
// Timer 1 Prototypes:
_bif void setup_timer_1(int16 mode); 
_bif int16 get_timer1(void);
_bif void set_timer1(int16 value);
// Constants used for SETUP_TIMER_1() are:
//      (or (via |) together constants from each group)
#define T1_DISABLED         0
#define T1_INTERNAL         0x07      
#define T1_EXTERNAL         0x87      
#define T1_EXTERNAL_SYNC    0x83      
#define T1_FOSC             0x47      

#define T1_ENABLE_SOSC      0x08      

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

#define T1_GATE             0x8000
#define T1_GATE_INVERTED    0xC000
#define T1_GATE_TOGGLE      0xA000
#define T1_GATE_SINGLE      0x9000
#define T1_GATE_TIMER2      0x8100         
#define T1_GATE_COMP1       0x8200      
#define T1_GATE_COMP2       0x8300       

////////////////////////////////////////////////////////////////// Timer 2
// Timer 2 Functions: SETUP_TIMER_2, GET_TIMER2, SET_TIMER2
// Timer 2 Prototypes:
_bif void setup_timer_2(int8 mode, int8 period, int8 postscale); 
_bif int8 get_timer2(void);
_bif void set_timer2(int8 value);
// Constants used for SETUP_TIMER_2() are:
#define T2_DISABLED         0
#define T2_DIV_BY_1         4
#define T2_DIV_BY_4         5
#define T2_DIV_BY_16        6 
#define T2_DIV_BY_64        7 

////////////////////////////////////////////////////////////////// CCP
// CCP Functions: SETUP_CCPx, SET_PWMx_DUTY 
// CCP Variables: CCP_x, CCP_x_LOW, CCP_x_HIGH
// CCP1 Prototypes:
_bif void setup_ccp1(int32 mode);
_bif void setup_ccp1(int32 mode, int8 pwm); 
_bif void set_pwm1_duty(int8 value); 
_bif void set_pwm1_duty(int16 value); 
// Constants used for SETUP_CCPx() are:
#define CCP_OFF                          0
#define CCP_CAPTURE_FE                   0x04
#define CCP_CAPTURE_RE                   0x05
#define CCP_CAPTURE_DIV_4                0x06
#define CCP_CAPTURE_DIV_16               0x07
#define CCP_COMPARE_TOGGLE               0x02 
#define CCP_COMPARE_SET_ON_MATCH         0x08
#define CCP_COMPARE_CLR_ON_MATCH         0x09
#define CCP_COMPARE_INT                  0x0A               
#define CCP_COMPARE_RESET_TIMER          0x0B              
#define CCP_PWM                          0x0C    
#define CCP_PWM_PLUS_1                   0x1c               
#define CCP_PWM_PLUS_2                   0x2c               
#define CCP_PWM_PLUS_3                   0x3c               

#word   CCP_1       =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_LOW   =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_HIGH  =                    getenv("SFR:CCPR1H") 

#define CCP1_C2             0x000
#define CCP1_B0             0x100
#define CCP2_C1             0x000
#define CCP2_B3             0x100
#define CCP3_E0             0x000  
#define CCP3_B5             0x100  

// CCP2 Prototypes:
_bif void setup_ccp2(int32 mode);
_bif void setup_ccp2(int32 mode, int8 pwm); 
_bif void set_pwm2_duty(int8 value); 
_bif void set_pwm2_duty(int16 value); 
#word   CCP_2       =                    getenv("SFR:CCPR2L") 
#byte   CCP_2_LOW   =                    getenv("SFR:CCPR2L") 
#byte   CCP_2_HIGH  =                    getenv("SFR:CCPR2H") 

// CCP3 Prototypes:
_bif void setup_ccp3(int32 mode);
_bif void setup_ccp3(int32 mode, int8 pwm);
_bif void set_pwm3_duty(int8 value);
_bif void set_pwm3_duty(int16 value);
#word   CCP_3       =                    getenv("SFR:CCPR3L")
#byte   CCP_3_LOW   =                    getenv("SFR:CCPR3L")
#byte   CCP_3_HIGH  =                    getenv("SFR:CCPR3H")

////////////////////////////////////////////////////////////////// SPI
// SPI Functions: SETUP_SPI, SPI_WRITE, SPI_READ, SPI_DATA_IS_IN 
// SPI Prototypes:
_bif void setup_spi(int32 mode);
_bif void spi_write(int8 value);
_bif void spi_write(int1 wait, int8 value);
_bif int8 spi_read(void);
_bif int8 spi_read(int8 value);
_bif int1 spi_data_is_in(void);
// Constants used in SETUP_SPI() are: 
#define SPI_DISABLED             0x00 
#define SPI_MASTER               0x20 
#define SPI_SLAVE                0x24 
#define SPI_SCK_IDLE_HIGH        0x10 
#define SPI_SCK_IDLE_LOW         0x00 
#define SPI_CLK_DIV_4            0x00 
#define SPI_CLK_DIV_16           0x01 
#define SPI_CLK_DIV_64           0x02 
#define SPI_CLK_T2               0x03 
#define SPI_SS_DISABLED          0x01 

#define SPI_DI_C4                0x000000
#define SPI_DI_B6                0x010000
#define SPI_SCK_C3               0x000000
#define SPI_SCK_B7               0x020000
#define SPI_DO_C5                0x000000
#define SPI_DO_B5                0x040000
#define SPI_SS_A5                0x000000  
#define SPI_SS_A0                0x080000  
#define SPI_SS_B4                0x100000  

#define SPI_XMIT_L_TO_H          0x4000
#define SPI_XMIT_H_TO_L          0x0000

#define SPI_SAMPLE_AT_MIDDLE     0x0000
#define SPI_SAMPLE_AT_END        0x8000

//The following are provided for compatibility
#define SPI_L_TO_H       SPI_SCK_IDLE_LOW
#define SPI_H_TO_L       SPI_SCK_IDLE_HIGH

////////////////////////////////////////////////////////////////// UART
// UART Prototypes:
_bif void setup_uart(int32 baud);
_bif void setup_uart(int32 baud, int8 stream);
_bif void setup_uart(int32 baud, int8 stream, int32 clock);
_bif void set_uart_speed(int32 baud);
_bif void set_uart_speed(int32 baud, int8 stream);
_bif void set_uart_speed(int32 baud, int8 stream, int32 clock);
// Constants used in setup_uart() are:
// FALSE - Turn UART off
// TRUE  - Turn UART on
#define UART_ADDRESS           2
#define UART_DATA              4
#define UART_AUTODETECT        8
#define UART_AUTODETECT_NOWAIT 9
#define UART_WAKEUP_ON_RDA     10
#define UART_SEND_BREAK        13

////////////////////////////////////////////////////////////////// COMP
// Comparator Variables: C1OUT, C2OUT, C3OUT, C4OUT 
// Comparator Prototypes:
_bif void setup_comparator_1(int16 mode);
_bif void setup_comparator_2(int16 mode);
_bif void setup_comparator_3(int16 mode);
_bif void setup_comparator_4(int16 mode);  
// Constants used in setup_comparator_x() are:
//
#define NC_NC           0x003F

// Constants used in setup_comparator_1() are:
// Pick one of the following
#define CP1_A0_A2       0x8000
#define CP1_A1_A2       0x8001
#define CP1_B3_A2       0x8002
#define CP1_B1_A2       0x8003
#define CP1_VSS_A2      0x8007
#define CP1_A0_A3       0x8008
#define CP1_A1_A3       0x8009
#define CP1_B3_A3       0x800A
#define CP1_B1_A3       0x800B
#define CP1_VSS_A3      0x800F
#define CP1_A0_DAC      0x8028
#define CP1_A1_DAC      0x8029
#define CP1_B3_DAC      0x802A
#define CP1_B1_DAC      0x802B
#define CP1_VSS_DAC     0x802F
#define CP1_A0_FVR      0x8030
#define CP1_A1_FVR      0x8031
#define CP1_B3_FVR      0x8032
#define CP1_B1_FVR      0x8033
#define CP1_VSS_FVR     0x8037
#define CP1_A0_VSS      0x8038
#define CP1_A1_VSS      0x8039
#define CP1_B3_VSS      0x803A
#define CP1_B1_VSS      0x803B
#define CP1_VSS_VSS     0x803F
//Optionally OR any of the following with the above
#define CP1_INT_L2H     0x0080
#define CP1_INT_H2L     0x0040
#define CP1_OUT_ON_A4   0x2000
#define CP1_INVERT      0x1000
#define CP1_FAST        0x0400
#define CP1_HYST        0x0200
#define CP1_SYNC        0x0100

// Constants used in setup_comparator_2() are:
// Pick one of the following
#define CP2_A0_A2       0x8000
#define CP2_A1_A2       0x8001
#define CP2_B3_A2       0x8002
#define CP2_B1_A2       0x8003
#define CP2_VSS_A2      0x8007
#define CP2_A0_B0       0x8008
#define CP2_A1_B0       0x8009
#define CP2_B3_B0       0x800A
#define CP2_B1_B0       0x800B
#define CP2_VSS_B0      0x800F
#define CP2_A0_DAC      0x8028
#define CP2_A1_DAC      0x8029
#define CP2_B3_DAC      0x802A
#define CP2_B1_DAC      0x802B
#define CP2_VSS_DAC     0x802F
#define CP2_A0_FVR      0x8030
#define CP2_A1_FVR      0x8031
#define CP2_B3_FVR      0x8032
#define CP2_B1_FVR      0x8033
#define CP2_VSS_FVR     0x8037
#define CP2_A0_VSS      0x8038
#define CP2_A1_VSS      0x8039
#define CP2_B3_VSS      0x803A
#define CP2_B1_VSS      0x803B
#define CP2_VSS_VSS     0x803F
//Optionally OR any of the following with the above
#define CP2_INT_L2H     0x00080
#define CP2_INT_H2L     0x00040
#define CP2_OUT_ON_A5   0x02000
#define CP2_OUT_ON_A6   0x12000
#define CP2_INVERT      0x01000
#define CP2_FAST        0x00400
#define CP2_HYST        0x00200
#define CP2_SYNC        0x00100

// Constants used in setup_comparator_3() are:
// Pick one of the following
#define CP3_A0_A2       0x8000
#define CP3_A1_A2       0x8001
#define CP3_B3_A2       0x8002
#define CP3_B1_A2       0x8003
#define CP3_VSS_A2      0x8007
#define CP3_A0_B4       0x8008
#define CP3_A1_B4       0x8009
#define CP3_B3_B4       0x800A
#define CP3_B1_B4       0x800B
#define CP3_VSS_B4      0x800F
#define CP3_A0_DAC      0x8028
#define CP3_A1_DAC      0x8029
#define CP3_B3_DAC      0x802A
#define CP3_B1_DAC      0x802B
#define CP3_VSS_DAC     0x802F
#define CP3_A0_FVR      0x8030
#define CP3_A1_FVR      0x8031
#define CP3_B3_FVR      0x8032
#define CP3_B1_FVR      0x8033
#define CP3_VSS_FVR     0x8037
#define CP3_A0_VSS      0x8038
#define CP3_A1_VSS      0x8039
#define CP3_B3_VSS      0x803A
#define CP3_B1_VSS      0x803B
#define CP3_VSS_VSS     0x803F
//Optionally OR any of the following with the above
#define CP3_INT_L2H     0x0080
#define CP3_INT_H2L     0x0040
#define CP3_OUT_ON_D6   0x2000  
#define CP3_INVERT      0x1000
#define CP3_FAST        0x0400
#define CP3_HYST        0x0200
#define CP3_SYNC        0x0100

// Constants used in setup_comparator_4() are:  
// Pick one of the following  
#define CP4_A0_A2       0x8000  
#define CP4_A1_A2       0x8001  
#define CP4_B5_A2       0x8002  
#define CP4_B1_A2       0x8003  
#define CP4_VSS_A2      0x8007  
#define CP4_A0_B6       0x8008  
#define CP4_A1_B6       0x8009  
#define CP4_B5_B6       0x800A  
#define CP4_B1_B6       0x800B  
#define CP4_VSS_B6      0x800F  
#define CP4_A0_DAC      0x8028  
#define CP4_A1_DAC      0x8029  
#define CP4_B5_DAC      0x802A  
#define CP4_B1_DAC      0x802B  
#define CP4_VSS_DAC     0x802F  
#define CP4_A0_FVR      0x8030  
#define CP4_A1_FVR      0x8031  
#define CP4_B5_FVR      0x8032  
#define CP4_B1_FVR      0x8033  
#define CP4_VSS_FVR     0x8037  
#define CP4_A0_VSS      0x8038  
#define CP4_A1_VSS      0x8039  
#define CP4_B5_VSS      0x803A  
#define CP4_B1_VSS      0x803B  
#define CP4_VSS_VSS     0x803F  
//Optionally OR any of the following with the above  
#define CP4_INT_L2H     0x0080  
#define CP4_INT_H2L     0x0040  
#define CP4_OUT_ON_D7   0x2000  
#define CP4_INVERT      0x1000  
#define CP4_FAST        0x0400  
#define CP4_HYST        0x0200  
#define CP4_SYNC        0x0100  

#bit C1OUT = getenv("SFR:CM1CON0").6
#bit C2OUT = getenv("SFR:CM2CON0").6
#bit C3OUT = getenv("SFR:CM3CON0").6
#bit C4OUT = getenv("SFR:CM4CON0").6  

////////////////////////////////////////////////////////////////// VREF
// VREF Prototypes:
_bif void setup_vref(int8 mode);
// Constants used in setup_vref() are:
#define VREF_OFF                        0
#define VREF_ON                         0x80
// The following may be OR'ed in with the above using |
#define VREF_ADC_OFF                    0
#define VREF_ADC_1v024                  0x81
#define VREF_ADC_2v048                  0x82
#define VREF_ADC_4v096                  0x83  
// The following may be OR'ed in with the above using |  
#define VREF_COMP_DAC_OFF               0  
#define VREF_COMP_DAC_1v024             0x84  
#define VREF_COMP_DAC_2v048             0x88  
#define VREF_COMP_DAC_4v096             0x8C  
// The following may be OR'ed in with the above using | 
#define TEMPERATURE_INDICATOR_DISABLED  0 
#define TEMPERATURE_INDICATOR_ENABLED   0x20 
#define TEMPERATURE_RANGE_LOW           0 
#define TEMPERATURE_RANGE_HIGH          0x10 

////////////////////////////////////////////////////////////////// DAC
// Digital to Analog Functions: SETUP_DAC(), DAC_WRITE(), 
//                              SETUP_DAC2(), DAC_WRITE2(), 
//                              SETUP_DAC3(), DAC_WRITE3(), 
//                              SETUP_DAC4(), DAC_WRITE4() 
// DAC Prototypes:
_bif void setup_dac(int8 mode);
_bif void dac_write(int8 value);
_bif void setup_dac2(int8 mode);  
_bif void dac_write2(int8 value);  
_bif void setup_dac3(int8 mode);  
_bif void dac_write3(int8 value);  
_bif void setup_dac4(int8 mode);  
_bif void dac_write4(int8 value);  
// Constants used in SETUP_DAC() are:
#define DAC_OFF       0
#define DAC_VSS_VDD   0x80
#define DAC_VREF_VDD  0x81  //Can only be used with setup_dac() 
#define DAC_VSS_VREF  0x84
#define DAC_VREF_VREF 0x85  //Can only be used with setup_dac() 
#define DAC_VSS_FVR   0x88  //Can only be used with setup_dac() 
#define DAC_VREF_FVR  0x89  //Can only be used with setup_dac() 
// The Following may be OR'ed in with the above using |
#define DAC_OUTPUT1   0x20  // DAC voltage level outputted on DACxOUT1 pin 
#define DAC_OUTPUT2   0x10  // DAC voltage level outputted on DACxOUT1 pin 

////////////////////////////////////////////////////////////////// OPAMP
// OPAMP Functions: SETUP_OPAMPx()
// OPAMP Prototypes:
_bif void setup_opamp1(int8 mode);   
_bif void setup_opamp2(int8 mode);    
_bif void setup_opamp3(int8 mode); 
// Constants used in SETUP_OPAMPx() are: 
#define OPAMP_ENABLED                       0x80    
#define OPAMP_DISABLED                      0x00    
#define OPAMP_HIGH_GBWP_MODE                0x40    
#define OPAMP_LOW_GBWP_MODE                 0x00    
#define OPAMP_NI_TO_DAC4                    0x07 //Non-inverting input connects to DAC4 output 
#define OPAMP_NI_TO_DAC3                    0x06 //Non-inverting input connects to DAC3 output 
#define OPAMP_NI_TO_DAC2                    0x05 //Non-inverting input connects to DAC2 output 
#define OPAMP_NI_TO_DAC1                    0x04 //Non-inverting input connects to DAC1 output 
#define OPAMP_NI_TO_FVR                     0x03 //Non-inverting input connects to FVR Buffer 2 output 
#define OPAMP_NI_TO_OPAIN                   0x00 //Non-inverting input connects to OPAxIN+ pin 
#define OPAMP_NI_TO_VSS                            0
#define OPAMP_RESISTOR_LADDER_DISABLED             0
#define OPAMP_RESISTOR_LADDER_ENABLED       0x080000
#define OPAMP_OVERRIDE_MODE_DISABLED               0
#define OPAMP_OVERRIDE_MODE_ENABLED           0x8000

////////////////////////////////////////////////////////////////// PSMC PWM
// PSMC Functions:  SETUP_PSMC(), PSMC_DEADBAND(), PSMC_SYNC(),
//                  PSMC_MODULATION(), PSMC_SHUTDOWN(), PSMC_PINS()
// PSMC Prototypes:
_bif void setup_psmc(int8 unit, int8 mode, int16 period, int8 rising_edge, int16 rise_time, int8 falling_edge, int16 fall_time);
_bif void psmc_duty(int8 unit, int16 duty);
// Constants used for second param to setup_psmc():
#define PSMC_SINGLE               0x00
#define PSMC_PUSH_PULL            0x02
#define PSMC_BRIDGE_PUSH_PULL     0x04
#define PSMC_PULSE_SKIPPING       0x06
#define PSMC_ECCP_BRIDGE_REVERSE  0x08
#define PSMC_ECCP_BRIDGE_FORWARD  0x09
#define PSMC_VARIABLE_FREQ        0x0A
#define PSMC_3_PHASE              0x0C
// The following may be OR'ed with the above
#define PSMC_COMPLEMENTARY        0x01
#define PSMC_ENABLE_NOW           0x80
// Event constants used for params 3-5:
#define PSMC_EVENT_TIME           0x01
#define PSMC_EVENT_C1OUT          0x02
#define PSMC_EVENT_C2OUT          0x04
#define PSMC_EVENT_C3OUT          0x08
#define PSMC_EVENT_C4OUT          0x10
#define PSMC_EVENT_IN_PIN         0x80
// Constants used for third param to setup_psmc():
// Use one of the above events OR'ed with both a source and prescaler below:
#define PSMC_SOURCE_FOSC          0
#define PSMC_SOURCE_64MHZ         1
#define PSMC_SOURCE_CLK_PIN       2
#define PSMC_DIV_1                0x00
#define PSMC_DIV_2                0x10
#define PSMC_DIV_4                0x20
#define PSMC_DIV_8                0x30
// Constants used for fourth and fifth params to setup_psmc():
// Use one of the above events
_bif void psmc_deadband(int8 unit, int8 rising_edge, int8 falling_edge);
_bif void psmc_blanking(int8 unit, int8 rising_edge, int8 rise_time, int8 falling_edge, int8 fall_time);
// Constants for the second and third param are one of the PSMC_EVENT_... defines
// Use FALSE to disable blanking for an edge
_bif void psmc_sync(int8 slave_unit, int8 master_unit, int8 options);
// Constants used for third param to psmc_sync():
#define PSMC_SOURCE_IS_PHASE   0x80
#define PSMC_SOURCE_IS_PERIOD  0x100
#define PSMC_DISCONNECT        0x00
// The following may be OR'ed with the above
#define PSMC_INVERT_DUTY       0x20
#define PSMC_INVERT_PERIOD     0x40
_bif void psmc_modulation(int8 unit, int8 options);
// Constants psmc_modulation():
#define PSMC_MOD_OFF        0x00
#define PSMC_MOD_ACTIVE     0xA0
#define PSMC_MOD_INACTIVE   0x80
#define PSMC_MOD_C1OUT      0x81
#define PSMC_MOD_C2OUT      0x82
#define PSMC_MOD_C3OUT      0x83
#define PSMC_MOD_C4OUT      0x84
#define PSMC_MOD_CCP1       0x85
#define PSMC_MOD_CCP2       0x86
#define PSMC_MOD_IN_PIN     0x88
// The following may be OR'ed with the above
#define PSMC_MOD_INVERT     0x40
#define PSMC_MOD_NOT_BDF    0x200
#define PSMC_MOD_NOT_ACE    0x100
_bif void psmc_pins(int8 unit, int8 pins_used, int8 pins_active_low);
// Constants psmc_pins(): // or together pins
#define PSMC_A              0x01
#define PSMC_B              0x02
#define PSMC_C              0x04
#define PSMC_D              0x08
#define PSMC_E              0x10
#define PSMC_F              0x20
#define PSMC_ON_NEXT_PERIOD 0x100
_bif int8 psmc_shutdown(int8 unit, int8 options, int8 source, int8 pins_high);
// Constants used for second param to psmc_shutdown():
#define PSMC_SHUTDOWN_OFF          0x00
#define PSMC_SHUTDOWN_NORMAL       0x40
#define PSMC_SHUTDOWN_AUTO_RESTART 0x60
// Constants used for second param to psmc_shutdown():
#define PSMC_SHUTDOWN_C1OUT        0x02
#define PSMC_SHUTDOWN_C2OUT        0x04
#define PSMC_SHUTDOWN_C3OUT        0x08
#define PSMC_SHUTDOWN_C4OUT        0x10
#define PSMC_SHUTDOWN_IN_PIN       0x80
// Constants used for forth param to psmc_shutdown() are from psmc_pins().
// **See constants for psmc_pins()
_bif int8 psmc_shutdown(int8 unit, int16 command);
// Constants used for second param to psmc_shutdown():
#define PSMC_SHUTDOWN_RESTART     0x100
#define PSMC_SHUTDOWN_FORCE       0x200
#define PSMC_SHUTDOWN_CHECK       0x300


////////////////////////////////////////////////////////////////// INTERNAL RC
// Oscillator Prototypes:
_bif void setup_oscillator(int8 mode);
// Constants used in setup_oscillator() are:
// First parameter:
#define OSC_31KHZ   0
#define OSC_31250   (2*8)
#define OSC_62KHZ   (4*8)
#define OSC_125KHZ  (5*8)
#define OSC_250KHZ  (6*8)
#define OSC_500KHZ  (7*8)
#define OSC_1MHZ    (11*8)
#define OSC_2MHZ    (12*8)
#define OSC_4MHZ    (13*8)
#define OSC_8MHZ    (14*8)
#define OSC_16MHZ   (15*8)
// The following may be OR'ed in with the above using |
#define OSC_TIMER1  1 
#define OSC_INTRC   2
#define OSC_NORMAL  0
// The following may be OR'ed in with the above using |
#define OSC_PLL_ON  0x80
#define OSC_PLL_OFF 0


////////////////////////////////////////////////////////////////// ADC
// ADC Functions: SETUP_ADC(), SETUP_ADC_PORTS() (aka SETUP_PORT_A),
//                SET_ADC_CHANNEL(), READ_ADC(), ADC_DONE()
// ADC Prototypes:
_bif void setup_adc(int16 mode);
_bif int8 read_adc(void);
_bif int8 read_adc(int8 mode);
_bif int16 read_adc(void);
_bif int16 read_adc(int8 mode);
_bif int1 adc_done(void);
// Constants used for SETUP_ADC() are: 
#define ADC_OFF                0              // ADC Off
#define ADC_CLOCK_DIV_2    0x100  
#define ADC_CLOCK_DIV_4     0x40  
#define ADC_CLOCK_DIV_8     0x10
#define ADC_CLOCK_DIV_32    0x20
#define ADC_CLOCK_DIV_16    0x50
#define ADC_CLOCK_DIV_64    0x60
#define ADC_CLOCK_INTERNAL  0x30              // Internal 2-6us

//ADC Prototypes:
_bif void setup_adc_ports(int32 pins);
_bif void setup_adc_ports(int32 pins, int32 reference);
_bif void set_analog_pins(int8 pin, ...);
_bif void setup_adc_reference(int32 reference);
_bif void set_adc_channel(int8 channel);
// Constants used is SETUP_ADC_PORTS() are:
// First argument:
// OR together desired pins
#define sAN0            0x01000000  //| A0
#define sAN1            0x02000000  //| A1
#define sAN2            0x04000000  //| A2
#define sAN3            0x08000000  //| A3
#define sAN4            0x20000000  //| A5
#define sAN5            0x00010000  //| E0  
#define sAN6            0x00020000  //| E1  
#define sAN7            0x00040000  //| E2  
#define sAN8            0x00000400  //| B2
#define sAN9            0x00000800  //| B3
#define sAN10           0x00000200  //| B1
#define sAN11           0x00001000  //| B4
#define sAN12           0x00000100  //| B0
#define sAN13           0x00002000  //| B5
#define sAN21           0x00000002  //| D1  
#define NO_ANALOGS               0  // None
#define ALL_ANALOG      0x2F073F02  // A0 A1 A2 A3 A5 E0 E1 E2 B2 B3 B1 B4 B0 B5 D1 

// Optional Second parameter for SETUP_ADC_PORTS, and parameter for SETUP_ADC_REFERENCE:
#define VSS_VDD         0x00000000  //| Range 0 to Vdd
#define VSS_VREF        0x00000001  //| Range 0 to Vref+
#define VSS_FVR         0x00000003  //| Range 0 to FVR
#define VREF_VDD        0x00000004  //| Range Vref- to Vdd
#define VREF_VREF       0x00000005  //| Range Vref- to Vref+
#define VREF_FVR        0x00000007  //| Range Vref- to FVR

// Constants used for first parameter of SET_ADC_CHANNEL() are:
// either use the channel number or one of the following
#define TEMPERATURE_INDICATOR     0x1D 
#define DAC_CHANNEL               0x1E
#define FVR_CHANNEL               0x1F
// Constants used for second parameter of SET_ADC_CHANNEL() are:
// either use the channel number or the following
#define ADC_NEG_REFERENCE         0x0F  // Analog Negative Channel connected to ADC Negative Reference

// Constants used in READ_ADC() are:
#define ADC_START_AND_READ     7   // This is the default if nothing is specified
#define ADC_START_ONLY         1
#define ADC_READ_ONLY          6

////////////////////////////////////////////////////////////////// BIF
// Built In Functions Prototypes
//
// Math Prototypes:
_bif signed int8 abs(signed int8 x);
_bif signed int16 abs(signed int16 x);
_bif signed int32 abs(signed int32 x);
_bif float32 abs(float32 x);
_bif unsigned int16 _mul(unsigned int8, unsigned int8);
_bif signed int16 _mul(signed int8, signed int8);
_bif unsigned int32 _mul(unsigned int16, unsigned int16);
_bif signed int32 _mul(signed int16, signed int16);

// Memory Manipulation Prototypes:
_bif int8 read_bank(int8 bank, int8 offset);
_bif void write_bank(int8 bank, int8 offset, int8 value);
_bif void strcpy(char* dest, char* src);
_bif void memset(unsigned int8* destination, unsigned int8 value, unsigned int16 num);
_bif void memcpy(unsigned int8* destination, unsigned int8* source, unsigned int16 num);

// String Prototypes:
_bif char toupper(char cvalue);
_bif char tolower(char cvalue);
_bif void sprintf(char* string, char* cstring, ...);

// Data Manipulators Prototypes:
_bif int1 shift_left(unsigned int8* address, unsigned int8 bytes, int1 value);
_bif int1 shift_right(unsigned int8* address, unsigned int8 bytes, int1 value);
_bif void rotate_left(unsigned int8* address, unsigned int8 bytes);
_bif void rotate_right(unsigned int8* address, unsigned int8 bytes);
_bif void swap(unsigned int8 value);
_bif unsigned int8 make8(unsigned int16 var, unsigned int8 offset);
_bif unsigned int8 make8(unsigned int32 var, unsigned int8 offset);
_bif unsigned int16 make16(unsigned int8 varhigh, unsigned int8 varlow);
_bif unsigned int32 make32(unsigned int16 var1);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int16 var2);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int8 var2);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int8 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int8 var3, unsigned int8 var4);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int16 var2);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int16 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int16 var3);
_bif void bit_set(unsigned int8 var, unsigned int8 bit);
_bif void bit_set(unsigned int16 var, unsigned int8 bit);
_bif void bit_set(unsigned int32 var, unsigned int8 bit);
_bif void bit_clear(unsigned int8 var, unsigned int8 bit);
_bif void bit_clear(unsigned int16 var, unsigned int8 bit);
_bif void bit_clear(unsigned int32 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int8 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int16 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int32 var, unsigned int8 bit);

// #use delay() Prototypes:
_bif void delay_cycles(unsigned int8 count);
_bif void delay_ms(unsigned int16 time);
_bif void delay_us(unsigned int16 time);

// #use rs232() Prototypes:
_bif void putchar(char cdata);
_bif void putchar(char cdata, unsigned int8 stream);
_bif void puts(char* string);
_bif void puts(char* string, unsigned int8 stream);
_bif char getch(void);
_bif char getch(unsigned int8 stream);
_bif void gets(char* string);
_bif void gets(char* string, unsigned int8 stream);
_bif int1 kbhit(void);
_bif int1 kbhit(unsigned int8 stream);
_bif void printf(char* string, ...);
_bif void fprintf(unsigned int8 stream, char* string, ...);
_bif void putc_send(void);
_bif void fputc_send(unsigned int8 stream);
_bif int1 rcv_buffer_full(void);
_bif int1 rcv_buffer_full(unsigned int8 stream);
_bif unsigned int16 rcv_buffer_bytes(void);
_bif unsigned int16 rcv_buffer_bytes(unsigned int8 stream);
_bif int1 tx_buffer_full(void);
_bif int1 tx_buffer_full(unsigned int8 stream);
_bif unsigned int16 tx_buffer_bytes(void);
_bif unsigned int16 tx_buffer_bytes(unsigned int8 stream);
_bif unsigned int8 linbus_header(unsigned int8 identifier);
_bif unsigned int8 linbus_header(unsigned int8 stream, unsigned int8 identifier);
_bif int1 linbus_header_hit(void);
_bif int1 linbus_header_hit(unsigned int8 stream);
_bif unsigned int8 linbus_header_get(void);
_bif unsigned int8 linbus_header_get(unsigned int8 stream);
_bif unsigned int8 linbus_rx_response(unsigned int8 *DataPtr, unsigned int8 Count);
_bif unsigned int8 linbus_rx_response(unsigned int8 stream, unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_tx_response(unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_tx_response(unsigned int8 stream, unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_checksum_type(unsigned int8 type);
_bif void linbus_checksum_type(unsigned int8 stream, unsigned int8 type);

// #use i2c() Prototypes:
_bif unsigned int8 i2c_read(void);
_bif unsigned int8 i2c_read(unsigned int8 ack);
_bif unsigned int8 i2c_read(unsigned int8 stream, unsigned int8 ack);
_bif int1 i2c_write(unsigned int8 data);
_bif int1 i2c_write(unsigned int8 stream, unsigned int8 data);
_bif void i2c_start(void);
_bif void i2c_start(unsigned int8 stream);
_bif void i2c_start(unsigned int8 stream, unsigned int8 restart);
_bif void i2c_stop(void);
_bif void i2c_stop(unsigned int8 stream);
_bif int8 i2c_isr_state(void);
_bif void i2c_slaveaddr(unsigned int8 addr);
_bif void i2c_slaveaddr(unsigned int8 stream, unsigned int8 addr);
_bif int1 i2c_poll(void);
_bif int1 i2c_poll(unsigned int8 stream);
_bif void i2c_init(unsigned int32 baud);
_bif void i2c_init(unsigned int8 stream, unsigned int32 baud);
_bif int8 i2c_transfer(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount, unsigned int8 *rData, unsigned int8 *rCount);
_bif int8 i2c_transfer(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount, unsigned int8 *rData, unsigned int8 *rCount);
_bif int8 i2c_transfer_out(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer_out(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer_in(unsigned int8 Address, unsigned int8 *rData, unsigned int8 rCount);
_bif int8 i2c_transfer_in(unsigned int8 stream, unsigned int8 Address, unsigned int8 *rData, unsigned int8 rCount);

// #use spi() Prototypes:
_bif unsigned int8 spi_xfer(void);
_bif unsigned int16 spi_xfer(void);
_bif unsigned int32 spi_xfer(void);
_bif unsigned int8 spi_xfer(unsigned int8 data);
_bif unsigned int16 spi_xfer(unsigned int16 data);
_bif unsigned int32 spi_xfer(unsigned int32 data);
_bif unsigned int8 spi_xfer(unsigned int8 stream, unsigned int8 data);
_bif unsigned int16 spi_xfer(unsigned int8 stream, unsigned int16 data);
_bif unsigned int32 spi_xfer(unsigned int8 stream, unsigned int32 data);
_bif unsigned int8 spi_xfer(unsigned int8 stream, unsigned int8 data, unsigned int8 bits);
_bif unsigned int16 spi_xfer(unsigned int8 stream, unsigned int16 data, unsigned int8 bits);
_bif unsigned int32 spi_xfer(unsigned int8 stream, unsigned int32 data, unsigned int8 bits);
_bif void spi_init(unsigned int32 baud);
_bif void spi_init(unsigned int8 stream, unsigned int32 baud);
_bif void spi_speed(unsigned int32 baud);
_bif void spi_speed(unsigned int8 stream, unsigned int32 baud);
_bif void spi_speed(unsigned int8 stream, unsigned int32 baud, unsigned int32 clock);
_bif void spi_prewrite(unsigned int8 data);
_bif void spi_prewrite(unsigned int16 data);
_bif void spi_prewrite(unsigned int32 data);
_bif void spi_prewrite(unsigned int8, unsigned int8 data);
_bif void spi_prewrite(unsigned int8, unsigned int16 data);
_bif void spi_prewrite(unsigned int8, unsigned int32 data);
_bif unsigned int8 spi_xfer_in(void);
_bif unsigned int16 spi_xfer_in(void);
_bif unsigned int32 spi_xfer_in(void);
_bif unsigned int8 spi_xfer_in(unsigned int8 bits);
_bif unsigned int16 spi_xfer_in(unsigned int8 bits);
_bif unsigned int32 spi_xfer_in(unsigned int8 bits);
_bif unsigned int8 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif unsigned int16 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif unsigned int32 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif void spi_transfer(unsigned int8 *wData, unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer(unsigned int8 stream, unsigned int8 *wData, unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer_write(unsigned int8 *wData, unsigned int16 Count);
_bif void spi_transfer_write(unsigned int8 stream, unsigned int8 *wData, unsigned int16 Count);
_bif void spi_transfer_read(unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer_read(unsigned int8 stream, unsigned int8 *rData, unsigned int16 Count);

// #use rtos() Prototypes:
_bif void rtos_run(void);
_bif void rtos_yield(void);
_bif void rtos_enable(unsigned int8 task);
_bif void rtos_disable(unsigned int8 task);
_bif void rtos_terminate(void);
_bif void rtos_await(int1 flag);
_bif void rtos_wait(unsigned int8 sem);
_bif void rtos_signal(unsigned int8 sem);
_bif void rtos_msg_send(unsigned int8 task, unsigned int8 msg);
_bif unsigned int8 rtos_msg_read(void);
_bif unsigned int8 rtos_msg_poll(void);
_bif int1 rtos_overrun(unsigned int8 task);
_bif void rtos_stats(unsigned int8 task, unsigned int8* stat);

// #use timer() Prototypes:
_bif unsigned int8 get_ticks(void);
_bif unsigned int16 get_ticks(void);
_bif unsigned int32 get_ticks(void);
_bif unsigned int8 get_ticks(unsigned int8 stream);
_bif unsigned int16 get_ticks(unsigned int8 stream);
_bif unsigned int32 get_ticks(unsigned int8 stream);
_bif void set_ticks(unsigned int8 value);
_bif void set_ticks(unsigned int16 value);
_bif void set_ticks(unsigned int32 value);
_bif void set_ticks(unsigned int8 stream, unsigned int8 value);
_bif void set_ticks(unsigned int8 stream, unsigned int16 value);
_bif void set_ticks(unsigned int8 stream, unsigned int32 value);

// #use pwm() Prototypes:
_bif void pwm_on(void);
_bif void pwm_on(unsigned int8 stream);
_bif void pwm_off(void);
_bif void pwm_off(unsigned int8 stream);
_bif void pwm_set_duty(unsigned int16 duty);
_bif void pwm_set_duty(unsigned int8 stream, unsigned int16 duty);
_bif void pwm_set_duty_percent(unsigned int16 percent);
_bif void pwm_set_duty_percent(unsigned int8 stream, unsigned int16 percent);
_bif void pwm_set_frequency(unsigned int32 frequency);
_bif void pwm_set_frequency(unsigned int8 stream, unsigned int32 frequency);
_bif unsigned int16 pwm_get_duty_period(void);
_bif unsigned int16 pwm_get_duty_period(unsigned int8 stream);

// #use capture() Prototypes:
_bif unsigned int16 get_capture_time(void);
_bif unsigned int16 get_capture_time(unsigned int8 stream);
_bif int1 get_capture_event(void);
_bif int1 get_capture_event(unsigned int8 stream);

// Enviroment Prototypes:
//_bif unsigned int8 getenv(char* cstring);

// Address Prototypes:
#ifndef __ADDRESS__
 #if defined(__PCM__) || defined(__PCB__)
  #define __ADDRESS__ unsigned int16
 #else
  #define __ADDRESS__ unsigned int32
 #endif
#endif
_bif void goto_address(__ADDRESS__ address);
_bif __ADDRESS__ label_address(__ADDRESS__ label);

// Program Memory Read Prototypes:
_bif void read_program_memory(__ADDRESS__ address, unsigned int8* dataptr, unsigned int16 count);
_bif unsigned int16 read_program_eeprom(__ADDRESS__ address);
_bif void read_configuration_memory(unsigned int8* dataptr, unsigned int8 count); 
_bif void read_configuration_memory(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 

// Program Memory Write Prototypes:
_bif void write_program_memory(__ADDRESS__ address, unsigned int8* dataptr, unsigned int16 count);
_bif void write_program_eeprom(__ADDRESS__ address, unsigned int16 data);
_bif void write_configuration_memory(unsigned int8* dataptr, unsigned int8 count); 
_bif void write_configuration_memory(unsigned int8 offset, unsigned int8* dataptr, unsigned int8 count); 

// EEPROM Prototypes:
#ifndef __EEADDRESS__
 #if getenv("DATA_EEPROM")>256
  #define __EEADDRESS__ unsigned int16
 #else
  #define __EEADDRESS__ unsigned int8
 #endif
#endif
_bif unsigned int8 read_eeprom(__EEADDRESS__ address);
_bif void write_eeprom(__EEADDRESS__ address, unsigned int8 value);

////////////////////////////////////////////////////////////////// INT
// Interrupt Functions: ENABLE_INTERRUPTS(), DISABLE_INTERRUPTS(),
//                      CLEAR_INTERRUPT(), INTERRUPT_ACTIVE(),  
//                      EXT_INT_EDGE()  
// INT Prototypes:
_bif void enable_interrupts(int32 interrupt);
_bif void disable_interrupts(int32 interrupt);
_bif void clear_interrupt(int32 interrupt);
_bif int1 interrupt_active(int32 interrupt);
_bif int1 interrupt_enabled(int32 interrupt);
_bif void ext_int_edge(int8 source, int8 edge);  
_bif void jump_to_isr(int16 address);
// Constants used in EXT_INT_EDGE() are:  
#define L_TO_H              0x40  
#define H_TO_L                 0  
// Constants used in ENABLE/DISABLE_INTERRUPTS() are:
#define GLOBAL                    0x0BC0
#define PERIPH                    0x0B40
#define INT_IOC_A0                   0x30010B08
#define INT_IOC_A0_L2H               0x10010B08
#define INT_IOC_A0_H2L               0x20010B08
#define INT_IOC_A1                   0x30020B08
#define INT_IOC_A1_L2H               0x10020B08
#define INT_IOC_A1_H2L               0x20020B08
#define INT_IOC_A2                   0x30040B08
#define INT_IOC_A2_L2H               0x10040B08
#define INT_IOC_A2_H2L               0x20040B08
#define INT_IOC_A3                   0x30080B08
#define INT_IOC_A3_L2H               0x10080B08
#define INT_IOC_A3_H2L               0x20080B08
#define INT_IOC_A4                   0x30100B08
#define INT_IOC_A4_L2H               0x10100B08
#define INT_IOC_A4_H2L               0x20100B08
#define INT_IOC_A5                   0x30200B08
#define INT_IOC_A5_L2H               0x10200B08
#define INT_IOC_A5_H2L               0x20200B08
#define INT_IOC_A6                   0x30400B08
#define INT_IOC_A6_L2H               0x10400B08
#define INT_IOC_A6_H2L               0x20400B08
#define INT_IOC_A7                   0x30800B08
#define INT_IOC_A7_L2H               0x10800B08
#define INT_IOC_A7_H2L               0x20800B08
#define INT_IOC_B0                   0x31010B08
#define INT_IOC_B0_L2H               0x11010B08
#define INT_IOC_B0_H2L               0x21010B08
#define INT_IOC_B1                   0x31020B08
#define INT_IOC_B1_L2H               0x11020B08
#define INT_IOC_B1_H2L               0x21020B08
#define INT_IOC_B2                   0x31040B08
#define INT_IOC_B2_L2H               0x11040B08
#define INT_IOC_B2_H2L               0x21040B08
#define INT_IOC_B3                   0x31080B08
#define INT_IOC_B3_L2H               0x11080B08
#define INT_IOC_B3_H2L               0x21080B08
#define INT_IOC_B4                   0x31100B08
#define INT_IOC_B4_L2H               0x11100B08
#define INT_IOC_B4_H2L               0x21100B08
#define INT_IOC_B5                   0x31200B08
#define INT_IOC_B5_L2H               0x11200B08
#define INT_IOC_B5_H2L               0x21200B08
#define INT_IOC_B6                   0x31400B08
#define INT_IOC_B6_L2H               0x11400B08
#define INT_IOC_B6_H2L               0x21400B08
#define INT_IOC_B7                   0x31800B08
#define INT_IOC_B7_L2H               0x11800B08
#define INT_IOC_B7_H2L               0x21800B08
#define INT_IOC_C0                   0x32010B08
#define INT_IOC_C0_L2H               0x12010B08
#define INT_IOC_C0_H2L               0x22010B08
#define INT_IOC_C1                   0x32020B08
#define INT_IOC_C1_L2H               0x12020B08
#define INT_IOC_C1_H2L               0x22020B08
#define INT_IOC_C2                   0x32040B08
#define INT_IOC_C2_L2H               0x12040B08
#define INT_IOC_C2_H2L               0x22040B08
#define INT_IOC_C3                   0x32080B08
#define INT_IOC_C3_L2H               0x12080B08
#define INT_IOC_C3_H2L               0x22080B08
#define INT_IOC_C4                   0x32100B08
#define INT_IOC_C4_L2H               0x12100B08
#define INT_IOC_C4_H2L               0x22100B08
#define INT_IOC_C5                   0x32200B08
#define INT_IOC_C5_L2H               0x12200B08
#define INT_IOC_C5_H2L               0x22200B08
#define INT_IOC_C6                   0x32400B08
#define INT_IOC_C6_L2H               0x12400B08
#define INT_IOC_C6_H2L               0x22400B08
#define INT_IOC_C7                   0x32800B08
#define INT_IOC_C7_L2H               0x12800B08
#define INT_IOC_C7_H2L               0x22800B08
#define INT_IOC_E3                   0x34080B08
#define INT_IOC_E3_L2H               0x14080B08
#define INT_IOC_E3_H2L               0x24080B08
#define INT_EXT_L2H               0x50000B10
#define INT_EXT_H2L               0x60000B10
#define INT_EXT                   0x300B10
#define INT_TIMER0                0x300B20
#define INT_TIMER1                0x309101
#define INT_TIMER2                0x309102
#define INT_CCP1                  0x309104
#define INT_SSP                   0x309108
#define INT_TBE                   0x309110
#define INT_RDA                   0x309120
#define INT_AD                    0x309140
#define INT_TIMER1_GATE            0x309180
#define INT_BUSCOL                0x309208
#define INT_EEPROM                0x309210
#define INT_COMP                  0x309220
#define INT_COMP2                 0x309240
#define INT_OSC_FAIL              0x309280
#define INT_CCP3                  0x309310
#define INT_CCP2                  0x309201
#define INT_COMP3                 0x309202
#define INT_PSMC1S                0x309401
#define INT_PSMC2S                0x309402
#define INT_PSMC1T                0x309410
#define INT_PSMC2T                0x309420
#define INT_COMP4                 0x309204
#define INT_PSMC3S                0x309404
#define INT_PSMC3T                0x309440

#list
