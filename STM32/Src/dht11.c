#include "dht11.h"


void DHT11_IO_OUT (void){ //�˿ڱ�Ϊ���
	GPIO_InitTypeDef  GPIO_InitStructure; 	
  GPIO_InitStructure.Pin = GPIO_PIN_7; //ѡ��˿ںţ�0~15��all��                        
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //ѡ��IO�ӿڹ�����ʽ       
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT11_IO_IN (void){ //�˿ڱ�Ϊ����
	GPIO_InitTypeDef  GPIO_InitStructure; 	
  GPIO_InitStructure.Pin = GPIO_PIN_7; //ѡ��˿ںţ�0~15��all��                        
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT ; //ѡ��IO�ӿڹ�����ʽ       
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//��λDHT11
void DHT11_Rst(void)           
{                 
    DHT11_IO_OUT();         //SET OUTPUT
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_RESET);                 //����DQΪ0
    delay_ms(20);            //��ʱ18ms����
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_SET);                    //DQ=1
    delay_us(30);             //��ʱ20~40us
}
//���DHT11�Ƿ���� 1�����ڣ�0����
uint8_t DHT11_Check(void)            
{   
    uint8_t retry=0;
    DHT11_IO_IN();//SET INPUT
    while (DHT11_DQ_IN&&retry<100)//DHT11��ʱ40~80us
    {
        retry++;
        delay_us(3);// 1us
    };
    if(retry>=100)return 1;
    else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11��ʱ40~80us
    {
        retry++;
        delay_us(3);
    };
    if(retry>=100)return 1;
    return 0;
}
//��ȡ����λ����
uint8_t DHT11_Read_Bit(void)                          
{
    uint8_t retry=0;
    while (DHT11_DQ_IN&&retry<100)//DHT11��ʱ40~80us
    {
        retry++;
        delay_us(3);
    }
    retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11��ʱ40~80us
    {
        retry++;
        delay_us(3);
    }
    delay_us(50);//??40us
    if(DHT11_DQ_IN)
        return 1;
    else
        return 0;
}
//��ȡ�ֽ�����ֵ

uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
    for (i=0;i<8;i++)
    {
        dat<<=1;
        dat|=DHT11_Read_Bit();
    }
    return dat;
}
//��ȡ��ʪ������ֵ
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
    uint8_t buf[5];
    uint8_t i;
    DHT11_Rst();
    if(DHT11_Check()==0)
    {
        for(i=0;i<5;i++)//��ȡ40bit����
        {
            buf[i]=DHT11_Read_Byte();
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
        {
            *humi=buf[0];
            *temp=buf[2];
        }
    }else return 1;
    return 0;
}
//��ʼ��DHT11��ʪ�ȴ�����           
uint8_t DHT11_Init(void)
{          
    DHT11_Rst();  //��λHT11
    return DHT11_Check();//�鿴DHT11�ķ���
}

