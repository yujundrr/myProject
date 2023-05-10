#include "em.h"

#define PRINT_SENT 1

//
//// EIS MethodSCRIPT
//char const * EIS_ON_WE_C =   "e\n"
//                             // declare variables for frequency, real and imaginary parts of complex result
//                             "var f\n"
//                             "var r\n"
//                             "var j\n"
//                             // set to channel 0 (Lemo)
//                             "set_pgstat_chan 0\n"
//                             // set mode to High Speed
//                             "set_pgstat_mode 3\n"
//                             // set to 50 uA current range
//                             "set_range ba 50u\n"
//                             "set_autoranging ba 50u 500u\n"
//                             "cell_on\n"
//                             // call the EIS loop with 15 mV amplitude, f_start = 200 kHz, f_end = 100 Hz, nrOfPoints = 51, 0 mV DC
//                             "meas_loop_eis f r j 15m 200k 100 51 0m\n"
//                             // add the returned variables to the data package
//                             "pck_start\n"
//                             "pck_add f\n"
//                             "pck_add r\n"
//                             "pck_add j\n"
//                             "pck_end\n"
//                             "endloop\n"
//                             "on_finished:\n"
//                             "cell_off\n"
//                             "\n";

// The MethodSCRIPT communication object.  MethodSCRIPTͨ�Ŷ���
MSComm _msComm;

int _nDataPoints = 0;
char _versionString[30];

static int s_printSent = 0;
static int s_printReceived = 1;
char const * CMD_VERSION_STRING = "t\n";

// We have to give MSComm some functions to communicate with the EmStat Pico (in MSCommInit).
// However, because the C compiler doesn't understand C++ classes,
// we must wrap the write/read functions from the Serial class in a normal function first.
// We are using Serial and Serial1 here, but you can use any serial port.
//���Ǳ����MSCommһЩ��������EmStat Picoͨ��(��MSComm��)��
//���ǣ���ΪC�����������c++�࣬
//���ȣ����Ǳ��뽫Serial���е�д/��������װ��һ����ͨ�����С�
//����������ʹ����Serial��Serial1���������ʹ���κδ��ڡ�

// ͨ��serial1���͵�emstat pico��
int writeToem(uint8_t c)
{
  if (PRINT_SENT) {
    // Send all data to PC if required for debugging purposes (s_printReceived to be set to true)
    // ���������Ҫ�����������ݷ��͵�PC (s_printReceived����Ϊtrue)
    comSendChar(COM1, c);
  }
  // Write a character to the device  ���豸д��һ���ַ�
  comSendChar(COM3, c);
  return 1;
}

// ͨ��serial1��ȡemstat pico
uint8_t readFromEm()
{
  // Read a character from the device  ���豸��ȡһ���ַ�
  uint8_t c;
  if (comGetChar(COM3, &c))
  {
      if (PRINT_SENT && (c != -1)) { // -1 means no data  -1��ʾû������
        // Send all received data to PC if required for debugging purposes (s_printReceived to be set to true)
        //���������Ҫ�������н��յ������ݷ��͵�PC��(s_printReceived����Ϊtrue)
        comSendChar(COM1, c);
      }
  }
  return c;
}

// ͨ��serial1���͵�emstat pico��
int writeToPc(uint8_t c)
{
//  if (PRINT_SENT) {
//    // Send all data to PC if required for debugging purposes (s_printReceived to be set to true)
//    // ���������Ҫ�����������ݷ��͵�PC (s_printReceived����Ϊtrue)
//    comGetChar(COM1, &c);
//  }
  // Write a character to the device  ���豸д��һ���ַ�
   comSendChar(COM1, c);
  return 1;
}

// ͨ��serial1��ȡemstat pico
uint8_t readToPc()
{
  // Read a character from the device  ���豸��ȡһ���ַ�
  uint8_t c;
  comGetChar(COM1, &c);

  if (PRINT_SENT && (c != -1)) { // -1 means no data  -1��ʾû������
    // Send all received data to PC if required for debugging purposes (s_printReceived to be set to true)
    //���������Ҫ�������н��յ������ݷ��͵�PC��(s_printReceived����Ϊtrue)
    comSendChar(COM3, c);
  }
  return c;
}

