/**
  ******************************************************************************
  * @file    audio_speaker_node.c
  * @author  MCD Application Team 
  * @brief   speaker node implementation.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019  STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "usbd_audio.h"
#include "audio_speaker_node.h"
#include "usb_audio.h"

/* Private defines -----------------------------------------------------------*/
#define SPEAKER_CMD_STOP                1
#define SPEAKER_CMD_EXIT                2
#define SPEAKER_CMD_CHANGE_FREQUENCE    4
#define VOLUME_DB_256_TO_PERCENT(volume_db_256) ((uint8_t)((((int)(volume_db_256) - VOLUME_SPEAKER_MIN_DB_256)*100)/\
                                                          (VOLUME_SPEAKER_MAX_DB_256 - VOLUME_SPEAKER_MIN_DB_256)))

#if USB_AUDIO_CONFIG_PLAY_RES_BIT == 24
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K
#define AUDIO_SPEAKER_MAX_INJECTION_LENGTH(audio_desc)\
         (((audio_desc)->frequency == USB_AUDIO_CONFIG_FREQ_44_1_K)? AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc) + \
           (4*(audio_desc)->channels_count) : AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc))
#endif /* USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K */
#define AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc) AUDIO_MS_PACKET_SIZE((audio_desc)->frequency, (audio_desc)->channels_count, 4)
#else /* USB_AUDIO_CONFIG_PLAY_RES_BIT == 24  */
#define AUDIO_SPEAKER_MAX_INJECTION_LENGTH(audio_desc) \
         (((audio_desc)->frequency == USB_AUDIO_CONFIG_FREQ_44_1_K)?AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc) + \
           ((audio_desc)->resolution*(audio_desc)->channels_count) : AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc))
#define AUDIO_SPEAKER_INJECTION_LENGTH(audio_desc) AUDIO_MS_PACKET_SIZE((audio_desc)->frequency, (audio_desc)->channels_count, (audio_desc)->resolution)
#endif /* USB_AUDIO_CONFIG_PLAY_RES_BIT == 24  */
           
/* alt buffer max size */
#if USB_AUDIO_CONFIG_PLAY_RES_BIT == 24
#define SPEAKER_ALT_BUFFER_SIZE ((USB_AUDIO_CONFIG_PLAY_FREQ_MAX+999)/1000)*2*4*2
#else  /* USB_AUDIO_CONFIG_PLAY_RES_BIT == 24 */
#define SPEAKER_ALT_BUFFER_SIZE ((USB_AUDIO_CONFIG_PLAY_FREQ_MAX+999)/1000)*2*3*2        
#endif /*  USB_AUDIO_CONFIG_PLAY_RES_BIT == 24*/

//#define DEBUG_SPEAKER_NODE 1 /* define when debug is required*/
#ifdef DEBUG_SPEAKER_NODE
#define SPEAKER_DEBUG_BUFFER_SIZE 1000
#endif /*DEBUG_SPEAKER_NODE*/
 
/* Private function prototypes -----------------------------------------------*/
static int8_t  AUDIO_SpeakerDeInit(uint32_t node_handle);
static int8_t  AUDIO_SpeakerStart(AUDIO_CircularBuffer_t* buffer, uint32_t node_handle);
static int8_t  AUDIO_SpeakerStop( uint32_t node_handle);
static int8_t  AUDIO_SpeakerChangeFrequency( uint32_t node_handle);
static int8_t  AUDIO_SpeakerMute( uint16_t channel_number,  uint8_t mute , uint32_t node_handle);
static int8_t  AUDIO_SpeakerSetVolume( uint16_t channel_number,  int volume ,  uint32_t node_handle);
static void    AUDIO_SpeakerInitInjectionsParams( AUDIO_SpeakerNode_t* speaker);
#if USB_AUDIO_CONFIG_PLAY_RES_BIT == 24 
static void AUDIO_DoPadding_24_32(AUDIO_CircularBuffer_t *buff_src,  uint8_t *data_dest ,  int size);
#endif /* USB_AUDIO_CONFIG_PLAY_RES_BIT == 24   */
static int8_t  AUDIO_SpeakerStartReadCount( uint32_t node_handle);
static uint16_t AUDIO_SpeakerGetLastReadCount( uint32_t node_handle);

/* Private typedef -----------------------------------------------------------*/
#ifdef DEBUG_SPEAKER_NODE
typedef struct
{
  uint32_t time;
  uint16_t injection_size;
  uint16_t read;
  uint16_t dump;
  uint8_t* data;
} AUDIO_SpeakerNodeBufferStats_t;
#endif /* DEBUG_SPEAKER_NODE*/

/* Private macros ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
#ifdef DEBUG_SPEAKER_NODE
extern __IO uint32_t uwTick;
#endif /* DEBUG_SPEAKER_NODE*/

/* Private variables -----------------------------------------------------------*/
static AUDIO_SpeakerNode_t *AUDIO_SpeakerHandler = 0;
#ifdef DEBUG_SPEAKER_NODE
static AUDIO_SpeakerNodeBufferStats_t AUDIO_SpeakerDebugStats[SPEAKER_DEBUG_BUFFER_SIZE];
static  int AUDIO_SpeakerDebugStats_count =0;
#endif /* DEBUG_SPEAKER_NODE*/

