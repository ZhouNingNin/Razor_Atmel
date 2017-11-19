/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

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
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static u32 UserApp1_u32DataMsgCount = 0;             /* Counts the number of ANT_DATA packets received */
static u32 UserApp1_u32TickMsgCount = 0;             /* Counts the number of ANT_TICK packets received */

static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;  
static bool bRole=FALSE;
static bool bLcdPrintf=TRUE;
static bool bWelcome=FALSE;
static bool bStart=TRUE;
static bool bDisplay=TRUE;
AntAssignChannelInfoType sAntSetupData,sAntSetupData1;
/* Timeout counter used across states */


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
  if( 1 )
  {
    /* Channel assignment is queued so start timer */

    UserApp1_StateMachine = UserApp1SM_Configure;
  }
  else
  {
    UserApp1_StateMachine = UserApp1SM_Error;
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
static void CloseAllLed(void)
{
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for the ANT channel assignment to finish */

static void UserApp1SM_Configure(void)
{
  CloseAllLed();
  
  /* Clear screen and place start messages */

 /* Configure ANT for this application */
  /*master*/
  sAntSetupData.AntChannel          = ANT_CHANNEL_1;
  sAntSetupData.AntChannelType      = CHANNEL_TYPE_MASTER;
  sAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  
  sAntSetupData.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

  sAntSetupData.AntNetwork = ANT_NETWORK_DEFAULT;
  /*slave*/
  sAntSetupData1.AntChannel          = ANT_CHANNEL_USERAPP;
  sAntSetupData1.AntChannelType      = CHANNEL_TYPE_SLAVE;
  sAntSetupData1.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData1.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  
  sAntSetupData1.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData1.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData1.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData1.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData1.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData1.AntTxPower          = ANT_TX_POWER_USERAPP;
  
  sAntSetupData1.AntNetwork = ANT_NETWORK_DEFAULT;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sAntSetupData.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    sAntSetupData1.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
    
  /* If good initialization, set state to Idle */
  if( AntAssignChannel(&sAntSetupData) )
  {
    /* Channel assignment is queued so start timer */

    UserApp1_StateMachine = UserApp1SM_WaitChannelAssignMaster;
  }
  else
  {
    UserApp1_StateMachine = UserApp1SM_Error;
  }
  
}


static void UserApp1SM_WaitChannelAssignMaster(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL_1)== ANT_CONFIGURED)
  {
    AntAssignChannel(&sAntSetupData1);
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssignSlave;
  }  
  /* Monitor for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}

static void UserApp1SM_WaitChannelAssignSlave(void)
{
  if (bDisplay==TRUE)
  {
    bDisplay=FALSE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "choose your role"); 
    LCDMessage(LINE2_START_ADDR, "B1:Seeker  B2:Hider"); 
  }
  
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP)== ANT_CONFIGURED)
  {
    UserApp1_StateMachine=UserApp1SM_Idle;
  }
  
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}



  /*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserApp1SM_Idle(void)
{
  CloseAllLed();
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    bRole=FALSE;
    bWelcome=TRUE;
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    bRole=TRUE;
    bWelcome=TRUE;
  }  
  if(bWelcome==TRUE)
  {
    bWelcome=FALSE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Hide and Seek"); 
    LCDMessage(LINE2_START_ADDR,"Press B0 to Start"); 
  }
  /* Look for BUTTON 0 to open channel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button, so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON0);
    /* Queue open channel and change LED0 from yellow to blinking green to indicate channel is opening */
    AntOpenChannelNumber(ANT_CHANNEL_1);
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);    
    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
  }
}
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to open */
static void UserApp1SM_WaitChannelOpen(void)
{
  
  /* Monitor the channel status to check if channel is opened */
  if((AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_OPEN)&&
    (AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN))
  { 
    if(bRole==TRUE)
    {
      bStart=TRUE;
      UserApp1_StateMachine = UserApp1SM_ChannelOpen;
    }
    else
    {
      bLcdPrintf=TRUE;
      UserApp1_StateMachine = UserApp1SM_Timedelay;
    }
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE) )
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);    
    UserApp1_StateMachine = UserApp1SM_Idle;
  }      
}

