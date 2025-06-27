#include "interface.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std; // @suppress("Symbol is not resolved")
unsigned long SITE = 0, SITE_ID = 0, ACTIVE_SITE_NUM = 0;

#define SITE_NUM 4
//#define UPDATE_VCT_NUM8 8
#define UPDATE_VCT_NUM16 14
#define Vvref_VOLT125 1.0

#define Vvref_VOLT15 4.001
#define Vvref_VOLT20 2.001
#define Vvref_VOLT30 3.001

#define FvrefL  32000
#define FvrefH  62500

ULONG VHSX2;
ULONG VHSX3;
ULONG VHSX4;

ULONG HRC_VALUE[64] = {0};
ULONG LRC_VALUE[64] = {0};
ULONG IP_VALUE[64] = {0};
ULONG Verify_Config_bg_flag[SITE_NUM] = {0};
ULONG gVrefData_read[SITE_NUM] = {0x0000,0x0000};
//原始修调值
ULONG gVrefData[SITE_NUM] = {0x0000,0x0000};
ULONG gVrefData1[SITE_NUM] = {0x0000,0x0000};
ULONG gVrefData2[SITE_NUM] = {0x0000,0x0000};
ULONG gVrefData3[SITE_NUM] = {0x0000,0x0000};
ULONG gVrefData4[SITE_NUM] = {0x0000,0x0000};
ULONG gVrefData1P5[SITE_NUM] = {0x0000,0x0000};
ULONG gVrefData2P0[SITE_NUM] = {0x0000,0x0000};
ULONG gVrefData3P0[SITE_NUM] = {0x0000,0x0000};
//写入取反值
ULONG gAD2VDataN[SITE_NUM] = {0x0000,0x0000};
ULONG gAD3VDataN[SITE_NUM] = {0x0000,0x0000};
ULONG gAD4VDataN[SITE_NUM] = {0x0000,0x0000};

ULONG g_WriteChsmL[SITE_NUM] = {0x0000};
ULONG g_WriteChsmH[SITE_NUM] = {0x0000};
ULONG gFrefDataL[SITE_NUM] = {0x0000,0x0000};
ULONG gFrefDataH[SITE_NUM] = {0x0000,0x0000};

ULONG gVrefORbgDataN[SITE_NUM] = {0x0000,0x0000};
ULONG gVrefDataN[SITE_NUM] = {0x0000,0x0000};
ULONG gFrefDataLN[SITE_NUM] = {0x0000,0x0000};
ULONG gFrefDataHN[SITE_NUM] = {0x0000,0x0000};

const uint16_t CRC16Table[256] =
{
      0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
      0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
      0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
      0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
      0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
      0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
      0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
      0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
      0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
      0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
      0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
      0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
      0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
      0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
      0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
      0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
      0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
      0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
      0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
      0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
      0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
      0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
      0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
      0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
      0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
      0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
      0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
      0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
      0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
      0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
      0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
      0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};



SITE_DOUBLE Hvalue;
SITE_DOUBLE Lvalue;
SITE_DOUBLE Hvalue2P4;
SITE_DOUBLE Lvalue2P4;
SITE_DOUBLE Hvalue1;
SITE_DOUBLE Lvalue1;
SITE_LONG Pass_Hrc;
SITE_LONG Pass_Lrc;

const uint16_t dataPlase[] = {0x10,0x08,0x04,0x02,0x01};
const uint16_t dataPlaseBG1[] = {0x1000,0x0800,0x0400,0x0200};
const uint16_t dataPlaselck[] = {0x0004,0x002,0x001};

ULONG gTrimOK_Count[SITE_NUM] = {0};
ULONG gFTrimOK_Count[SITE_NUM] = {0};
int minOK_Count[SITE_NUM] = {0};

ULONG ulTrimDataOk1 = 0, ulTrimDataOk2 = 0;
double dbTrimValue1 = 0, dbTrimValue2 = 0;
int iCount = 0;


///////////////////20230516  增加读 ID  ////////
ULONG RDVrefData[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataL[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataH[SITE_NUM] = {0x0000,0x0000};
ULONG RDVrefData1[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataL1[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataH1[SITE_NUM] = {0x0000,0x0000};
ULONG RDVrefData2[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataL2[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataH2[SITE_NUM] = {0x0000,0x0000};
ULONG RDgAD2V[SITE_NUM] = {0x0000,0x0000};
ULONG RDgAD3V[SITE_NUM] = {0x0000,0x0000};
ULONG RDgAD4V[SITE_NUM] = {0x0000,0x0000};
ULONG RDgCHKSUMH[SITE_NUM] = {0x0000,0x0000};
ULONG RDgCHKSUML[SITE_NUM] = {0x0000,0x0000};


ULONG RDVrefDataN[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataLN[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataHN[SITE_NUM] = {0x0000,0x0000};
ULONG RDVrefDataN1[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataLN1[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataHN1[SITE_NUM] = {0x0000,0x0000};
ULONG RDVrefDataN2[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataLN2[SITE_NUM] = {0x0000,0x0000};
ULONG RDgFrefDataHN2[SITE_NUM] = {0x0000,0x0000};

//////////////////////////////////////////////////////////////////////

USER_CODE void ProjectReuse() {
  cout << "ProjectReuse" << endl;
}

USER_CODE void ProjectInit() {
  cout << "ProjectInit" << endl;
}

USER_CODE void ProjectReset() {
  cout << "ProjectReset" << endl;
}

USER_CODE void site_init() {
  cout << "site_init" << endl;
}

USER_CODE void site_reset() {
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(0).SetVol(0).SetActiveLoadStatus(OFF).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
  cout << "site_reset" << endl;
}

void EndOfWafer() {
  cout << "EndOfWafer" << endl;
}

void EndOfLot() {
  cout << "EndOfLot" << endl;
}

inline void Enable_EOW_Func() {
  proberObj.EnableWaferEndCheck(EndOfWafer);
}

inline void Enable_EOL_Func() {
  proberObj.EnableLotEndCheck(EndOfLot);
}

void Plvl_Set(string strSigName)
{
	pinlevel.Signal(strSigName).Connect();
	sys.DelayUs(2000);
	pinlevel.Signal(strSigName).SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
}

void UpdateVvref(string patName, int iLen, char *ch)
{
	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", "0");
		}

		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", (ch[j] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", (ch[j] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", (ch[j] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", (ch[j] == '0') ? "0" : "1");
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}
}
void UpdateVHSX(string patName, int iLen, char *ch)
{
	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+3, "PDA", "0");
		}

		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4, "PDA", (ch[j] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+1, "PDA", (ch[j] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+2, "PDA", (ch[j] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+3, "PDA", (ch[j] == '0') ? "0" : "1");
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_vhsx", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}
}
void UpdateHCKPat(string patName, ULONG siteID)
{
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;
	memset(ch, 0, UPDATE_VCT_NUM16);

	strData = to_string((gFrefDataH[siteID]));//gFrefDataHN[siteID]
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

//	sys.PrintDatalog("SITE[%d] iLen: %d \n", SITE_ID,iLen);
//	cout << "UpdateTrimWritePat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", "0");
//			cout << "i: " << i << endl;
		}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", wfc);
//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update IPvref Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}

}
void UpdateHCLKPat(string patName, ULONG siteID)
{
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string((gFrefDataHN[siteID]));//gFrefDataHN[siteID]
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

//	sys.PrintDatalog("SITE[%d] iLen: %d \n", SITE_ID,iLen);
//	cout << "UpdateTrimWritePat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", "0");
//			cout << "i: " << i << endl;
		}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", wfc);
//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update IPvref Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}

}
void UpdateLCKPat(string patName, ULONG siteID)
{
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;


	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string((gFrefDataL[siteID]));
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

//	sys.PrintDatalog("SITE[%d] iLen: %d \n", SITE_ID,iLen);
//	cout << "UpdateTrimWritePat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", "0");
//			cout << "i: " << i << endl;
		}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", wfc);
//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update IPvref Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}

}
void UpdatevrefPat(string patName, ULONG siteID)
{
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;


	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string((gVrefData[siteID]));
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

//	sys.PrintDatalog("SITE[%d] iLen: %d \n", SITE_ID,iLen);
//	cout << "UpdateTrimWritePat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", "0");
//			cout << "i: " << i << endl;
		}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", wfc);
//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update IPvref Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}

}

void UpdateCPfusePat(string patName, ULONG siteID)
{
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	ULONG tempDat[SITE_NUM] = {0};
	string strData;

//	sys.PrintDatalog("SITE[%d] ,gVrefData:%x, gVrefData1p5:%x,gVrefData2p0:%x,gVrefData3p0:%x\n", SITE,gVrefData[SITE],gVrefData1P5[SITE],gVrefData2P0[SITE],gVrefData3P0[SITE]);
//  sys.PrintDatalog("SITE[%d] ,gFrefDataL:%x,gFrefDataH:%x \n", SITE,gFrefDataL[SITE],gFrefDataH[SITE]);
/*********************************   Update  Vref    START    *************************/

	strData = to_string(gVrefORbgDataN[siteID]);

	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+3, "PDA", "0");
//			cout << "i: " << i << endl;
		}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+3, "PDA", wfc);
//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_bghrc", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update vrefMode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}


/*********************************   Update  Vref    END    *************************/

/*********************************   Update  lclk    START    *************************/
	memset(ch, 0, UPDATE_VCT_NUM16);

//	gFrefDataLN[siteID] = 0x0004;
//	sys.PrintDatalog("SITE[%d] gFrefDataLN:%x \n", SITE_ID,gFrefDataLN[SITE_ID]);
	strData = to_string(gFrefDataLN[siteID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

//	cout << "UpdateTrimVerifyPat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+3, "PDA", "0");
//			cout << "i: " << i << endl;
		}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+3, "PDA", wfc);
//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_lrc", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update lclk Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}
/*********************************   Update  lclk    END    *************************/

/*********************************   Update  insert_2v    START    *************************/
			memset(ch, 0, UPDATE_VCT_NUM16);
//			gAD2VDataN[siteID] = 0x4e;
//			sys.PrintDatalog("SITE[%d] gAD2VDataN:%x \n", SITE_ID,gAD2VDataN[SITE_ID]);
			strData = to_string(gAD2VDataN[siteID]);
			pTemp = strData.c_str();
			ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
			pch = &ch[0];
			iLen = strlen(pch);

		//	cout << "UpdateTrimVerifyPat, iLen:" << iLen << endl;

			if(iLen < UPDATE_VCT_NUM16)
			{
				int i = 0;
				int j = 0;
				//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
				for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
				{
					//第一个开始update data为ch[0],ch[1]......
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4, "PDA", "0");
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+1, "PDA", "0");
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+2, "PDA", "0");
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+3, "PDA", "0");
		//			cout << "i: " << i << endl;
				}

				string wfc;
				//接着update data为ch[0],ch[1]......
				for(; j < iLen; i++,j++)
				{
					wfc = (ch[j] == '0') ? "0" : "1";
					//第一个开始update data为ch[0],ch[1]......
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4, "PDA", wfc);
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+1, "PDA", wfc);
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+2, "PDA", wfc);
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+3, "PDA", wfc);
		//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
				}
			}
			else if(iLen == UPDATE_VCT_NUM16)
			{
				for(int i = 0; i < UPDATE_VCT_NUM16; i++)
				{
					//第一个开始update data为ch[0],ch[1]......
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
					dio.Pattern(patName).UpdateVctData("insert_2v", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
				}
			}
			else
			{
				cout << "update vrefMode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
			}
/*********************************   Update  insert_2v    END    *************************/

/*********************************   Update  insert_3v    START    *************************/
		memset(ch, 0, UPDATE_VCT_NUM16);

//		gAD3VDataN[siteID] = 0x4a;
//		sys.PrintDatalog("SITE[%d] gAD3VDataN:%x \n", SITE_ID,gAD3VDataN[SITE_ID]);
		strData = to_string(gAD3VDataN[siteID]);
		pTemp = strData.c_str();
		ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
		pch = &ch[0];
		iLen = strlen(pch);

//	cout << "UpdateTrimVerifyPat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
		//第一个开始update data为ch[0],ch[1]......
		dio.Pattern(patName).UpdateVctData("insert_3v", i*4, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_3v", i*4+1, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_3v", i*4+2, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_3v", i*4+3, "PDA", "0");
		//			cout << "i: " << i << endl;
			}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_3v", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_3v", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_3v", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_3v", i*4+3, "PDA", wfc);
	//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
			}
		}
		else if(iLen == UPDATE_VCT_NUM16)
    	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_3v", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_3v", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_3v", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_3v", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");					}
			}
			else
			{
					cout << "update lclk Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
			}
/*********************************   Update  insert_3v    END    *************************/

/*********************************   Update  insert_4v    START    *************************/
		memset(ch, 0, UPDATE_VCT_NUM16);

//		gAD4VDataN[siteID] = 0x43;
//		sys.PrintDatalog("SITE[%d] gAD4VDataN:%x \n", SITE_ID,gAD4VDataN[SITE_ID]);
		strData = to_string(gAD4VDataN[siteID]);
		pTemp = strData.c_str();
		ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
		pch = &ch[0];
		iLen = strlen(pch);

//	cout << "UpdateTrimVerifyPat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
		//第一个开始update data为ch[0],ch[1]......
		dio.Pattern(patName).UpdateVctData("insert_4v", i*4, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_4v", i*4+1, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_4v", i*4+2, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_4v", i*4+3, "PDA", "0");
		//			cout << "i: " << i << endl;
			}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_4v", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_4v", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_4v", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_4v", i*4+3, "PDA", wfc);
	//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
			}
		}
		else if(iLen == UPDATE_VCT_NUM16)
    	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_4v", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_4v", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_4v", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_4v", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");					}
			}
			else
			{
					cout << "update lclk Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
			}
/*********************************   Update  insert_4v    END    *************************/

/*********************************   Update  insert_chkl    START    *************************/
		memset(ch, 0, UPDATE_VCT_NUM16);
//		g_WriteChsmL[siteID] = 0xb08;
//		sys.PrintDatalog("SITE[%d] g_WriteChsmL:%x \n", SITE_ID,g_WriteChsmL[SITE_ID]);
		strData = to_string(g_WriteChsmL[siteID]);
		pTemp = strData.c_str();
		ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
		pch = &ch[0];
		iLen = strlen(pch);

//	cout << "UpdateTrimVerifyPat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
		//第一个开始update data为ch[0],ch[1]......
		dio.Pattern(patName).UpdateVctData("insert_chkl", i*4, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+1, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+2, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+3, "PDA", "0");
		//			cout << "i: " << i << endl;
			}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_chkl", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+3, "PDA", wfc);
	//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
			}
		}
		else if(iLen == UPDATE_VCT_NUM16)
    	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_chkl", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_chkl", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");					}
			}
			else
			{
					cout << "update lclk Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
			}
/*********************************   Update  insert_chkl    END    *************************/

/*********************************   Update  insert_chkh    START    *************************/
		memset(ch, 0, UPDATE_VCT_NUM16);
//		g_WriteChsmH[siteID] = 0x5;
//		sys.PrintDatalog("SITE[%d] g_WriteChsmH:%x \n", SITE_ID,g_WriteChsmH[SITE_ID]);
		strData = to_string(g_WriteChsmH[siteID]);
		pTemp = strData.c_str();
		ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
		pch = &ch[0];
		iLen = strlen(pch);

//	cout << "UpdateTrimVerifyPat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
		//第一个开始update data为ch[0],ch[1]......
		dio.Pattern(patName).UpdateVctData("insert_chkh", i*4, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+1, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+2, "PDA", "0");
		dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+3, "PDA", "0");
		//			cout << "i: " << i << endl;
			}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_chkh", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+3, "PDA", wfc);
	//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
			}
		}
		else if(iLen == UPDATE_VCT_NUM16)
    	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert_chkh", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert_chkh", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");					}
			}
			else
			{
					cout << "update lclk Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
			}
/*********************************   Update  insert_chkh    END    *************************/
}

