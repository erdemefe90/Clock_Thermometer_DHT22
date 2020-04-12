#byte _PORTA   = 0x005
#byte _PORTB   = 0x006
#byte _TRISA   = 0x085
#byte _TRISB   = 0x086
#byte _CCPR1L  = 0x015
#byte _CCP1CON = 0x017
#byte _TMR1L   = 0x00E
#byte _TMR1H   = 0x00F
#byte _T1CON   = 0x010

#define PORTA _PORTA
#define PORTB _PORTB  
#define TRISA _TRISA  
#define TRISB _TRISB  
#define CCPR1L _CCPR1L  
#define CCP1CON _CCP1CON 
#define TMR1L _TMR1L   
#define TMR1H _TMR1H   
#define T1CON _T1CON   

#define SET_BIT(p,n) ((p) |= (1 << (n)))
#define CLR_BIT(p,n) ((p) &= ~((1) << (n)))
#define CHECK_BIT(p,n) ((p) & ((1) << (n)))
#define TOGGLE_BIT(p,n) ((p) ^= ((1) << (n)))