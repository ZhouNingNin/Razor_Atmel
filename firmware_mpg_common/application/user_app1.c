/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

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
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


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
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  
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
/*Judge what we input is right*/
static bool JudgeString(u8 *au8Input_)
{
  u8 *pu8Pointer = au8Input_;
  bool First=FALSE;
  u8 au8LedNames[] = {'W','P','B','C','G','Y','O','R','w','p','b','c','g','y','o','r'};
	if(!pu8Pointer)
  {
		return FALSE;
	}
	else
  {
    for(u8 i=0;i<16;i++)
    {
      if(*pu8Pointer==au8LedNames[i])
      {
        First=TRUE;
      }      
    }
		if(First==FALSE )
    {
			return FALSE;
		}
		else
    {
			pu8Pointer++;
			if(*pu8Pointer != '-')
      {
				return FALSE;
			}
			else
      {
				pu8Pointer++;
				if(*pu8Pointer > '9' || *pu8Pointer < '0')
        {
					return FALSE;
				}
				else
        {
					while(*pu8Pointer <= '9' && *pu8Pointer >= '0')
          {
						pu8Pointer++;
					}
					if(*pu8Pointer != '-')
          {
						return FALSE;
					}
					else
          {
						pu8Pointer++;
						if(*pu8Pointer > '9' || *pu8Pointer < '0')
            {
							return FALSE;
						}
						else
            {
							while(*pu8Pointer <= '9' && *pu8Pointer >= '0')
              {
								pu8Pointer++;
							}
              if(*pu8Pointer=='\0')
              {
							  return TRUE; 
              }
              else
              {
                return FALSE;
              }
						}
          }
        }
      }
    }
  }
}
/*extract the ON time from the command of this line*/    
static u32 OnTime(u8 *au8Input_)
{
  u32 u32num=0;
  u8 *pu8Pointer=au8Input_;
  
  while(*pu8Pointer<'0'||*pu8Pointer>'9')
  {
    pu8Pointer++;
  }
  while(*pu8Pointer>='0'&&*pu8Pointer<='9')
  {
    u32num*=10;
    u32num+=(*pu8Pointer-'0');
    pu8Pointer++;
  }
  return u32num;  
}
/*extract the OFF time from the command of this line*/   
static u32 OffTime(u8 *au8Input_)
{
  u32 u32num=0;
  u8 *pu8Pointer=au8Input_;
  
  while(*pu8Pointer!='-')
  {
    pu8Pointer++;
  }
  pu8Pointer++;
  while(*pu8Pointer!='-')
  {
    pu8Pointer++;
  }
  pu8Pointer++;
  while(*pu8Pointer>='0'&&*pu8Pointer<='9')
  {
    u32num*=10;
    u32num+=(*pu8Pointer-'0');
    pu8Pointer++;
  }
  return u32num; 
}
/*Match the 'R' to RED,'W' to WHITE and so on*/ 
static LedNumberType LedColor(u8 *au8Input_)
{
  switch(*au8Input_)
  {
  case 'R':return RED; break;
  case 'W':return WHITE; break;  
  case 'P':return PURPLE; break;
  case 'B':return BLUE; break;
  case 'C':return CYAN;  break;
  case 'G':return GREEN;  break;
  case 'Y':return YELLOW; break;
  case 'O':return ORANGE;break;
  case 'r':return RED; break;
  case 'w':return WHITE; break;  
  case 'p':return PURPLE; break;
  case 'b':return BLUE; break;
  case 'c':return CYAN;  break;
  case 'g':return GREEN;  break;
  case 'y':return YELLOW; break;
  case 'o':return ORANGE;break;
  default :return LCD_RED;break;
  }  
}
  


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for input */
static void UserApp1SM_Idle(void)
{
  static bool bProgramInterface=TRUE;
  static u8 au8bProgramInterface[]="**************************************************\n\rLED Programming Interface \n\rPress 1 to program LED command sequence\n\rPress 2 to show current USER program\n\r**************************************************\n\r";
  static u8 au8Press1[]="Enter commands as LED-ONLINE-OFFTIME and press Enter\n\rTime is in milliseconds,max 100 commands\n\rLED colors:R,O,Y,G,C,B,P,W\n\rExample: R-100-200 (Red on at 100ms and off at 200ms)\n\rPress Enter on blank line to end\n\r";
  static u8 au8Input[20];
  u8 au8Enter[2]={10,10};
  u8 au8Enter1[2]={10,10};
  static u8 Index1=0;
  static u8 Index2=0;
  static bool bClear=TRUE;
  static bool bChoose=TRUE;
  static u8 u8Press=0;
  static u8 u8ListSize=0;
  static LedCommandType Store;
  static u8 u8ListNumber=1;
  static bool bPrint=TRUE;
  static u32 u32TimeCount=0;
   
  /*Display the interface*/
  if(bProgramInterface==TRUE)
  {
    DebugPrintf(au8bProgramInterface);
    bProgramInterface=FALSE;    
  }
  /*each time when we input one line command ,clear the array*/
  if(bClear==TRUE)
  {
    for(u8 i=0;i<20;i++)
    {
      au8Input[i]='\0';   
    }
    bClear=FALSE; 
  }
  /*Judge which pattern we enter*/
  if(bChoose==TRUE)
  {
    DebugScanf(au8Enter);
    if(au8Enter[0]=='1')
    {
      LedDisplayStartList();
      u8ListNumber=1;
      u8ListSize=0;
      u8Press=1;
      DebugPrintf("\n\r");
      DebugPrintf(au8Press1);
      DebugPrintf("1:");
      bChoose=FALSE;
    }
    if(au8Enter[0]=='2')
    {
      u8Press=2;
      DebugPrintf("\n\r");
      bChoose=FALSE;   
      bPrint=TRUE;
      u32TimeCount=0;
      Index2=0;
    }
  }
  /*when we press 1*/
  if(u8Press==1)
  {    
    DebugScanf(au8Enter1);
    if(au8Enter1[0]!=10)
    {      
      if(au8Enter1[0]!='\r')//Judge whether we press 'enter' or not
      {
        if(au8Enter1[0]=='\b')//if we enter the button of delect
        {
          if(Index1!=0)
          {
            au8Input[Index1-1]='\0';
            Index1--;
          }
          else
          {
            DebugPrintf(":");
          }
        }
        else
        {
          au8Input[Index1]=au8Enter1[0];
          Index1++;                   
        }
      }
      else
      {
        DebugPrintf("\n");
        if(Index1==0)//This means we continuous input 'enter' twice
        {
          u8Press=2;
          bClear=TRUE; 
          bPrint=TRUE;
          u32TimeCount=0;
          Index2=0;
        }
        else
        {
          if(JudgeString(au8Input)==TRUE)//Judge whether what we input is right command
          {
            u8ListNumber++;
            DebugPrintNumber(u8ListNumber);
            DebugPrintf(":");
            Store.eLED = LedColor(au8Input);
            Store.u32Time = OnTime(au8Input);
            Store.bOn = TRUE;
            LedDisplayAddCommand(USER_LIST,&Store);

            Store.eLED = LedColor(au8Input);
            Store.u32Time = OffTime(au8Input);
            Store.bOn = FALSE;
            LedDisplayAddCommand(USER_LIST,&Store);
            Index1=0;
            bClear=TRUE;
            u8ListSize+=1;
          }
          else
          {
            DebugPrintf("Invalid command:incorrect format. Please use L-ONTIME-OFFTIME\n\r");
            DebugPrintNumber(u8ListNumber);
            DebugPrintf(":");
            Index1=0;
            bClear=TRUE;                   
          }
        }
      }
    }   
  }
  if(u8Press==2)
  { 
    u32TimeCount++;
    if(bPrint)
    {
      DebugPrintf("\n\rCommand entry completeCommand count line is:");
      DebugPrintNumber(u8ListSize);
      DebugPrintf("\n\r");
      DebugPrintf("LED  ON TIME   OFF TIME\n\r-----------------------   \n\r");
      bPrint=FALSE; 
    }
    if(u32TimeCount==15&&Index2<u8ListSize)
    {
      LedDisplayPrintListLine(Index2);
      u32TimeCount=0;
      Index2++;
    }
    if(u32TimeCount==16)
    {
      DebugPrintf("\n\r");
      DebugPrintf(au8bProgramInterface);
      bChoose=TRUE;
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
