/**************************************************************************
   File Name:       main.c
   Description:     WT588D test programme
   compiler:        Keil C51 9.02a
   Author:          LJW
   Created:         2011/09/02
   Modified:        NO
   Revision:        1.0
   MCU:             STC10F04
   Oscillator Crystal Frequency:    11.0592MHz
***************************************************************************/

/****************************************************/
/*                                                  */
/* Included files                                   */
/*                                                  */
/****************************************************/
#include "STC8051.H"            /* STC8051 definitions       */
#include <Delay.h>


/****************************************************/
/*                                                  */
/* �ܽŶ���                     */
/*                                                  */
/****************************************************/
/////////////////////////////////
sbit 	key1 = P3^7;
sbit 	key2 = P3^6;
sbit 	key3 = P2^4;
sbit 	key4 = P2^3;

sbit    LED_OUT   = P2^0;
sbit    FLAG_OUT   = P2^1;

//1Line_A
sbit 	P_DATA    = P0^0;			//�������ݴ���˿ڶ�Ӧ����оƬP03

//3Line_A
sbit 	P_DATA_3A = P0^0;			//�������ݴ���˿�
sbit 	CS_3A     = P0^1;	 		//����Ƭѡ�źŶ˿�
sbit 	CLK_3A    = P0^2;	 		//����ʱ�Ӷ˿�



//һ�ߣ����ߣ������й�
#define 	H  		1	//����1Ϊ�ߵ�ƽ
#define 	L 		0	//����0Ϊ�͵�ƽ

//////////////����/////////////////
//bit 	flag 	  = 1;
bit 	B_DATA;						//��������һλ
unsigned char  	SB_DATA   = 0;				//һ�ֽڹ������ݻ�����
unsigned char  	S_DATA    = 0x00;

/******************************************
   ����ɨ�躯��
*******************************************/


unsigned char 	key,key_zhi;

unsigned int ADJ_NUM = 6;
unsigned int S_time = 5;

/*-------------------------------------- 
;ģ������:Line_1A(void)
;��    ��:ʵ��һ�ߵ��ֽڵ�λ��ǰ����ͨ�ź���
;��    ��:��
;��    ��:0	   ����������
;-------------------------------------*/
void Line_1A(unsigned char dat)
{
	unsigned char i;
	
	
//********************************************************

	P_DATA = 0;

	Delay_1ms(5);   				//��������5ms

	B_DATA = dat&0X01;
	for(i=0;i<8;i++)
	{
		P_DATA = 1;				//�������ݴ����ߣ�׼����������
		if(B_DATA==0)
		{	/*��ʾ�߼���ƽ0*/
			Delay_10us(20);     // ��ʱ200us
			P_DATA = 0;
			Delay_10us(20);	    // ��ʱ600us
			Delay_10us(20);
			Delay_10us(20);	    
		}
		else
		{  /*��ʾ�߼���ƽ1*/
			Delay_10us(20);
			Delay_10us(20);
			Delay_10us(20);
			P_DATA = 0;
			Delay_10us(20);
		}
		dat = dat>>1;
		B_DATA = dat&0X01;
	}
	P_DATA = 1;					//���ߵ�ƽ
}

/*-------------------------------------- 
;ģ������:Line_3A(void)
;��    ��:ʵ�����ߵ��ֽڵ�λ��ǰ����ͨ�ź���������WT588D
;��    ��:��
;��    ��:0	   ����������
;-------------------------------------*/
void Line_3A(unsigned char dat)
{
	unsigned char i,key_copy = 0X00;
	P_DATA = 1;
	CLK_3A    = H;		 		//����
	
	CS_3A= L;							  //����Ƭѡ�ź�
	Delay_1ms(5);  						//5ms
	B_DATA = dat&0X01;
		
	for(i=0;i<8;i++)
	{
		CLK_3A    = L;					// ����
		P_DATA_3A = B_DATA;			//��������һλ
		
		Delay_10us(15);	 				//��ʱ150us
		CLK_3A    = H;		 		//����
		Delay_10us(15);  		 		//��ʱ150us
							
		dat = dat>>1;	 			
		B_DATA = dat&0X01;	  
	}
	P_DATA_3A  = 1;						//���ߵ�ƽ
	CS_3A  = H;		   
	CLK_3A = H;				 
}

/*************************************************
  Function:       Main
  Description:    Main Function
  Input:          void
  Output:         NULL
  Return:         void
*************************************************/
void main()
{
	//P0����Ϊ��©�������ʽ
	P0M0 = 0xff;
	P0M1 = 0xff;
	P0 = 0xFF;

	//P0����Ϊ���������ʽ
	P2M0 = 0x01;

	P4SW = (1<<6);	//P4.6ΪIO��
	P_DATA  = 1;			//�������ݶ˿�
	P_DATA_3A  = 1;						//���ߵ�ƽ
	CS_3A  = H;		   
	CLK_3A = H;

	while(1)
	{		
		if(key1 == 0)
		{
			Delay_1ms(30);
			if(key1 == 0)
			{
				LED_OUT = !LED_OUT;
				ADJ_NUM ++;
				//Line_1A(ADJ_NUM);
				Line_3A(ADJ_NUM);
			}
			while(key1 == 0);
		}
		if(key2 == 0)
		{
			Delay_1ms(30);
			if(key2 == 0)
			{
				LED_OUT = !LED_OUT;
				ADJ_NUM --;
				//Line_1A(ADJ_NUM);
				Line_3A(ADJ_NUM);
			}
			while(key2 == 0);
		}
		Delay_10ms(150);
	}

}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
