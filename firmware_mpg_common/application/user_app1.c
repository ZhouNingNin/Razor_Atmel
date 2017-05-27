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
static bool JudgeString(u8 *au8Input_)
{
  u8 *p = au8Input_;
  bool First=FALSE;
  u8 au8LedNames[] = {'W','P','B','C','G','Y','O','R'};
	if(!p)
  {
		return FALSE;
	}
	else
  {
    for(u8 i=0;i<8;i++)
    {
      if(*p==au8LedNames[i])
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
			p++;
			if(*p != '-')
      {
				return FALSE;
			}
			else
      {
				p++;
				if(*p > '9' || *p < '0')
        {
					return FALSE;
				}
				else
        {
					while(*p <= '9' && *p >= '0')
          {
						p++;
					}
					if(*p != '-')
          {
						return FALSE;
					}
					else
          {
						p++;
						if(*p > '9' || *p < '0')
            {
							return FALSE;
						}
						else
            {
							while(*p <= '9' && *p >= '0')
              {
								p++;
							}
              if(*p=='\0')
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
    
static u32 OnTime(u8 *au8Input_)
{
  u32 u32num=0;
  u8 *p=au8Input_;
  
  while(*p<'0'||*p>'9')
  {
    p++;
  }
  while(*p>='0'&&*p<='9')
  {
    u32num*=10;
    u32num+=(*p-'0');
    p++;
  }
  return u32num;  
}

static u32 OffTime(u8 *au8Input_)
{
  u32 u32num=0;
  u8 *p=au8Input_;
  
  while(*p!='-')
  {
    p++;
  }
  p++;
  while(*p!='-')
  {
    p++;
  }
  p++;
  while(*p>='0'&&*p<='9')
  {
    u32num*=10;
    u32num+=(*p-'0');
    p++;
  }
  return u32num; 
}
 
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
  static u8 au8bProgramInterface[]="LED Programming Interface \n\rPress 1 to program LED command sequence\n\rPress 2 to show current USER program\n\r**************************************************\n\r";
  static u8 au8Press1[]="Enter commands as LED-ONLINE-OFFTIME and press Enter\n\rTime is in milliseconds,max 100 commands\n\rLED colors:R,O,Y,G,C,B,P,W\n\rExample: R-100-200 (Red on at 100ms and off at 200ms)\n\rPress Enter on blank line to end\n\r";
  static u8 au8Input[20];
  u8 au8Enter[2]={10,10};
  u8 au8Enter1[2]={10,10};
  static u8 Index1=0;
  static bool bClear=TRUE;
  static bool bChoose=TRUE;
  static u8 u8Iput=0;
  static u8 u8ListSize=0;
  static LedCommandType Store;
   
  if(bProgramInterface==TRUE)
  {
    DebugPrintf(au8bProgramInterface);
    bProgramInterface=FALSE;    
  }
  
  if(bClear==TRUE)
  {
    for(u8 i=0;i<20;i++)
    {
      au8Input[i]='\0';   
    }
    bClear=FALSE;
    
  }

  if(bChoose==TRUE)
  {
    DebugScanf(au8Enter);
    if(au8Enter[0]=='1')
    {
      LedDisplayStartList();
      u8Iput=1;
      DebugPrintf("\n\r");
      DebugPrintf(au8Press1);
      bChoose=FALSE;
    }
    if(au8Enter[0]=='2')
    {
      u8Iput=2;
      DebugPrintf("\n\r");
      bChoose=FALSE;      
    }
  }
  if(u8Iput==1)
  {    

    DebugScanf(au8Enter1);
    if(au8Enter1[0]!=10)
    {      
      if(au8Enter1[0]!='\r')
      {
        au8Input[Index1]=au8Enter1[0];
        Index1++;
      }
      else
      {
        DebugPrintf("\n");
        if(Index1==0)
        {
          u8Iput=2;
          bClear=TRUE; 
        }
        if(JudgeString(au8Input)==TRUE)
        {
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
          u8ListSize+=2;
        }
        else
        {
          Index1=0;
          bClear=TRUE;                   
        }

      }
    }   
  }
  if(u8Iput==2)
  {  
    LedDisplayPrintListLine(u8ListSize);

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
