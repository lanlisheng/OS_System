#ifndef _PARA_H
#define _PARA_H

#define PARA_DTC_SUM 20 //֧��̽������������

#define STU_DEVICEPARA_OFFSET 0

/* 2. ̽������������ */
typedef enum {
  ZONE_TYO_24HOURS, // 24Сʱ����
  ZONE_TYP_1ST,     //��������
  ZONE_TYP_2ND,     //�ڼҲ���
  STG_DEV_AT_SUM
} ZONE_TYPED_TYPEDEF;

/* 3. ̽�������� */
typedef enum {
  DTC_DOOR,       //�Ŵ�̽����
  DTC_PIR_MOTION, //������̽����
  DTC_REMOTE,     //ң����
  DTC_TYP_SUM
} DTC_TYPE_TYPEDEF; //̽������������ֵ

/*  1. ����̽�������� �Ľṹ�� */
typedef struct {
  unsigned char ID;            //̽����ID
  unsigned char Mark;          //״̬��0-δѧϰ  1-��ѧϰ
  unsigned char NameNum;       //̽�����������
  unsigned char Name[16];      //�豸����
  DTC_TYPE_TYPEDEF DTCType;    //�豸����
  ZONE_TYPED_TYPEDEF ZoneType; //��������
  unsigned char Code[3];       // 1527  2262 24Bit����
} Stu_DTC;

void ParaInit(void);
void FactoryReset(void);
unsigned char DtcMatching(unsigned char *pCode);
unsigned char GetDtcNum(void);
void DelSpecifyDtc(Stu_DTC *pdPara);
unsigned char AddDtc(Stu_DTC *pDevPara);
unsigned char CheckPresenceofDtc(unsigned char i);
unsigned char GetDtcID(unsigned char idx);
void SetDtcAbt(unsigned char id, Stu_DTC *psDevPara);

void GetDtcStu(Stu_DTC *pdDevPara, unsigned char id);

#endif