/******************************************************************************
 ** File Name:                                                  *
 ** Author:         gang.tong                                              *
 ** DATE:           03/13/2008                                                *
 ** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 large block memory including malloc and free              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/13/2008     gang.tong        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <config.h>


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/ 
#define MTRQUEUE_INVALID_INDEX         0xFFFFFFFF//��Ч��������

typedef struct
{
    uint32 max_count;
	uint32 get_index;
	uint32 current_count;
	uint32 node_size;
	BOOLEAN busy;
    void *queue;    
}QUEUE_NODE_T;

typedef  BOOLEAN (*QUEUE_COMPARE_FUNC)(void *data_ptr1, void *data_ptr2);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   create a queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC QUEUE_NODE_T* QUEUE_InitQueue(uint32 max_queue_count, uint32 node_size);

/*****************************************************************************/
//  Description:   destory a queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void QUEUE_DestoryQueue(QUEUE_NODE_T *queue_ptr);

/*****************************************************************************/
//  Description:   enter to queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN QUEUE_InQueue(QUEUE_NODE_T *queue_ptr, void *data);

/*****************************************************************************/
//  Description:   out of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void QUEUE_OutQueue(QUEUE_NODE_T *queue_ptr);

/*****************************************************************************/
//  Description:   out of queue head
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void QUEUE_RemoveNode(QUEUE_NODE_T *queue_ptr, 
								uint32 index);

/*****************************************************************************/
//  Description:   get current node of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void* QUEUE_PeekQueue(QUEUE_NODE_T *queue_ptr, uint32 index);

/*****************************************************************************/
//  Description:   get current index of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC uint32 QUEUE_GetCurrentIndex(QUEUE_NODE_T *queue_ptr);

/*****************************************************************************/
//  Description:   get queue index by its node vlude
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC uint32 QUEUE_GetNodeIndex(QUEUE_NODE_T *queue_ptr, 
									void *data, 
									QUEUE_COMPARE_FUNC func);

/*****************************************************************************/
//  Description:   is queue empty
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN QUEUE_QueueIsEmpty(QUEUE_NODE_T *queue_ptr);

/*****************************************************************************/
//  Description:   is queue full
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN QUEUE_QueueIsFull(QUEUE_NODE_T *queue_ptr);

PUBLIC BOOLEAN QUEUE_EnQueue(QUEUE_NODE_T *queue_ptr, void *data);

PUBLIC void QUEUE_DeQueue(QUEUE_NODE_T *queue_ptr, void* data);

PUBLIC BOOLEAN QUEUE_CleanQueue(QUEUE_NODE_T *queue_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /* _MTR_QUEUE_H_ */ 
