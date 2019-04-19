/*
* Copyright 2018 Wenbin Yang <bysin7@gmail.com>
* This file is part of BLITZ (Behavioral Learning In The Zebrafish),
* which is adapted from MindControl (Andrew Leifer et al <leifer@fas.harvard.edu>
* Leifer, A.M., Fang-Yen, C., Gershow, M., Alkema, M., and Samuel A. D.T.,
* 	"Optogenetic manipulation of neural activity with high spatial resolution in
*	freely moving Caenorhabditis elegans," Nature Methods, Submitted (2010).
*
* BLITZ is a free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the license, or
* (at your option) any later version.
*
* Filename: talk2relay.cpp
* Abstract: this file contains all function definitions
*			used in serial communication
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/

// Include user-defined libraries
#include "talk2relay.h"



bool Relay::initialize(int com_num)
{
	if (!sPort.InitPort(com_num))
	{
		std::cout << "SerialPort initiation failed! " << std::endl;
		return false;
	}
	else
	{
		std::cout << "SerialPort initiation succeeded! " << std::endl << std::endl;
	}
	return true;
}
/* Open channel by index */
bool Relay::givePulse(int idxChannel)
{
	bool res = sPort.WriteData(openCommands[idxChannel], LEN_COMMAND);
	return res;
}

bool Relay::givePulse(vector<int> channelIndices,double openDuration)
{
	// TODO: write the code to open multiple channesl simultaneously,
	// might need to generate new 16-base code
	/*16路串口继电器RS232通信协议
	detail:https://item.taobao.com/item.htm?spm=a1z10.1-c-s.w4004-17114410235.18.f4cf2355YNSEG9&id=16171044415
  信号为一个unsigned char openCommand[9]
  其中每一个元素表示为0（0x00)~255(0xFF)的2位16进制数
  前两字节固定为0x00,0x5A
  第三字节：设备号，固定为0x60
  第四字节：地址字节，第一张卡为0x01，依此类推
  第五字节：命令字节，第六，七，八为命令参数,共同组成一条命令

  命令列表：
  0x01位开：吸合第六位参数表示的继电器，七，八位参数无效，设为0x00. e.g.{0x01,0x04,0x00,0x00}(第四个继电器开)
  0x02位关：断开第六位参数表示的继电器，七，八位参数无效，设为0x00. e.g.{0x02,0x06,0x00,0x00}(第六个继电器开)
  0x03全开：吸合全部继电器，六，七，八位参数无效，设为0x00. e.g.{0x03,0x00,0x00,0x00}(全开)
  0x04全关：断开全部继电器，六，七，八位参数无效，设为0x00. e.g.{0x04,0x00,0x00,0x00}(全关)
  0x05组开：吸合部分继电器，选定哪些继电器由六，七位参数控制,表示方法见后文，八位无效，设为0x00.e.g. {0x05,0x78,0xAA,0x00}(第4，5，6，7，10，12，14，16继电器开)
  0x06组关：断开部分继电器，选定哪些继电器由六，七位参数控制,表示方法见后文，八位无效，设为0x00.e.g. {0x05,0x78,0xAA,0x00}(第4，5，6，7，10，12，14，16继电器关)
  0x07读状态：读取继电器状态，六，七，八位参数无效，设为0x00.e.g.{0x07,0x00,0x00,0x00}(读状态)
  0x08组合开关：吸合部分继电器，断开剩余继电器，具体为吸合继电器地址表示法中记为1的继电器，断开记为0的继电器，八位无效，设为0x00.e.g.{0x05,0x78,0xAA,0x00}(第4，5，6，7，10，12，14，16继电器开;1,2,3,8,9,11,13,15关)
  0x10读地址：读取板卡地址，六，七，八位参数无效，设为0x00.根据第四字节，若有返回则地址为第四字节地址；若第四字节设为0xFF则为广播寻址，返回地址。
  0x11设置地址：将地址为第四字节卡的地址设为第六字节，七，八位参数无效设为0x00.e.g.{0x11,0x02,0x00,0x00}(将当前卡地址设为02)
  0x12点动：部分继电器吸合一段时间后断开，六，七位参数控制继电器地址，第八位参数控制时间（单位0.1s).e.g.{0x05,0x78,0xAA,0x03}(第4，5，6，7，10，12，14，16继电器开0.3s)

  第九字节：校验和，为前八字节求和，取16进制末两位。若不等则为无效信号

  继电器地址表示方法：
  第n个继电器记作Yn，由大到小排列
  {Y16,Y15,Y14,Y13,Y12,Y11,Y10,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1}
  将选定继电器记作1，非选定记作0.例如选择4，5，6，7，10，12，14，16
  {1，0，1，0，1，0，1，0，0，1，1，1，1，0，0，0}
  每八位分为一组，作为一个二进制数，转为十六进制
  0b10101010=0xAA,0b01111000=0x78
  则上述继电器地址表示为0x78,0xAA

  常用信号举例：
  { 0x00,0x5A,0x60,0x01,0x12,0x00,0x02,0x01,0xD0 }(10号继电器开0.1s)

	ps:实际使用过程中不一定需要使用十六进制表示，0x10与16效果相同

  */
	int channelIndices_0_7 = 0, channelIndices_8_15 = 0;
	int num_01s = ceil(openDuration * 10);
	if (!num_01s > 0) {
		cout << "incorrect relay openDuration!" << endl;
		return false;
	}
	for (int i = 7; i >= 0; i--) {
		channelIndices_0_7 <<= 1;
		channelIndices_0_7 += channelIndices[i];
	}
	for (int i = 15; i >= 8; i--) {
		channelIndices_8_15 <<= 1;
		channelIndices_8_15 += channelIndices[i];
	}
	int check_relay = 0x00 + 0x5A + 0x60 + 0x01 + 0x12 + channelIndices_0_7 + channelIndices_8_15 + num_01s;
	unsigned char openCommand[LEN_COMMAND]= { 0x00,0x5A,0x60,0x01,0x12,channelIndices_0_7,channelIndices_8_15,num_01s,check_relay };
	bool res = sPort.WriteData(openCommand, LEN_COMMAND);
	return res;
}
