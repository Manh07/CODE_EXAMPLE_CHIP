/******************************************************************************
 ** File Name:      mtr_queue.c                                               *
 ** Author:         gang.tong                                              *
 ** DATE:           05/15/2008                                                *
 ** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 large block memory including malloc and free              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/15/2008     gang.tong        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "queue.h"
//#include <avr/string.h>
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   create a queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC QUEUE_NODE_T* QUEUE_InitQueue(uint32 max_queue_count, uint32 node_size)
{

	QUEUE_NODE_T *queue_ptr = (QUEUE_NODE_T *)malloc(sizeof(QUEUE_NODE_T));

	if(queue_ptr != NULL)
	{
		queue_ptr->max_count = max_queue_count;
		queue_ptr->node_size = node_size;
		queue_ptr->current_count = 0;
		queue_ptr->get_index = 0;
		queue_ptr->busy      = FALSE;
		queue_ptr->queue = malloc(max_queue_count*node_size); 
		if(queue_ptr->queue != NULL)
		{
			memset(queue_ptr->queue, 0x00, (max_queue_count*node_size));
		}
	}
	
	return queue_ptr;
	
}

/*****************************************************************************/
//  Description:   destory a queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void QUEUE_DestoryQueue(QUEUE_NODE_T *queue_ptr)
{

	if(queue_ptr != NULL)
	{
		if(queue_ptr->queue != NULL)
		{
			free(queue_ptr->queue);
			queue_ptr->queue = NULL;
		}
		free(queue_ptr);
		queue_ptr = NULL;
	}
}

/*****************************************************************************/
//  Description:   enter to queue tail
//	Global resource dependence: 
//  Author: trongkn
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN QUEUE_InQueue(QUEUE_NODE_T *queue_ptr, void *data)
{

	BOOLEAN result = FALSE;
	uint8*  buf_ptr = NULL;
	while(!(queue_ptr->busy))
	{
		break;
	}
	queue_ptr->busy = TRUE;
	if(queue_ptr != NULL && (queue_ptr->queue != NULL))
	{
        if(queue_ptr->current_count < queue_ptr->max_count)
		{
			buf_ptr = (uint8 *)queue_ptr->queue + (queue_ptr->node_size *queue_ptr->current_count);
			if(buf_ptr != NULL)//lint !e774
			{
				memcpy(buf_ptr, data, queue_ptr->node_size);
				
				
				result = TRUE;
				queue_ptr->current_count++;
			}
		}
	}
	queue_ptr->busy = FALSE;
	return result;
	
}

/*****************************************************************************/
//  Description:   
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN QUEUE_EnQueue(QUEUE_NODE_T *queue_ptr, void *data)
{

	BOOLEAN result = FALSE;
	uint8*  buf_ptr = NULL;
	uint32  put_index = 0;
	
	while(!(queue_ptr->busy))
	{
		break;
	}
	queue_ptr->busy = TRUE;
	if(queue_ptr != NULL && (queue_ptr->queue != NULL))
	{

		// caculate put index
		put_index = ( queue_ptr->get_index + queue_ptr->current_count ) % queue_ptr->max_count;

		// insert data to queue
		buf_ptr = (uint8 *)queue_ptr->queue + (queue_ptr->node_size * put_index);
		
		if(buf_ptr)
		{
			memcpy(buf_ptr, data, queue_ptr->node_size);
		//	while()
			result = TRUE;
		}
			
		// overwrite happend
		if( ( put_index == queue_ptr->get_index ) && (!QUEUE_QueueIsEmpty(queue_ptr)))
		{
			queue_ptr->get_index = (queue_ptr->get_index + 1) % queue_ptr->max_count;
		}

		// update number of record
		if(queue_ptr->current_count < queue_ptr->max_count)
		{
			queue_ptr->current_count++;	
		}
		
	}
	queue_ptr->busy = FALSE;
	return result;
	
}

/*****************************************************************************/
//  Description:   
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void QUEUE_DeQueue(QUEUE_NODE_T *queue_ptr, void* data)
{	
	
	while(!(queue_ptr->busy))
	{
		break;
	}
	queue_ptr->busy = TRUE;
	
	if(
		(queue_ptr != NULL) && 
		(queue_ptr->queue != NULL) &&
		(data)
	)
	{
		// queue not empty
        if(queue_ptr->current_count > 0)
		{
		
			memcpy(data,(uint8 *)queue_ptr->queue + queue_ptr->node_size * queue_ptr->get_index, queue_ptr->node_size);
		
			queue_ptr->current_count--;

			queue_ptr->get_index = ( queue_ptr->get_index + 1 ) % queue_ptr->max_count;


			
		}
	}
	queue_ptr->busy = FALSE;
}

