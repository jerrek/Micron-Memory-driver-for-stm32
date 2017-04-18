#include "stm32f10x_gpio.h"


#include "define.h"

extern uint8_t MemoryError;


void ReinitMemsPinsForOuts (void);
void ReinitMemsPinsForIns (void);
volatile uint8_t MEMReadData (void);
void MEMWriteData (volatile uint8_t byte);
void MEMWriteAdress (volatile uint8_t byte);
void MEMWriteCommand (volatile uint8_t byte);
uint8_t MEM_Ready_Busy (void);
void MEMRead (uint8_t status);
void MEMWrite (uint8_t status);
void MEMAddress (uint8_t status);
void MEMCommand(uint8_t status);
void MEMChipSelect (uint8_t status);
void MEMWriteProtect (uint8_t status);
uint8_t ReadStatus(void);
void ReadManufacturID( uint8_t *data);
void MEMReset(void);
uint8_t MEMInit(void);
void MEMCycleWriteData (volatile uint8_t *byte, uint8_t *adr);
void MEMCycleReadData (uint8_t reset, uint8_t *adr,  uint8_t *data);
void MEMEraseData (uint8_t reset, uint8_t *adr);

void ReinitMemsPinsForOuts (void)//настройте номера выводов сами
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ; // IO7 | IO6 |IO5 | IO4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//уточнить
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   =   GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ; // IO3 | IO2 | IO1 | IO0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//уточнить
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 );
	GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

}

void ReinitMemsPinsForIns (void)//настройте номера выводов сами
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ; // IO7 | IO6 |IO5 | IO4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//уточнить
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   =   GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ; // IO3 | IO2 | IO1 | IO0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//уточнить
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//GPIO_ResetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_12 | GPIO_Pin_13);
	//GPIO_ResetBits(GPIOA, GPIO_Pin_11 |  GPIO_Pin_12 |  GPIO_Pin_15);

}

volatile uint8_t MEMReadData (void)// функция чтения одного байта
{
	volatile uint8_t data=0;
	uint8_t data_correct=0;
	uint8_t confirm=0;
	uint8_t c=0;
	uint8_t i=0;


	ReinitMemsPinsForIns ();
    //while(MEM_Ready_Busy())
	delay(MEMTIME);

	if(MEM_Ready_Busy()==MEMReady)
	{
		  MEMChipSelect(ENABLE);

		  MEMRead(ENABLE);
			for(i=0;i<10;i++)
			{
			  if((GPIOC->IDR & GPIO_Pin_9))
			  {
				   data=(data|0x01);
			  }
			  else
			  {
				   data=(data|0x00);
			  }

			  if((GPIOC->IDR & GPIO_Pin_8))
			  {
				   data=(data|0x02);
			  }
			  else
			  {
				   data=(data|0x00);
			  }

			  if((GPIOC->IDR & GPIO_Pin_7))
			  {
				   data=(data|0x04);
			  }
			  else
			  {
				   data=(data|0x00);
			  }

			  if((GPIOC->IDR & GPIO_Pin_6))
			  {
				   data=(data|0x08);
			  }
			  else
			  {
				   data=(data|0x00);
			  }

			  if((GPIOB->IDR & GPIO_Pin_15))
			  {
				   data=(data|0x10);
			  }
			  else
			  {
				   data=(data|0x00);
			  }

			  if((GPIOB->IDR & GPIO_Pin_14))
			  {
				   data=(data|0x20);
			  }
			  else
			  {
				   data=(data|0x00);
			  }

			  if((GPIOB->IDR & GPIO_Pin_13))
			  {
				   data=(data|0x40);
			  }
			  else
			  {
				   data=(data|0x00);
			  }

			  if((GPIOB->IDR & GPIO_Pin_12))
			  {
				   data=(data|0x80);
			  }
			  else
			  {
				   data=(data|0x00);
			  }
			  if(c>0)
			  {
				if(data_correct==data)
				{
					confirm++;
					if(i>=9)
					{
						if(confirm>=8)
						{
							break;
						}
						else {
							i=0;
						}
					}
				}
			  }
			  else
			  {
				  data_correct=data;
				  c=1;
			  }
			}
		  MEMRead(DISABLE);

		  MEMChipSelect(DISABLE);

	}

	//MemLedStatusChange();

	  return data_correct;

}