void UpdateCPfusePat111(string patName, ULONG siteID)
{
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

//	g_WriteChsmL[SITE_ID] = (g_adjChSumCalt[SITE_ID] & 0x0FFF);
//	g_WriteChsmH[SITE_ID] = ((g_adjChSumCalt[SITE_ID] >> 12) & 0x000F);
	//0xB的0~11存放校验和的0~11bit
	//0xC的0~3存放校验和的12~15bit


//	sys.PrintDatalog("SITE[%d] ,gVrefData:%x, gVrefData1p5:%x,gVrefData2p0:%x,gVrefData3p0:%x\n", SITE,gVrefData[SITE],gVrefData1P5[SITE],gVrefData2P0[SITE],gVrefData3P0[SITE]);
//    sys.PrintDatalog("SITE[%d] ,gFrefDataL:%x,gFrefDataH:%x \n", SITE,gFrefDataL[SITE],gFrefDataH[SITE]);
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(g_WriteChsmL[siteID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

//	cout << "UpdateTrimVerifyPat, iLen:" << iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert0", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert0", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert0", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert0", i*4+3, "PDA", "0");
//			cout << "i: " << i << endl;
		}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert0", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert0", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert0", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert0", i*4+3, "PDA", wfc);
//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert0", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert0", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert0", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert0", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update vrefMode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}
	//update hclk TrimData
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(g_WriteChsmH[siteID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

//	cout << "UpdateTrimVerifyPat, iLen:" <<iLen << endl;

	if(iLen < UPDATE_VCT_NUM16)
	{
		int i = 0;
		int j = 0;
		//update 前（UPDATE_VCT_NUM16 - iLen）个vct为0
		for(i = 0; i < UPDATE_VCT_NUM16 - iLen; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert1", i*4, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert1", i*4+1, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert1", i*4+2, "PDA", "0");
			dio.Pattern(patName).UpdateVctData("insert1", i*4+3, "PDA", "0");
//			cout << "i: " << i << endl;
		}

		string wfc;
		//接着update data为ch[0],ch[1]......
		for(; j < iLen; i++,j++)
		{
			wfc = (ch[j] == '0') ? "0" : "1";
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert1", i*4, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert1", i*4+1, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert1", i*4+2, "PDA", wfc);
			dio.Pattern(patName).UpdateVctData("insert1", i*4+3, "PDA", wfc);
//			cout << "i: " << i << ", j: " << j << ", ch: " << ch[j] << ", wfc:" << wfc << endl;
		}
	}
	else if(iLen == UPDATE_VCT_NUM16)
	{
		for(int i = 0; i < UPDATE_VCT_NUM16; i++)
		{
			//第一个开始update data为ch[0],ch[1]......
			dio.Pattern(patName).UpdateVctData("insert1", i*4, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert1", i*4+1, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert1", i*4+2, "PDA", (ch[i] == '0') ? "0" : "1");
			dio.Pattern(patName).UpdateVctData("insert1", i*4+3, "PDA", (ch[i] == '0') ? "0" : "1");
		}
	}
	else
	{
		cout << "update hclk Mode vct num is err("<< iLen << ", " << UPDATE_VCT_NUM16 << ")" << endl;
	}

}

int TrimfreProcessH(ULONG trimdata[], ULONG ulSite, string patName,  double (&dbCurFRE)[64])
{                                                                  //Fvref8M, dbCurFRE
	vector<ST_MEAS_RESULT> stTMUMeasResult;
	ULONG ulTrimData;
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	FOR_EACH_SITE_START(SITE,SITE_ID)//----------
	ulTrimData = trimdata[SITE_ID];

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref(patName, iLen, ch);
	NEXT_SITE

	dio.Pattern(patName).Run("cycle_start").WaitEnd();
	sys.DelayUs(2000);

	tmu.Freq().Signal("TMU_CH29").StartMeas();
	tmu.Freq().Signal("TMU_CH29").GetMeasResult(stTMUMeasResult);
	//tmu.DisConnect("TMU_CH09");
	sys.DelayUs(2000);

	FOR_EACH_SITE_START(SITE,SITE_ID)//----------
	sys.PrintDatalog("SITE[%d] %d time means Freq: %f, trimdata:%0x \n", SITE_ID, iCount, stTMUMeasResult[0].dbValue, ulTrimData);
	iCount++;
	dbCurFRE[SITE_ID] = stTMUMeasResult[SITE].dbValue;
	NEXT_SITE

	return gFTrimOK_Count[ulSite];
}

int TrimfreProcessL(ULONG trimdata[], ULONG ulSite, string patName,  double (&dbCurFRE)[64])
{                                                                  //Fvref8M, dbCurFRE
	vector<ST_MEAS_RESULT> stTMUMeasResult;
	ULONG ulTrimData;
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	FOR_EACH_SITE_START(SITE,SITE_ID)//----------
	ulTrimData = trimdata[SITE_ID];

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref(patName, iLen, ch);
	NEXT_SITE

	dio.Pattern(patName).Run("cycle_start").WaitEnd();
	sys.DelayUs(2000);

	tmu.Freq().Signal("TMU_CH29").StartMeas();
	tmu.Freq().Signal("TMU_CH29").GetMeasResult(stTMUMeasResult);
	sys.DelayUs(2000);

	FOR_EACH_SITE_START(SITE,SITE_ID)//----------
	sys.PrintDatalog("SITE[%d] %d time means Freq: %f, trimdata:%0x \n", SITE_ID, iCount, stTMUMeasResult[0].dbValue, ulTrimData);
	iCount++;
	dbCurFRE[SITE_ID] = stTMUMeasResult[SITE].dbValue;
	NEXT_SITE

	return gFTrimOK_Count[ulSite];
}
int TrimvoltProcess(ULONG trimdata[], ULONG ulSite, string patName, double (&CurVolt)[64])
//int TrimvoltProcess(ULONG trimdata, ULONG ulSite, string patName, double trimVolt, double &CurVolt)
{
	vector<ST_MEAS_RESULT> PpmuResult;
	ULONG ulTrimData;
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	FOR_EACH_SITE_START(SITE,SITE_ID)//----------
	ulTrimData = trimdata[SITE_ID];

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);
	UpdateVvref(patName, iLen, ch);
	NEXT_SITE//-------------


//	Plvl_Set("PIN_VREF");
	dio.Pattern(patName).Run("cycle_start").WaitEnd();
	sys.DelayUs(2000);

	ppmu.Signal("PIN_VREF").SetMode("FNMV").Execute();
	sys.DelayUs(2000);
	PpmuResult.clear();
	ppmu.Measure(PpmuResult,MEAS_AVG,50,50);

	FOR_EACH_SITE_START(SITE,SITE_ID)//-------------------
	sys.PrintDatalog("SITE[%d] %d time meas volt(PIN_VREF): %f, trimdata:%x \n", SITE_ID, iCount, PpmuResult[SITE].dbValue, ulTrimData);
	iCount++;

	CurVolt[SITE_ID] = PpmuResult[SITE].dbValue;
	NEXT_SITE//--------------
	return gTrimOK_Count[ulSite];
}
int TrimvoltProcess04(ULONG trimdata, ULONG ulSite, string patName, double &CurVolt)
{
	vector<ST_MEAS_RESULT> PpmuResult;
	ULONG ulTrimData;
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	ulTrimData = trimdata;

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);
	sys.PrintDatalog("iLen: %d ", iLen);
	UpdateVvref(patName, iLen, ch);

//	Plvl_Set("PIN_ADOUT");
	dio.Pattern(patName).Run("cycle_start").WaitEnd();
	sys.DelayUs(2000);

	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);
	PpmuResult.clear();
	ppmu.Measure(PpmuResult,MEAS_AVG,50,50);
	sys.PrintDatalog("SITE[%d] %d time meas volt(PIN_ADOUT): %f, trimdata:%x \n", SITE_ID, iCount, PpmuResult[0].dbValue, ulTrimData);
	iCount++;
	CurVolt = PpmuResult[0].dbValue;
	return gTrimOK_Count[ulSite];
}
int Minfind(ULONG ulSite,  ULONG trimdata[][6], double volt_test[][6],double VOLTref, int length)
{
	ULONG MINTrimData[64][length]={0};
	double MINvolt_test[64][length]={0};
	double MINvoltref;
//	ULONG MINnum = num;

//	ULONG length=sizeof(trimdata)/sizeof(ULONG);
	memcpy(MINTrimData[SITE_ID],trimdata[SITE_ID],length*sizeof(ULONG));
	memcpy(MINvolt_test[SITE_ID],volt_test[SITE_ID],length*sizeof(double));
	minOK_Count[ulSite]=0;

    MINvoltref=VOLTref;

    double MINVOT;
    MINVOT=fabs(MINvolt_test[SITE_ID][0]-MINvoltref);
	for(int j=1; j<length-1; j++)
	{
		if(fabs(MINvolt_test[SITE_ID][j] - MINvoltref) < MINVOT)
		{
			MINVOT = fabs(MINvolt_test[SITE_ID][j]-MINvoltref);
			minOK_Count[ulSite]=j;
		}

	}
	return minOK_Count[ulSite];
}

int Minfind(ULONG ulSite,  ULONG trimdata[][16], double volt_test[][16],double VOLTref, int length)
{
	ULONG MINTrimData[64][length]={0};
	double MINvolt_test[64][length]={0};
	double MINvoltref;
//	ULONG MINnum = num;

//	ULONG length=sizeof(trimdata)/sizeof(ULONG); ????????????????????不能用二维数组

		memcpy(MINTrimData[SITE_ID],trimdata[SITE_ID],length*sizeof(ULONG));
		memcpy(MINvolt_test[SITE_ID],volt_test[SITE_ID],length*sizeof(double));

	minOK_Count[ulSite]=0;

    MINvoltref=VOLTref;

    double MINVOT;
    MINVOT=fabs(MINvolt_test[SITE_ID][0]-MINvoltref);
	for(int j=1; j<length-1; j++)
	{
		if(fabs(MINvolt_test[SITE_ID][j] - MINvoltref) < MINVOT)
		{
			MINVOT = fabs(MINvolt_test[SITE_ID][j]-MINvoltref);
			minOK_Count[ulSite]=j;
		}

	}

	return minOK_Count[ulSite];

}


uint16_t CalulateCRC16(const uint8_t* dat, uint8_t len)
{
    uint16_t crc = 0;

    while (len-- != 0)
    {
        crc = (crc << 8) ^ CRC16Table[((crc >> 8) ^ (*dat++)) & 0xFF];
    }

    return crc;
}


