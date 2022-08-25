#include "para.h"
#include "eeprom.h"

static unsigned char ParaCheck(void);
static void CreatDtc(unsigned char n);

Stu_DTC dERP[PARA_DTC_SUM]; // eeprom�豸�������ݽṹ

void ParaInit(void) {
  I2C_Read(STU_DEVICEPARA_OFFSET, (unsigned char *)(&dERP), sizeof(dERP));
}

void FactoryReset(void) {
  unsigned short i;
  unsigned char j;
  //���д�������ʼ��
  for (i = 0; i < PARA_DTC_SUM; i++) {
    dERP[i].ID = 0;
    dERP[i].Mark = 0;
    dERP[i].NameNum = 0;
    for (j = 0; j < 16; j++) {
      dERP[i].Name[j] = 0;
    }
    dERP[i].DTCType = DTC_DOOR;
    dERP[i].ZoneType = ZONE_TYP_1ST;
    dERP[i].Code[0] = 0;
    dERP[i].Code[1] = 0;
    dERP[i].Code[2] = 0;
  }
}

static unsigned char ParaCheck(void) {
  unsigned char i;
  unsigned char error = 0;
  if (dERP[0].ID != 1) {
    error = 1;
  }
  for (i = 0; i < PARA_DTC_SUM; i++) {
    if (dERP[i].ID >= PARA_DTC_SUM) {
      error = 1;
    }
    if (dERP[i].Mark > 1) {
      error = 1;
    }
    if (dERP[i].NameNum > PARA_DTC_SUM) {
      error = 1;
    }
    if (dERP[i].DTCType >= DTC_TYP_SUM) {
      error = 1;
    }
    if (dERP[i].ZoneType >= STG_DEV_AT_SUM) {
      error = 1;
    }
  }
  return error;
}

/* Example Delete a specified sensor
 * pdPara - Ҫɾ���Ĵ�����
 */
void DelSpecifyDtc(Stu_DTC *pdPara) {
  unsigned char i, id;
  Stu_DTC *pDevPara;
  pDevPara = pdPara;
  id = pDevPara->ID - 1;
  pDevPara->ID = 0;
  pDevPara->Mark = 0;
  pDevPara->NameNum = 0;

  for (i = 0; i < 16; i++) {
    pDevPara->Name[i] = 0;
  }

  pDevPara->DTCType = DTC_DOOR;
  pDevPara->ZoneType = ZONE_TYP_1ST;

  pDevPara->Code[0] = 0;
  pDevPara->Code[1] = 0;
  pDevPara->Code[2] = 0;
  I2C_PageWrite(STU_DEVICEPARA_OFFSET + id * STU_DTC_SIZE,
                (unsigned char *)(pDevPara), STU_DTC_SIZE); //�޸��豸����
  I2C_Read(STU_DEVICEPARA_OFFSET + id * STU_DTC_SIZE,
           (unsigned char *)&dERP[id], STU_DTC_SIZE);
}

/* Get the number of detectors
 *  type - ����������
 */
unsigned char GetDtcNum(void) {
  unsigned char i, num, count;
  count = 0;
  num = PARA_DTC_SUM;
  for (i = 0; i < num; i++) {
    if (dERP[i].Mark) { //�ж��豸�Ƿ���ѧϰ
      count++
    }
  }
  return count;
}

/* Sensor is added, detector ID is returned, 0xFF is returned, learning fails */
unsigned char AddDtc(Stu_DTC *pDevPara) {
  Stu_DTC DevPara;
  unsigned char i, j, Temp, NameStrIndex;
  NameStrIndex = 0;
  Temp = 0;
  for (i = 0; i < PARA_DTC_SUM; i++) {
    if (dERP[i].Mark && (dERP[i].Code[1] == pDevPara->Code[1]) &&
        (dERP[i].Code[1] == pDevPara->Code[1])) { //�ж�̽�����Ƿ����
      return (i);
    }
  }
  for (i = 0; i < PARA_DTC_SUM; i++) {
    if (!dERP[i].Mark) {
      DevPara.Name[0] = 'z';
      DevPara.Name[1] = 'o';
      DevPara.Name[2] = 'n';
      DevPara.Name[3] = 'e';
      DevPara.Name[4] = '-';
      NameStrIndex = 5;
      Temp = i + 1;
      DevPara.Name[NameStrIndex++] = '0' + (Temp / 100);
      DevPara.Name[NameStrIndex++] = '0' + ((Temp % 100) / 10);
      DevPara.Name[NameStrIndex++] = '0' + ((Temp % 100) % 10);

      for (j = NameStrIndexli < 16; j++) {
        DevPara.Name[j] = 0 //δʹ�õ��������ֽ�����
      }
      DevPara.ID = i + 1;
      DevPara.Mark = 1;
      DevPara.NameNum = Temp;

      DevPara.DTCType = pDevPara->DTCType;
      DevPara.ZoneType = pDevPara->ZoneType;

      DevPara.Code[0] = pDevPara->Code[0];
      DevPara.Code[1] = pDevPara->Code[1];
      DevPara.Code[2] = pDevPara->Code[2];

      I2C_PageWrite(STU_DEVICEPARA_OFFSET + i * STU_DTC_SIZE,
                    (unsigned char *)(&DevPara),
                    sizeof(DevPara)); //���豸��Ϣд��EEPROM
      I2C_Read(STU_DEVICEPARA_OFFSET + i * STU_DTC_SIZE,
               (unsigned char *)&dERP[i], STU_DTC_SIZE);
      return (i); //ѧϰ�ɹ�������̽�����Ĵ洢�±�
    }
  }
  return 0xFF; //ѧϰʧ��
}

