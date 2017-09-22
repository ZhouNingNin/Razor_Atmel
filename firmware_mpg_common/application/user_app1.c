/**********************************************************************************************************************
File: user_app1.c                                                                
Zhou Ning
----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/
static bool bLightControl=TRUE;
static u16 u16Timecount=0;
static u8 u8Print[2]="0";
static u8 u8Save[2]="0";
/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  PWMAudioSetFrequency(BUZZER1,200);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_state1;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_state1(void)
{
  DebugScanf(u8Print);
  /*Judge whether enter '2' and then enter '\r'*/
  if(u8Print[0] == '2')
  {
    u8Save[0] = '2';
    u8Print[0] = '0';
  }
  
  if(u8Save[0]=='2'&&u8Print[0]=='\r')
  {
    u8Save[1] = '\r';
    u8Print[0] = '0';
  }
  
  if(u8Print[0]!= '0'&&u8Print[0] != '\r')
  {
    u8Save[0] = '0';
    u8Save[1] = '\0';
    u8Print[0] = '0';
  }
  /*Start to light up and display something what we want*/
  if(bLightControl == TRUE)
  {
    DebugPrintf("\n\rEntering state 1\n\r");
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"STATE 1");

    LedOff(CYAN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    LedOff(GREEN);
    LedOff(LCD_RED);
    LedOff(LCD_BLUE);
    LedOff(LCD_GREEN);
    LedOn(WHITE);
    LedOn(PURPLE);
    LedOn(BLUE);
    LedOn(CYAN);
    LedOn(LCD_RED);
    LedOn(LCD_BLUE);
    PWMAudioOff(BUZZER1); 
    bLightControl = FALSE;
    ButtonAcknowledge(BUTTON2);/*in case we press BUTTON2 than 2 or more times*/
  }
  /*when we press down BUTTON2 or input '2<CR>'*/
   if(WasButtonPressed(BUTTON2)||(u8Save[0] == '2'&&u8Save[1] == '\r'))
  {
    ButtonAcknowledge(BUTTON2);
    UserApp1_StateMachine = UserApp1SM_state2;
    bLightControl = TRUE;
    u16Timecount = 0;
    u8Save[0] = '0';
    u8Save[1] = '\0';
    u8Print[0] = '0';
  }
}
static void UserApp1SM_state2(void)
{
  u16Timecount++;
  DebugScanf(u8Print);
  /*Judge whether enter '1' and then enter '\r'*/  
  if(u8Print[0] == '1')
  {
    u8Save[0] = '1';
    u8Print[0] = '0';
  }
  
  if(u8Save[0] == '1'&&u8Print[0] == '\r')
  {
    u8Save[1] = '\r';
    u8Print[0] = '0';
  }
  
  if(u8Print[0] != '0'&&u8Print[0] != '\r')
  {
    u8Save[0] = '0';
    u8Save[1] = '\0';
    u8Print[0] = '0';
  }
  /*Start to light up and display something what we want*/  
  if(bLightControl == TRUE)
  {
    DebugPrintf("\n\rEntering state 2\n\r");
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"STATE 2");
    
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(WHITE);
    LedOff(LCD_RED);
    LedOff(LCD_BLUE);
    LedOff(LCD_GREEN);
    LedPWM(LCD_RED,LED_PWM_70);  
    LedPWM(LCD_GREEN,LED_PWM_25);
    LedPWM(LCD_BLUE,LED_PWM_5);
    LedBlink(GREEN,LED_1HZ);
    LedBlink(YELLOW,LED_2HZ);
    LedBlink(ORANGE,LED_4HZ);
    LedBlink(RED,LED_8HZ);
    bLightControl = FALSE;
    PWMAudioOn(BUZZER1);
    ButtonAcknowledge(BUTTON1);/*in case we press BUTTON1 than 2 or more times*/
  }
  
   if(u16Timecount == 100)
   {
     PWMAudioOff(BUZZER1);
   }
  
   if(u16Timecount == 1000)
   {
     PWMAudioOn(BUZZER1);
     u16Timecount = 0;
   }  
  /*when we press down BUTTON1 or input '1<CR>'*/  
   if(WasButtonPressed(BUTTON1)||(u8Save[0] == '1'&&u8Save[1] == '\r'))
  {
    ButtonAcknowledge(BUTTON1);
    UserApp1_StateMachine = UserApp1SM_state1;
    bLightControl = TRUE;
    u8Save[0] = '0';
    u8Save[1] = '\0';
    u8Print[0] = '0';
  }
}



/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  
} /* end UserApp1SM_Idle() */
    
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