void MEMWriteData (volatile uint8_t byte)// функция записи одного байта данных
{
	ReinitMemsPinsForOuts();

	volatile uint8_t shift=0;
	volatile uint16_t i=0;

	delay(MEMTIME);

	 for(i=0;i<10000;i++)
	  if(MEM_Ready_Busy())
	   {
		 MEMChipSelect(ENABLE);

		 MEMRead (DISABLE);

		 MEMCommand(DISABLE);

		 MEMAddress(DISABLE);

		 MEMWrite(ENABLE);

              if(((byte>>shift) & 0x01)==0x01)
     		  {
     				GPIO_SetBits(GPIOC, GPIO_Pin_9 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOC, GPIO_Pin_9 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOC, GPIO_Pin_8 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOC, GPIO_Pin_8 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOC, GPIO_Pin_7 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOC, GPIO_Pin_7 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOC, GPIO_Pin_6 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOC, GPIO_Pin_6 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOB, GPIO_Pin_15 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOB, GPIO_Pin_15 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOB, GPIO_Pin_14 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOB, GPIO_Pin_14 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOB, GPIO_Pin_13 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOB, GPIO_Pin_13 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOB, GPIO_Pin_12 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOB, GPIO_Pin_12 );
     		  }
     		  shift=0;
     		  break;
	   }

     		 delay(MEMTIME);
		 //MEMChipSelect(ENABLE);

		// MEMCommand(DISABLE);

		// MEMAddress(DISABLE);

		 MEMWrite(DISABLE);


			//MemLedStatusChange();

}

void MEMWriteAdress (volatile uint8_t byte)// функция записи одного байта адреса
{

	ReinitMemsPinsForOuts();

	volatile uint8_t shift=0;
	volatile uint16_t i=0;

	delay(MEMTIME);

	 for(i=0;i<10000;i++)
	  if(MEM_Ready_Busy())
	   {

		 MEMChipSelect(ENABLE);

		 MEMRead (DISABLE);

		 MEMCommand(DISABLE);

		 MEMAddress(ENABLE);

		 MEMWrite(ENABLE);



              if(((byte>>shift) & 0x01)==0x01)
     		  {
     				GPIO_SetBits(GPIOC, GPIO_Pin_9 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOC, GPIO_Pin_9 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOC, GPIO_Pin_8 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOC, GPIO_Pin_8 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOC, GPIO_Pin_7 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOC, GPIO_Pin_7 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOC, GPIO_Pin_6 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOC, GPIO_Pin_6 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOB, GPIO_Pin_15 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOB, GPIO_Pin_15 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOB, GPIO_Pin_14 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOB, GPIO_Pin_14 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOB, GPIO_Pin_13 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOB, GPIO_Pin_13 );
     		  }
     		  shift++;

     		  if(((byte>>shift) & 0x01)==0x01)
     		  {
     			    GPIO_SetBits(GPIOB, GPIO_Pin_12 );
     		  }
     		  else
     		  {
     			  GPIO_ResetBits(GPIOB, GPIO_Pin_12 );
     		  }
     		  shift=0;
     		 break;
	   }

     		 delay(MEMTIME);
		// MEMChipSelect(ENABLE);

		// MEMCommand(DISABLE);

          MEMWrite(DISABLE);

		 MEMAddress(DISABLE);// возможно лучше включать и выключать внешне

			//MemLedStatusChange();

}

