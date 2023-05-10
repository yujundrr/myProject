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

// The MethodSCRIPT communication object.  MethodSCRIPT通信对象。
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
//我们必须给MSComm一些函数来与EmStat Pico通信(在MSComm中)。
//但是，因为C编译器不理解c++类，
//首先，我们必须将Serial类中的写/读函数包装在一个普通函数中。
//我们在这里使用了Serial和Serial1，但你可以使用任何串口。

// 通过serial1发送到emstat pico上
int writeToem(uint8_t c)
{
  if (PRINT_SENT) {
    // Send all data to PC if required for debugging purposes (s_printReceived to be set to true)
    // 如果调试需要，将所有数据发送到PC (s_printReceived设置为true)
    comSendChar(COM1, c);
  }
  // Write a character to the device  向设备写入一个字符
  comSendChar(COM3, c);
  return 1;
}

// 通过serial1读取emstat pico
uint8_t readFromEm()
{
  // Read a character from the device  从设备读取一个字符
  uint8_t c;
  if (comGetChar(COM3, &c))
  {
      if (PRINT_SENT && (c != -1)) { // -1 means no data  -1表示没有数据
        // Send all received data to PC if required for debugging purposes (s_printReceived to be set to true)
        //如果调试需要，将所有接收到的数据发送到PC机(s_printReceived设置为true)
        comSendChar(COM1, c);
      }
  }
  return c;
}

// 通过serial1发送到emstat pico上
int writeToPc(uint8_t c)
{
//  if (PRINT_SENT) {
//    // Send all data to PC if required for debugging purposes (s_printReceived to be set to true)
//    // 如果调试需要，将所有数据发送到PC (s_printReceived设置为true)
//    comGetChar(COM1, &c);
//  }
  // Write a character to the device  向设备写入一个字符
   comSendChar(COM1, c);
  return 1;
}

// 通过serial1读取emstat pico
uint8_t readToPc()
{
  // Read a character from the device  从设备读取一个字符
  uint8_t c;
  comGetChar(COM1, &c);

  if (PRINT_SENT && (c != -1)) { // -1 means no data  -1表示没有数据
    // Send all received data to PC if required for debugging purposes (s_printReceived to be set to true)
    //如果调试需要，将所有接收到的数据发送到PC机(s_printReceived设置为true)
    comSendChar(COM3, c);
  }
  return c;
}

// The MethodSCRIPT is sent to the device through the Serial1 port
// MethodSCRIPT通过Serial1端口发送到设备
void SendScriptToDevice(char const * scriptText)
{
    RetCode code = MSCommInit(&_msComm, &writeToem, &readFromEm);
    WriteStr(&_msComm, scriptText);
}

#if 0
// Verify if connected to EmStat Pico by checking the version string contains the "espico"
// 检查包含"espico"的版本字符串，验证是否连接到EmStat Pico
int VerifyESPico()
{
  int i = 0;
  int isConnected = 0;
  RetCode code;

  SendScriptToDevice(CMD_VERSION_STRING);
  while (!Serial1.available()) {
    // Wait until data is available on Serial1  //等待Serial1上的数据可用
  }
  while (Serial1.available()) {
    code = ReadBuf(&_msComm, _versionString);
    if (code == CODE_VERSION_RESPONSE) {
      if (strstr(_versionString, "espbl") != NULL) {
        writeToPc("EmStat Pico is connected in boot loader mode.");
        isConnected = 0;

      // Verify if the device is EmStat Pico by looking for "espico" in the version response
      // 通过在版本响应中查找"espico"来验证设备是否为EmStat Pico
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

    //Init MSComm struct (one for every EmStat Pico).   Init MSComm结构体(每个EmStat Pico对应一个)。
    RetCode code = MSCommInit(&_msComm, &writeToem, &readFromEm);
    if (code == CODE_OK) 
    {
        while (VerifyESPico()) 
        {
            // 将“LSV_ON_10KOHM”方法脚本发送到设备
            // SendScriptToDevice(EIS_ON_WE_C); 
        }
    }

    while(1) 
    {
        int package_nr = 0;
          // put your main code here, to run repeatedly:  把你的主代码放在这里，反复运行:
          MscrPackage package;
          char current[20];
          char readingStatus[16];
          // If we have any buffered messages waiting for us  如果我们有缓冲消息等着我们
          // Read from the device and parses the response  //从设备读取数据并解析响应
            RetCode code = ReceivePackage(&_msComm, &package);
          while (code != -1) {
            switch (code) {
            case CODE_RESPONSE_BEGIN: // Measurement response begins  //开始测量响应
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
            case CODE_OK: // Received valid package, print it.  //打印收到的有效包
              if (package_nr++ == 0) {
                writeToPc("\n");
                writeToPc("Receiving measurement response:\n");
              }
              // Print package index (starting at 1 on the console)  //打印包的索引(从控制台的1开始)
              writeToPc(package_nr);
              // Print all subpackages in  //打印所有的子包
              for (int i = 0; i < package.nr_of_subpackages; i++) {
                PrintSubpackage(package.subpackages[i]);
              }
              writeToPc("\n");
              break;
            case CODE_MEASUREMENT_DONE: // Measurement loop complete  //测量循环完成
              writeToPc("\n");
              writeToPc("\n");("Measurement completed.\n");
              break;
            case CODE_RESPONSE_END: // Measurement response end  //测量响应结束
              writeToPc(package_nr);
              writeToPc(" data point(s) received.\n");
              break;
            default: // Failed to parse or identify package  //无法解析或识别包
              writeToPc("\n");
              writeToPc("\n");("Failed to parse package: \n");
              writeToPc("\n");
            }
          }
   }

}

#endif