static void UserApp1SM_Timedelay(void)
{
  static u32 u32Timecount=0;
  static u8 Index=1;
  static u8 au8Print[2]="0";
  
  u32Timecount++;
  if(bLcdPrintf==TRUE)
  {
    bLcdPrintf=FALSE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Seeker"); 
  }
  if(u32Timecount==(1000*Index))
  {
    au8Print[1]=HexToASCIICharUpper(Index);
    LCDMessage(LINE2_START_ADDR,au8Print);
    Index++;
  }
  
  if(u32Timecount==10000)
  {
    bStart=TRUE;
    u32Timecount=0;
    Index=1;    
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Ready or not");  
    LCDMessage(LINE2_START_ADDR,"Here i come"); 
    UserApp1_StateMachine = UserApp1SM_ChannelOpen;
  } 
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Channel is open, so monitor data */

static void UserApp1SM_ChannelOpen(void)
{
  static s8 s8Rssi=-99;
  static s8 as8Level[]={-95,-90,-84,-78,-73,-67,-62,-55};
  static u8 au8Led[]={WHITE, PURPLE, BLUE, CYAN, GREEN, YELLOW, ORANGE, RED};
  static u16 au8Frequency[]={50,100,140,180,230,280,320,380};

  /* Check for BUTTON0 to close channel */

  
  if(bStart==TRUE)
  {
    bStart=FALSE;
    if(bRole==TRUE)
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Hide");
    }
    else
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Seek");
    }
  }
  
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) != ANT_OPEN)
  {   
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Time is Over");
    UserApp1_StateMachine=UserApp1SM_Error;
  } /* if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) != ANT_OPEN) */
  
  /* Always check for ANT messages */
  if( AntReadAppMessageBuffer() )
  {
     /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel==0)
      {
        s8Rssi=G_sAntApiCurrentMessageExtData.s8RSSI;
        
        for(u8 i=0;i<8;i++)
        {
          if(s8Rssi>as8Level[i])
          {
            PWMAudioSetFrequency(BUZZER1, au8Frequency[i]);
            PWMAudioOn(BUZZER1);
            LedOn(au8Led[i]);
          }
          else
          {
            LedOff(au8Led[i]);
          }
        }
        if(s8Rssi>-55)
        {
          PWMAudioOff(BUZZER1);
          LCDCommand(LCD_CLEAR_CMD);
          if(bRole==FALSE)
          {
            
            LCDMessage(LINE1_START_ADDR,"I found you");
            bRole=!bRole;
            AntCloseChannelNumber(ANT_CHANNEL_0);
            AntCloseChannelNumber(ANT_CHANNEL_1);
            UserApp1_StateMachine=UserApp1SM_Time;
            bWelcome=TRUE;
          }
          else
          {
            LCDMessage(LINE1_START_ADDR,"you find me");
            bRole=!bRole;
            AntCloseChannelNumber(ANT_CHANNEL_0);
            AntCloseChannelNumber(ANT_CHANNEL_1);            
            UserApp1_StateMachine=UserApp1SM_Time;
            bWelcome=TRUE;
          }    
         }
        if(G_sAntApiCurrentMessageExtData.u8Channel==1)
        {}
      }

    } /* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */
    
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel==0)
      {}
      if(G_sAntApiCurrentMessageExtData.u8Channel==1)
      {}

    } /* end else if(G_eAntApiCurrentMessageClass == ANT_TICK) */
    
  } /* end AntReadAppMessageBuffer() */
  

      
} /* end UserApp1SM_ChannelOpen() */

/*Each 5s change the role*/
static void UserApp1SM_Time(void)
{
  static u32 u32Time=0;
  
  u32Time++;
  if(u32Time==3000)
  {
    u32Time=0;
    bStart=TRUE;
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_ClosingChannels;
  }  
}
static void UserApp1SM_ClosingChannels(void)
{
  /* Ensure that both channels have opened */
  if( (AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CLOSED) &&
      (AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_CLOSED) )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;    
  }

  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Channel close failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to close */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
