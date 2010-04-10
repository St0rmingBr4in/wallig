#include <string.h>
#include "stm32f10x.h"
#include "DRV_Quad.h"
#include "DRV_Quad_Cfg.h"

#define QUAD_DEVICE_COUNT ( sizeof(tQuad_DeviceCfg)/sizeof(DRV_Quad_Device_Cfg_t))

typedef struct
{
	GPIO_TypeDef* PORT;
	u16 Pin1;
	u16 Pin2;
	uint32_t GPIO_Remap;
	FunctionalState NewState;
} DRV_Quad_PioCfg;

typedef struct
{
	char *pcName;
	TIM_TypeDef *Timer;
	TIM_TimeBaseInitTypeDef TimerInit;
	void (*ClockCmd)(uint32_t RCC_APB1Periph, FunctionalState NewState);
	uint32_t  RCC_Periph;
	DRV_Quad_PioCfg PioCfg;
	uint16_t TIM_EncoderMode;
	uint16_t TIM_IC1Polarity;
	uint16_t TIM_IC2Polarity;


} DRV_Quad_Device_Cfg_t;

typedef struct
{
	DRV_Quad_Device_Cfg_t *pCfg;
	DRV_Quad_Device_State eState;
	int32_t iPosition;

}DRV_Quad_Device_Data_t;

static DRV_Quad_Device_Cfg_t tQuad_DeviceCfg[]= DRV_QUAD_INIT_DATA;
DRV_Quad_Device_Data_t tQuad_DeviceDataList[QUAD_DEVICE_COUNT];

void DRV_Quadm_GPIO_config( DRV_Quad_PioCfg *pGpioCFG);
void DRV_Quad_Init(void )
{
	int iDevicecount;

	for( iDevicecount = 0 ; iDevicecount < QUAD_DEVICE_COUNT ; iDevicecount++)
	{
		tQuad_DeviceDataList[iDevicecount].pCfg = (DRV_Quad_Device_Cfg_t*)&tQuad_DeviceCfg[iDevicecount];
		tQuad_DeviceDataList[iDevicecount].eState = Quad_Device_Close;
		tQuad_DeviceDataList[iDevicecount].iPosition=0;
	}

}

DRV_Quad_Error DRV_Quad_Open( char *pcName , DRV_Quad_Handle *pHandle)
{
	DRV_Quad_Error eError = Quad_Device_Not_Found;
	DRV_Quad_Device_Data_t *pQuadData;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	int iQuadIndex;

	for ( iQuadIndex = 0 ; iQuadIndex < QUAD_DEVICE_COUNT ; iQuadIndex++ )
	{
		if( !strcmp( pcName , tQuad_DeviceDataList[iQuadIndex].pCfg->pcName) )
		{
			pQuadData = &tQuad_DeviceDataList[iQuadIndex];
			eError=Quad_No_Error;
			break;
		}
	}
	if( eError==Quad_Device_Not_Found )
		return eError;
	*pHandle=(DRV_Quad_Handle) pQuadData;

	pQuadData->eState =Quad_Device_Open;

	/* Enable Timer clock*/
	pQuadData->pCfg->ClockCmd(pQuadData->pCfg->RCC_Periph,ENABLE);
	/* Timer base configuration */
	TIM_TimeBaseInit(pQuadData->pCfg->Timer , &pQuadData->pCfg->TimerInit);
	/* Pio configuration */
	DRV_Quadm_GPIO_config(&pQuadData->pCfg->PioCfg);
	/* Quad1 Mode configuration*/
	TIM_EncoderInterfaceConfig( pQuadData->pCfg->Timer , pQuadData->pCfg->TIM_EncoderMode , pQuadData->pCfg->TIM_IC1Polarity , pQuadData->pCfg->TIM_IC2Polarity);

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;//ICx_FILTER;
	TIM_ICInit(pQuadData->pCfg->Timer, &TIM_ICInitStructure);


	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_UpdateDisableConfig(pQuadData->pCfg->Timer,DISABLE);
	TIM_UpdateRequestConfig(pQuadData->pCfg->Timer,TIM_UpdateSource_Global);
	TIM_ITConfig(pQuadData->pCfg->Timer, TIM_IT_Update , ENABLE);
	TIM_Cmd(pQuadData->pCfg->Timer, ENABLE);

	return eError;

}

DRV_Quad_Error DRV_Quad_Position_Get( DRV_Quad_Handle Handle , int *piPosition)
{
	DRV_Quad_Error eError = Quad_Bad_Param;
	DRV_Quad_Device_Data_t *pQuadData=(DRV_Quad_Device_Data_t*)Handle;

	if (( Handle != NULL) && pQuadData->eState == Quad_Device_Open)
	{
		//pQuadData->iPosition += (uint32_t) pQuadData->pCfg->Timer->CNT;
		*piPosition= pQuadData->iPosition+pQuadData->pCfg->Timer->CNT;
	}
	return eError;
}



DRV_Quad_Error DRV_Quad_Position_Reset( DRV_Quad_Handle Handle )
{
	DRV_Quad_Error eError = Quad_Bad_Param;
	DRV_Quad_Device_Data_t *pQuadData=(DRV_Quad_Device_Data_t*)Handle;

	if (( Handle != NULL) && pQuadData->eState == Quad_Device_Open)
	{
		pQuadData->pCfg->Timer->CNT=0;
		pQuadData->iPosition = 0;
	}
	return eError;
}


void DRV_Quadm_GPIO_config( DRV_Quad_PioCfg *pGpioCFG)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure alternate output for GPIO*/
	GPIO_InitStructure.GPIO_Pin = pGpioCFG->Pin1|pGpioCFG->Pin2;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(pGpioCFG->PORT , &GPIO_InitStructure);
	GPIO_PinRemapConfig(pGpioCFG->GPIO_Remap , pGpioCFG->NewState);
}

//void DRV_Quad_OverFlowIrq( DRV_Quad_Handle Handle )
void TIM3_IRQHandler(void)
{
	//DRV_Quad_Device_Data_t *pQuadData=(DRV_Quad_Device_Data_t*)Handle;
	DRV_Quad_Device_Data_t *pQuadData=&tQuad_DeviceDataList[0];

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		if( pQuadData->pCfg->Timer->CR1 & 0x0010 )
			pQuadData->iPosition -= (int32_t)0xFFF0;
		else
			pQuadData->iPosition += (int32_t)0xFFF0;
	}

}