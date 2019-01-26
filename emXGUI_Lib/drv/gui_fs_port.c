/**
  *********************************************************************
  * @file    gui_fs_port.c
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   �ļ�ϵͳ�ӿڣ���Ҫʵ��FileSystem_Init����
  *********************************************************************
  * @attention
  * ����    :www.emXGUI.com
  *
  **********************************************************************
  */ 
#include "gui_fs_port.h"
#include "ff.h"

	/* FatFs�ļ�ϵͳ���� */
static FATFS fs;									

void FileSystem_Test(void);

/**
	*********************************************************************************************************
	* Description : ��ʼ��BL8782 WiFiģ��ʹ�����ţ�������WiFiģ��
	* Argument(s) : none.
	* Return(s)   : none.
	*********************************************************************************************************
	*/
static void BL8782_PDN_INIT(void)
{
  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOG,GPIO_Pin_9);  //����WiFiģ��
}

/**
  * @brief  �ļ�ϵͳ��ʼ��
  * @param  ��
  * @retval TRUE  FALSE
  */
BOOL FileSystem_Init(void)
{ 
  /* �ļ�������� */
  FRESULT res_sd;               

	/* �������������ʹ��rtt �� printf��ӡ
		 ֻ��ʹ��printf	*/
	
	/* ���жϣ�������ӻ�����������board.c�н��г�ʼ��
		 �������жϣ��������г�ʼ����ʱ������Ҫ���ж� */
//	rt_base_t level = 0;
//	rt_hw_interrupt_enable(level);	

  /* ����WiFiģ�� */
	BL8782_PDN_INIT();

	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	res_sd = f_mount(&fs,"0:",1);
	
	if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("��SD����û���ļ�ϵͳ...\r\n");
//    /* ��ʽ�� */
//		res_sd=f_mkfs("0:",0,0);							
//		
//		if(res_sd == FR_OK)
//		{
//			printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
//      /* ��ʽ������ȡ������ */
//			res_sd = f_mount(NULL,"0:",1);			
//      /* ���¹���	*/			
//			res_sd = f_mount(&fs,"0:",1);
//		}
//		else
//		{
////			LED_RED;
//			printf("������ʽ��ʧ�ܡ�����\r\n");
////			while(1);
//		}
    return FALSE;
	}
  else if(res_sd!=FR_OK)
  {
    printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
    printf("��������ԭ��û�н���SD����\r\n");
    return FALSE;

  }
  else
  {
    printf("���ļ�ϵͳ���سɹ�\r\n");
    
#if 0
    /* �ļ�ϵͳ���� */
    FileSystem_Test();
#endif 
    
    /* ���Խ���unicode����ת����
      ��ʹ��extern_cc936ʱ���ɲ����Ƿ����cc936��Դ*/
    ff_convert('a',1);
    
    return TRUE;
  }
	
	/* �ر��жϣ�������ӻ�����������board.c�н��г�ʼ��
		 �����ر��жϣ��������г�ʼ����ʱ������Ҫ�ر��ж� */
//		rt_hw_interrupt_disable();
}

#if 1
FIL fnew;													/* �ļ����� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32 F429������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  

/**
  * @brief  �ļ�ϵͳ��д����
  * @param  ��
  * @retval TRUE  FALSE
  */
void FileSystem_Test(void)
{
  /* �ļ�������� */
  FRESULT res_sd;               

	/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
	/* ���ļ�������ļ��������򴴽��� */
	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
	res_sd = f_open(&fnew, "0:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_sd == FR_OK )
	{
		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
    /* ��ָ���洢������д�뵽�ļ��� */
		res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_sd==FR_OK)
    {
      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
    }    
		/* ���ٶ�д���ر��ļ� */
    f_close(&fnew);
	}
	else
	{	
//		LED_RED;
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_sd = f_open(&fnew, "0:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{
//		LED_GREEN;
		printf("�����ļ��ɹ���\r\n");
		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_sd==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
    }	
		/* ���ٶ�д���ر��ļ� */
		f_close(&fnew);	
	}
	else
	{
//		LED_RED;
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
//	/* ���ٶ�д���ر��ļ� */
//	f_close(&fnew);	
  
//	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
//	f_mount(NULL,"0:",1);
	
}
#endif
/********************************END OF FILE****************************/