void MEMWriteCommand (volatile uint8_t byte)// функция записи одного байта команды
{

	ReinitMemsPinsForOuts();

	volatile uint8_t shift=0;
	volatile uint16_t i=0;

	delay(MEMTIME);

	 for(i=0;i<10000;i++)
	  if(MEM_Ready_Busy())
	   {
		 MEMCommand(ENABLE);

		 MEMWrite(ENABLE);

		 MEMChipSelect(ENABLE);

		 MEMRead (DISABLE);

		 MEMAddress(DISABLE);

		  if(((byte>>shift) & 0x01)==0x01)
			  {
					GPIO_SetBits(GPIOC, GPIO_Pin_9 );
			  }
			  else
			  {
				  GPIO_ResetBits(GPIOC, GPIO_Pin_9 );
			  }
			  shift++;

			  if(((byte>>shift) & 0x01)==0x01)
			  {
					GPIO_SetBits(GPIOC, GPIO_Pin_8 );
			  }
			  else
			  {
				  GPIO_ResetBits(GPIOC, GPIO_Pin_8 );
			  }
			  shift++;

			  if(((byte>>shift) & 0x01)==0x01)
			  {
					GPIO_SetBits(GPIOC, GPIO_Pin_7 );
			  }
			  else
			  {
				  GPIO_ResetBits(GPIOC, GPIO_Pin_7 );
			  }
			  shift++;

			  if(((byte>>shift) & 0x01)==0x01)
			  {
					GPIO_SetBits(GPIOC, GPIO_Pin_6 );
			  }
			  else
			  {
				  GPIO_ResetBits(GPIOC, GPIO_Pin_6 );
			  }
			  shift++;

			  if(((byte>>shift) & 0x01)==0x01)
			  {
					GPIO_SetBits(GPIOB, GPIO_Pin_15 );
			  }
			  else
			  {
				  GPIO_ResetBits(GPIOB, GPIO_Pin_15 );
			  }
			  shift++;

			  if(((byte>>shift) & 0x01)==0x01)
			  {
					GPIO_SetBits(GPIOB, GPIO_Pin_14 );
			  }
			  else
			  {
				  GPIO_ResetBits(GPIOB, GPIO_Pin_14 );
			  }
			  shift++;

			  if(((byte>>shift) & 0x01)==0x01)
			  {
					GPIO_SetBits(GPIOB, GPIO_Pin_13 );
			  }
			  else
			  {
				  GPIO_ResetBits(GPIOB, GPIO_Pin_13 );
			  }
			  shift++;

			  if(((byte>>shift) & 0x01)==0x01)
			  {
					GPIO_SetBits(GPIOB, GPIO_Pin_12 );
			  }
			  else
			  {
				  GPIO_ResetBits(GPIOB, GPIO_Pin_12 );
			  }
			  shift=0;
			  break;

	   }

		  delay(MEMTIME);

	     MEMWrite(DISABLE);

		 //MEMChipSelect(ENABLE);

		 //MEMAddress(DISABLE);

		 MEMCommand(DISABLE);

			//MemLedStatusChange();
}

uint8_t MEM_Ready_Busy (void)// хардварная проверка на занятость памяти
{
	uint8_t status=0;

	if((GPIOA->IDR & GPIO_Pin_7))
	{
		status=MEMReady;
	}
	else
	{
		status=MEMBusy;
	}

	return status;

}

void MEMRead (uint8_t status)// включения режима чтения
{
	if(status==ENABLE)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	}
	else if(status==DISABLE)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_4);
	}

    delay(MEMTIME);
}

void MEMWrite (uint8_t status)// включения режима записи данных
{
	if(status==ENABLE)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	}
	else if(status==DISABLE)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
	}

    delay(MEMTIME);
}

void MEMAddress (uint8_t status)// включения режима записи адреса
{
	if(status==ENABLE)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_2);
	}
	else if(status==DISABLE)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	}

    delay(MEMTIME);
}

void MEMCommand(uint8_t status)// включения режима записи команды
{
	if(status==ENABLE)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
	}
	else if(status==DISABLE)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	}

    delay(MEMTIME);
}

void MEMChipSelect (uint8_t status)// выбор устройства
{
	if(status==ENABLE)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	}
	else if(status==DISABLE)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
	}

    delay(MEMTIME);
}

void MEMWriteProtect (uint8_t status)// включения режима защиты от записи
{
	if(status==ENABLE)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	}
	else if(status==DISABLE)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
	}

    delay(MEMTIME);
}

uint8_t ReadStatus(void)// чтения байта статуса памяти
{
	uint8_t data=0;

	 MEMWriteCommand(0x70);
	 data=MEMReadData();

	 return data;
}

