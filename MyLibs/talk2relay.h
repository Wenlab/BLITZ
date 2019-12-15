/*
* Copyright 2019 Wenbin Yang <bysin7@gmail.com> (This project started from Jan., 2018.)
* This file is part of [BLITZ (Behavioral Learning In The Zebrafish)](https://github.com/Wenlab/BLITZ),
* which is adapted from MindControl (Andrew Leifer et al., 2011) <leifer@fas.harvard.edu>
*
* BLITZ is a free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the license, or
* (at your option) any later version.
*
* Filename: talk2relay.h
* Abstract: this file contains all classes and functions' declarations
*			used in serial communication
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>, Jingyuan Ji also contributed.
* Created on: Jan. 15, 2018
* Modified on: Apr. 20, 2019
*/

/*16·���ڼ̵���RS232ͨ��Э��
This part is contributed by Jingyuan Ji. If you need help to translate to English,
please feel free to contact me (Wenbin Yang, bysin7@gmail.com).

[See more details](https://item.taobao.com/item.htm?spm=a1z10.1-c-s.w4004-17114410235.18.f4cf2355YNSEG9&id=16171044415)

�ź�Ϊһ��unsigned char openCommand[9]
��һ�ֽڣ�֡�ף��̶�Ϊ0x00
�ڶ��ֽڣ�֡�ף��̶�Ϊ0x5A
�����ֽڣ��豸�ţ��̶�Ϊ0x60
�����ֽڣ���ַ�ֽڣ���һ�ſ�Ϊ0x01����������
�����ֽڣ������ֽڣ��������ߣ���Ϊ�������,��ͬ���һ������
�����б�
0x01λ�������ϵ���λ������ʾ�ļ̵������ߣ���λ������Ч����Ϊ0x00. e.g.{0x01,0x04,0x00,0x00}(���ĸ��̵�����)
0x02λ�أ��Ͽ�����λ������ʾ�ļ̵������ߣ���λ������Ч����Ϊ0x00. e.g.{0x02,0x06,0x00,0x00}(�������̵�����)
0x03ȫ��������ȫ���̵����������ߣ���λ������Ч����Ϊ0x00. e.g.{0x03,0x00,0x00,0x00}(ȫ��)
0x04ȫ�أ��Ͽ�ȫ���̵����������ߣ���λ������Ч����Ϊ0x00. e.g.{0x04,0x00,0x00,0x00}(ȫ��)
0x05�鿪�����ϲ��̵ּ�����ѡ����Щ�̵�����������λ��������,��ʾ���������ģ���λ��Ч����Ϊ0x00.e.g. {0x05,0x78,0xAA,0x00}(��4��5��6��7��10��12��14��16�̵�����)
0x06��أ��Ͽ����̵ּ�����ѡ����Щ�̵�����������λ��������,��ʾ���������ģ���λ��Ч����Ϊ0x00.e.g. {0x05,0x78,0xAA,0x00}(��4��5��6��7��10��12��14��16�̵�����)
0x07��״̬����ȡ�̵���״̬�������ߣ���λ������Ч����Ϊ0x00.e.g.{0x07,0x00,0x00,0x00}(��״̬)
0x08��Ͽ��أ����ϲ��̵ּ������Ͽ�ʣ��̵���������Ϊ���ϼ̵�����ַ��ʾ���м�Ϊ1�ļ̵������Ͽ���Ϊ0�ļ̵�������λ��Ч����Ϊ0x00.e.g.{0x05,0x78,0xAA,0x00}(��4��5��6��7��10��12��14��16�̵�����;1,2,3,8,9,11,13,15��)
0x10����ַ����ȡ�忨��ַ�������ߣ���λ������Ч����Ϊ0x00.���ݵ����ֽڣ����з������ַΪ�����ֽڵ�ַ���������ֽ���Ϊ0xFF��Ϊ�㲥Ѱַ�����ص�ַ��
0x11���õ�ַ������ַΪ�����ֽڿ��ĵ�ַ��Ϊ�����ֽڣ��ߣ���λ������Ч��Ϊ0x00.e.g.{0x11,0x02,0x00,0x00}(����ǰ����ַ��Ϊ02)
0x12�㶯�����̵ּ�������һ��ʱ���Ͽ���������λ�������Ƽ̵�����ַ���ڰ�λ��������ʱ�䣨��λ0.1s).e.g.{0x05,0x78,0xAA,0x03}(��4��5��6��7��10��12��14��16�̵�����0.3s)
�ھ��ֽڣ�У��ͣ�Ϊǰ���ֽ���ͣ�ȡ16����ĩ��λ����������Ϊ��Ч�ź�
�̵�����ַ��ʾ������
��n���̵�������Yn���ɴ�С����
{Y16,Y15,Y14,Y13,Y12,Y11,Y10,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1}
��ѡ���̵�������1����ѡ������0.����ѡ��4��5��6��7��10��12��14��16
{1��0��1��0��1��0��1��0��0��1��1��1��1��0��0��0}
ÿ��λ��Ϊһ�飬��Ϊһ������������תΪʮ������
0b10101010=0xAA,0b01111000=0x78
�������̵�����ַ��ʾΪ0x78,0xAA
�����źž�����
{ 0x00,0x5A,0x60,0x01,0x12,0x00,0x02,0x01,0xD0 }(10�ż̵�����0.1s)

ps:ʵ��ʹ�ù����в�һ����Ҫʹ��ʮ�����Ʊ�ʾ��int�ͱ�ʾ������ͬ��Ч��
*/


