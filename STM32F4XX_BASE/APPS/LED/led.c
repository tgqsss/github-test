#include "led.h" 
#include "delay.h"
#include "Types.h"

uint32 ledcnt=0;    //LED����
uint8  led_dis=0;   //LED����
	    
//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_0);//GPIOF9,F10���øߣ�����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
	GPIO_SetBits(GPIOD,GPIO_Pin_6);//
	
	//W5500_CS,W5500_nRST
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         //����
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_SetBits(GPIOA,GPIO_Pin_3);                     
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

//GPIO IO��ʼ��
void GPIO_InitAll(void){
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC, ENABLE);	
	
	//TCE1,SPI2 OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);             //��ʼ��GPIO
	
	TCE1_L();
	SPI2_CLK_L();
	SPI2_MOSI_L();
	
	//SPI2 IN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //��ͨ���ģʽ
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);             //��ʼ��GPIO
	
  //SPI3 OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //����
  GPIO_Init(GPIOC, &GPIO_InitStructure);             //��ʼ��GPIO
	
	SPI3_CLK_L();
	SPI3_MOSI_L();
	
	//SPI3 IN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //��ͨ���ģʽ
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //����
  GPIO_Init(GPIOC, &GPIO_InitStructure);             //��ʼ��GPIO
	
}


//LED��˸����
void LED_Run(void){
	
	 ledcnt++;
	  if(ledcnt>5000){
			ledcnt=0; led_dis=~led_dis;
		}
    if(led_dis==0){ LED_ON(); } else{ LED_OFF(); }
		
}
