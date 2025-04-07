/*
 * Flash.h
 *
 *  Created on: 2025��4��6��
 *      Author: zhuji
 */

#ifndef CODE_FLASH_H_
#define CODE_FLASH_H_

#include "zf_common_headfile.h"

// ��κ궨����Ϊ����ʾ����� FLASH ʹ�û�����д��Ͷ�ȡ���ݶ�����
#define FLASH_SECTION_INDEX       (63)                                          // �洢�����õ����� ������һ������
#define FLASH_PAGE_INDEX          (3)                                           // �洢�����õ�ҳ�� ������һ��ҳ��

void  Read_Flash(void);
void  Write_Flash(void);

#endif /* CODE_FLASH_H_ */