#ifndef _GUARD_TALK2RELAY_H
#define _GUARD_TALK2RELAY_H

/* Disable warning to function std::copy */
#pragma warning(disable : 4996)

// Include 3rd party libraries
#include "C:\Users\USER\source\repos\rewrite-learning\rewrite-learning\3rdPartyLibs\SerialCom\SerialPort.h"

// Include standard libraries
#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>

 // put this into relay module
#define NUM_CHANNEL 12
#define LEN_COMMAND 9

class Relay
{
private:
	;
public:
	// methods
	Relay()
		: BIT_1(0x00)
		, BIT_2(0x5A)
		, BIT_3(0x54)
		, BIT_4(0x00)
	{

	}

	/* Initialize the serial port by COM number*/
	void initialize(int com_num);

	void giveFixedFishPulse();

	/* Open channel for 0.1 seconds by index */
	void givePulse(int idxChannel);

	/* Open multiple channels simultaneously */
	void givePulse(std::vector<bool> channelStatuses, float openDuration = 0.1);//TODO: finish the implementation of this method

	/* Generate open command: open for a few tenth-seconds.*/
	void generateOpenCommand(
		unsigned char* openCommand, // command to the relay
		std::vector<bool> channelStatuses, // each element stands for on/off state for a channel
		float openDuration // in seconds
	);

	/* Generate open command: stay open */
	void generateOpenCommand(
		unsigned char* openCommand, // command to the relay
		std::vector<bool> channelStatuses
	);

private:
	// properties
	CSerialPort sPort; // serialPort
	
	const unsigned char BIT_1; // address initial, 0x00 as the default
	const unsigned char BIT_2; // address initial, 0x5A as the default
	const unsigned char BIT_3; // board number
	const unsigned char BIT_4; // address number
	/* unsigned char BIT_5;
	 operation number, this is set in methods as a local variable,
	 since it is changing in the context, not const.
	 The same to BIT_6, 7, 8, and 9.
	*/
	unsigned char openCommandForFixedFish[9] = { 0x00,0x5A,0x54,0x00,0x12,0x01,0x00,0x01,0xC2 };

};

// global functions
int binaryAdding(std::vector<bool>::iterator vecBegin, std::vector<bool>::iterator vecEnd);

#endif // !_GUARD_TALK2RELAY_H