void ReadManufacturID( uint8_t *data)// чтения байтов номера партии
{
	uint8_t i=0;

	 MEMWriteCommand(0x90);
	 MEMWriteAdress(0x00);
	 for(i=0;i<5;i++)
	 data[i]=MEMReadData();
}

void MEMReset(void)// ресет памяти
{
	 MEMWriteCommand(0xFF);
	 delay(10000);
}

uint8_t MEMInit(void)// инициалазация памяти
{

	 volatile uint8_t status=0;

     if(MEM_Ready_Busy())
     {
	  MEMReset();
     }

	 status=ReadStatus();

	 if((status==0xE0))
	   return MEM_OK;
	 else
	   return MEM_BAD;

}

void MEMEraseData (uint8_t reset, uint8_t *adr)// стирание 1 блока
{

	MEMChipSelect(ENABLE);
	MEMWriteProtect(DISABLE);

		uint8_t i=0;
		uint8_t status=0;
		static uint8_t count=0;

		if(count>=255)
		{
		MemLedStatusChange();
		count=0;
		}
		else
		count++;

		if(reset==0xFF)
		{
			for(i=0;i<3;i++)
				adr[i]=0;
		}
		else
		{
				status = ReadStatus();
			   if(((status>>6) & 0x01)==1)
			   {


					MEMWriteCommand(MEM_ERASE_START);

					for(i=0;i<3;i++)
						MEMWriteAdress(adr[i]);

					MEMWriteCommand(MEM_ERASE_END);

					status = ReadStatus();
					if((status & 0x01)==0)
					{
						status=0xFF;
					}

			   }

		}

		MEMWriteProtect(ENABLE);
		MEMChipSelect(DISABLE);

}

void MEMCycleWriteData (volatile uint8_t *byte, uint8_t *adr)// цикличная запись в память
{

	MEMChipSelect(ENABLE);
	MEMWriteProtect(DISABLE);

	uint16_t i=0;
	uint16_t i1=0;
	uint16_t i2=0;
	uint16_t i3=0;

	uint8_t status=0;

	for(i=0;i<10000;i++)
  {

			status = ReadStatus();
		   if(((status>>6) & 0x01)==1)
		   {
			 for(i1=0;i1<10;i1++)
			  if(MEM_Ready_Busy())
			   {

					    MEMWriteCommand(MEM_WRITE_START);

						for(i2=0;i2<5;i2++)
							MEMWriteAdress(adr[i2]);

						for(i3=0;i3<MEM_PAGE_SIZE;i3++)
						MEMWriteData(byte[i3]);

						MEMWriteCommand(MEM_WRITE_END);

						status = ReadStatus();
						if((status & 0x01)==0)
						{
							status=0xFF;

							 break;
						}
						else{
							status=0x00;
						}

			   }
					 break;
           }
  }

	MEMWriteProtect(ENABLE);
	MEMChipSelect(DISABLE);

}


void MEMCycleReadData (uint8_t reset, uint8_t *adr,  uint8_t *data)// цикличная чтение из памяти
{
	MEMChipSelect(ENABLE);
	MEMWriteProtect(DISABLE);

	uint32_t i=0;
	uint32_t i1=0;
	uint32_t i2=0;

	uint8_t status=0;


	static uint8_t count=0;

	if(count>=255)
	{
	MemLedStatusChange();
	count=0;
	}
	else count++;


	//MemLedStatusChange();
	if(reset==0xFF)
	{
		for(i=0;i<5;i++)
			adr[i]=0;
	}
	else
	{

			for(i=0;i<10000;i++)
			  {

						status = ReadStatus();
					   if(((status>>6) & 0x01)==1)
					   {
							MEMWriteCommand(MEM_READ_START);

									for(i1=0;i1<5;i1++)
										MEMWriteAdress(adr[i1]);

									MEMWriteCommand(MEM_READ_END);

									for(i2=0;i2<MEM_PAGE_SIZE;i2++)
									data[i2]=MEMReadData();

									status = ReadStatus();
									if((status & 0x01)==0)
									{
										status=0xFF;
									}


						 break;

					   }
			  }
    }

	MEMWriteProtect(ENABLE);
	MEMChipSelect(DISABLE);

}
