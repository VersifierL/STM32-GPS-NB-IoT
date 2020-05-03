#include "flash.h"
uint32_t writeFlashData = 0x55555555;
uint32_t addr = 0x0801F000;

//FLASHд�����
void writeFlashTest(void)
{
	//1����FLASH
  HAL_FLASH_Unlock();
	
	//2����FLASH
	//��ʼ��FLASH_EraseInitTypeDef
	FLASH_EraseInitTypeDef flash;
	flash.TypeErase = FLASH_TYPEERASE_PAGES;
	flash.PageAddress = addr;
	flash.NbPages = 1;
	//���ò�����־λPageError
	uint32_t PageError = 0;
	//���ò�������
	HAL_FLASHEx_Erase(&flash, &PageError);

	//3��дFLASH
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, writeFlashData);
	
	//��סFLASH
  HAL_FLASH_Lock();
}

//FLASH��ȡ���ݲ���
uint32_t printFlashTest(void)
{
  uint32_t temp = *(__IO uint32_t*)(addr);
	return temp;
}
