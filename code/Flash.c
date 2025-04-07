/*
 * Flash.c
 *
 *  Created on: 2025年4月6日
 *      Author: zhuji
 */

#include "Flash.h"


#include "flash.h"

/*-------------------------------------------------------------------------------------------------------------------
  @brief     写入flash
  @param     null
  @return    null
  Sample     Write_Flash();
  @note      将各项参数写入flash
-------------------------------------------------------------------------------------------------------------------*/
void  Write_Flash(void)
{
    flash_union_buffer[0].float_type  =sptr1.P;
    flash_union_buffer[1].float_type  =sptr2.P;

    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);        // 向指定 Flash 扇区的页码写入缓冲区数据
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     读取flash
  @param     null
  @return    null
  Sample     Read_Flash();
  @note      将各项参数从flash中读取
-------------------------------------------------------------------------------------------------------------------*/
void  Read_Flash(void)
{
    flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // 将数据从 flash 读取到缓冲区

    sptr1.P       =flash_union_buffer[0].float_type;
    sptr2.P       =flash_union_buffer[1].float_type;


}