// The MethodSCRIPT is sent to the device through the Serial1 port
// MethodSCRIPTͨ��Serial1�˿ڷ��͵��豸
void SendScriptToDevice(char const * scriptText)
{
    RetCode code = MSCommInit(&_msComm, &writeToem, &readFromEm);
    WriteStr(&_msComm, scriptText);
}

#if 0
// Verify if connected to EmStat Pico by checking the version string contains the "espico"
// ������"espico"�İ汾�ַ�������֤�Ƿ����ӵ�EmStat Pico
int VerifyESPico()
{
  int i = 0;
  int isConnected = 0;
  RetCode code;

  SendScriptToDevice(CMD_VERSION_STRING);
  while (!Serial1.available()) {
    // Wait until data is available on Serial1  //�ȴ�Serial1�ϵ����ݿ���
  }
  while (Serial1.available()) {
    code = ReadBuf(&_msComm, _versionString);
    if (code == CODE_VERSION_RESPONSE) {
      if (strstr(_versionString, "espbl") != NULL) {
        writeToPc("EmStat Pico is connected in boot loader mode.");
        isConnected = 0;

      // Verify if the device is EmStat Pico by looking for "espico" in the version response
      // ͨ���ڰ汾��Ӧ�в���"espico"����֤�豸�Ƿ�ΪEmStat Pico
      } else if (strstr(_versionString, "espico") != NULL) {
        writeToPc("Connected to EmStat Pico");
        isConnected = 1;
      }

    //Read until end of response and break
    } else if(strstr(_versionString, "*\n") != NULL) {
      break;

    } else {
      writeToPc("Connected device is not EmStat Pico");
      isConnected = 0;
    }
    delay(20);
  }
  return isConnected;
}


int EmTask ()
{

    //Init MSComm struct (one for every EmStat Pico).   Init MSComm�ṹ��(ÿ��EmStat Pico��Ӧһ��)��
    RetCode code = MSCommInit(&_msComm, &writeToem, &readFromEm);
    if (code == CODE_OK) 
    {
        while (VerifyESPico()) 
        {
            // ����LSV_ON_10KOHM�������ű����͵��豸
            // SendScriptToDevice(EIS_ON_WE_C); 
        }
    }

    while(1) 
    {
        int package_nr = 0;
          // put your main code here, to run repeatedly:  �������������������������:
          MscrPackage package;
          char current[20];
          char readingStatus[16];
          // If we have any buffered messages waiting for us  ��������л�����Ϣ��������
          // Read from the device and parses the response  //���豸��ȡ���ݲ�������Ӧ
            RetCode code = ReceivePackage(&_msComm, &package);
          while (code != -1) {
            switch (code) {
            case CODE_RESPONSE_BEGIN: // Measurement response begins  //��ʼ������Ӧ
              writeToPc("\n");
              writeToPc("Response begin\n");
              writeToPc("\n");
              break;
            case CODE_MEASURING:
              writeToPc("\n");
              Serial.print("Measuring...\n");
              writeToPc("\n");
              package_nr = 0;
              break;
            case CODE_OK: // Received valid package, print it.  //��ӡ�յ�����Ч��
              if (package_nr++ == 0) {
                writeToPc("\n");
                writeToPc("Receiving measurement response:\n");
              }
              // Print package index (starting at 1 on the console)  //��ӡ��������(�ӿ���̨��1��ʼ)
              writeToPc(package_nr);
              // Print all subpackages in  //��ӡ���е��Ӱ�
              for (int i = 0; i < package.nr_of_subpackages; i++) {
                PrintSubpackage(package.subpackages[i]);
              }
              writeToPc("\n");
              break;
            case CODE_MEASUREMENT_DONE: // Measurement loop complete  //����ѭ�����
              writeToPc("\n");
              writeToPc("\n");("Measurement completed.\n");
              break;
            case CODE_RESPONSE_END: // Measurement response end  //������Ӧ����
              writeToPc(package_nr);
              writeToPc(" data point(s) received.\n");
              break;
            default: // Failed to parse or identify package  //�޷�������ʶ���
              writeToPc("\n");
              writeToPc("\n");("Failed to parse package: \n");
              writeToPc("\n");
            }
          }
   }

}

#endif