/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  AUDIO_SpeakerInit
  *         Initializes the audio speaker node, set callbacks and start the codec. As no data are ready. The 
  *         SAI is feeded from the alternate buffer (filled by zeros)
  * @param  audio_description(IN): audio information
  * @param  session_handle(IN):   session handle
  * @param  node_handle(IN):      speaker node handle must be allocated
  * @retval 0 if no error
  */
 int8_t  AUDIO_SpeakerInit(AUDIO_Description_t* audio_description,  AUDIO_Session_t* session_handle, 
                           uint32_t node_handle)
{
  return 0;
}


/**
  * @brief  BSP_AUDIO_OUT_Error_CallBack
  *         Manages the DMA error event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  Error_Handler();
}

/**
  * @brief  BSP_AUDIO_OUT_TransferComplete_CallBack
  *         Manages the DMA full Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{

}

/**
  * @brief  BSP_AUDIO_OUT_HalfTransfer_CallBack
  *         This function is called when half of the requested buffer has been transferred.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
}
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_SpeakerDeInit
  *         De-Initializes the audio speaker node 
  * @param  node_handle(IN): speaker node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_SpeakerDeInit(uint32_t node_handle)
{
  return 0;
}

/**
  * @brief  AUDIO_SpeakerStart
  *         Start the audio speaker node 
  * @param  buffer(IN):     buffer to use while node is being started
  * @param  node_handle(IN): speaker node handle must be initialized
  * @retval 0 if no error
  */
static int8_t  AUDIO_SpeakerStart(AUDIO_CircularBuffer_t* buffer,  uint32_t node_handle)
{
  return 0;
}

 /**
  * @brief  AUDIO_SpeakerStop
  *         Stop speaker node. the speaker will be stopped after finalizing current packet transfer.
  * @param  node_handle(IN): speaker node handle must be Started
  * @retval 0 if no error
  */  
static int8_t  AUDIO_SpeakerStop( uint32_t node_handle)
{
  return 0;
}

 /**
  * @brief  AUDIO_SpeakerChangeFrequency
  *         change frequency then stop speaker node
  * @param  node_handle: speaker node handle must be Started
  * @retval 0 if no error
  */  
static int8_t  AUDIO_SpeakerChangeFrequency( uint32_t node_handle)
{
  return 0;
}

 /**
  * @brief  AUDIO_SpeakerInitInjectionsParams
  *         Stop speaker node
  * @param  speaker(IN): speaker node handle must be Started
  * @retval 0 if no error
  */
static void  AUDIO_SpeakerInitInjectionsParams( AUDIO_SpeakerNode_t* speaker)
{
  
}
 /**
  * @brief  AUDIO_SpeakerMute
  *         set Mute value to speaker
  * @param  channel_number(IN): channel number
* @param  mute(IN): mute value (0 : mute , 1 unmute)
  * @param  node_handle(IN): speaker node handle must be Started
  * @retval  : 0 if no error
  */ 
static int8_t  AUDIO_SpeakerMute( uint16_t channel_number,  uint8_t mute , uint32_t node_handle)
{
  return 0;
}

 /**
  * @brief  AUDIO_SpeakerSetVolume
  *         set Volume value to speaker
  * @param  channel_number(IN): channel number
  * @param  volume_db_256(IN):  volume value in db
  * @param  node_handle(IN):    speaker node handle must be Started
  * @retval 0 if no error
  */ 
static int8_t  AUDIO_SpeakerSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle)
{
  return 0;
}      

#if USE_AUDIO_TIMER_VOLUME_CTRL
/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
}
#endif /* USE_AUDIO_TIMER_VOLUME_CTRL */

#if USB_AUDIO_CONFIG_PLAY_RES_BIT == 24  
/**
  * @brief  AUDIO_DoPadding_24_32
  *         padding 24bit  sample to 32 sample by adding zeros .
  * @param  buff_src(IN):          
  * @param  data_dest(OUT):          
  * @param  size(IN):               
  * @retval None
  */
 static void AUDIO_DoPadding_24_32(AUDIO_CircularBuffer_t *buff_src,  uint8_t *data_dest ,  int size)
 {
 }
#endif /* USB_AUDIO_CONFIG_PLAY_RES_BIT == 24   */

 /**
  * @brief  AUDIO_SpeakerStartReadCount
  *         Start a counter of how much of byte has been read from the buffer(transmitted to SAI)
  * @param  node_handle: mic node handle must be started
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_SpeakerStartReadCount( uint32_t node_handle)
{
    return 0;    
}

 
 /**
  * @brief  AUDIO_SpeakerGetLastReadCount
  *         return the number of bytes have been read and reset the counter
  * @param  node_handle: speaker node handle must be started
  * @retval  :  number of read bytes , 0 if  an error
  */    

static uint16_t  AUDIO_SpeakerGetLastReadCount( uint32_t node_handle)
{
    return 0;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