/*****************************************************************************/
//  Description:   out of queue head
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void QUEUE_OutQueue(QUEUE_NODE_T *queue_ptr)
{
	if(queue_ptr != NULL && (queue_ptr->queue != NULL))
	{
        if(queue_ptr->current_count > 0)
		{
			queue_ptr->current_count--;
			memcpy(queue_ptr->queue, (uint8 *)queue_ptr->queue + queue_ptr->node_size, queue_ptr->node_size*queue_ptr->current_count);
		}
	}
}

/*****************************************************************************/
//  Description:   out of queue head
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void QUEUE_RemoveNode(QUEUE_NODE_T *queue_ptr, uint32 index)
{
	if(queue_ptr != NULL && (queue_ptr->queue != NULL))
	{
        if(index < queue_ptr->current_count)
		{
			queue_ptr->current_count--;
            if(index < queue_ptr->current_count)
            {
                memcpy((uint8 *)queue_ptr->queue + (queue_ptr->node_size*index), (uint8 *)queue_ptr->queue + (queue_ptr->node_size*(index+1)), queue_ptr->node_size*(queue_ptr->current_count - index));
            }
		}
	}
}

/*****************************************************************************/
//  Description:   get head node of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC void* QUEUE_PeekQueue(QUEUE_NODE_T *queue_ptr, uint32 index)
{

	uint8*  buf_ptr = NULL;
	if(queue_ptr != NULL && (queue_ptr->queue != NULL))
	{
        if(queue_ptr->current_count > 0)
		{
			buf_ptr = (uint8 *)queue_ptr->queue + (queue_ptr->node_size *(index));
		}
	}

	return (void*)buf_ptr;
}

/*****************************************************************************/
//  Description:   get current index of queue 
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC uint32 QUEUE_GetCurrentIndex(QUEUE_NODE_T *queue_ptr)
{

	uint32 index = MTRQUEUE_INVALID_INDEX;
	if(queue_ptr != NULL)
	{
        if(queue_ptr->current_count > 0)
		{
			index = queue_ptr->current_count -1;
		}
	}
	return index;
}

/*****************************************************************************/
//  Description:   get queue index by its node vlude
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC uint32 QUEUE_GetNodeIndex(QUEUE_NODE_T *queue_ptr, void *data, QUEUE_COMPARE_FUNC func)
{
	uint32 i = 0;
	uint32 index = MTRQUEUE_INVALID_INDEX;
	uint8*  buf_ptr = NULL;
	if(queue_ptr != NULL && (queue_ptr->queue != NULL) && (func != NULL))
	{
        for(i =0; i < queue_ptr->current_count; i++)
		{
			buf_ptr = (uint8 *)queue_ptr->queue + (queue_ptr->node_size *(i));
            if(func(data, buf_ptr))
			{
				index = i;
				break;
			}
		}
	}
	return index;
}

/*****************************************************************************/
//  Description:   is queue empty
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN QUEUE_QueueIsEmpty(QUEUE_NODE_T *queue_ptr)
{

	
	BOOLEAN result = TRUE;
	
	
	while(!(queue_ptr->busy))
	{
		break;
	}
	queue_ptr->busy = TRUE;
	
	if(queue_ptr != NULL)
	{
        if(queue_ptr->current_count > 0)
		{
			result = FALSE;
		}
	}
	queue_ptr->busy = FALSE;
	return result;
}

/*****************************************************************************/
//  Description:   is queue full
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN QUEUE_QueueIsFull(QUEUE_NODE_T *queue_ptr)
{

	BOOLEAN result = FALSE;
	if(queue_ptr != NULL)
	{
        if(queue_ptr->current_count >= queue_ptr->max_count)
		{
			result = TRUE;
		}
	}
	return result;
}

/*****************************************************************************/
//  Description:   is queue full
//	Global resource dependence: 
//  Author: baokun.yin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN QUEUE_CleanQueue(QUEUE_NODE_T *queue_ptr)
{    
	BOOLEAN  ret = FALSE;
	
	if(queue_ptr != NULL)
	{
		if(queue_ptr->queue != NULL)
		{
			memset(queue_ptr->queue, 0x00, (queue_ptr->max_count*queue_ptr->node_size));
		}
		queue_ptr->current_count = 0;
		queue_ptr->get_index     = 0;
		ret = TRUE;
	}

	return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