USER_CODE void OSN()
{
	TEST_BEGIN
	vector<ST_MEAS_RESULT>  N_PPMU_RESULT;
	ppmu.Signal("ALLPIN").Connect().SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(2000);

	ppmu.Signal("VDD").SetMode("FIMV").CurrForce(-100.0e-6).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(0,N_PPMU_RESULT,1);
	ppmu.Signal("VDD").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("P02").SetMode("FIMV").CurrForce(-100.0e-6).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(1,N_PPMU_RESULT,1);
	ppmu.Signal("P02").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("P03").SetMode("FIMV").CurrForce(-100.0e-6).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(2,N_PPMU_RESULT,1);
	ppmu.Signal("P03").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();


	ppmu.Signal("P09").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(8,N_PPMU_RESULT,1);
	ppmu.Signal("P09").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();


	ppmu.Signal("P05").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(4,N_PPMU_RESULT,1);
	ppmu.Signal("P05").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("PDA").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(5,N_PPMU_RESULT,1);
	ppmu.Signal("PDA").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("P07").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(6,N_PPMU_RESULT,1);
	ppmu.Signal("P07").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("P08").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(7,N_PPMU_RESULT,1);
	ppmu.Signal("P08").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();


	ppmu.Signal("VPP").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(3,N_PPMU_RESULT,1);
	ppmu.Signal("VPP").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("P10").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(9,N_PPMU_RESULT,1);
	ppmu.Signal("P10").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("P11").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(10,N_PPMU_RESULT,1);
	ppmu.Signal("P11").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("P12").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(11,N_PPMU_RESULT,1);
	ppmu.Signal("P12").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("P13").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(12,N_PPMU_RESULT,1);
	ppmu.Signal("P13").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("PDA").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(13,N_PPMU_RESULT,1);
	ppmu.Signal("PDA").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("PCK").SetMode("FIMV").CurrForce(-1.0e-4).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(N_PPMU_RESULT);
	binObj.CheckResultAndBin(14,N_PPMU_RESULT,1);
	ppmu.Signal("PCK").SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();

	ppmu.Signal("ALLPIN").Connect().SetMode("FVMI").VoltForce(0.0).CurrRange(2.0e-4).VoltClamp(2,-2).Execute();
	sys.DelayUs(2000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	sys.DelayUs(2000);
	ppmu.Signal("ALLPIN").DisConnect();


	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}
int Minfind(ULONG ulSite,  ULONG trimdata[], double volt_test[],double VOLTref, int length)
{
	ULONG MINTrimData[length]={0};
	double MINvolt_test[length]={0};
	double MINvoltref;
//	ULONG MINnum = num;

//	ULONG length=sizeof(trimdata)/sizeof(ULONG);
	memcpy(MINTrimData,trimdata,length*sizeof(ULONG));
	memcpy(MINvolt_test,volt_test,length*sizeof(double));
	minOK_Count[ulSite]=0;

    MINvoltref=VOLTref;

    double MINVOT;
    MINVOT=fabs(MINvolt_test[0]-MINvoltref);
	for(int j=1; j<length-1; j++)
	{
		if(fabs(MINvolt_test[j] - MINvoltref) < MINVOT)
		{
			MINVOT = fabs(MINvolt_test[j]-MINvoltref);
			minOK_Count[ulSite]=j;
		}

	}
	return minOK_Count[ulSite];
}
USER_CODE void Vvref_test() {
	TEST_BEGIN
	vector<ST_MEAS_RESULT> PpmuResult1;
	vector<ST_MEAS_RESULT> PpmuResult;

	gVrefData[0] = 0x0000;
	gVrefData[1] = 0x0000;

	iCount = 0;
	double dbCurvolt[64] = {0.0};

	ULONG MIN_trimData[64][6]={0};
	double MIN_CurVolt[64][6]={0};
	int MIN_i = 0;

	ULONG ulTrimData[64] ={0x0000}, ulTrimStart[64] = {0x00FF}; // ulTrimEnd = 0x3FFD;
	ulTrimDataOk1 = 0, ulTrimDataOk2 = 0;
	dbTrimValue1 = 0, dbTrimValue2 = 0;
	ppmu.Signal("DIO_TRIM").Connect();
	ppmu.Signal("PIN_VREF").Connect();
	sys.DelayUs(2000);

//	Plvl_Set("DIO_TRIM_GRP");

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(1500);


	//first time: run pat, and meas volt of P09
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(1500);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(2.5).SetVol(2.0).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(1500);

	dio.Pattern("Vvref_Pat").Test();
	sys.DelayUs(10000);

	ppmu.Signal("PIN_VREF").SetMode("FNMV").Execute();
	sys.DelayUs(1000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);

//	FOR_EACH_SITE_START(SITE,SITE_ID)
//
//	sys.PrintDatalog("SITE[%d] Vref first time (0000 0000 1111 1111--00FF) means volt(PIN_VREF): %f \n", SITE_ID, PpmuResult1[SITE].dbValue);
//	NEXT_SITE
	//update pat data, run pat, and meas volt of PIN_VREF
	//////////////////////////////////////////////////////////
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen,i = 0;
	string strData;

	ulTrimData[64] = {0x0000};  //0000 0000 0000 0000   初始值

	for(i = 0;i<4;i++)
	{
		FOR_EACH_SITE_START(SITE,SITE_ID)
		ulTrimData[SITE_ID] += dataPlaseBG1[i]; //bit 2置1  12
		NEXT_SITE

		TrimvoltProcess(ulTrimData, SITE_ID, "Vvref_Pat", dbCurvolt);

		FOR_EACH_SITE_START(SITE,SITE_ID)
		MIN_trimData[SITE_ID][i]=ulTrimData[SITE_ID];
		MIN_CurVolt[SITE_ID][i]=dbCurvolt[SITE_ID];
		if(dbCurvolt[SITE_ID] < Vvref_VOLT125)
		{
			ulTrimData[SITE_ID] -= dataPlaseBG1[i];
		}
		NEXT_SITE
	}


sys.DelayUs(2);

	FOR_EACH_SITE_START(SITE,SITE_ID)
    int ulTrimDataLength = sizeof(MIN_trimData[0])/sizeof(MIN_trimData[0][0]);// 数组总字节大小/数组元素字节大小==数组长度
    MIN_i = Minfind(SITE_ID, MIN_trimData, MIN_CurVolt, Vvref_VOLT125 ,ulTrimDataLength);//与参考电压值1.0差距最小的电压值
    gVrefData[SITE_ID] = MIN_trimData[SITE_ID][MIN_i];//最小电压值
    sys.PrintDatalog(" gVrefData %x \n", gVrefData[SITE_ID]);
    gVrefDataN[SITE_ID] = (0x1E00&(0x1E00-gVrefData[SITE_ID]));//？？？
    sys.PrintDatalog("gVrefDataN %x \n", gVrefDataN[SITE_ID]);
    NEXT_SITE

//the last time trim and judge
	memset(ch, 0, UPDATE_VCT_NUM16);
    FOR_EACH_SITE_START(SITE,SITE_ID)
	strData = to_string(gVrefData[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("Vvref_Pat", iLen, ch);
	NEXT_SITE

	Plvl_Set("PIN_VREF");
	dio.Pattern("Vvref_Pat").Run("cycle_start").WaitEnd();
//	sys.DelayUs(2000);

	PpmuResult1.clear();
//	ppmu.Signal("PIN_VREF").SetMode("FNMV").CurrRange(0.04).Execute();
	ppmu.Signal("PIN_VREF").SetMode("FNMV").Execute();
	sys.DelayUs(1000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);
//	FOR_EACH_SITE_START(SITE,SITE_ID)
	sys.PrintDatalog("SITE[%d] Vvref The last time meas volt(PIN_VREF): %f, TrimData:%x \n", SITE_ID, PpmuResult1[0].dbValue, gVrefData[SITE_ID]);
//	NEXT_SITE
	binObj.CheckResultAndBin(0, PpmuResult1, 1); //select site时,必须放在for_each里面

	SITE_DOUBLE PpmuResult2;

	FOR_EACH_SITE_START(SITE,SITE_ID)
	PpmuResult2[SITE_ID] = (double)gVrefData[SITE_ID];

	IP_VALUE[SITE_ID] = gVrefData[SITE_ID];
	NEXT_SITE

	binObj.CheckResultAndBin(1, PpmuResult2);

	FOR_EACH_SITE_START(SITE,SITE_ID)
	cout << hex << "!!!!!!!!!!!![] Site" << SITE_ID << ", trimdataT:" << gVrefData[SITE_ID] << endl;
	NEXT_SITE

	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

//NEXT_SITE

	FOR_EACH_SITE_START(SITE,SITE_ID)
	ulTrimData[SITE_ID] = ulTrimStart[SITE_ID];
//	char ch[UPDATE_VCT_NUM16];
//	char *chEnd, *pch;
//	const char *pTemp;
//	int iLen;
//	string strData;

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("Vvref_Pat", iLen, ch);
	NEXT_SITE
	dio.Pattern("VDD_init").Test();
//	sys.DelayUs(10000);

	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	ppmu.Signal("ALLPIN").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

//USER_CODE void Vvref_test1() {
//	TEST_BEGIN
//	vector<ST_MEAS_RESULT> PpmuResult1;
//	vector<ST_MEAS_RESULT> PpmuResult;
//
//	gVrefData[0] = 0x0000;
//	gVrefData[1] = 0x0000;
//
//	iCount = 0;
//	double dbCurvolt[64] = {0.0};
//
//	ULONG MIN_trimData[64][6]={0};
//	double MIN_CurVolt[64][6]={0};
//	int MIN_i = 0;
//
//	ULONG ulTrimData[64] ={0x0000}, ulTrimStart[64] = {0x00FF}; // ulTrimEnd = 0x3FFD;
//	ulTrimDataOk1 = 0, ulTrimDataOk2 = 0;
//	dbTrimValue1 = 0, dbTrimValue2 = 0;
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PIN_VREF").Connect();
//	sys.DelayUs(2000);
//
////	Plvl_Set("DIO_TRIM_GRP");
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(1500);
//
//
//	//first time: run pat, and meas volt of P09
////	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(1500);
//	pinlevel.Signal("VDD").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(2.5).SetVol(2.0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(1500);
//
//	dio.Pattern("Vvref_Pat").Test();
//	sys.DelayUs(10000);
//
//	ppmu.Signal("PIN_VREF").SetMode("FNMV").Execute();
//	sys.DelayUs(1000);
//	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);
//
////	FOR_EACH_SITE_START(SITE,SITE_ID)
////
////	sys.PrintDatalog("SITE[%d] Vref first time (0000 0000 1111 1111--00FF) means volt(PIN_VREF): %f \n", SITE_ID, PpmuResult1[SITE].dbValue);
////	NEXT_SITE
//	//update pat data, run pat, and meas volt of PIN_VREF
//	//////////////////////////////////////////////////////////
//	char ch[UPDATE_VCT_NUM16];
//	char *chEnd, *pch;
//	const char *pTemp;
//	int iLen;
//	string strData;
//
//	ulTrimData[64] = {0x0000};  //0000 0000 0000 0000   初始值
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	ulTrimData[SITE_ID] = ulTrimData[SITE_ID] | 0x0800; //bit 2置1  12
//	//ulTrimData[SITE_ID] = 0x1000;
//	NEXT_SITE
//
//	TrimvoltProcess(ulTrimData, SITE_ID, "Vvref_Pat", dbCurvolt);
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	MIN_trimData[SITE_ID][1]=ulTrimData[SITE_ID];
//	MIN_CurVolt[SITE_ID][1]=dbCurvolt[SITE_ID];
//	if(dbCurvolt[SITE_ID] < Vvref_VOLT125)
//	{
//		ulTrimData[SITE_ID] = ulTrimData[SITE_ID] & 0xF7FF; //bit 2置0
//	}
//	NEXT_SITE
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	ulTrimData[SITE_ID] = ulTrimData[SITE_ID] | 0x0400; //bit 1置1  11
//	NEXT_SITE
//
//	TrimvoltProcess(ulTrimData, SITE_ID, "Vvref_Pat", dbCurvolt);
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	MIN_trimData[SITE_ID][2]=ulTrimData[SITE_ID];
//	MIN_CurVolt[SITE_ID][2]=dbCurvolt[SITE_ID];
//	if(dbCurvolt[SITE_ID] < Vvref_VOLT125)
//	{
//		ulTrimData[SITE_ID] = ulTrimData[SITE_ID] & 0xFBFF; //bit 1置0
//	}
//	NEXT_SITE
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	ulTrimData[SITE_ID] = ulTrimData[SITE_ID] | 0x0200; //bit 0置1
//	NEXT_SITE
//
//	TrimvoltProcess(ulTrimData, SITE_ID, "Vvref_Pat", dbCurvolt);
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	MIN_trimData[SITE_ID][3]=ulTrimData[SITE_ID];
//	MIN_CurVolt[SITE_ID][3]=dbCurvolt[SITE_ID];
//	if(dbCurvolt[SITE_ID] < Vvref_VOLT125)
//	{
//		ulTrimData[SITE_ID] = ulTrimData[SITE_ID] & 0xFFFF; //bit 0置0
//	}
//	NEXT_SITE
//
//	TrimvoltProcess(ulTrimData, SITE_ID, "Vvref_Pat", dbCurvolt);
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	MIN_trimData[SITE_ID][4]=ulTrimData[SITE_ID];
//	MIN_CurVolt[SITE_ID][4]=dbCurvolt[SITE_ID];
//	NEXT_SITE
//
//sys.DelayUs(2);
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//    int ulTrimDataLength = sizeof(MIN_trimData[0])/sizeof(MIN_trimData[0][0]);// 数组总字节大小/数组元素字节大小==数组长度
//    MIN_i = Minfind(SITE_ID, MIN_trimData, MIN_CurVolt, Vvref_VOLT125 ,ulTrimDataLength);//与参考电压值1.0差距最小的电压值
//    gVrefData[SITE_ID] = MIN_trimData[SITE_ID][MIN_i];//最小电压值
////    sys.PrintDatalog(" gVrefData %x \n", gVrefData[SITE_ID]);
//    gVrefDataN[SITE_ID] = (0x0E00&(0x0E00-gVrefData[SITE_ID]));//？？？
////    sys.PrintDatalog("gVrefDataN %x \n", gVrefDataN[SITE_ID]);
//    NEXT_SITE
//
////the last time trim and judge
//	memset(ch, 0, UPDATE_VCT_NUM16);
//    FOR_EACH_SITE_START(SITE,SITE_ID)
//	strData = to_string(gVrefData[SITE_ID]);
//	pTemp = strData.c_str();
//	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
//	pch = &ch[0];
//	iLen = strlen(pch);
//
//	UpdateVvref("Vvref_Pat", iLen, ch);
//	NEXT_SITE
//
//	Plvl_Set("PIN_VREF");
//	dio.Pattern("Vvref_Pat").Run("cycle_start").WaitEnd();
////	sys.DelayUs(2000);
//
//	PpmuResult1.clear();
////	ppmu.Signal("PIN_VREF").SetMode("FNMV").CurrRange(0.04).Execute();
//	ppmu.Signal("PIN_VREF").SetMode("FNMV").Execute();
//	sys.DelayUs(1000);
//	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);
////	FOR_EACH_SITE_START(SITE,SITE_ID)
////	sys.PrintDatalog("SITE[%d] Vvref The last time meas volt(PIN_VREF): %f, TrimData:%x \n", SITE_ID, PpmuResult1[0].dbValue, gVrefData[SITE_ID]);
////	NEXT_SITE
//	binObj.CheckResultAndBin(0, PpmuResult1, 1); //select site时,必须放在for_each里面
//
//	SITE_DOUBLE PpmuResult2;
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	PpmuResult2[SITE_ID] = (double)gVrefData[SITE_ID];
//
//	IP_VALUE[SITE_ID] = gVrefData[SITE_ID];
//	NEXT_SITE
//
//	binObj.CheckResultAndBin(1, PpmuResult2);
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	cout << hex << "!!!!!!!!!!!![] Site" << SITE_ID << ", trimdataT:" << gVrefData[SITE_ID] << endl;
//	NEXT_SITE
//
//	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
////NEXT_SITE
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	ulTrimData[SITE_ID] = ulTrimStart[SITE_ID];
////	char ch[UPDATE_VCT_NUM16];
////	char *chEnd, *pch;
////	const char *pTemp;
////	int iLen;
////	string strData;
//
//	memset(ch, 0, UPDATE_VCT_NUM16);
//	strData = to_string(ulTrimData[SITE_ID]);
//	pTemp = strData.c_str();
//	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
//	pch = &ch[0];
//	iLen = strlen(pch);
//
//	UpdateVvref("Vvref_Pat", iLen, ch);
//	NEXT_SITE
//	dio.Pattern("VDD_init").Test();
////	sys.DelayUs(10000);
//
//	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
//	ppmu.Signal("ALLPIN").DisConnect();
//	sys.DelayUs(2000);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}
//SITE_LONG stTMUMeas_CodeH;
//SITE_LONG stTMUMeas_CodeL;

USER_CODE void Hrc_test() {
	TEST_BEGIN
	vector<ST_MEAS_RESULT> stTMUMeasResult;

//	gFrefDataH[0] = {0x4400};
	iCount = 0;
	double dbCurfre[64] = {0.0};

	ULONG MIN_trimData[64][16]={0};
	double MIN_CurVolt[64][16]={0};
	int MIN_i = 0,cnt_i = 0;

	ULONG ulTrimData[64] = {0x0000}, ulTrimStart[64] = {0x0000}; // ulTrimEnd = 0x3FFD;

//	tmu.SetupTmuOutputCh(1, 1, 0, 6);tmu.SetupTmuOutputCh(1, 1, 1, 14);tmu.SetupTmuOutputCh(1, 1, 2, 22);tmu.SetupTmuOutputCh(1, 1, 3, 30);
//	tmu.SetupTmuOutputCh(1, 2, 0, 6);tmu.SetupTmuOutputCh(1, 2, 1, 14);tmu.SetupTmuOutputCh(1, 2, 2, 22);tmu.SetupTmuOutputCh(1, 2, 3, 30);
//	tmu.SetupTmuOutputCh(1, 3, 0, 6);tmu.SetupTmuOutputCh(1, 3, 1, 14);tmu.SetupTmuOutputCh(1, 3, 2, 22);tmu.SetupTmuOutputCh(1, 3, 3, 30);
//	tmu.SetupTmuOutputCh(1, 4, 0, 6);tmu.SetupTmuOutputCh(1, 4, 1, 14);tmu.SetupTmuOutputCh(1, 4, 2, 22);tmu.SetupTmuOutputCh(1, 4, 3, 30);
//	tmu.SetupTmuOutputCh(1, 5, 0, 6);tmu.SetupTmuOutputCh(1, 5, 1, 14);tmu.SetupTmuOutputCh(1, 5, 2, 22);tmu.SetupTmuOutputCh(1, 5, 3, 30);
//	tmu.SetupTmuOutputCh(1, 6, 0, 6);tmu.SetupTmuOutputCh(1, 6, 1, 14);tmu.SetupTmuOutputCh(1, 6, 2, 22);tmu.SetupTmuOutputCh(1, 6, 3, 30);
//	tmu.SetupTmuOutputCh(1, 7, 0, 6);tmu.SetupTmuOutputCh(1, 7, 1, 14);tmu.SetupTmuOutputCh(1, 7, 2, 22);tmu.SetupTmuOutputCh(1, 7, 3, 30);
//	tmu.SetupTmuOutputCh(1, 8, 0, 6);tmu.SetupTmuOutputCh(1, 8, 1, 14);tmu.SetupTmuOutputCh(1, 8, 2, 22);tmu.SetupTmuOutputCh(1, 8, 3, 30);
//	tmu.SetupTmuOutputCh(1, 9, 0, 6);tmu.SetupTmuOutputCh(1, 9, 1, 14);tmu.SetupTmuOutputCh(1, 9, 2, 22);tmu.SetupTmuOutputCh(1, 9, 3, 30);
//	tmu.SetupTmuOutputCh(1, 10, 0, 6);tmu.SetupTmuOutputCh(1, 10, 1, 14);tmu.SetupTmuOutputCh(1, 10, 2, 22);tmu.SetupTmuOutputCh(1, 10, 3, 30);
//	tmu.SetupTmuOutputCh(1, 11, 0, 6);tmu.SetupTmuOutputCh(1, 11, 1, 14);tmu.SetupTmuOutputCh(1, 11, 2, 22);tmu.SetupTmuOutputCh(1, 11, 3, 30);
//	tmu.SetupTmuOutputCh(1, 12, 0, 6);tmu.SetupTmuOutputCh(1, 12, 1, 14);tmu.SetupTmuOutputCh(1, 12, 2, 22);tmu.SetupTmuOutputCh(1, 12, 3, 30);
//	tmu.SetupTmuOutputCh(1, 13, 0, 6);tmu.SetupTmuOutputCh(1, 13, 1, 14);tmu.SetupTmuOutputCh(1, 13, 2, 22);tmu.SetupTmuOutputCh(1, 13, 3, 30);
//	tmu.SetupTmuOutputCh(1, 14, 0, 6);tmu.SetupTmuOutputCh(1, 14, 1, 14);tmu.SetupTmuOutputCh(1, 14, 2, 22);tmu.SetupTmuOutputCh(1, 14, 3, 30);
//	tmu.SetupTmuOutputCh(1, 15, 0, 6);tmu.SetupTmuOutputCh(1, 15, 1, 14);tmu.SetupTmuOutputCh(1, 15, 2, 22);tmu.SetupTmuOutputCh(1, 15, 3, 30);
//	tmu.SetupTmuOutputCh(1, 16, 0, 6);tmu.SetupTmuOutputCh(1, 16, 1, 14);tmu.SetupTmuOutputCh(1, 16, 2, 22);tmu.SetupTmuOutputCh(1, 16, 3, 30);
	tmu.SetupTmuOutputCh(1, 1, 2, 29);
	tmu.SetupTmuOutputCh(1, 1, 3, 21);
	tmu.Connect("TMU_CH29");
	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(2.5).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(50).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();

	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);


	//first time: run pat, and meas volt of P09
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

	dio.Pattern("Hrc_Pat").Test();
//	sys.DelayUs(10000);

	tmu.Freq().Signal("TMU_CH29").StartMeas();
	tmu.Freq().Signal("TMU_CH29").GetMeasResult(stTMUMeasResult);
//	tmu.DisConnect("TMU_CH09");
//	sys.DelayUs(10000);
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	sys.PrintDatalog("SITE[%d] Fref first time (0100 0100 0000 0000 --4400) means freq (): %f \n", SITE_ID, stTMUMeasResult[SITE].dbValue);
//	NEXT_SITE
	//update pat data, run pat, and meas volt of PIN_VREF
	//////////////////////////////////////////////////////////
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;
	FOR_EACH_SITE_START(SITE,SITE_ID)
	ulTrimData[SITE_ID] = 0;
	NEXT_SITE
	  //0000 010 0 0000 0000   初始值
	for(cnt_i = 0;cnt_i<9;cnt_i++)
	{
		FOR_EACH_SITE_START(SITE,SITE_ID)
		ulTrimData[SITE_ID] |= (1<<(9-cnt_i-1)); //bit 8置1
		NEXT_SITE
		TrimfreProcessH(ulTrimData, SITE_ID, "Hrc_Pat", dbCurfre);
		FOR_EACH_SITE_START(SITE,SITE_ID)
		MIN_trimData[SITE_ID][cnt_i]=ulTrimData[SITE_ID];
		MIN_CurVolt[SITE_ID][cnt_i]=dbCurfre[SITE_ID];
		if(dbCurfre[SITE_ID] > FvrefH)
		{
			ulTrimData[SITE_ID] &= ~(1<<(9-cnt_i-1)); //bit 8置0
		}
		NEXT_SITE
	}


sys.DelayUs(2);
	FOR_EACH_SITE_START(SITE,SITE_ID)
    int ulTrimDataLength = sizeof(MIN_trimData[0])/sizeof(MIN_trimData[0][0]);
    MIN_i = Minfind(SITE_ID, MIN_trimData, MIN_CurVolt, FvrefH ,ulTrimDataLength);
    gFrefDataH[SITE_ID] = MIN_trimData[SITE_ID][MIN_i];
    gFrefDataHN[SITE_ID] = ( 0x1FF & (0x1FF-gFrefDataH[SITE_ID]));
    sys.PrintDatalog("gFrefDataHN %x,gFrefDataH %x \n", gFrefDataHN[SITE_ID],gFrefDataH[SITE_ID]);
    NEXT_SITE
//the last time trim and judge
	memset(ch, 0, UPDATE_VCT_NUM16);

    FOR_EACH_SITE_START(SITE,SITE_ID)
	strData = to_string(gFrefDataH[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("Hrc_Pat", iLen, ch);
	NEXT_SITE

//	Plvl_Set("P09");
	dio.Pattern("Hrc_Pat").Run("cycle_start").WaitEnd();
//	sys.DelayUs(10000);

	tmu.Freq().Signal("TMU_CH29").StartMeas();
	tmu.Freq().Signal("TMU_CH29").GetMeasResult(stTMUMeasResult);
	tmu.DisConnect("TMU_CH29");
	sys.DelayUs(2000);

	FOR_EACH_SITE_START(SITE,SITE_ID)
//	sys.PrintDatalog("SITE[%d] Fvref The last time P07: %f, TrimData:%x, gTrimOK_Count:%d \n", SITE_ID, stTMUMeasResult[SITE].dbValue, gFrefDataH[SITE_ID],gFTrimOK_Count[SITE_ID]);

	gTrimOK_Count[SITE_ID] = 0;
	NEXT_SITE

	binObj.CheckResultAndBin(0, stTMUMeasResult, 1); //select site时,必须放在for_each里面

	SITE_DOUBLE stTMUMeas_Code;
	FOR_EACH_SITE_START(SITE,SITE_ID)
	stTMUMeas_Code[SITE_ID] = (LONG)gFrefDataH[SITE_ID];
	HRC_VALUE[SITE_ID] = gFrefDataH[SITE_ID];
//	cout << hex << "!!!!!!!!!!!![] Site" << SITE_ID << ", trimdataT:" << gFrefDataH[SITE_ID] << endl;
	NEXT_SITE

	binObj.CheckResultAndBin(1, stTMUMeas_Code);

	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

//    NEXT_SITE

	FOR_EACH_SITE_START(SITE,SITE_ID)
	ulTrimData[SITE_ID] = ulTrimStart[SITE_ID];
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("Hrc_Pat", iLen, ch);
	NEXT_SITE
	dio.Pattern("VDD_init").Test();
//	sys.DelayUs(10000);

//NEXT_SITE

	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	tmu.DisConnect("TMU_CH29");
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}
USER_CODE void Lrc_test() {
	TEST_BEGIN

	vector<ST_MEAS_RESULT> stTMUMeasResult;
	double dbCurfre[64] = {0.0};

	ULONG MIN_trimData[64][16]={0};
	double MIN_CurVolt[64][16]={0};
	int MIN_i = 0;

	ULONG ulTrimData[64] = {0x0000}, ulTrimStart[64] = {0x0000}; // ulTrimEnd = 0x3FFD;

//	tmu.SetupTmuOutputCh(1, 1, 0, 6);tmu.SetupTmuOutputCh(1, 1, 1, 14);tmu.SetupTmuOutputCh(1, 1, 2, 22);tmu.SetupTmuOutputCh(1, 1, 3, 30);
//	tmu.SetupTmuOutputCh(1, 2, 0, 6);tmu.SetupTmuOutputCh(1, 2, 1, 14);tmu.SetupTmuOutputCh(1, 2, 2, 22);tmu.SetupTmuOutputCh(1, 2, 3, 30);
//	tmu.SetupTmuOutputCh(1, 3, 0, 6);tmu.SetupTmuOutputCh(1, 3, 1, 14);tmu.SetupTmuOutputCh(1, 3, 2, 22);tmu.SetupTmuOutputCh(1, 3, 3, 30);
//	tmu.SetupTmuOutputCh(1, 4, 0, 6);tmu.SetupTmuOutputCh(1, 4, 1, 14);tmu.SetupTmuOutputCh(1, 4, 2, 22);tmu.SetupTmuOutputCh(1, 4, 3, 30);
//	tmu.SetupTmuOutputCh(1, 5, 0, 6);tmu.SetupTmuOutputCh(1, 5, 1, 14);tmu.SetupTmuOutputCh(1, 5, 2, 22);tmu.SetupTmuOutputCh(1, 5, 3, 30);
//	tmu.SetupTmuOutputCh(1, 6, 0, 6);tmu.SetupTmuOutputCh(1, 6, 1, 14);tmu.SetupTmuOutputCh(1, 6, 2, 22);tmu.SetupTmuOutputCh(1, 6, 3, 30);
//	tmu.SetupTmuOutputCh(1, 7, 0, 6);tmu.SetupTmuOutputCh(1, 7, 1, 14);tmu.SetupTmuOutputCh(1, 7, 2, 22);tmu.SetupTmuOutputCh(1, 7, 3, 30);
//	tmu.SetupTmuOutputCh(1, 8, 0, 6);tmu.SetupTmuOutputCh(1, 8, 1, 14);tmu.SetupTmuOutputCh(1, 8, 2, 22);tmu.SetupTmuOutputCh(1, 8, 3, 30);
//	tmu.SetupTmuOutputCh(1, 9, 0, 6);tmu.SetupTmuOutputCh(1, 9, 1, 14);tmu.SetupTmuOutputCh(1, 9, 2, 22);tmu.SetupTmuOutputCh(1, 9, 3, 30);
//	tmu.SetupTmuOutputCh(1, 10, 0, 6);tmu.SetupTmuOutputCh(1, 10, 1, 14);tmu.SetupTmuOutputCh(1, 10, 2, 22);tmu.SetupTmuOutputCh(1, 10, 3, 30);
//	tmu.SetupTmuOutputCh(1, 11, 0, 6);tmu.SetupTmuOutputCh(1, 11, 1, 14);tmu.SetupTmuOutputCh(1, 11, 2, 22);tmu.SetupTmuOutputCh(1, 11, 3, 30);
//	tmu.SetupTmuOutputCh(1, 12, 0, 6);tmu.SetupTmuOutputCh(1, 12, 1, 14);tmu.SetupTmuOutputCh(1, 12, 2, 22);tmu.SetupTmuOutputCh(1, 12, 3, 30);
//	tmu.SetupTmuOutputCh(1, 13, 0, 6);tmu.SetupTmuOutputCh(1, 13, 1, 14);tmu.SetupTmuOutputCh(1, 13, 2, 22);tmu.SetupTmuOutputCh(1, 13, 3, 30);
//	tmu.SetupTmuOutputCh(1, 14, 0, 6);tmu.SetupTmuOutputCh(1, 14, 1, 14);tmu.SetupTmuOutputCh(1, 14, 2, 22);tmu.SetupTmuOutputCh(1, 14, 3, 30);
//	tmu.SetupTmuOutputCh(1, 15, 0, 6);tmu.SetupTmuOutputCh(1, 15, 1, 14);tmu.SetupTmuOutputCh(1, 15, 2, 22);tmu.SetupTmuOutputCh(1, 15, 3, 30);
//	tmu.SetupTmuOutputCh(1, 16, 0, 6);tmu.SetupTmuOutputCh(1, 16, 1, 14);tmu.SetupTmuOutputCh(1, 16, 2, 22);tmu.SetupTmuOutputCh(1, 16, 3, 30);

	tmu.SetupTmuOutputCh(1, 1, 2, 29);
	tmu.SetupTmuOutputCh(1, 1, 3, 21);
	tmu.Connect("TMU_CH29");
//	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(2.5).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(50).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();
//	sys.DelayUs(2000);
	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
	sys.DelayUs(2000);

//	Plvl_Set("DIO_TRIM_GRP");
	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(2.5).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(50).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(3000);


	//first time: run pat, and meas volt of P09
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

	dio.Pattern("Lrc_Pat").Test();
//	sys.DelayUs(10000);

	tmu.Freq().Signal("TMU_CH29").StartMeas();
	tmu.Freq().Signal("TMU_CH29").GetMeasResult(stTMUMeasResult);
//	tmu.DisConnect("TMU_CH09");
//	sys.DelayUs(10000);

//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	sys.PrintDatalog("SITE[%d] Fref first time (0100 0100 0000 0000 --4400) means freq (): %f \n", SITE_ID, stTMUMeasResult[SITE].dbValue);
//	NEXT_SITE
	//update pat data, run pat, and meas volt of PIN_VREF
	//////////////////////////////////////////////////////////
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;
	FOR_EACH_SITE_START(SITE,SITE_ID)
	ulTrimData[SITE_ID] = {0x0000};  //0000 010 0 0000 0000   初始值
	NEXT_SITE
	for(int i=0;i<3;i++)
	{
		ulTrimData[SITE_ID] += dataPlaselck[i];
		TrimfreProcessL(ulTrimData, SITE_ID, "Lrc_Pat", dbCurfre);
		FOR_EACH_SITE_START(SITE,SITE_ID)
		MIN_trimData[SITE_ID][i]=ulTrimData[SITE_ID];
		MIN_CurVolt[SITE_ID][i]=dbCurfre[SITE_ID];
		if(dbCurfre[SITE_ID] < FvrefL)
		{
			ulTrimData[SITE_ID] -= dataPlaselck[i];

		}
		NEXT_SITE
	}
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	sys.PrintDatalog("SITE[%d] \n", SITE_ID);
//	NEXT_SITE
sys.DelayUs(2);
	FOR_EACH_SITE_START(SITE,SITE_ID)
	int ulTrimDataLength = sizeof(MIN_trimData[0])/sizeof(MIN_trimData[0][0]);
    MIN_i = Minfind(SITE_ID, MIN_trimData, MIN_CurVolt, FvrefL ,ulTrimDataLength);
    gFrefDataL[SITE_ID] = MIN_trimData[SITE_ID][MIN_i];
    sys.PrintDatalog("gFrefDataL %x \n", gFrefDataL[SITE_ID]);
    gFrefDataLN[SITE_ID] = (0x7 & (0x7-gFrefDataL[SITE_ID]));
    sys.PrintDatalog("gVrefDataLN %x \n", gFrefDataLN[SITE_ID]);
    NEXT_SITE
//the last time trim and judge
	memset(ch, 0, UPDATE_VCT_NUM16);
    FOR_EACH_SITE_START(SITE,SITE_ID)
	strData = to_string(gFrefDataL[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("Lrc_Pat", iLen, ch);
	NEXT_SITE

	dio.Pattern("Lrc_Pat").Run("cycle_start").WaitEnd();
//	sys.DelayUs(10000);

	tmu.Freq().StartMeas();
	tmu.Freq().GetMeasResult(stTMUMeasResult);
	tmu.DisConnect("TMU_CH29");
	sys.DelayUs(2000);

	FOR_EACH_SITE_START(SITE,SITE_ID)
//	sys.PrintDatalog("SITE[%d] Fvref The last time P07: %f, TrimData:%x, gTrimOK_Count:%d \n", SITE_ID, stTMUMeasResult[SITE].dbValue, gFrefDataH[SITE_ID],gFTrimOK_Count[SITE_ID]);

	gTrimOK_Count[SITE_ID] = 0;
	NEXT_SITE

	binObj.CheckResultAndBin(0, stTMUMeasResult, 1); //select site时,必须放在for_each里面
	SITE_DOUBLE stTMUMeas_Code;
	FOR_EACH_SITE_START(SITE,SITE_ID)
	stTMUMeas_Code[SITE_ID] = (LONG)gFrefDataL[SITE_ID];
	LRC_VALUE[SITE_ID] = gFrefDataL[SITE_ID];
//	cout << hex << "!!!!!!!!!!!![] Site" << SITE_ID << ", trimdataT:" << gFrefDataL[SITE_ID] << endl;
	NEXT_SITE

	binObj.CheckResultAndBin(1, stTMUMeas_Code);

	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

//NEXT_SITE
	FOR_EACH_SITE_START(SITE,SITE_ID)
	ulTrimData[SITE_ID] = ulTrimStart[SITE_ID];

	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("Lrc_Pat", iLen, ch);
	NEXT_SITE
	dio.Pattern("VDD_init").Test();
//	sys.DelayUs(10000);

	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

//USER_CODE void Test_vref_config() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_MEAS_RESULT> PpmuResult1;
//	vector<ST_DIO_RESULT> FailResult;
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	ppmu.Signal("PIN_VREF").Connect();
//	sys.DelayUs(2000);
//
//	ppmu.Signal("PIN_VREF").SetMode("FNMV").Execute();
//	sys.DelayUs(1000);
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(4.5).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	UpdatevrefPat("Vvref_Pat",SITE_ID);
//	NEXT_SITE
//
//	dio.Pattern("Vvref_Pat").Test();
//	sys.DelayUs(1000);
//
//	ppmu.Signal("PIN_VREF").SetMode("FNMV").CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);
//
////	FOR_EACH_SITE_START(SITE,SITE_ID)
////	sys.PrintDatalog("SITE[%d] Vvref The last time meas volt(PIN_VREF): %f, TrimData:%x \n", SITE_ID, PpmuResult1[SITE].dbValue, RDVrefDataN[SITE_ID]);
////	NEXT_SITE
//
//
//
//
//	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
//	ppmu.Signal("ALLPIN").DisConnect();
//	sys.DelayUs(2000);
//
//	binObj.CheckResultAndBin(0, PpmuResult1, 1); //select site时,必须放在for_each里面
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Test_hclk_test() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_MEAS_RESULT> stTMUMeasResult;
//
//	tmu.SetupTmuOutputCh(1, 1, 0, 6);tmu.SetupTmuOutputCh(1, 1, 1, 14);tmu.SetupTmuOutputCh(1, 1, 2, 22);tmu.SetupTmuOutputCh(1, 1, 3, 30);
//	tmu.SetupTmuOutputCh(1, 2, 0, 6);tmu.SetupTmuOutputCh(1, 2, 1, 14);tmu.SetupTmuOutputCh(1, 2, 2, 22);tmu.SetupTmuOutputCh(1, 2, 3, 30);
//	tmu.SetupTmuOutputCh(1, 3, 0, 6);tmu.SetupTmuOutputCh(1, 3, 1, 14);tmu.SetupTmuOutputCh(1, 3, 2, 22);tmu.SetupTmuOutputCh(1, 3, 3, 30);
//	tmu.SetupTmuOutputCh(1, 4, 0, 6);tmu.SetupTmuOutputCh(1, 4, 1, 14);tmu.SetupTmuOutputCh(1, 4, 2, 22);tmu.SetupTmuOutputCh(1, 4, 3, 30);
//	tmu.SetupTmuOutputCh(1, 5, 0, 6);tmu.SetupTmuOutputCh(1, 5, 1, 14);tmu.SetupTmuOutputCh(1, 5, 2, 22);tmu.SetupTmuOutputCh(1, 5, 3, 30);
//	tmu.SetupTmuOutputCh(1, 6, 0, 6);tmu.SetupTmuOutputCh(1, 6, 1, 14);tmu.SetupTmuOutputCh(1, 6, 2, 22);tmu.SetupTmuOutputCh(1, 6, 3, 30);
//	tmu.SetupTmuOutputCh(1, 7, 0, 6);tmu.SetupTmuOutputCh(1, 7, 1, 14);tmu.SetupTmuOutputCh(1, 7, 2, 22);tmu.SetupTmuOutputCh(1, 7, 3, 30);
//	tmu.SetupTmuOutputCh(1, 8, 0, 6);tmu.SetupTmuOutputCh(1, 8, 1, 14);tmu.SetupTmuOutputCh(1, 8, 2, 22);tmu.SetupTmuOutputCh(1, 8, 3, 30);
//	tmu.SetupTmuOutputCh(1, 9, 0, 6);tmu.SetupTmuOutputCh(1, 9, 1, 14);tmu.SetupTmuOutputCh(1, 9, 2, 22);tmu.SetupTmuOutputCh(1, 9, 3, 30);
//	tmu.SetupTmuOutputCh(1, 10, 0, 6);tmu.SetupTmuOutputCh(1, 10, 1, 14);tmu.SetupTmuOutputCh(1, 10, 2, 22);tmu.SetupTmuOutputCh(1, 10, 3, 30);
//	tmu.SetupTmuOutputCh(1, 11, 0, 6);tmu.SetupTmuOutputCh(1, 11, 1, 14);tmu.SetupTmuOutputCh(1, 11, 2, 22);tmu.SetupTmuOutputCh(1, 11, 3, 30);
//	tmu.SetupTmuOutputCh(1, 12, 0, 6);tmu.SetupTmuOutputCh(1, 12, 1, 14);tmu.SetupTmuOutputCh(1, 12, 2, 22);tmu.SetupTmuOutputCh(1, 12, 3, 30);
//	tmu.SetupTmuOutputCh(1, 13, 0, 6);tmu.SetupTmuOutputCh(1, 13, 1, 14);tmu.SetupTmuOutputCh(1, 13, 2, 22);tmu.SetupTmuOutputCh(1, 13, 3, 30);
//	tmu.SetupTmuOutputCh(1, 14, 0, 6);tmu.SetupTmuOutputCh(1, 14, 1, 14);tmu.SetupTmuOutputCh(1, 14, 2, 22);tmu.SetupTmuOutputCh(1, 14, 3, 30);
//	tmu.SetupTmuOutputCh(1, 15, 0, 6);tmu.SetupTmuOutputCh(1, 15, 1, 14);tmu.SetupTmuOutputCh(1, 15, 2, 22);tmu.SetupTmuOutputCh(1, 15, 3, 30);
//	tmu.SetupTmuOutputCh(1, 16, 0, 6);tmu.SetupTmuOutputCh(1, 16, 1, 14);tmu.SetupTmuOutputCh(1, 16, 2, 22);tmu.SetupTmuOutputCh(1, 16, 3, 30);
//
//	tmu.Connect("TMU_CH29");
//	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(2.5,3.0).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(10).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(5000);
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(4.5).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	FOR_EACH_SITE_START(SITE,SITE_ID)
////  sys.PrintDatalog("SITE[%d] ,RgFrefDataH:%x,RgFrefDataHN:%x \n", SITE_ID,RDgFrefDataH[SITE_ID],gFrefDataH[SITE_ID]);
//
//	UpdateHCKPat("Hrc_Pat",SITE_ID);   // 此处向量替换的还是vref
//	NEXT_SITE
//
//	dio.Pattern("Hrc_Pat").Test();
//	sys.DelayUs(10000);
//
//	tmu.Freq().Signal("TMU_CH29").StartMeas();
//	tmu.Freq().GetMeasResult(stTMUMeasResult);
//	sys.DelayUs(10000);
//
//	tmu.DisConnect("TMU_CH29");
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	Hvalue[SITE_ID]=stTMUMeasResult[SITE].dbValue;
////	sys.PrintDatalog("SITE[%d]: FrefH (P07): %f \n", SITE_ID, stTMUMeasResult[SITE].dbValue);
////	sys.PrintDatalog("SITE[%d]: FrefH5v (P07): %f \n", SITE_ID, Hvalue[SITE_ID]);
//	NEXT_SITE
//
//
//
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//	sys.DelayUs(2000);
//
//	binObj.CheckResultAndBin(0, stTMUMeasResult, 1); //select site时,必须放在for_each里面
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Test_lclk_test() {
//	TEST_BEGIN
//
//	vector<ST_MEAS_RESULT> stTMUMeasResult;
//
//	tmu.SetupTmuOutputCh(1, 1, 0, 6);tmu.SetupTmuOutputCh(1, 1, 1, 14);tmu.SetupTmuOutputCh(1, 1, 2, 22);tmu.SetupTmuOutputCh(1, 1, 3, 30);
//	tmu.SetupTmuOutputCh(1, 2, 0, 6);tmu.SetupTmuOutputCh(1, 2, 1, 14);tmu.SetupTmuOutputCh(1, 2, 2, 22);tmu.SetupTmuOutputCh(1, 2, 3, 30);
//	tmu.SetupTmuOutputCh(1, 3, 0, 6);tmu.SetupTmuOutputCh(1, 3, 1, 14);tmu.SetupTmuOutputCh(1, 3, 2, 22);tmu.SetupTmuOutputCh(1, 3, 3, 30);
//	tmu.SetupTmuOutputCh(1, 4, 0, 6);tmu.SetupTmuOutputCh(1, 4, 1, 14);tmu.SetupTmuOutputCh(1, 4, 2, 22);tmu.SetupTmuOutputCh(1, 4, 3, 30);
//	tmu.SetupTmuOutputCh(1, 5, 0, 6);tmu.SetupTmuOutputCh(1, 5, 1, 14);tmu.SetupTmuOutputCh(1, 5, 2, 22);tmu.SetupTmuOutputCh(1, 5, 3, 30);
//	tmu.SetupTmuOutputCh(1, 6, 0, 6);tmu.SetupTmuOutputCh(1, 6, 1, 14);tmu.SetupTmuOutputCh(1, 6, 2, 22);tmu.SetupTmuOutputCh(1, 6, 3, 30);
//	tmu.SetupTmuOutputCh(1, 7, 0, 6);tmu.SetupTmuOutputCh(1, 7, 1, 14);tmu.SetupTmuOutputCh(1, 7, 2, 22);tmu.SetupTmuOutputCh(1, 7, 3, 30);
//	tmu.SetupTmuOutputCh(1, 8, 0, 6);tmu.SetupTmuOutputCh(1, 8, 1, 14);tmu.SetupTmuOutputCh(1, 8, 2, 22);tmu.SetupTmuOutputCh(1, 8, 3, 30);
//	tmu.SetupTmuOutputCh(1, 9, 0, 6);tmu.SetupTmuOutputCh(1, 9, 1, 14);tmu.SetupTmuOutputCh(1, 9, 2, 22);tmu.SetupTmuOutputCh(1, 9, 3, 30);
//	tmu.SetupTmuOutputCh(1, 10, 0, 6);tmu.SetupTmuOutputCh(1, 10, 1, 14);tmu.SetupTmuOutputCh(1, 10, 2, 22);tmu.SetupTmuOutputCh(1, 10, 3, 30);
//	tmu.SetupTmuOutputCh(1, 11, 0, 6);tmu.SetupTmuOutputCh(1, 11, 1, 14);tmu.SetupTmuOutputCh(1, 11, 2, 22);tmu.SetupTmuOutputCh(1, 11, 3, 30);
//	tmu.SetupTmuOutputCh(1, 12, 0, 6);tmu.SetupTmuOutputCh(1, 12, 1, 14);tmu.SetupTmuOutputCh(1, 12, 2, 22);tmu.SetupTmuOutputCh(1, 12, 3, 30);
//	tmu.SetupTmuOutputCh(1, 13, 0, 6);tmu.SetupTmuOutputCh(1, 13, 1, 14);tmu.SetupTmuOutputCh(1, 13, 2, 22);tmu.SetupTmuOutputCh(1, 13, 3, 30);
//	tmu.SetupTmuOutputCh(1, 14, 0, 6);tmu.SetupTmuOutputCh(1, 14, 1, 14);tmu.SetupTmuOutputCh(1, 14, 2, 22);tmu.SetupTmuOutputCh(1, 14, 3, 30);
//	tmu.SetupTmuOutputCh(1, 15, 0, 6);tmu.SetupTmuOutputCh(1, 15, 1, 14);tmu.SetupTmuOutputCh(1, 15, 2, 22);tmu.SetupTmuOutputCh(1, 15, 3, 30);
//	tmu.SetupTmuOutputCh(1, 16, 0, 6);tmu.SetupTmuOutputCh(1, 16, 1, 14);tmu.SetupTmuOutputCh(1, 16, 2, 22);tmu.SetupTmuOutputCh(1, 16, 3, 30);
//
//	tmu.Connect("TMU_CH29");
//	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(2.5).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(10).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(4.5).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	FOR_EACH_SITE_START(SITE,SITE_ID)
////  sys.PrintDatalog("SITE[%d] ,RgFrefDataL:%x,gFrefDataLN:%x \n", SITE_ID,RDgFrefDataL[SITE_ID],gFrefDataL[SITE_ID]);
//
//    UpdateLCKPat("Lrc_Pat",SITE_ID);   // 此处向量替换的还是vref
//    NEXT_SITE
//
//	dio.Pattern("Lrc_Pat").Test();
//	sys.DelayUs(20000);
//
//	tmu.Freq().Signal("TMU_CH29").StartMeas();
//	tmu.Freq().GetMeasResult(stTMUMeasResult);
//	sys.DelayUs(10000);
//
//	tmu.DisConnect("TMU_CH29");
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//
//	Lvalue[SITE_ID]=stTMUMeasResult[SITE].dbValue;
////	sys.PrintDatalog("SITE[%d]: FrefH (P07): %f \n", SITE_ID, stTMUMeasResult[SITE].dbValue);
////	sys.PrintDatalog("SITE[%d]: FrefH5V (P07): %f \n", SITE_ID, Lvalue[SITE_ID]);
//	NEXT_SITE
//
//
//
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//	sys.DelayUs(2000);
//
//	binObj.CheckResultAndBin(0, stTMUMeasResult, 1); //select site时,必须放在for_each里面
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

USER_CODE void Prog_ifen() {
	TEST_BEGIN

	// TODO Edit your code here

	vector<ST_DIO_RESULT> FailResult;

	ULONG tempDat[64] = {0};
	uint8_t g_caculateAdjChSum_buff[64][6] = {0};
	ULONG g_adjChSumCalt[64] = {0};

//	sys.PrintDatalog("SITE[%d] gFrefDataHN:%x \n", SITE_ID,gFrefDataHN[SITE_ID]);
//	sys.PrintDatalog("SITE[%d] gVrefDataN:%x \n", SITE_ID,gVrefDataN[SITE_ID]);
//	sys.PrintDatalog("SITE[%d] gFrefDataLN:%x \n", SITE_ID,gFrefDataLN[SITE_ID]);
	if(Verify_Config_bg_flag[SITE_ID] != 1)
	{
		gVrefORbgDataN[SITE_ID] = ((gFrefDataHN[SITE_ID] & 0x01FF) | (gVrefDataN[SITE_ID] & 0x1E00));
	}
	else
	{
		tempDat[SITE_ID] = ((gFrefDataHN[SITE_ID] & 0x01FF) | (RDVrefData[SITE_ID] & 0x1E00));
		gVrefORbgDataN[SITE_ID] = tempDat[SITE_ID];
	}

	tempDat[SITE_ID] = gVrefORbgDataN[SITE_ID];
	g_caculateAdjChSum_buff[SITE_ID][0] = (tempDat[SITE_ID] >> 8);
	g_caculateAdjChSum_buff[SITE_ID][1] = tempDat[SITE_ID] & 0x00FF;
	g_caculateAdjChSum_buff[SITE_ID][2] = (gFrefDataLN[SITE_ID] & 0x0007);
	g_caculateAdjChSum_buff[SITE_ID][3] = gAD2VDataN[SITE_ID];
	g_caculateAdjChSum_buff[SITE_ID][4] = gAD3VDataN[SITE_ID];
	g_caculateAdjChSum_buff[SITE_ID][5] = gAD4VDataN[SITE_ID];
	g_adjChSumCalt[SITE_ID] = CalulateCRC16(g_caculateAdjChSum_buff[SITE_ID], 6);
	g_WriteChsmL[SITE_ID] = (g_adjChSumCalt[SITE_ID] & 0x0FFF);
	g_WriteChsmH[SITE_ID] = ((g_adjChSumCalt[SITE_ID] >> 12) & 0x000F);
//	sys.PrintDatalog("SITE[%d] tempDat:%x \n", SITE_ID,tempDat[SITE_ID]);

	sys.PrintDatalog("SITE[%d] g_caculateAdjChSum_gFrefDataHN:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][0]);
	sys.PrintDatalog("SITE[%d] g_caculateAdjChSum_gVrefDataN:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][1]);
	sys.PrintDatalog("SITE[%d] g_caculateAdjChSum_gFrefDataLN:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][2]);
	sys.PrintDatalog("SITE[%d] g_caculateAdj2v:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][3]);
	sys.PrintDatalog("SITE[%d] g_caculateAdj3v:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][4]);
	sys.PrintDatalog("SITE[%d] g_caculateAdj4v:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][5]);
	sys.PrintDatalog("SITE[%d] g_WriteChsmL:%x \n", SITE_ID,g_WriteChsmL[SITE_ID]);
	sys.PrintDatalog("SITE[%d] g_WriteChsmH:%x \n", SITE_ID,g_WriteChsmH[SITE_ID]);
	//0xB的0~11存放校验和的0~11bit
	//0xC的0~3存放校验和的12~15bit

	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.3).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.98e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);

	pinlevel.Signal("VPP").Connect(HIGH_VOLTAGE_MODE);
	sys.DelayUs(2000);
	pinlevel.Signal("VPP").SetMode("VHH").SetVih(5.0).SetVil(0).SetVhh(8.5).SetIoh(0).SetIol(0).Execute();
	FOR_EACH_SITE_START(SITE,SITE_ID)
	
	
//	sys.PrintDatalog("SITE[%d] gVrefData:%x,gFrefDataH:%x,gFrefDataL:%x \n", SITE_ID,gVrefData[SITE_ID],gFrefDataH[SITE_ID],gFrefDataL[SITE_ID]);
//	sys.PrintDatalog("SITE[%d] gVrefDataN:%x,gFrefDataHN:%x,gFrefDataLN:%x \n", SITE_ID,gVrefDataN[SITE_ID],gFrefDataHN[SITE_ID],gFrefDataLN[SITE_ID]);
	UpdateCPfusePat("Prog_ifen_Pat",SITE_ID);
	NEXT_SITE
	dio.Pattern("Prog_ifen_Pat").SetTimeOut(10000).Run();//Test();
	dio.Pattern("Prog_ifen_Pat").GetFailResult(FailResult);



	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	ppmu.Signal("ALLPIN").DisConnect();
//	cbit.Signal("VP0").SetOff();
	sys.DelayUs(2000);
	binObj.CheckResultAndBin(0, FailResult);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

//USER_CODE void Verify_ifen() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_DIO_RESULT> FailResult;
//	vector<ST_DIO_RECORD>  vctRecordList1;
//
//	int tmpbit[226]={0};
//
//	int RD_Data_Sum[SITE_NUM]={0};
//
//	SITE_DOUBLE ADVref_Data_RD_Out;
//	SITE_DOUBLE ADFH_Data_RD_Out;
//	SITE_DOUBLE ADFL_Data_RD_Out;
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//
//	sys.DelayUs(2000);
//
//	dio.Pattern("Verify_ifen_Pat").SetHRamMode(EN_STV_MODE);
//	dio.Pattern("Verify_ifen_Pat").SetTimeOut(1000000).Run();//
//	dio.Pattern("Verify_ifen_Pat").HRam().SetReadOption("PDA", 0, 43);
//
//FOR_EACH_SITE_START(SITE,SITE_ID)
//
//	dio.Pattern("Verify_ifen_Pat").HRam().GetHistData(vctRecordList1);
//
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//
//	for(int i=0; i<int(vctRecordList1.size());i++)
//	 {
//			  if(strcmp(vctRecordList1[i].CurrWfc.c_str(),"L")==0)   //读H为 1   读L为 0.
//			  {
//				tmpbit[i]=0;    //
//			  }
//			  else
//			  {
//				tmpbit[i]=1;
//			  }
//	 }
//
//	for(int i=0;i<14;i++)  //VREF
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i]*(pow(2,(13-i)));
//
//	}
//
//	ADVref_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDVrefData[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDVrefDataN[SITE_ID] = (0x0E00&(0x0E00-(RDVrefData[SITE_ID]&0x0E00)));
//	RD_Data_Sum[SITE_ID]={0};
//
//
//	for(int i=0;i<14;i++) //FREH
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+14]*(pow(2,(13-i)));
//
//	}
//	ADFH_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDgFrefDataH[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDgFrefDataHN[SITE_ID] = ((0x1FF&(0x1FF-(RDgFrefDataH[SITE_ID]&0x1FF)))|RDVrefDataN[SITE_ID]);
//	RD_Data_Sum[SITE_ID]={0};
//
//	for(int i=0;i<14;i++) //FREL
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+28]*(pow(2,(13-i)));
//
//	}
//	ADFL_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDgFrefDataL[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDgFrefDataLN[SITE_ID] = (0x7 & (0x7-(RDgFrefDataL[SITE_ID]&0x7)));
//	RD_Data_Sum[SITE_ID]={0};
//
////    sys.PrintDatalog(" --------------READ------------------  \n");
////	sys.PrintDatalog("SITE[%d] read : RgVrefData:%x,gFrefData:%x,gFrefDataL:%x \n", SITE_ID,RDVrefData[SITE_ID],RDgFrefDataH[SITE_ID],RDgFrefDataL[SITE_ID]);
////	sys.PrintDatalog("SITE[%d] read : RgVrefDataN:%x,gFrefDataH:%x,gFrefDataLN:%x \n", SITE_ID,RDVrefDataN[SITE_ID],RDgFrefDataHN[SITE_ID],RDgFrefDataLN[SITE_ID]);
//
//
//
//NEXT_SITE
//	pinlevel.Signal("DIO_TRIM_GRP").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("DIO_TRIM_GRP").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//	sys.DelayUs(2000);
//
//	binObj.CheckResultAndBin(0, ADVref_Data_RD_Out);
//	binObj.CheckResultAndBin(1, ADFH_Data_RD_Out);
//	binObj.CheckResultAndBin(2, ADFL_Data_RD_Out);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

USER_CODE void Verify_ifenMrg1() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_DIO_RESULT> FailResult;
	vector<ST_DIO_RECORD>  vctRecordList1;
	SITE_DOUBLE Result;
	int tmpbit[226]={0};

	int RD_Data_Sum[SITE_NUM]={0};

	SITE_DOUBLE ADVrefORFH_Data_RD_Out;
	SITE_DOUBLE ADFL_Data_RD_Out;
	SITE_DOUBLE AD2v_Data_RD_Out;
	SITE_DOUBLE AD3v_Data_RD_Out;
	SITE_DOUBLE AD4v_Data_RD_Out;
	SITE_DOUBLE ADchksumh_Data_RD_Out;
	SITE_DOUBLE ADchksuml_Data_RD_Out;

	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);

	dio.Pattern("Verify_ifen_Pat").SetHRamMode(EN_STV_MODE);
	dio.Pattern("Verify_ifen_Pat").SetTimeOut(1000000).Run();//
	dio.Pattern("Verify_ifen_Pat").HRam().SetReadOption("PDA", 0, 98);

FOR_EACH_SITE_START(SITE,SITE_ID)


	dio.Pattern("Verify_ifen_Pat").HRam().GetHistData(vctRecordList1);

	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	ppmu.Signal("ALLPIN").DisConnect();

	for(int i=0; i<int(vctRecordList1.size());i++)
	 {
			  if(strcmp(vctRecordList1[i].CurrWfc.c_str(),"L")==0)   //读H为 1   读L为 0.
			  {
				tmpbit[i]=0;    //
			  }
			  else
			  {
				tmpbit[i]=1;
			  }
	 }

	for(int i=0;i<14;i++)  //VREF
	{
		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i]*(pow(2,(13-i)));

	}

	ADVrefORFH_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
	RDVrefData[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDVrefDataN2[SITE_ID] = (0x1E00&(0x1E00-RDVrefData[SITE_ID]));
	RD_Data_Sum[SITE_ID]={0};

	for(int i=0;i<14;i++) //FREL
	{
		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+14]*(pow(2,(13-i)));

	}
	ADFL_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
	RDgFrefDataL2[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDgFrefDataLN2[SITE_ID] = (0x7 & (0x7-RDgFrefDataL[SITE_ID]));
	RD_Data_Sum[SITE_ID]={0};


//	for(int i=0;i<14;i++)  //VREF2
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+42]*(pow(2,(13-i)));
//
//	}
//
//	ADVref_Data_RD_Out2[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDVrefData2[SITE_ID] = RD_Data_Sum[SITE_ID];
////	RDVrefDataN2[SITE_ID] = (0x1E00&(0x1E00-RDVrefData[SITE_ID]));
//	RD_Data_Sum[SITE_ID]={0};

	for(int i=0;i<14;i++)  //AD2V
	{
		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+28]*(pow(2,(13-i)));

	}

	AD2v_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
	RDgAD2V[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDVrefDataN2[SITE_ID] = (0x1E00&(0x1E00-RDVrefData[SITE_ID]));
	RD_Data_Sum[SITE_ID]={0};

	for(int i=0;i<14;i++)  //AD3V
	{
		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+42]*(pow(2,(13-i)));

	}

	AD3v_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
	RDgAD3V[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDVrefDataN2[SITE_ID] = (0x1E00&(0x1E00-RDVrefData[SITE_ID]));
	RD_Data_Sum[SITE_ID]={0};

	for(int i=0;i<14;i++)  //AD4V
	{
		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+56]*(pow(2,(13-i)));

	}

	AD4v_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
	RDgAD4V[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDVrefDataN2[SITE_ID] = (0x1E00&(0x1E00-RDVrefData[SITE_ID]));
	RD_Data_Sum[SITE_ID]={0};

	for(int i=0;i<14;i++)  //CHKSUML
	{
		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+70]*(pow(2,(13-i)));

	}

	ADchksuml_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
	RDgCHKSUML[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDVrefDataN2[SITE_ID] = (0x1E00&(0x1E00-RDVrefData[SITE_ID]));
	RD_Data_Sum[SITE_ID]={0};

	for(int i=0;i<14;i++)  //CHKSUMH
	{
		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+84]*(pow(2,(13-i)));

	}

	ADchksumh_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
	RDgCHKSUMH[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDVrefDataN2[SITE_ID] = (0x1E00&(0x1E00-RDVrefData[SITE_ID]));
	RD_Data_Sum[SITE_ID]={0};

    sys.PrintDatalog(" --------------READ------------------  \n");
	sys.PrintDatalog("SITE[%d] write : gVrefData:%x,gFrefDataL:%x \n", SITE_ID,RDVrefData[SITE_ID],RDgFrefDataL2[SITE_ID]);
	sys.PrintDatalog("SITE[%d] write : RDgAD2V:%x,RDgAD3V:%x \n", SITE_ID,RDgAD2V[SITE_ID],RDgAD3V[SITE_ID]);
	sys.PrintDatalog("SITE[%d] write : RDgAD4V:%x,RDgCHKSUML:%x,RDgCHKSUMH:%x \n", SITE_ID,RDgAD4V[SITE_ID],RDgCHKSUML[SITE_ID],RDgCHKSUMH[SITE_ID]);
	if(RDVrefData[SITE_ID] !=gVrefORbgDataN[SITE_ID] && RDgFrefDataL2[SITE_ID] != gFrefDataLN[SITE_ID] && RDgAD2V[SITE_ID] != gAD2VDataN[SITE_ID] && RDgAD3V[SITE_ID] != gAD3VDataN[SITE_ID]&&RDgAD4V[SITE_ID] != gAD4VDataN[SITE_ID] &&RDgCHKSUMH[SITE_ID] != g_WriteChsmH[SITE_ID] &&RDgCHKSUML[SITE_ID] != g_WriteChsmL[SITE_ID] )
	{
		Result[SITE_ID] = 0;
	}
	else
	{
		Result[SITE_ID] = 1;
	}
	NEXT_SITE
	pinlevel.Signal("ALLPIN").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("ALLPIN").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	ppmu.Signal("ALLPIN").DisConnect();
	sys.DelayUs(2000);

	binObj.CheckResultAndBin(0, Result);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

//USER_CODE void Verify_ifen2P4() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_DIO_RESULT> FailResult;
//	vector<ST_DIO_RECORD>  vctRecordList1;
//
//	int tmpbit[226]={0};
//	int RD_Data_Sum[SITE_NUM]={0};
//
//	SITE_DOUBLE ADVref_Data_RD_Out;
//	SITE_DOUBLE ADFH_Data_RD_Out;
//	SITE_DOUBLE ADFL_Data_RD_Out;
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(5000);
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(2.4).SetVil(0.0).SetVoh(1.75).SetVol(0.7).SetActiveLoadStatus(ON).SetVt(2).SetIoh(-3e-3).SetIol(0.39e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(2.4).SetVil(0.0).SetVoh(1.75).SetVol(0.7).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//	dio.Pattern("Verify_ifen_Pat").SetHRamMode(EN_STV_MODE);
//	dio.Pattern("Verify_ifen_Pat").SetTimeOut(1000000).Run();//
//	dio.Pattern("Verify_ifen_Pat").HRam().SetReadOption("PDA", 0, 43);
//
//FOR_EACH_SITE_START(SITE,SITE_ID)
//
//
//	dio.Pattern("Verify_ifen_Pat").HRam().GetHistData(vctRecordList1);
//
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//
//	for(int i=0; i<int(vctRecordList1.size());i++)
//	 {
//			  if(strcmp(vctRecordList1[i].CurrWfc.c_str(),"L")==0)   //读H为 1   读L为 0.
//			  {
//				tmpbit[i]=0;    //
//			  }
//			  else
//			  {
//				tmpbit[i]=1;
//			  }
//	 }
//
//	for(int i=0;i<14;i++)  //VREF
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i]*(pow(2,(13-i)));
//
//	}
//
//	ADVref_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDVrefData1[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDVrefDataN1[SITE_ID] = (0x0E00&(0x0E00-RDVrefData[SITE_ID]));
//	RD_Data_Sum[SITE_ID]={0};
//
//	for(int i=0;i<14;i++) //FREH
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+14]*(pow(2,(13-i)));
//
//	}
//	ADFH_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDgFrefDataH1[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDgFrefDataHN1[SITE_ID] = ( 0x1FF & (0x1FF-RDgFrefDataH[SITE_ID]));
//	RD_Data_Sum[SITE_ID]={0};
//
//	for(int i=0;i<14;i++) //FREL
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+28]*(pow(2,(13-i)));
//
//	}
//	ADFL_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDgFrefDataL1[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDgFrefDataLN1[SITE_ID] = (0x7 & (0x7-RDgFrefDataL[SITE_ID]));
//	RD_Data_Sum[SITE_ID]={0};
//
////    sys.PrintDatalog(" --------------READ------------------  \n");
////	sys.PrintDatalog("SITE[%d] write : gVrefData:%x,gFrefData:%x,gFrefDataL:%x \n", SITE_ID,RDVrefData1[SITE_ID],RDgFrefDataH1[SITE_ID],RDgFrefDataL1[SITE_ID]);
////	sys.PrintDatalog("SITE[%d] read : RDgVrefDataN:%x,RDgFrefDataH:%x,RDgFrefDataLN:%x \n", SITE_ID,RDVrefDataN1[SITE_ID],RDgFrefDataHN1[SITE_ID],RDgFrefDataLN1[SITE_ID]);
//
//
//
//NEXT_SITE
//	pinlevel.Signal("DIO_TRIM_GRP").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("DIO_TRIM_GRP").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//	sys.DelayUs(2000);
//
//
//	binObj.CheckResultAndBin(0, ADVref_Data_RD_Out);
//	binObj.CheckResultAndBin(1, ADFH_Data_RD_Out);
//	binObj.CheckResultAndBin(2, ADFL_Data_RD_Out);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Test_hclk_test2P4() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_MEAS_RESULT> stTMUMeasResult;
//
//	tmu.SetupTmuOutputCh(1, 1, 0, 6);tmu.SetupTmuOutputCh(1, 1, 1, 14);tmu.SetupTmuOutputCh(1, 1, 2, 22);tmu.SetupTmuOutputCh(1, 1, 3, 30);
//	tmu.SetupTmuOutputCh(1, 2, 0, 6);tmu.SetupTmuOutputCh(1, 2, 1, 14);tmu.SetupTmuOutputCh(1, 2, 2, 22);tmu.SetupTmuOutputCh(1, 2, 3, 30);
//	tmu.SetupTmuOutputCh(1, 3, 0, 6);tmu.SetupTmuOutputCh(1, 3, 1, 14);tmu.SetupTmuOutputCh(1, 3, 2, 22);tmu.SetupTmuOutputCh(1, 3, 3, 30);
//	tmu.SetupTmuOutputCh(1, 4, 0, 6);tmu.SetupTmuOutputCh(1, 4, 1, 14);tmu.SetupTmuOutputCh(1, 4, 2, 22);tmu.SetupTmuOutputCh(1, 4, 3, 30);
//	tmu.SetupTmuOutputCh(1, 5, 0, 6);tmu.SetupTmuOutputCh(1, 5, 1, 14);tmu.SetupTmuOutputCh(1, 5, 2, 22);tmu.SetupTmuOutputCh(1, 5, 3, 30);
//	tmu.SetupTmuOutputCh(1, 6, 0, 6);tmu.SetupTmuOutputCh(1, 6, 1, 14);tmu.SetupTmuOutputCh(1, 6, 2, 22);tmu.SetupTmuOutputCh(1, 6, 3, 30);
//	tmu.SetupTmuOutputCh(1, 7, 0, 6);tmu.SetupTmuOutputCh(1, 7, 1, 14);tmu.SetupTmuOutputCh(1, 7, 2, 22);tmu.SetupTmuOutputCh(1, 7, 3, 30);
//	tmu.SetupTmuOutputCh(1, 8, 0, 6);tmu.SetupTmuOutputCh(1, 8, 1, 14);tmu.SetupTmuOutputCh(1, 8, 2, 22);tmu.SetupTmuOutputCh(1, 8, 3, 30);
//	tmu.SetupTmuOutputCh(1, 9, 0, 6);tmu.SetupTmuOutputCh(1, 9, 1, 14);tmu.SetupTmuOutputCh(1, 9, 2, 22);tmu.SetupTmuOutputCh(1, 9, 3, 30);
//	tmu.SetupTmuOutputCh(1, 10, 0, 6);tmu.SetupTmuOutputCh(1, 10, 1, 14);tmu.SetupTmuOutputCh(1, 10, 2, 22);tmu.SetupTmuOutputCh(1, 10, 3, 30);
//	tmu.SetupTmuOutputCh(1, 11, 0, 6);tmu.SetupTmuOutputCh(1, 11, 1, 14);tmu.SetupTmuOutputCh(1, 11, 2, 22);tmu.SetupTmuOutputCh(1, 11, 3, 30);
//	tmu.SetupTmuOutputCh(1, 12, 0, 6);tmu.SetupTmuOutputCh(1, 12, 1, 14);tmu.SetupTmuOutputCh(1, 12, 2, 22);tmu.SetupTmuOutputCh(1, 12, 3, 30);
//	tmu.SetupTmuOutputCh(1, 13, 0, 6);tmu.SetupTmuOutputCh(1, 13, 1, 14);tmu.SetupTmuOutputCh(1, 13, 2, 22);tmu.SetupTmuOutputCh(1, 13, 3, 30);
//	tmu.SetupTmuOutputCh(1, 14, 0, 6);tmu.SetupTmuOutputCh(1, 14, 1, 14);tmu.SetupTmuOutputCh(1, 14, 2, 22);tmu.SetupTmuOutputCh(1, 14, 3, 30);
//	tmu.SetupTmuOutputCh(1, 15, 0, 6);tmu.SetupTmuOutputCh(1, 15, 1, 14);tmu.SetupTmuOutputCh(1, 15, 2, 22);tmu.SetupTmuOutputCh(1, 15, 3, 30);
//	tmu.SetupTmuOutputCh(1, 16, 0, 6);tmu.SetupTmuOutputCh(1, 16, 1, 14);tmu.SetupTmuOutputCh(1, 16, 2, 22);tmu.SetupTmuOutputCh(1, 16, 3, 30);
//
//	tmu.Connect("TMU_CH29");
//	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(1.5).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(50).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(2.4).SetVil(0.0).SetVoh(1.75).SetVol(0.7).SetActiveLoadStatus(ON).SetVt(2).SetIoh(-2e-2).SetIol(0.39e-3).VoltClamp(6.5,-2.0).Execute();
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(2.4).SetVil(0.0).SetVoh(1.75).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	FOR_EACH_SITE_START(SITE,SITE_ID)
////  sys.PrintDatalog("SITE[%d] ,RgFrefDataH:%x,gFrefDataHN:%x \n", SITE_ID,RDgFrefDataH1[SITE_ID],RDgFrefDataHN1[SITE_ID]);
//
//	UpdateHCKPat("Hrc_Pat",SITE_ID);   // 此处向量替换的还是vref
//	NEXT_SITE
//
//	dio.Pattern("Hrc_Pat").Test();
//	sys.DelayUs(20000);
//
//	tmu.Freq().Signal("TMU_CH29").StartMeas();
//	tmu.Freq().GetMeasResult(stTMUMeasResult);
//	sys.DelayUs(10000);
//
//	tmu.DisConnect("TMU_CH29");
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	Hvalue2P4[SITE_ID]=stTMUMeasResult[SITE].dbValue;
//
//
//
//
////	sys.PrintDatalog("SITE[%d]: FrefH (P07): %f \n", SITE_ID, Hvalue[SITE_ID]);
////	sys.PrintDatalog("SITE[%d]: FrefH (P07): %f \n", SITE_ID, Hvalue2P4[SITE_ID]);
//
//	Hvalue1[SITE_ID]=(Hvalue[SITE_ID]-Hvalue2P4[SITE_ID]);
//	Pass_Hrc[SITE_ID]=Hvalue1[SITE_ID];
//	NEXT_SITE
//
//
//
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//	sys.DelayUs(2000);
//
//
//	binObj.CheckResultAndBin(0, stTMUMeasResult, 1);
//	binObj.CheckResultAndBin(1, Pass_Hrc);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Test_lclk_test2P4() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_MEAS_RESULT> stTMUMeasResult;
//
//	tmu.SetupTmuOutputCh(1, 1, 0, 6);tmu.SetupTmuOutputCh(1, 1, 1, 14);tmu.SetupTmuOutputCh(1, 1, 2, 22);tmu.SetupTmuOutputCh(1, 1, 3, 30);
//	tmu.SetupTmuOutputCh(1, 2, 0, 6);tmu.SetupTmuOutputCh(1, 2, 1, 14);tmu.SetupTmuOutputCh(1, 2, 2, 22);tmu.SetupTmuOutputCh(1, 2, 3, 30);
//	tmu.SetupTmuOutputCh(1, 3, 0, 6);tmu.SetupTmuOutputCh(1, 3, 1, 14);tmu.SetupTmuOutputCh(1, 3, 2, 22);tmu.SetupTmuOutputCh(1, 3, 3, 30);
//	tmu.SetupTmuOutputCh(1, 4, 0, 6);tmu.SetupTmuOutputCh(1, 4, 1, 14);tmu.SetupTmuOutputCh(1, 4, 2, 22);tmu.SetupTmuOutputCh(1, 4, 3, 30);
//	tmu.SetupTmuOutputCh(1, 5, 0, 6);tmu.SetupTmuOutputCh(1, 5, 1, 14);tmu.SetupTmuOutputCh(1, 5, 2, 22);tmu.SetupTmuOutputCh(1, 5, 3, 30);
//	tmu.SetupTmuOutputCh(1, 6, 0, 6);tmu.SetupTmuOutputCh(1, 6, 1, 14);tmu.SetupTmuOutputCh(1, 6, 2, 22);tmu.SetupTmuOutputCh(1, 6, 3, 30);
//	tmu.SetupTmuOutputCh(1, 7, 0, 6);tmu.SetupTmuOutputCh(1, 7, 1, 14);tmu.SetupTmuOutputCh(1, 7, 2, 22);tmu.SetupTmuOutputCh(1, 7, 3, 30);
//	tmu.SetupTmuOutputCh(1, 8, 0, 6);tmu.SetupTmuOutputCh(1, 8, 1, 14);tmu.SetupTmuOutputCh(1, 8, 2, 22);tmu.SetupTmuOutputCh(1, 8, 3, 30);
//	tmu.SetupTmuOutputCh(1, 9, 0, 6);tmu.SetupTmuOutputCh(1, 9, 1, 14);tmu.SetupTmuOutputCh(1, 9, 2, 22);tmu.SetupTmuOutputCh(1, 9, 3, 30);
//	tmu.SetupTmuOutputCh(1, 10, 0, 6);tmu.SetupTmuOutputCh(1, 10, 1, 14);tmu.SetupTmuOutputCh(1, 10, 2, 22);tmu.SetupTmuOutputCh(1, 10, 3, 30);
//	tmu.SetupTmuOutputCh(1, 11, 0, 6);tmu.SetupTmuOutputCh(1, 11, 1, 14);tmu.SetupTmuOutputCh(1, 11, 2, 22);tmu.SetupTmuOutputCh(1, 11, 3, 30);
//	tmu.SetupTmuOutputCh(1, 12, 0, 6);tmu.SetupTmuOutputCh(1, 12, 1, 14);tmu.SetupTmuOutputCh(1, 12, 2, 22);tmu.SetupTmuOutputCh(1, 12, 3, 30);
//	tmu.SetupTmuOutputCh(1, 13, 0, 6);tmu.SetupTmuOutputCh(1, 13, 1, 14);tmu.SetupTmuOutputCh(1, 13, 2, 22);tmu.SetupTmuOutputCh(1, 13, 3, 30);
//	tmu.SetupTmuOutputCh(1, 14, 0, 6);tmu.SetupTmuOutputCh(1, 14, 1, 14);tmu.SetupTmuOutputCh(1, 14, 2, 22);tmu.SetupTmuOutputCh(1, 14, 3, 30);
//	tmu.SetupTmuOutputCh(1, 15, 0, 6);tmu.SetupTmuOutputCh(1, 15, 1, 14);tmu.SetupTmuOutputCh(1, 15, 2, 22);tmu.SetupTmuOutputCh(1, 15, 3, 30);
//	tmu.SetupTmuOutputCh(1, 16, 0, 6);tmu.SetupTmuOutputCh(1, 16, 1, 14);tmu.SetupTmuOutputCh(1, 16, 2, 22);tmu.SetupTmuOutputCh(1, 16, 3, 30);
//
//	tmu.Connect("TMU_CH29");
//	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(1.5).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(50).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(2.4).SetVil(0.0).SetVoh(1.75).SetVol(0.7).SetActiveLoadStatus(ON).SetVt(2).SetIoh(0).SetIol(0.39e-3).VoltClamp(6.5,-2.0).Execute();
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(2.4).SetVil(0.0).SetVoh(1.75).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	FOR_EACH_SITE_START(SITE,SITE_ID)
////	 sys.PrintDatalog("SITE[%d] ,R1gFrefDataL:%x,R1gFrefDataLN:%x \n", SITE_ID,RDgFrefDataL1[SITE_ID],RDgFrefDataLN1[SITE_ID]);
//
//    UpdateLCKPat("Lrc_Pat",SITE_ID);   // 此处向量替换的还是vref
//	NEXT_SITE
//
//	dio.Pattern("Lrc_Pat").Test();
//	sys.DelayUs(20000);
//
//	tmu.Freq().Signal("TMU_CH29").StartMeas();
//	tmu.Freq().GetMeasResult(stTMUMeasResult);
//	sys.DelayUs(10000);
//
//	tmu.DisConnect("TMU_CH29");
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
//	Lvalue2P4[SITE_ID]=stTMUMeasResult[SITE].dbValue;
//
////	sys.PrintDatalog("SITE[%d]: FrefL5V (P07): %f \n", SITE_ID, Lvalue[SITE_ID]);
////	sys.PrintDatalog("SITE[%d]: FrefL (P07): %f \n", SITE_ID, Lvalue2P4[SITE_ID]);
//
//	Lvalue1[SITE_ID]=(Lvalue[SITE_ID]-Lvalue2P4[SITE_ID]);
//	Pass_Lrc[SITE_ID]=Lvalue1[SITE_ID];
//	NEXT_SITE
//
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//	sys.DelayUs(2000);
//
//	binObj.CheckResultAndBin(0, stTMUMeasResult, 1);
//	binObj.CheckResultAndBin(1, Pass_Lrc);
//
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Verify_OtpBlank() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_DIO_RESULT> FailResult;
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//	dio.Pattern("Verify_OtpBlank_Pat").SetTimeOut(10000).Run();//Test();
//	dio.Pattern("Verify_OtpBlank_Pat").GetFailResult(FailResult);
//
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//	sys.DelayUs(2000);
//	binObj.CheckResultAndBin(0, FailResult);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Verify_OtpBlank2P4() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_DIO_RESULT> FailResult;
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(20000);
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(2.4).SetVil(0.0).SetVoh(1.75).SetVol(0.7).SetActiveLoadStatus(ON).SetVt(2).SetIoh(-2e-3).SetIol(0.39e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(2.4).SetVil(0.0).SetVoh(1.75).SetVol(0.7).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//	dio.Pattern("Verify_OtpBlank_Pat").SetTimeOut(10000).Test();//Test();
//	dio.Pattern("Verify_OtpBlank_Pat").GetFailResult(FailResult);
//
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
//	sys.DelayUs(2000);
//	binObj.CheckResultAndBin(0, FailResult);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Verify_OtpBlankMrg1() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_DIO_RESULT> FailResult;
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	sys.DelayUs(2000);
//
//	dio.Pattern("Verify_OtpBlankMrg1_Pat").SetTimeOut(10000).Test();//Test();
//	dio.Pattern("Verify_OtpBlankMrg1_Pat").GetFailResult(FailResult);
//
//	ppmu.Signal("DIO_TRIM").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM").DisConnect();
//	sys.DelayUs(2000);
//	binObj.CheckResultAndBin(0, FailResult);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

USER_CODE void ISS_Test() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_MEAS_RESULT>Iddq_RESULT;

	pinlevel.Signal("ALLPIN").Connect();
	sys.DelayUs(5000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();

	sys.DelayUs(2000);

	dio.Pattern("T_iss_Pat").SetTimeOut(10000).Test();//Test();

//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);

	ppmu.Signal("VDD").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(0, Iddq_RESULT, 1);

	ppmu.Signal("P02").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(1, Iddq_RESULT, 1);
	ppmu.Signal("P02").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P03").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(2, Iddq_RESULT, 1);
	ppmu.Signal("P03").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("VPP").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(3, Iddq_RESULT, 1);
	ppmu.Signal("VPP").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P05").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(4, Iddq_RESULT, 1);
	ppmu.Signal("P05").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P06").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(5, Iddq_RESULT, 1);
	ppmu.Signal("P06").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P07").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(6, Iddq_RESULT, 1);
	ppmu.Signal("P07").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P08").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(7, Iddq_RESULT, 1);
	ppmu.Signal("P08").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P09").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(8, Iddq_RESULT, 1);
	ppmu.Signal("P09").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P10").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(9, Iddq_RESULT, 1);
	ppmu.Signal("P10").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P11").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(10, Iddq_RESULT, 1);
	ppmu.Signal("P11").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P12").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(11, Iddq_RESULT, 1);
	ppmu.Signal("P12").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("P13").SetMode("FVMI").VoltForce(0.15).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(12, Iddq_RESULT, 1);
	ppmu.Signal("P13").SetMode("FVMI").VoltForce(0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}


USER_CODE void SetPassBin() {
	TEST_BEGIN

	// TODO Edit your code here
	binObj.SetBin(1);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

//USER_CODE void VPP_ISS_Test() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_MEAS_RESULT>Iddq_RESULT1,DEBUG;
//
//	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(10000);
//	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
//	ppmu.Signal("ALLPIN").DisConnect();
//	sys.DelayUs(2000);
//
//	cbit.Signal("VP0").SetOn();
//	sys.DelayUs(2000);
//
//	ppmu.Signal("VDD").Connect();
//	ppmu.Signal("VPP").Connect();
//	sys.DelayUs(3000);
//
////	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
////	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
////	sys.DelayUs(2000);
//	ppmu.Signal("VDD").SetMode("FVMV").VoltForce(5.0).CurrRange(4E-3).VoltClamp(6,-2).Execute();
//	sys.DelayUs(1500);
//	ppmu.Signal("VDD").Measure(DEBUG);
//
//	ppmu.Signal("VPP").SetMode("FVMV").VoltForce(5.0).CurrRange(4E-3).VoltClamp(6,-2).Execute();
//	sys.DelayUs(1500);
//	ppmu.Signal("VPP").Measure(DEBUG);
//	ppmu.Signal("VPP").SetMode("FVMI").VoltForce(5.0).CurrRange(2E-6).VoltClamp(6,-2).Execute();
//	sys.DelayUs(5000);
//	ppmu.Signal("VPP").Measure(Iddq_RESULT1);
//
//
//	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
//	ppmu.Signal("ALLPIN").DisConnect();
//	sys.DelayUs(2000);
//	binObj.CheckResultAndBin(0, Iddq_RESULT1, 1);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Prog_ifen_checksum() {
//	TEST_BEGIN
//
//	vector<ST_DIO_RESULT> FailResult;
//
//	cbit.Signal("VP0").SetOn();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("DIO_TRIM").Connect();
//	pinlevel.Signal("PDA").Connect();
//	pinlevel.Signal("VPP").Connect(HIGH_VOLTAGE_MODE);
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("VPP").SetMode("VHH").SetVih(5.0).SetVil(0).SetVhh(8.1).SetIoh(0).SetIol(0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//	ULONG tempDat[64] = {0};
//	uint8_t g_caculateAdjChSum_buff[64][3] = {0};
//	ULONG g_adjChSumCalt[64] = {0};
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
////	sys.PrintDatalog("SITE[%d] gVrefData:%x,gFrefDataH:%x,gFrefDataL:%x \n", SITE_ID,gVrefData[SITE_ID],gFrefDataH[SITE_ID],gFrefDataL[SITE_ID]);
////	sys.PrintDatalog("SITE[%d] gVrefDataN:%x,gFrefDataHN:%x,gFrefDataLN:%x \n", SITE_ID,gVrefDataN[SITE_ID],gFrefDataHN[SITE_ID],gFrefDataLN[SITE_ID]);
//	tempDat[SITE_ID] = ((gFrefDataHN[SITE_ID] & 0x01FF) | (gVrefDataN[SITE_ID] & 0x0E00));
//	g_caculateAdjChSum_buff[SITE_ID][0] = (tempDat[SITE_ID] >> 8);
//	g_caculateAdjChSum_buff[SITE_ID][1] = tempDat[SITE_ID] & 0x00FF;
//	g_caculateAdjChSum_buff[SITE_ID][2] = (gFrefDataLN[SITE_ID] & 0x0007);
//	g_adjChSumCalt[SITE_ID] = CalulateCRC16(g_caculateAdjChSum_buff[SITE_ID], 3);
//	g_WriteChsmL[SITE_ID] = (g_adjChSumCalt[SITE_ID] & 0x0FFF);
//	g_WriteChsmH[SITE_ID] = ((g_adjChSumCalt[SITE_ID] >> 12) & 0x000F);
//	sys.PrintDatalog("SITE[%d] gFrefDataHN:%x \n", SITE_ID,gFrefDataHN[SITE_ID]);
//	sys.PrintDatalog("SITE[%d] gVrefDataN:%x \n", SITE_ID,gVrefDataN[SITE_ID]);
//	sys.PrintDatalog("SITE[%d] tempDat:%x \n", SITE_ID,tempDat[SITE_ID]);
//	sys.PrintDatalog("SITE[%d] gFrefDataLN:%x \n", SITE_ID,gFrefDataLN[SITE_ID]);
//
//	sys.PrintDatalog("SITE[%d] g_caculateAdjChSum_buff:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][0]);
//	sys.PrintDatalog("SITE[%d] g_caculateAdjChSum_buff:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][1]);
//	sys.PrintDatalog("SITE[%d] g_caculateAdjChSum_buff:%x \n", SITE_ID,g_caculateAdjChSum_buff[SITE_ID][2]);
//
//	sys.PrintDatalog("SITE[%d] g_WriteChsmL:%x \n", SITE_ID,g_WriteChsmL[SITE_ID]);
//	sys.PrintDatalog("SITE[%d] g_WriteChsmH:%x \n", SITE_ID,g_WriteChsmH[SITE_ID]);
//	//0xB的0~11存放校验和的0~11bit
//	//0xC的0~3存放校验和的12~15bit
//	UpdateCPfusePat111("Prog_ifen_checksum_Pat",SITE_ID);
//	NEXT_SITE
//
//	dio.Pattern("Prog_ifen_checksum_Pat").SetTimeOut(10000).Test();//Test();
//	dio.Pattern("Prog_ifen_checksum_Pat").GetFailResult(FailResult);
//
//
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(0).SetVil(0).SetVoh(0).SetVol(0).SetActiveLoadStatus(OFF).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("VPP").SetMode("VHH").SetVih(0).SetVil(0).SetVhh(0).SetIoh(0).SetIol(0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0).SetVil(0).SetVoh(0).SetVol(0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("ALLPIN").DisConnect();
//	cbit.Signal("VP0").SetOff();
//	sys.DelayUs(2000);
//	binObj.CheckResultAndBin(0, FailResult);
//
//
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void Verify_ifen_checksum() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_DIO_RESULT> FailResult;
//	vector<ST_DIO_RECORD>  vctRecordList1;
//
//	int tmpbit[226]={0};
//
//	int RD_Data_Sum[SITE_NUM]={0};
//
//	SITE_DOUBLE ADFH_Data_RD_Out;
//	SITE_DOUBLE ADFL_Data_RD_Out;
//
//	ppmu.Signal("DIO_TRIM").Connect();
//	ppmu.Signal("PDA").Connect();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(20000);
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//	dio.Pattern("Verify_ifen_checksum_Pat").SetHRamMode(EN_STV_MODE);
//	dio.Pattern("Verify_ifen_checksum_Pat").SetTimeOut(1000000).Run();//
//	dio.Pattern("Verify_ifen_checksum_Pat").HRam().SetReadOption("PDA", 0, 28);
//
//FOR_EACH_SITE_START(SITE,SITE_ID)
//
//
//	dio.Pattern("Verify_ifen_checksum_Pat").HRam().GetHistData(vctRecordList1);
//
//	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
//	ppmu.Signal("ALLPIN").DisConnect();
//
//	for(int i=0; i<int(vctRecordList1.size());i++)
//	 {
//			  if(strcmp(vctRecordList1[i].CurrWfc.c_str(),"L")==0)   //读H为 1   读L为 0.
//			  {
//				tmpbit[i]=0;    //
//			  }
//			  else
//			  {
//				tmpbit[i]=1;
//			  }
//	 }
//
////	for(int i=0;i<14;i++)  //VREF
////	{
////		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i]*(pow(2,(13-i)));
////
////	}
////
////	ADVref_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
////	RDVrefData2[SITE_ID] = RD_Data_Sum[SITE_ID];
////	RDVrefDataN2[SITE_ID] = (0x0E00&(0x0E00-RDVrefData[SITE_ID]));
////	RD_Data_Sum[SITE_ID]={0};
//
//	for(int i=0;i<12;i++) //FREL
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+2]*(pow(2,(11-i)));
//
//	}
//	ADFL_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDgFrefDataL2[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDgFrefDataLN2[SITE_ID] = (0x7 & (0x7-RDgFrefDataL[SITE_ID]));
//	RD_Data_Sum[SITE_ID]={0};
//
//	for(int i=0;i<4;i++) //FREH
//	{
//		RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i+24]*(pow(2,(3-i)));
//
//	}
//	ADFH_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID];
//	RDgFrefDataH2[SITE_ID] = RD_Data_Sum[SITE_ID];
//	RDgFrefDataHN2[SITE_ID] = ( 0x1FF & (0x1FF-RDgFrefDataH[SITE_ID]));
//	RD_Data_Sum[SITE_ID]={0};
//
//
//
////    sys.PrintDatalog(" --------------READ------------------  \n");
////	sys.PrintDatalog("SITE[%d] write : gVrefData:%x,gFrefData:%x,gFrefDataL:%x \n", SITE_ID,RDVrefData2[SITE_ID],RDgFrefDataH2[SITE_ID],RDgFrefDataL2[SITE_ID]);
////	sys.PrintDatalog("SITE[%d] read : RDgVrefDataN:%x,RDgFrefDataH:%x,RDgFrefDataLN:%x \n", SITE_ID,RDVrefDataN2[SITE_ID],RDgFrefDataHN2[SITE_ID],RDgFrefDataLN2[SITE_ID]);
//
//NEXT_SITE
//	pinlevel.Signal("ALLPIN").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("ALLPIN").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	ppmu.Signal("ALLPIN").DisConnect();
//	sys.DelayUs(2000);
//
//	binObj.CheckResultAndBin(0, ADFL_Data_RD_Out);
//	binObj.CheckResultAndBin(1, ADFH_Data_RD_Out);
//
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}



//USER_CODE void Write_Check_ChsmLH() {
//	TEST_BEGIN
//
//
//	vector<ST_DIO_RESULT> FailResult;
//
//	cbit.Signal("VP0").SetOn();
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("DIO_TRIM").Connect();
//	pinlevel.Signal("PDA").Connect();
//	pinlevel.Signal("VPP").Connect(HIGH_VOLTAGE_MODE);
//	sys.DelayUs(2000);
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("VPP").SetMode("VHH").SetVih(5.0).SetVil(0).SetVhh(8.3).SetIoh(0).SetIol(0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//
//	ULONG tempDat[64] = {0};
//	uint8_t g_caculateAdjChSum_buff[64][3] = {0};
//	ULONG g_adjChSumCalt[64] = {0};
//
//
//
//	FOR_EACH_SITE_START(SITE,SITE_ID)
////	sys.PrintDatalog("SITE[%d] gVrefData:%x,gFrefDataH:%x,gFrefDataL:%x \n", SITE_ID,gVrefData[SITE_ID],gFrefDataH[SITE_ID],gFrefDataL[SITE_ID]);
////	sys.PrintDatalog("SITE[%d] gVrefDataN:%x,gFrefDataHN:%x,gFrefDataLN:%x \n", SITE_ID,gVrefDataN[SITE_ID],gFrefDataHN[SITE_ID],gFrefDataLN[SITE_ID]);
//	tempDat[SITE_ID] = ((HRC_VALUE[SITE_ID] & 0x01FF) | (IP_VALUE[SITE_ID] & 0x0E00));
//	g_caculateAdjChSum_buff[SITE_ID][0] = (tempDat[SITE_ID] >> 8);
//	g_caculateAdjChSum_buff[SITE_ID][1] = tempDat[SITE_ID] & 0x00FF;
//	g_caculateAdjChSum_buff[SITE_ID][2] = (LRC_VALUE[SITE_ID] & 0x0007);
//	g_adjChSumCalt[SITE_ID] = CalulateCRC16(g_caculateAdjChSum_buff[SITE_ID], 3);
//	g_WriteChsmL[SITE_ID] = (g_adjChSumCalt[SITE_ID] & 0x0FFF);
//	g_WriteChsmH[SITE_ID] = ((g_adjChSumCalt[SITE_ID] >> 12) & 0x000F);
//	//0xB的0~11存放校验和的0~11bit
//	//0xC的0~3存放校验和的12~15bit
//
//
//	UpdateCPfusePat111("Prog_ifen_checksum_Pat",SITE_ID);
//
//	NEXT_SITE
//
//	dio.Pattern("Prog_ifen_checksum_Pat").SetTimeOut(10000).Test();//Test();
//	dio.Pattern("Prog_ifen_checksum_Pat").GetFailResult(FailResult);
//
//
//
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(0).SetVil(0).SetVoh(0).SetVol(0).SetActiveLoadStatus(OFF).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	pinlevel.Signal("VPP").SetMode("VHH").SetVih(0).SetVil(0).SetVhh(0).SetIoh(0).SetIol(0).Execute();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0).SetVil(0).SetVoh(0).SetVol(0).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("ALLPIN").DisConnect();
//	cbit.Signal("VP0").SetOff();
//	sys.DelayUs(2000);
//	binObj.CheckResultAndBin(0, FailResult);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

//USER_CODE void VPP_ISS_Test0() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_MEAS_RESULT>Iddq_RESULT1,DEBUG;
//
//	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(10000);
//	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
//	ppmu.Signal("ALLPIN").DisConnect();
//	sys.DelayUs(2000);
//
//	cbit.Signal("VP0").SetOn();
//	sys.DelayUs(2000);
//
//	ppmu.Signal("VDD").Connect();
//	ppmu.Signal("VPP").Connect();
//	sys.DelayUs(3000);
//
////	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
////	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
////	sys.DelayUs(2000);
//	ppmu.Signal("VDD").SetMode("FVMV").VoltForce(5.0).CurrRange(4E-3).VoltClamp(6,-2).Execute();
//	sys.DelayUs(1500);
//	ppmu.Signal("VDD").Measure(DEBUG);
//
//	ppmu.Signal("VPP").SetMode("FVMV").VoltForce(5.0).CurrRange(4E-3).VoltClamp(6,-2).Execute();
//	sys.DelayUs(1500);
//	ppmu.Signal("VPP").Measure(DEBUG);
//	ppmu.Signal("VPP").SetMode("FVMI").VoltForce(5.0).CurrRange(2E-6).VoltClamp(6,-2).Execute();
//	sys.DelayUs(5000);
//	ppmu.Signal("VPP").Measure(Iddq_RESULT1);
//
//
//	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
//	ppmu.Signal("ALLPIN").DisConnect();
//	sys.DelayUs(2000);
//	binObj.CheckResultAndBin(0, Iddq_RESULT1, 1);
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

USER_CODE void AD2P0_test() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_MEAS_RESULT> PpmuResult1;

//	gVrefData2P0[SITE_NUM] = {0x0000};
//	gTrimOK_Count[SITE_NUM] = {0000};
	int iCount_k = 0;
	iCount=0;
	double dbCurvolt = 0.0;

	ULONG MIN_trimData[32]={0};
	double MIN_CurVolt[32]={0};
	int MIN_i = 0;

	ULONG ulTrimData = 0x0000, ulTrimStart = 0x01FF; // ulTrimEnd = 0x3FFD;
//	ulTrimDataOk1 = 0, ulTrimDataOk2 = 0;
//	dbTrimValue1 = 0, dbTrimValue2 = 0;

	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

FOR_EACH_SITE_START(SITE,SITE_ID)
	//first time: run pat, and meas volt of P09
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(4.5).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(1000);

//	Plvl_Set("PIN_VREF");
	dio.Pattern("AD2p0_Pat").Test();
	sys.DelayUs(1000);

//	ppmu.Signal("PIN_VREF").SetMode("FNMV").CurrRange(0.04).Execute();
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);

	sys.PrintDatalog("SITE[%d] Vref first time (0000 0000 0000 0000--00FF) means volt(PIN_ADOUT): %f \n", SITE_ID, PpmuResult1[0].dbValue);

	//update pat data, run pat, and meas volt of PIN_VREF
	//////////////////////////////////////////////////////////
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;
	double OBj2V = 2.0;

	ulTrimData =0X320;
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[0]=ulTrimData;
	MIN_CurVolt[0]=dbCurvolt;
	if(dbCurvolt<OBj2V)
	{
		ulTrimData =0X340;
	}
	else
	{
		ulTrimData =0X300;
	}
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[1]=ulTrimData;
	MIN_CurVolt[1]=dbCurvolt;
	if(dbCurvolt<OBj2V)
	{
		ulTrimData |= 0x20;

	}
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[2]=ulTrimData;
	MIN_CurVolt[2]=dbCurvolt;

	for(int i=0;i<5;i++)
	{
		ulTrimData += dataPlase[i];
		TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
		MIN_trimData[3+i]=ulTrimData;
		MIN_CurVolt[3+i]=dbCurvolt;
		if(dbCurvolt < OBj2V)
		{
			ulTrimData -= dataPlase[i];
		}
	}


	sys.PrintDatalog("SITE[%d] iCount_k:%d\n", SITE_ID, iCount_k);
	iCount_k=0;

sys.DelayUs(2);

    int ulTrimDataLength = sizeof(MIN_trimData)/sizeof(ULONG);
    sys.PrintDatalog("count:%d\n", ulTrimDataLength);
    MIN_i = Minfind(SITE_ID, MIN_trimData, MIN_CurVolt, Vvref_VOLT20 ,ulTrimDataLength);
    gVrefData2P0[SITE_ID] = MIN_trimData[MIN_i];

//the last time trim and judge
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(gVrefData2P0[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("AD2p0_Pat", iLen, ch);

	Plvl_Set("PIN_ADOUT");
	dio.Pattern("AD2p0_Pat").Run("cycle_start").WaitEnd();
	sys.DelayUs(1000);

	PpmuResult1.clear();
//	ppmu.Signal("PIN_VREF").SetMode("FNMV").CurrRange(0.04).Execute();
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(2000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);

	sys.PrintDatalog("SITE[%d] Vvref The last time meas volt(PIN_ADOUT): %f, TrimData:%x \n", SITE_ID, PpmuResult1[0].dbValue, gVrefData2P0[SITE_ID]);
	binObj.CheckResultAndBin(0, PpmuResult1, 1); //select site时,必须放在for_each里面

	PpmuResult1[0].dbValue = (double)gVrefData2P0[SITE_ID];
	binObj.CheckResultAndBin(1, PpmuResult1, 1);
	cout << hex << "!!!!!!!!!!!![] Site" << SITE_ID << ", trimdataT:" << gVrefData2P0[SITE_ID] << endl;
    sys.PrintDatalog("gVrefData2P0 %x \n", gVrefData2P0[SITE_ID]);
	gAD2VDataN[SITE_ID] = ( 0x7F - (0xFF & gVrefData2P0[SITE_ID]));
    sys.PrintDatalog("gAD2VDataN %x \n", gAD2VDataN[SITE_ID]);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(10000);

    NEXT_SITE

	ulTrimData = ulTrimStart;
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("AD2p0_Pat", iLen, ch);

	dio.Pattern("VDD_init").Test();
	sys.DelayUs(10000);

	ppmu.Signal("DIO_TRIM").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void AD3P0_test() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_MEAS_RESULT> PpmuResult1;
	vector<ST_MEAS_RESULT> PpmuResult;

//	gVrefData3P0[SITE_NUM] = {0x0000};

	iCount=0;
	double dbCurvolt = 0.0;

	ULONG MIN_trimData[32]={0};
	double MIN_CurVolt[32]={0};
	int MIN_i = 0;

	ULONG ulTrimData = 0x0200, ulTrimStart = 0x02FF; // ulTrimEnd = 0x3FFD;

	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

FOR_EACH_SITE_START(SITE,SITE_ID)
	//first time: run pat, and meas volt of P09
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(4.5).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(1000);

	dio.Pattern("AD2p0_Pat").Test();
	sys.DelayUs(1000);

	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);

	sys.PrintDatalog("SITE[%d] Vref first time (0000 0000 0000 0000--00FF) means volt(PIN_ADOUT): %f \n", SITE_ID, PpmuResult1[0].dbValue);

	//update pat data, run pat, and meas volt of PIN_VREF
	//////////////////////////////////////////////////////////
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;
	double OBj2V = 3.0;

	ulTrimData =0X220;
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[0]=ulTrimData;
	MIN_CurVolt[0]=dbCurvolt;
	if(dbCurvolt<OBj2V)
	{
		ulTrimData =0X240;
	}
	else
	{
		ulTrimData =0X200;
	}
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[1]=ulTrimData;
	MIN_CurVolt[1]=dbCurvolt;
	if(dbCurvolt<OBj2V)
	{
		ulTrimData |= 0x20;

	}
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[2]=ulTrimData;
	MIN_CurVolt[2]=dbCurvolt;
	for(int i=0;i<5;i++)
	{
		ulTrimData += dataPlase[i];
		TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
		MIN_trimData[3+i]=ulTrimData;
		MIN_CurVolt[3+i]=dbCurvolt;
		if(dbCurvolt < OBj2V)
		{
			ulTrimData -= dataPlase[i];
		}
	}




	sys.DelayUs(2);

    int ulTrimDataLength = sizeof(MIN_trimData)/sizeof(ULONG);
    MIN_i = Minfind(SITE_ID, MIN_trimData, MIN_CurVolt, Vvref_VOLT30 ,ulTrimDataLength);
    gVrefData3P0[SITE_ID] = MIN_trimData[MIN_i];
    gVrefData3[SITE_ID] = (MIN_trimData[MIN_i] &= 0xFF);
    sys.PrintDatalog("gVrefData3 %x \n", gVrefData3[SITE_ID]);


//the last time trim and judge
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(gVrefData3P0[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("AD2p0_Pat", iLen, ch);

	Plvl_Set("PIN_ADOUT");
	dio.Pattern("AD2p0_Pat").Run("cycle_start").WaitEnd();
	sys.DelayUs(1000);

	PpmuResult1.clear();
//	ppmu.Signal("PIN_VREF").SetMode("FNMV").CurrRange(0.04).Execute();
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(2000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);

	sys.PrintDatalog("SITE[%d] Vvref The last time meas volt(PIN_ADOUT): %f, TrimData:%x \n", SITE_ID, PpmuResult1[0].dbValue, gVrefData3P0[SITE_ID]);
	binObj.CheckResultAndBin(0, PpmuResult1, 1); //select site时,必须放在for_each里面

	PpmuResult1[0].dbValue = (double)gVrefData3P0[SITE_ID];
	binObj.CheckResultAndBin(1, PpmuResult1, 1);
	cout << hex << "!!!!!!!!!!!![] Site" << SITE_ID << ", trimdataT:" << gVrefData3P0[SITE_ID] << endl;
	sys.PrintDatalog("gVrefData3P0 %x \n", gVrefData3P0[SITE_ID]);
	gAD3VDataN[SITE_ID] = ( 0x7F - (0xFF & gVrefData3P0[SITE_ID]));
	sys.PrintDatalog("gAD3VDataN %x \n", gAD3VDataN[SITE_ID]);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(10000);

    NEXT_SITE

	ulTrimData = ulTrimStart;
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("AD2p0_Pat", iLen, ch);

	dio.Pattern("VDD_init").Test();
	sys.DelayUs(10000);

	ppmu.Signal("DIO_TRIM").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void AD4P0_test() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_MEAS_RESULT> PpmuResult1;
	vector<ST_MEAS_RESULT> PpmuResult;


	iCount=0;
	double dbCurvolt = 0.0;

	ULONG MIN_trimData[32]={0};
	double MIN_CurVolt[32]={0};
	int MIN_i = 0;

	ULONG ulTrimData = 0x0000, ulTrimStart = 0x00FF; // ulTrimEnd = 0x3FFD;

	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

FOR_EACH_SITE_START(SITE,SITE_ID)
	//first time: run pat, and meas volt of P09
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(4.5).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(1000);

//	Plvl_Set("PIN_VREF");
	dio.Pattern("AD2p0_Pat").Test();
	sys.DelayUs(1000);

//	ppmu.Signal("PIN_VREF").SetMode("FNMV").CurrRange(0.04).Execute();
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);

	sys.PrintDatalog("SITE[%d] Vref first time (0000 0000 0000 0000--00FF) means volt(PIN_ADOUT): %f \n", SITE_ID, PpmuResult1[0].dbValue);

	//update pat data, run pat, and meas volt of PIN_VREF
	//////////////////////////////////////////////////////////
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;
	double OBj2V = 4.0;
	ulTrimData =0X120;
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[0]=ulTrimData;
	MIN_CurVolt[0]=dbCurvolt;
	if(dbCurvolt<OBj2V)
	{
		ulTrimData =0X140;
	}
	else
	{
		ulTrimData =0X100;
	}
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[1]=ulTrimData;
	MIN_CurVolt[1]=dbCurvolt;
	if(dbCurvolt<OBj2V)
	{
		ulTrimData |= 0x20;

	}
	TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
	MIN_trimData[2]=ulTrimData;
	MIN_CurVolt[2]=dbCurvolt;
	for(int i=0;i<5;i++)
	{
		ulTrimData += dataPlase[i];
		TrimvoltProcess04(ulTrimData, SITE_ID, "AD2p0_Pat", dbCurvolt);
		MIN_trimData[3+i]=ulTrimData;
		MIN_CurVolt[3+i]=dbCurvolt;
		if(dbCurvolt < OBj2V)
		{
			ulTrimData -= dataPlase[i];
		}
	}
	sys.DelayUs(2);

    int ulTrimDataLength = sizeof(MIN_trimData)/sizeof(ULONG);
    MIN_i = Minfind(SITE_ID, MIN_trimData, MIN_CurVolt, Vvref_VOLT15 ,ulTrimDataLength);
    gVrefData1P5[SITE_ID] = MIN_trimData[MIN_i];
    gVrefData4[SITE_ID] = ((gVrefData1P5[SITE_ID] & 0xFF)<< 8);
    gVrefData3[SITE_ID] |= gVrefData4[SITE_ID];
//    sys.PrintDatalog("gVrefData3 %x \n", gVrefData3[SITE_ID]);

//the last time trim and judge
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(gVrefData1P5[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("AD2p0_Pat", iLen, ch);

	Plvl_Set("PIN_ADOUT");
	dio.Pattern("AD2p0_Pat").Run("cycle_start").WaitEnd();
	sys.DelayUs(1000);

	PpmuResult1.clear();
//	ppmu.Signal("PIN_VREF").SetMode("FNMV").CurrRange(0.04).Execute();
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(2000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);

	sys.PrintDatalog("SITE[%d] Vvref The last time meas volt(PIN_ADOUT): %f, TrimData:%x \n", SITE_ID, PpmuResult1[0].dbValue, gVrefData1P5[SITE_ID]);
	binObj.CheckResultAndBin(0, PpmuResult1, 1); //select site时,必须放在for_each里面

	PpmuResult1[0].dbValue = (double)gVrefData1P5[SITE_ID];
	binObj.CheckResultAndBin(1, PpmuResult1, 1);
	cout << hex << "!!!!!!!!!!!![] Site" << SITE_ID << ", trimdataT:" << gVrefData1P5[SITE_ID] << endl;
	sys.PrintDatalog("gVrefData1P5 %x \n", gVrefData1P5[SITE_ID]);
	gAD4VDataN[SITE_ID] = ( 0x7F - (0xFF & gVrefData1P5[SITE_ID]));
	sys.PrintDatalog("gAD4VDataN %x \n", gAD4VDataN[SITE_ID]);
	pinlevel.Signal("VDD").SetMode("DIO").SetVih(0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(10000);

    NEXT_SITE

	ulTrimData = ulTrimStart;
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(ulTrimData);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVvref("AD2p0_Pat", iLen, ch);

	dio.Pattern("VDD_init").Test();
	sys.DelayUs(10000);

	ppmu.Signal("DIO_TRIM").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM").DisConnect();
	sys.DelayUs(2000);
	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void AD_T() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_DIO_RESULT> FailResult;
	vector<ST_DIO_RECORD> vctRecordList3;

	SITE_DOUBLE ADVref_Data_RD_Out;

	int tmpbit[226]={0};
	int RD_Data_Sum[SITE_NUM]={0};

	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);

	dio.Pattern("AD_T_Pat").SetHRamMode(EN_STV_MODE);
	dio.Pattern("AD_T_Pat").SetTimeOut(1000000).Run();//
	dio.Pattern("AD_T_Pat").HRam().SetReadOption("PDA", 0, 15);

	FOR_EACH_SITE_START(SITE,SITE_ID)

		dio.Pattern("AD_T_Pat").HRam().GetHistData(vctRecordList3);
		ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
		sys.DelayUs(1000);
		ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
		ppmu.Signal("ALLPIN").DisConnect();

		for(int i=0; i<int(vctRecordList3.size());i++)
		 {
				  if(strcmp(vctRecordList3[i].CurrWfc.c_str(),"L")==0)   //读H为 1   读L为 0.
				  {
					tmpbit[i]=0;    //
				  }
				  else
				  {
					tmpbit[i]=1;
				  }
		 }

//		for(int j=0;j<20;j++)
//		{
			RD_Data_Sum[SITE_ID]={0};
			for(int i=3;i<14;i++)  //VREF
			{
				RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i]*(pow(2,(13-i)));

			}
			ADVref_Data_RD_Out[SITE_ID] = RD_Data_Sum[SITE_ID];
			sys.PrintDatalog("RD_Data_Sum[%d] %x \n",SITE_ID, RD_Data_Sum[SITE_ID]);
//		}

	NEXT_SITE

	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	sys.DelayUs(2000);
	binObj.CheckResultAndBin(0, ADVref_Data_RD_Out);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void Config_bg() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_DIO_RESULT> FailResult;
	vector<ST_MEAS_RESULT> PpmuResult1;
	ppmu.Signal("DIO_TRIM").Connect();
	ppmu.Signal("PIN_VREF").Connect();

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5).SetVil(0.0).SetVoh(3.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(1500);
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(1500);

	dio.Pattern("Config_bg_Pat").SetTimeOut(10000).Test();
	dio.Pattern("Config_bg_Pat").GetFailResult(FailResult);
	sys.DelayUs(1000);

	ppmu.Signal("PIN_VREF").SetMode("FNMV").Execute();
	sys.DelayUs(1000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);

	binObj.CheckResultAndBin(0, FailResult);
	binObj.CheckResultAndBin(1, PpmuResult1, 1);

	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	ppmu.Signal("ALLPIN").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void LEAK() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_MEAS_RESULT>Iddq_RESULT;

	ppmu.Signal("ALLPIN").Connect().SetMode("FVMI").VoltForce(0.0).CurrRange(200.0e-6).VoltClamp(2,-2).Execute();
	pinlevel.Signal("ALLPIN").Connect();
	sys.DelayUs(1000);
	sys.DelayUs(2000);

	ppmu.Signal("VDD").SetMode("FVMI").VoltForce(5.0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();
	//ppmu.Signal("VPP").SetMode("FVMI").VoltForce(5.0).CurrRange(5.0e-3).VoltClamp(2,-2).Execute();

	sys.DelayUs(3000);

	ppmu.Signal("P02").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(0, Iddq_RESULT, 1);
	ppmu.Signal("P02").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("P03").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(20000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(1, Iddq_RESULT, 1);
	ppmu.Signal("P03").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("VPP").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(20000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(2, Iddq_RESULT, 1);
	ppmu.Signal("VPP").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("P05").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(20000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(3, Iddq_RESULT, 1);
	ppmu.Signal("P05").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("P06").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(20000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(4, Iddq_RESULT, 1);
	ppmu.Signal("P06").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("P07").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(20000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(5, Iddq_RESULT, 1);
	ppmu.Signal("P07").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);


	ppmu.Signal("P08").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(10000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(6, Iddq_RESULT, 1);
	ppmu.Signal("P08").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);


	ppmu.Signal("P09").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(7, Iddq_RESULT, 1);
	ppmu.Signal("P09").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("P10").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(8, Iddq_RESULT, 1);
	ppmu.Signal("P10").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("P11").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(9, Iddq_RESULT, 1);
	ppmu.Signal("P11").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("P12").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(10, Iddq_RESULT, 1);
	ppmu.Signal("P12").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("P13").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(11, Iddq_RESULT, 1);
	ppmu.Signal("P13").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("PDA").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(12, Iddq_RESULT, 1);
	ppmu.Signal("PDA").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("PCK").SetMode("FVMI").VoltForce(5.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);
	ppmu.Measure(Iddq_RESULT);
	binObj.CheckResultAndBin(13, Iddq_RESULT, 1);
	ppmu.Signal("PCK").SetMode("FVMI").VoltForce(0.0).CurrRange(100.0e-6).VoltClamp(2,-2).Execute();
	sys.DelayUs(1000);

	ppmu.Signal("ALLPIN").SetMode("FVMV").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	ppmu.Signal("ALLPIN").DisConnect();
	sys.DelayUs(2000);
	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void Verify_Config_bg() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_DIO_RESULT> FailResult;
	vector<ST_MEAS_RESULT> PpmuResult1;
	char ch[UPDATE_VCT_NUM16];
	char *chEnd, *pch;
	const char *pTemp;
	int iLen;
	string strData;

	ppmu.Signal("DIO_TRIM").Connect();
	ppmu.Signal("PIN_VREF").Connect();
	sys.DelayUs(2000);
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);


	FOR_EACH_SITE_START(SITE,SITE_ID)

	memset(ch, 0, UPDATE_VCT_NUM16);
	gVrefData_read[SITE_ID] = 0x1e00 - RDVrefData[SITE_ID];
	strData = to_string(gVrefData_read[SITE_ID]);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);
	UpdateVvref("Re_Vvref_Pat", iLen, ch);
	dio.Pattern("Re_Vvref_Pat").Test();
	sys.DelayUs(10000);

	ppmu.Signal("PIN_VREF").SetMode("FNMV").Execute();
	sys.DelayUs(1000);
	ppmu.Signal("PIN_VREF").Measure(PpmuResult1,MEAS_AVG,50,50);
	if(0.9<PpmuResult1[SITE_ID].dbValue&&PpmuResult1[SITE_ID].dbValue<1.1)
	{
		Verify_Config_bg_flag[SITE_ID] = 1;
	}
	else
	{
		Verify_Config_bg_flag[SITE_ID] = 0;
	}

	NEXT_SITE
	pinlevel.Signal("ALLPIN").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	pinlevel.Signal("ALLPIN").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);
	ppmu.Signal("ALLPIN").DisConnect();
	sys.DelayUs(2000);
	binObj.CheckResultAndBin(0, PpmuResult1, 1);



	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

//USER_CODE void Prog_Ft_Flag() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	vector<ST_DIO_RESULT> FailResult;
//	vector<ST_DIO_RESULT> FailResult2;
//	ppmu.Signal("DIO_TRIM").Connect();
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
//	sys.DelayUs(2000);
//	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
//	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(-3e-3).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
//	sys.DelayUs(2000);
//
//	dio.Pattern("Prog_Ft_Flag_Pat").SetTimeOut(10000).Test();//Test();
//	dio.Pattern("Prog_Ft_Flag_Pat").GetFailResult(FailResult);
//	sys.DelayUs(2000);
//	
//	dio.Pattern("Verify_Ft_Flag_Pat").SetTimeOut(10000).Test();//Test();
//	dio.Pattern("Verify_Ft_Flag_Pat").GetFailResult(FailResult2);
//
//	ppmu.Signal("DIO_TRIM").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
//	sys.DelayUs(1000);
//	ppmu.Signal("DIO_TRIM").SetMode("FNMV").Execute();
//	ppmu.Signal("DIO_TRIM").DisConnect();
//	sys.DelayUs(2000);
//	binObj.CheckResultAndBin(0, FailResult);
//	binObj.CheckResultAndBin(1, FailResult2);
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

USER_CODE void Config_hrc() {
	TEST_BEGIN
	vector<ST_MEAS_RESULT> stTMUMeasResult;
	// TODO Edit your code here

	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	dio.Pattern("Config_hrc_Pat").SetTimeOut(10000).Test();//Test();
	sys.DelayUs(2000);

	tmu.SetupTmuOutputCh(1, 1, 2, 29);
	tmu.SetupTmuOutputCh(1, 1, 3, 21);
	tmu.Connect("TMU_CH29");
	sys.DelayUs(5000);
	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(2.5).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(50).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();

	tmu.Freq().Signal("TMU_CH29").StartMeas();
	tmu.Freq().Signal("TMU_CH29").GetMeasResult(stTMUMeasResult);
	sys.DelayUs(2000);
	binObj.CheckResultAndBin(0, stTMUMeasResult, 1);
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(0).SetVol(0).SetActiveLoadStatus(OFF).SetVt(0).SetIoh(0).SetIol(0).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	tmu.DisConnect("TMU_CH29");
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void Config_lrc() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_MEAS_RESULT> stTMUMeasResult;
	// TODO Edit your code here

	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	dio.Pattern("Config_lrc_Pat").SetTimeOut(10000).Test();//Test();
	tmu.SetupTmuOutputCh(1, 1, 2, 29);
	tmu.SetupTmuOutputCh(1, 1, 3, 21);
	tmu.Connect("TMU_CH29");
	tmu.Freq().Signal("TMU_CH29").SetTrigLevel(2.5).SetTrigSlope(TMU_SLOPE_POS).SetSampleSize(50).SetArmSize(5).SetTimeOut(100).SetVoltClamp(6, -1.5).Execute();

	sys.DelayUs(2000);
	tmu.Freq().Signal("TMU_CH29").StartMeas();
	tmu.Freq().Signal("TMU_CH29").GetMeasResult(stTMUMeasResult);
	sys.DelayUs(2000);
	binObj.CheckResultAndBin(0, stTMUMeasResult, 1);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(0).SetVol(0).SetActiveLoadStatus(OFF).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	tmu.DisConnect("TMU_CH29");
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void Config_ad2() {
	TEST_BEGIN

	int iLen;
	char *chEnd, *pch;
	const char *pTemp;
	string strData;
	char ch[UPDATE_VCT_NUM16];

	vector<ST_MEAS_RESULT> PpmuResult1;
	// TODO Edit your code here
	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	VHSX2 = 0X0300;
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(VHSX2);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVHSX("Config_ad_Pat", iLen, ch);
	dio.Pattern("Config_ad_Pat").SetTimeOut(10000).Test();//Test();
	sys.DelayUs(2000);

	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(2000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);
	binObj.CheckResultAndBin(0, PpmuResult1, 1);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(0).SetVol(0).SetActiveLoadStatus(OFF).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void Config_ad3() {
	TEST_BEGIN

	// TODO Edit your code here
	int iLen;
	char *chEnd, *pch;
	const char *pTemp;
	string strData;
	char ch[UPDATE_VCT_NUM16];

	vector<ST_MEAS_RESULT> PpmuResult1;
	// TODO Edit your code here
	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	VHSX2 = 0X0200;
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(VHSX2);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVHSX("Config_ad_Pat", iLen, ch);
	dio.Pattern("Config_ad_Pat").SetTimeOut(10000).Test();//Test();
	sys.DelayUs(2000);

	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(2000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);
	binObj.CheckResultAndBin(0, PpmuResult1, 1);
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(0).SetVol(0).SetActiveLoadStatus(OFF).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void Config_ad4() {
	TEST_BEGIN

	// TODO Edit your code here
	int iLen;
	char *chEnd, *pch;
	const char *pTemp;
	string strData;
	char ch[UPDATE_VCT_NUM16];

	vector<ST_MEAS_RESULT> PpmuResult1;
	// TODO Edit your code here
	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").Connect();
	sys.DelayUs(2000);
	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(1000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	VHSX2 = 0X0100;
	memset(ch, 0, UPDATE_VCT_NUM16);
	strData = to_string(VHSX2);
	pTemp = strData.c_str();
	ltoa(strtol(pTemp, &chEnd, 10), ch, 2);
	pch = &ch[0];
	iLen = strlen(pch);

	UpdateVHSX("Config_ad_Pat", iLen, ch);
	dio.Pattern("Config_ad_Pat").SetTimeOut(10000).Test();//Test();
	sys.DelayUs(2000);

	ppmu.Signal("PIN_ADOUT").SetMode("FNMV").Execute();
	sys.DelayUs(2000);
	ppmu.Measure(PpmuResult1,MEAS_AVG,50,50);
	binObj.CheckResultAndBin(0, PpmuResult1, 1);
	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(0).SetVol(0).SetActiveLoadStatus(OFF).SetVt(4.5).SetIoh(0).SetIol(0.95e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("DIO_TRIM_GRP").SetMode("FNMV").Execute();
	ppmu.Signal("DIO_TRIM_GRP").DisConnect();
	sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

//USER_CODE void PV_Ft_Flag() {
//	TEST_BEGIN
//
//	// TODO Edit your code here
//	
//
//	TEST_ERROR
//	binObj.HandlerException(0);
//	TEST_END
//}

USER_CODE void Read_Config_bg() {
	TEST_BEGIN

	vector<ST_DIO_RESULT> FailResult;
	vector<ST_DIO_RECORD>  vctRecordList1;

	SITE_DOUBLE Result;
	int tmpbit[14]={0};

	int RD_Data_Sum[SITE_NUM]={0};

	SITE_DOUBLE ADVref_Data_RD_Out;
	// TODO Edit your code here
	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4.0).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.5).SetVol(1.0).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.88e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);

	dio.Pattern("Read_Config_bg_Pat").SetHRamMode(EN_STV_MODE);
	dio.Pattern("Read_Config_bg_Pat").SetTimeOut(1000000).Run();//
	dio.Pattern("Read_Config_bg_Pat").HRam().SetReadOption("PDA", 0, 14);

	FOR_EACH_SITE_START(SITE,SITE_ID)


		dio.Pattern("Read_Config_bg_Pat").HRam().GetHistData(vctRecordList1);

		ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
		sys.DelayUs(1000);
		ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
		ppmu.Signal("ALLPIN").DisConnect();
		sys.DelayUs(2000);
		for(int i=0; i<int(vctRecordList1.size());i++)
		 {
				  if(strcmp(vctRecordList1[i].CurrWfc.c_str(),"L")==0)   //读H为 1   读L为 0.
				  {
					tmpbit[i]=0;    //
				  }
				  else
				  {
					tmpbit[i]=1;
				  }
		 }

		for(int i=0;i<14;i++)  //VREF
		{
			RD_Data_Sum[SITE_ID] = RD_Data_Sum[SITE_ID] +tmpbit[i]*(pow(2,(13-i)));

		}

		ADVref_Data_RD_Out[SITE_ID]= RD_Data_Sum[SITE_ID] & 0x1E00 ;
		RDVrefData[SITE_ID] = RD_Data_Sum[SITE_ID]& 0x1E00 ;



	    sys.PrintDatalog(" --------------READ------------------  \n");
		sys.PrintDatalog("SITE[%d] write : gVrefData:%x  \n", SITE_ID,RDVrefData[SITE_ID]);
		NEXT_SITE

		pinlevel.Signal("ALLPIN").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
		sys.DelayUs(2000);
		pinlevel.Signal("ALLPIN").SetMode("DIO").SetVih(0.0).SetVil(0.0).SetVoh(4).SetVol(1).VoltClamp(6.5, -2).Execute();
		sys.DelayUs(2000);
		ppmu.Signal("ALLPIN").DisConnect();
		sys.DelayUs(2000);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void P_Ft_Flag() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_DIO_RESULT> FailResult;
	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.3).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.98e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);

	pinlevel.Signal("VPP").Connect(HIGH_VOLTAGE_MODE);
	sys.DelayUs(2000);
	pinlevel.Signal("VPP").SetMode("VHH").SetVih(5.0).SetVil(0).SetVhh(8.5).SetIoh(0).SetIol(0).Execute();

	dio.Pattern("Prog_Ft_Flag_Pat").SetTimeOut(10000).Run();//Test();
	dio.Pattern("Prog_Ft_Flag_Pat").GetFailResult(FailResult);

	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	ppmu.Signal("ALLPIN").DisConnect();

	sys.DelayUs(2000);
	binObj.CheckResultAndBin(0, FailResult);


	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void V_Ft_Flag() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_DIO_RESULT> FailResult;
	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.3).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.98e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);

	dio.Pattern("Verify_Ft_Flag_Pat").SetTimeOut(10000).Run();//Test();
	dio.Pattern("Verify_Ft_Flag_Pat").GetFailResult(FailResult);

	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	ppmu.Signal("ALLPIN").DisConnect();

	sys.DelayUs(2000);
	binObj.CheckResultAndBin(0, FailResult);

	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

USER_CODE void V_Ft_Flag2() {
	TEST_BEGIN

	// TODO Edit your code here
	vector<ST_DIO_RESULT> FailResult;
	ppmu.Signal("DIO_TRIM").Connect();
	sys.DelayUs(2000);

	pinlevel.Signal("DIO_TRIM").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(3.3).SetVol(1.0).VoltClamp(6.5, -2).Execute();
	pinlevel.Signal("PDA").SetMode("DIO").SetVih(5.0).SetVil(0.0).SetVoh(4).SetVol(1).SetActiveLoadStatus(ON).SetVt(4.5).SetIoh(0).SetIol(0.98e-3).VoltClamp(6.5,-2.0).Execute();
	sys.DelayUs(2000);

	dio.Pattern("Verify_Ft_Flag_Pat").SetTimeOut(10000).Run();//Test();
	dio.Pattern("Verify_Ft_Flag_Pat").GetFailResult(FailResult);

	ppmu.Signal("ALLPIN").SetMode("FVMI").VoltForce(0).CurrRange(0.04).Execute();
	sys.DelayUs(1000);
	ppmu.Signal("ALLPIN").SetMode("FNMV").Execute();
	ppmu.Signal("ALLPIN").DisConnect();

	sys.DelayUs(2000);
	binObj.CheckResult("Param", FailResult);
	TEST_ERROR
	binObj.HandlerException(0);
	TEST_END
}

