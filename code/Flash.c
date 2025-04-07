/*
 * Flash.c
 *
 *  Created on: 2025��4��6��
 *      Author: zhuji
 */

#include "Flash.h"


#include "flash.h"

/*-------------------------------------------------------------------------------------------------------------------
  @brief     д��flash
  @param     null
  @return    null
  Sample     Write_Flash();
  @note      ���������д��flash
-------------------------------------------------------------------------------------------------------------------*/
void  Write_Flash(void)
{
    flash_union_buffer[0].float_type  =sptr1.P;
    flash_union_buffer[1].float_type  =sptr2.P;

    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);        // ��ָ�� Flash ������ҳ��д�뻺��������
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ��ȡflash
  @param     null
  @return    null
  Sample     Read_Flash();
  @note      �����������flash�ж�ȡ
-------------------------------------------------------------------------------------------------------------------*/
void  Read_Flash(void)
{
    flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // �����ݴ� flash ��ȡ��������

    sptr1.P       =flash_union_buffer[0].float_type;
    sptr2.P       =flash_union_buffer[1].float_type;


}
