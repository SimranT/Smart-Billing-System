
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdio.h>


#include <string.h>


#include "lib/sim300/sim300.h"
#include "lib/usart/usart.h"
#include "lib/calc/IncFile1.h"



void Halt();
void Reply(const char *num,uint8_t stat);

int main(void)
{
	
	
	//Initialize SIM300 module
	
	int8_t r= SIM300Init();
	
	_delay_ms(1000);
	
	//Check the status of initialization
	switch(r)
	{
		case SIM300_OK:
		
		break;
		case SIM300_TIMEOUT:
		
		Halt();
		case SIM300_INVALID_RESPONSE:
		
		Halt();
		case SIM300_FAIL:
		
		Halt();
		default:
		
		Halt();
	}
	
	_delay_ms(1000);
	
	//Set message format to text
	r= SIM300SetTextMode();
	
	
	
	_delay_ms(1000);
	
	
	//Check Sim Card Presence
	
	
	_delay_ms(1000);
	
	r=SIM300IsSIMInserted();
	
	if (r==SIM300_SIM_NOT_PRESENT)
	{
		//Sim card is NOT present
		
		
		Halt();
	}
	else if(r==SIM300_TIMEOUT)
	{
		//Communication Error
		
		
		Halt();
	}
	else if(r==SIM300_SIM_PRESENT)
	{
		//Sim card present
		
		
		_delay_ms(1000);
	}
	
	//Network search
	
	
	uint8_t		nw_found=0;
	uint16_t	tries=0;
	uint8_t		x=0;
	
	while(!nw_found)
	{
		r=SIM300GetNetStat();
		
		if(r==SIM300_NW_SEARCHING)
		{
			x++;
			
			if(x==16) x=0;
			
			_delay_ms(50);
			
			tries++;
			
			if(tries==600)
			break;
		}
		else
		break;
		
	}
	
	if(r!=SIM300_NW_REGISTERED_HOME)
	{
		Halt();
	}
	
	
	_delay_ms(1000);
	
	
	//Send MSG
	

	uint8_t ref;


	r=SIM300SendMsg("+9779XXXXXXXX","check",&ref);//Change phone number to some valid value!

	
	_delay_ms(2000);

	
	
	
	
	
	//Wait for MSG
	uint8_t id;
	char oa[20];//Origin Address ()the number from where the msg came
	char ob[]="+9779.........";
	UFlushBuffer();
	
	while(1)
	{
		
		
		uint8_t x=0;
		
		
		while(SIM300WaitForMsg(&id)!=SIM300_OK)
		{
			if(x)
			{
				
				x=0;
			}
			else
			{
				
				x=1;
			}
		}
		
		
		
		_delay_ms(1000);
		
		
		//Now read the msg
		char msg[300];
		
		r=SIM300ReadMsg(id,msg,oa);
		
		if(r==SIM300_OK)
		{
			if(strcasecmp(msg,"check")==0)
			{
	
				Reply(ob,1);
			}
			
			else
			{
				//Invalid Choice
				Reply(ob,0);
			}
			
			_delay_ms(3000);
			
		}
		else
		{
			
			
			
			_delay_ms(3000);
			
		}
		
		//Finally delete the msg
		if (SIM300DeleteMsg(id)!=SIM300_OK)
		{
			
			
			_delay_ms(3000);
		}
		
	}
	
	
	Halt();
}

void Halt()
{
	while(1);
}

void Reply(const char *num,uint8_t stat)
{
	
	uint8_t ref;
	int8_t r;
	
	if(stat){
		uint8_t value=Readvalue(); //yesma timiharuko meter le read garne and adc hune function call gara..maile just simple calc maatra garya xu..maile xuttai library calc create garya xu..tyo vitra ko header file ra c file ma required changes haru gara
	    
	char reply_text[40];
	
	//Prepare reply
	sprintf_P(reply_text,PSTR("Calculated value: %03d"),value);
	r=SIM300SendMsg(num,reply_text,&ref);
	}	
	else
	r=SIM300SendMsg(num,"Invalid message...type check and send",&ref);
	
	if(r==SIM300_OK)
	{
		return;
	}
	else if(r==SIM300_TIMEOUT)
	{
		
		
		
		_delay_ms(3000);
	}
	else
	{
		
		_delay_ms(3000);
	}
	
	
}
