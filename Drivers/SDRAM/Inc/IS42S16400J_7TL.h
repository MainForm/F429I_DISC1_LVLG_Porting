#ifndef __SDRAM_IS42S16400J_7TL_H__
#define __SDRAM_IS42S16400J_7TL_H__

#include "stdint.h"
#include "main.h"

/* Set the refresh rate counter */
/* (15.62 us x Freq) - 20 */
#define REFRESH_COUNT           ((uint32_t)1386)   /* SDRAM refresh counter */
#define SDRAM_TIMEOUT           ((uint32_t)0xFFFF)

#ifdef __cplusplus
 extern "C" {
#endif

void BSP_SDRAM_IS42S16400J_7TL_Init(SDRAM_HandleTypeDef *hsdram, uint32_t RefreshCount);

#ifdef __cplusplus
}
#endif

#endif /* __SDRAM_IS42S16400J_7TL_H__ */