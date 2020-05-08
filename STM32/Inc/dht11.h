#ifndef __DHT11_H
#define __DHT11_H 
#include  "main.h"
#include  "delay.h"
#define	DHT11_DQ_IN  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) //���ݶ˿�	PB8
uint8_t DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
void DHT11_IO_IN(void);
void DHT11_Rst(void);
#endif















