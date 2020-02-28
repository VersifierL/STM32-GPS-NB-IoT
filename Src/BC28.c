#include "BC28.h"
#include <stdlib.h>

/*��������Ԫ��*/
#define PRODUCEKEY "a1K7KsoyAYX"        //�޸Ĳ�Ʒ��Կ
#define DEVICENAME "A001"           //�޸��豸����
#define DEVICESECRET "EdR2N2b75BLSRg8pcMIzBoiwg9XpMFgh" //�޸��豸��Կ
#define SUBSCRIBE "/a1K7KsoyAYX/A001/user/get"      //�������
#define ISSUE "/sys/a1K7KsoyAYX/A001/thing/event/property/post"


char *strx;
char atstr[BUFLEN];//����������
int err = 0;//ȫ�ֱ���

//�������ݵȴ���Ӧ���ط����ƣ�
int send_data_ack(char *cmd, char *ack, __IO uint32_t nTime)
{			
		clear_nbiot_buffer();
		HAL_UART_Transmit(&huart3, (uint8_t *)cmd, strlen(cmd), 100);  //����ATָ?
		HAL_Delay(nTime);				//�ȴ�
		printf("%s",USART_RX_NBIOT_BUF);
		strx = strstr((const char*)USART_RX_NBIOT_BUF,(const char*)ack);//�ж�
		 clear_nbiot_buffer();	//���bufferr
		if(strx != NULL)//�ж�
		{
			printf("success\n");	
			return 0;		
		}
		else
		{
			printf("error\n");
			return 1;
		}
}
//�������ݵȴ���Ӧ��һֱ�ȴ���
int send_data_wait_ack(char *cmd, char *ack, __IO uint32_t nTime)
{			
		clear_nbiot_buffer();
		HAL_UART_Transmit(&huart3, (uint8_t *)cmd, strlen(cmd), 100);  //����ATָ?
		HAL_Delay(nTime);				//�ȴ�
		printf("%s",USART_RX_NBIOT_BUF);
		strx = strstr((const char*)USART_RX_NBIOT_BUF,(const char*)ack);//�ж�
		while(strx==NULL)
    {
        strx = strstr((const char*)USART_RX_NBIOT_BUF,(const char*)ack);//�ж�
    }
		clear_nbiot_buffer();	//���bufferr

}
//��������
void send_data(char *cmd, __IO uint32_t nTime)
{
		HAL_UART_Transmit(&huart3, (uint8_t*)cmd, strlen(cmd), 100);   //����ATָ��
		HAL_Delay(nTime);				//�ȴ�
		clear_nbiot_buffer();	//���buffer
}
//��Ϊ��
int BC28_Init()
{
		while(send_data_ack("AT\r\n","OK",300)); 
		while(send_data_ack("AT+CGSN=1\r\n","OK",300)); 
		while(send_data_ack("AT+CMEE=1\r\n","OK",300));//�������ֵ
		while(!send_data_ack("AT+CIMI\r\n","ERROR",300)); //ֻҪ����ERROR������������
		while(send_data_ack("AT+CGATT=1\r\n","OK",300)); //PDP����
		while(send_data_ack("AT+CGATT?\r\n","+CGATT:1",300)); //����ɹ���ȡIP��ַ
		while(send_data_ack("AT+NCDP=49.4.85.232\r\n","OK",300));
		while(send_data_ack("AT+CGPADDR\r\n","OK",300));
		return err;
}

