/**
  ******************************************************************************
  * @file    audio_mic_node.c
  * @author  MCD Application Team 
  * @brief   mic node implementation.
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
#include "audio_mic_node.h"
#include "usb_audio.h"

/* Private defines -----------------------------------------------------------*/
#define MEMS_VOLUME_MIC_RES_DB_256     256 /* 1 db 1 * 256 = 256*/ 
#define MEMS_VOLUME_MIC_MAX_DB_256     8192 /* 32db == 32*256 = 8192*/
#define MEMS_VOLUME_MIC_MIN_DB_256     -8192 /* -32db == -32*256 = -8192*/
#define MIC_CMD_STOP  1
#define MIC_CMD_EXIT  2
#define MIC_CMD_CHANGE_FREQUENCE  4

 /* #define DEBUG_MIC_NODE 1 define if debug required*/
#ifdef DEBUG_MIC_NODE
#define MIC_DEBUG_BUFFER_SIZE 1000
#endif /* DEBUG_MIC_NODE */

/* Private macros -------------------------------------------------------------*/
#define VOLUME_DB_256_TO_PERCENT(volume_db_256) ((uint8_t)((((int)(volume_db_256) - MEMS_VOLUME_MIC_MIN_DB_256)*100)/\
                                                          (MEMS_VOLUME_MIC_MAX_DB_256 - MEMS_VOLUME_MIC_MIN_DB_256)))

/* Private typedef -----------------------------------------------------------*/
#ifdef DEBUG_MIC_NODE
typedef struct
{
  uint32_t time;
  uint16_t write;
  uint16_t read;
} AUDIO_MicDebugStats;
#endif /* DEBUG_MIC_NODE*/

/* externals variables ---------------------------------------------------------*/
#ifdef DEBUG_MIC_NODE
extern __IO uint32_t uwTick;
#endif /* DEBUG_MIC_NODE*/

#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
extern I2S_HandleTypeDef haudio_in_i2s;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/

/* Private function prototypes -----------------------------------------------*/
/* list of Mic Callbacks */
static int8_t  AUDIO_MicDeInit(uint32_t node_handle);
static int8_t  AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer,  uint32_t node_handle);
static int8_t  AUDIO_MicStop( uint32_t node_handle);
static int8_t  AUDIO_MicChangeFrequency( uint32_t node_handle);
static int8_t  AUDIO_MicMute(uint16_t channel_number,  uint8_t mute, uint32_t node_handle);
static int8_t  AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256, uint32_t node_handle);
static int8_t  AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle);
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
static int8_t  AUDIO_MicStartReadCount( uint32_t node_handle);
static uint16_t AUDIO_MicGetLastReadCount( uint32_t node_handle);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
static void AUDIO_MicFillDataToBuffer(uint32_t pdm_offset);
#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)
static void AUDIO_DoPadding(uint8_t* src,  uint8_t *dest,  int size);
#endif /* ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)  */
 
/* Private variables ---------------------------------------------------------*/ 
static AUDIO_MicNode_t *AUDIO_MicHandler = 0;
#ifdef DEBUG_MIC_NODE
static AUDIO_MicDebugStats mic_stats[MIC_DEBUG_BUFFER_SIZE];
static  int AUDIO_MicStatsCount = 0;
static  int AUDIO_MicStatsCounter = 0;
#endif /* DEBUG_MIC_NODE*/

/* exported functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_MicInit
  *         Initializes the audio mic node 
  * @param  audio_description:   audio parameters
  * @param  session_handle:     session handle
  * @param  node_handle:        mic node handle must be allocated
  * @retval 0 if no error
  */
 int8_t  AUDIO_MEMS_MicInit(AUDIO_Description_t* audio_description,  AUDIO_Session_t* session_handle,
                       uint32_t node_handle)
{
  return 0;
}
/**
  * @brief  BSP_AUDIO_IN_HalfTransfer_CallBack
            Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
}

/**
  * @brief  BSP_AUDIO_IN_TransferComplete_CallBack
  *         Manages the DMA Transfer complete interrupt    
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
}

/* private functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_MicDeInit
  *         De-Initializes the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicDeInit(uint32_t node_handle)
{
  
    return 0;
}

/**
  * @brief  AUDIO_MicStart
  *         Start the audio mic node 
  * @param  buffer:      Audio data buffer       
  * @param  node_handle: mic node handle must be initialized
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer ,  uint32_t node_handle)
{
    return 0;
}

/**
  * @brief  AUDIO_MicStop
  *         stop the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicStop( uint32_t node_handle)
{
    
    return 0;
}

/**
  * @brief  AUDIO_MicChangeFrequency
  *         change mic frequency 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicChangeFrequency( uint32_t node_handle)
{
    
    return 0;
}

/**
  * @brief  AUDIO_MicMute
  *         mute  mic 
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicMute(uint16_t channel_number,  uint8_t mute , uint32_t node_handle)
{
  
  return 0;
}

/**
  * @brief  AUDIO_MicSetVolume
  *         set  mic volume 
  * @param  channel_number : which channel to set the volume (not used currently)
  * @param  volume_db_256  : description is missing
  * @param  node_handle      mic  node handle
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle)
{
  
  return 0;
}
/**
  * @brief  AUDIO_MicGetVolumeDefaultsValues
  *         get  mic volume max, min & resolution value  in db
  * @param  vol_max            
  * @param  vol_min            
  * @param  vol_res             
  * @param  node_handle         
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle)
{
  return 0;
}


/**
  * @brief  AUDIO_MicFillDataToBuffer
   *        convert data to pdm then check if padding needed
  * @param  pdm_offset        
  * @retval None
  */

static void AUDIO_MicFillDataToBuffer(uint32_t pdm_offset)
{
  
}
/**
  * @brief  AUDIO_DoPadding
  *        Pad 24bit  sample to 32 sample by adding zeros .
  * @param  src(IN)                
  * @param  dest(OUT)               
  * @param  size(IN)                
  * @retval None
  */
#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)
 static void AUDIO_DoPadding(uint8_t* src,  uint8_t *dest ,  int size)
 {
 }
#endif /* #if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)*/

#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
/**
  * @brief  AUDIO_MicStartReadCount
  *         Start a count to compute read bytes from mic each ms 
  * @param  node_handle: mic node handle must be started
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicStartReadCount( uint32_t node_handle)
{
   return 0;
} 
/**
  * @brief  AUDIO_MicGetLastReadCount
  *         read the number of bytes that was read in the last 1ms 
  * @param  node_handle(IN): mic node handle must be started
  * @retval number of read bytes, 0 if an error
  */    
static uint16_t  AUDIO_MicGetLastReadCount( uint32_t node_handle)
{
    return 0;
}
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