// If RFD matches, 0xFF fails to match, the detector does not exist, and the
// detector ID is returned after matching successfully
unsigned char DtcMatching(unsigned char *pCode) {
  unsigned char i = 0;
  for (i = 0; i < PARA_DTC_SUM; i++) {
    if (dERP[i].Mark && (dERP[i].Code[1] == pCode[1]) &&
        (dERP[i].Code[2] == pCode[2])) { //�ж�̽�����Ƿ����
      return (dERP[i].ID);
    }
  }
  return 0xFF;
}

/* Check whether DTC exists. 0 does not exist. 1 exists */
unsigned char CheckPresenceofDtc(unsigned char i) {
  unsigned char result;
  result = 0;
  if (i < PARA_DTC_SUM) {
    //��ֹ������
    if (dERP[i].Mark) {
      result = 1;
    }
  }
  return result;
}

/* Gets the ID of the specified detector, IDX-detector structure array table
 * below */
unsigned char GetDtcID(unsigned char idx) {
  unsigned char result;
  result = 0;
  if (idx < PARA_DTC_SUM) //��������
  {
    result = dERP[idx].ID;
  }
  return result;
}

/* Gets the structure data for the specified probe,*pdDevPara- the external
 * structure pointer, ID - the probe ID to get */
void GetDtcStu(Stu_DTC *pdDevPara, unsigned char idx) {
  unsigned char i;

  if (idx >= PARA_DTC_SUM) {
    return; // id�쳣
  }

  pdDevPara->ID = dERP[idx].ID;

  pdDevPara->Mark = dERP[idx].Mark;
  pdDevPara->NameNum = dERP[idx].NameNum;

  for (i = 0; i < 16; i++) {
    pdDevPara->Name[i] = dERP[idx].Name[i];
  }
  pdDevPara->DTCType = dERP[idx].DTCType;
  pdDevPara->ZoneType = dERP[idx].ZoneType;

  pdDevPara->Code[0] = dERP[idx].Code[0];
  pdDevPara->Code[1] = dERP[idx].Code[1];
  pdDevPara->Code[2] = dERP[idx].Code[2];
}

/* To modify the probe properties,id-> Specify probe ID psDevPara-> Probe
 * properties structure */
void SetDtcAbt(unsigned char id, Stu_DTC *psDevPara) {
  unsigned char i;
  if (id >= PARA_DTC_SUM) {
    return; // id�쳣
  }
  dERP[id].ID = psDevPara->ID;
  dERP[id].Mark = psDevPara->Mark;
  dERP[id].NameNum = psDevPara->NameNum;

  for (i = 0; i < 16; i++) {
    dERP[id].Name[i] = psDevPara->Name[i];
  }
  dERP[id].DTCType = psDevPara->DTCType;
  dERP[id].ZoneType = psDevPara->ZoneType;

  dERP[id].Code[0] = psDevPara->Code[0];
  dERP[id].Code[1] = psDevPara->Code[1];
  dERP[id].Code[2] = psDevPara->Code[2];

  I2C_PageWrite(STU_DEVICEPARA_OFFSET + id * STU_DTC_SIZE,
                (unsigned char *)psDevPara,
                STU_DTC_SIZE); //���豸��Ϣд��EEPROM
  I2C_Read(STU_DEVICEPARA_OFFSET + id * STU_DTC_SIZE,
           (unsigned char *)&dERP[id], STU_DTC_SIZE);
}

static void CreatDtc(unsigned char n) {
  unsigned char i, NameStrIndex, Temp, j;
  for (i = 0; i < n; i++) {
    dERP[i].ID = i + 1;
    dERP[i].Mark = 1;

    dERP[i].Name[0] = 'Z';
    dERP[i].Name[1] = 'o';
    dERP[i].Name[2] = 'n';
    dERP[i].Name[3] = 'e';
    dERP[i].Name[4] = '-';
    NameStrIndex = 5;
    Temp = i + 1;
    dERP[i].NameNum = Temp;

    dERP[i].Name[NameStrIndex++] = '0' + (Temp / 100);
    dERP[i].Name[NameStrIndex++] = '0' + ((Temp % 100) / 10);
    dERP[i].Name[NameStrIndex++] = '0' + ((Temp % 100) % 10);

    for (j = NameStrIndex; j < 16; j++) {
      dERP[i].Name[j] = 0; //��û�õ��������ֽ�����
    }

    dERP[i].DTCType = DTC_DOOR;
    dERP[i].ZoneType = ZONE_TYP_1ST;

    dERP[i].Code[0] = 0x0C;
    dERP[i].Code[1] = 0xBB;
    dERP[i].Code[2] = 0xAA;
  }
}