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
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
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


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u32 u32Timecount=0;
  static u8 au8Line[20][21];//Define that we can input 20 lines content
  static u8 u8Input[2];
  static u8 Index1=0;
  static u8 Index2=0;
  static bool bOut=FALSE;
  static bool bInitialize=FALSE;
  static u8 IndexOut=0;
  u8 u8Inputvalue=1;

  /*Initialize the 2-d array one time*/
  if(bInitialize==FALSE)
  {
    for(u8 i=0;i<20;i++)
    {
      for(u8 j=0;j<21;j++)
      {
        au8Line[i][j]='\0';
      }
    }
    bInitialize=TRUE;
  }
  u8Inputvalue=DebugScanf(u8Input);
  /*Determine whether there is input */
  if(u8Inputvalue==1)
  {
    au8Line[Index1][Index2]=u8Input[0];
    Index2++;
    if(Index2==20)
    {
      Index1++;
      Index2=0;
    }  
  }
  /*Determine whether has confirmed that enter or exit the show the next input*/
  if(WasButtonPressed(BUTTON0))
  {
    LCDCommand(LCD_CLEAR_CMD);
    ButtonAcknowledge(BUTTON0);
    if(bOut==TRUE)
    {
      bInitialize=FALSE;
    }
    bOut=!bOut; 
    Index1=0;
    Index2=0;
    u32Timecount=0;
    IndexOut=0;
  }
  /*Start to display what you input*/
  if(bOut)
  {
    u32Timecount++;
    if(au8Line[1][0]=='\0')//when your input is only one line
    {
      if(u32Timecount==2000)
      {
        LCDMessage(LINE1_START_ADDR,&au8Line[IndexOut][0]);
      }
    }
    else//when your input is more than one line ,every two seconds to scroll up one line
    {
      if((u32Timecount==2000&&au8Line[IndexOut+1][0]!='\0'))
      {
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR,&au8Line[IndexOut][0]);
        LCDMessage(LINE2_START_ADDR,&au8Line[IndexOut+1][0]); 
        IndexOut++;    
        u32Timecount=0;
      }
    }
  }

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