void BC28_Senddata(uint8_t* len,uint8_t *data)
{	
		memset(atstr,0,BUFLEN);
		uint8_t data_temp[20]={'0','0','0','1'};
		strncat(data_temp, data,12);
		sprintf(atstr,"AT+NMGS=%s,%s\r\n",len,data_temp);
		send_data_ack(atstr,"OK",300); 
}
//UDP
int BC28_UDP_Init()
{
		while(send_data_ack("AT\r\n","OK",300)); 
		while(send_data_ack("AT+CGSN=1\r\n","OK",300)); 
		while(send_data_ack("AT+CMEE=1\r\n","OK",300));//�������ֵ
		while(!send_data_ack("AT+CIMI\r\n","ERROR",300)); //ֻҪ����ERROR������������
		while(send_data_ack("AT+CGATT=1\r\n","OK",300)); //PDP����
		while(send_data_ack("AT+CGATT?\r\n","+CGATT:1",300)); //����ɹ���ȡIP��ַ
		send_data_ack("AT+NSOCL=0\r\n","OK",300);//��socket
		while(send_data_ack("AT+NSOCR=DGRAM,17,8888,1\r\n","OK",300)); //����һ��udp socket
		return err;
}
void BC28_UDP_Senddata(uint8_t* len,uint8_t *data)
{	
		memset(atstr,0,BUFLEN);
	  sprintf(atstr,"AT+NSOST=0,47.116.9.52,8888,%s,%s\r\n",len,data);
		send_data_ack(atstr,"OK",300); 
}

//������ MQTT
int BC28_ALIYUN_Init()
{
		while(send_data_ack("AT\r\n","OK",300)); 
		while(send_data_ack("AT+CGSN=1\r\n","OK",300)); 
		while(send_data_ack("AT+CMEE=1\r\n","OK",300));//�������ֵ
		while(!send_data_ack("AT+CIMI\r\n","ERROR",300)); //ֻҪ����ERROR������������
		while(send_data_ack("AT+CGATT=1\r\n","OK",300)); //PDP����
		while(send_data_ack("AT+CGATT?\r\n","+CGATT:1",300)); //����ɹ���ȡIP��ַ
    send_data("AT+QMTCLOSE=0\r\n",300);//Disconnect a client from MQTT server
		send_data("AT+QMTDISC=0\r\n",300);//ɾ�����
		
		//���ÿ�ѡ���Ӱ����ƿ�ѡ��?
		memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
    printf("atstr = %s \r\n",atstr);
		send_data_wait_ack(atstr,"OK",300); 
	
		send_data_wait_ack("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n","+QMTOPEN: 0,0",300); //��¼������ƽ̨ 
		
		memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCONN=0,\"%s\"\r\n",DEVICENAME);
    printf("atstr = %s \r\n",atstr);
		send_data_wait_ack(atstr,"+QMTCONN: 0,0,0",300);//�������ӵ�������
		
		memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTSUB=0,1,\"%s\",0 \r\n",SUBSCRIBE);
    printf("atstr = %s \r\n",atstr);
		send_data_wait_ack(atstr,"+QMTSUB: 0,1,0,1",300);//����
		return err;
}

void BC28_ALIYUN_Senddata1(uint8_t *temp_data, uint8_t *humi_data, uint8_t *voltage_data)
{	
	char buf5[]={0x1A,0x0D,0x0A};
    memset(atstr,0,BUFLEN); //������������
    sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);
    printf("atstr = %s \r\n",atstr);
    send_data(atstr,300);
		
    memset(atstr,0,BUFLEN); //��������
		sprintf(atstr,"{params:{temp:%s,humi:%s,voltage:%s}}",temp_data,humi_data,voltage_data);
    printf("atstr = %s \r\n",atstr);
    send_data(atstr,300);
	
    HAL_Delay(30);
    HAL_UART_Transmit(&huart3, buf5, strlen(buf5), 30); 
}

void BC28_ALIYUN_Senddata2(uint8_t *latitude_data, uint8_t *longtitude_data)
{	
	char buf5[]={0x1A,0x0D,0x0A};
    memset(atstr,0,BUFLEN); //������������
    sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);
    printf("atstr = %s \r\n",atstr);
    send_data(atstr,300);
		
    memset(atstr,0,BUFLEN); //��������
		sprintf(atstr,"{params:{latitude:%s,longtitude:%s}}",latitude_data,longtitude_data);
    printf("atstr = %s \r\n",atstr);
    send_data(atstr,300);
	
    HAL_Delay(30);
    HAL_UART_Transmit(&huart3, buf5, strlen(buf5), 30); 
}


