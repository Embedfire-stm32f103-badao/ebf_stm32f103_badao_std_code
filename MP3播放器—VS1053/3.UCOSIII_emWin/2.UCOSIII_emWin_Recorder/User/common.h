#ifndef __COMMON_H_
#define __COMMON_H_

#include  <stdint.h>
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
/**************************************************************************************
*
*       Defines
*
***************************************************************************************
*/
#define COMDATA_SIZE			0x1000	//4096B

/*********************************************************************
*
*       data
*
**********************************************************************
*/
extern uint8_t comdata[COMDATA_SIZE];
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void com_data2null(uint8_t *data,uint32_t length);

void com_gbk2utf8(const char *src, char *str);
void com_createdir(const char *dir);
#endif //end of __COMMON_H_
