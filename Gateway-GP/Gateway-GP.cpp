#include <iostream>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

// https://github.com/JoachimSchurig/CppGPIO
#include <cppgpio.hpp>

using namespace std;
using namespace GPIO;

/* GW Purpose (dutch)
   
   De bedoeling is, dat als we de eerste (root) gateway aanzetten. Hij
   een bericht verstuurd en even wacht. Indien hij naar een korte
   periode niets waarneemt. Benoemt hij zich zelf dat root van de
   gateways (de eerste gateway). Dit betekend niet dat het een master/slave netwerk wordt.
   
   Het is nog steeds peer 2 peer maar dit is belangrijk voor traversal.

*/

struct LoRaPackage
{
  unsigned char priority_flg : 1;
  unsigned char hello_msg_type : 1;
  unsigned char* msg_contents;
};

int main(int argc, char *argv[])
{
  std::cout << "Hi\n";
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////// LoRaModule //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
class LoRaModule
{
private:
	
	enum SX1268_UART_BAUDRATE {
    BAUD_1200   = 0x00,
    BAUD_2400   = 0x20,
    BAUD_4800   = 0x40,
    BAUD_9600   = 0x60,
    BAUD_19200  = 0x80,
    BAUD_38400  = 0xA0,
    BAUD_57600  = 0XC0,
    BAUD_115200 = 0XE0
  };

  enum SX1268_PACKET_SIZE {
    PKGSIZE_240 = 0x00,
    PKGSIZE_128 = 0x40,
    PKGSIZE_64  = 0x80,
    PKGSIZE_32  = 0xC0
  };

  enum SX1268_POWER {
    PWR_22 = 0x00,
    PWR_17 = 0x01,
    PWR_13 = 0x02,
    PWR_10 = 0x03
  };

  enum SX1268_AIRSPEED {
    AIRSPD_1200  = 0x01,
    AIRSPD_2400  = 0x02,
    AIRSPD_4800  = 0x03,
    AIRSPD_9600  = 0x04,
    AIRSPD_19200 = 0x05,
    AIRSPD_38400 = 0x06,
    AIRSPD_57600 = 0X07
  };

  struct SX1268_CONFIG
  {
    enum SX1268_UART_BAUDRATE   baudrate;
    enum SX1268_PACKET_SIZE     packetsize;
    enum SX1268_POWER           power;
    enum SX1268_AIRSPEED        airspeed;

  };
	
  bool initialized_ready = false;
  bool rssi = false;
  unsigned int recip_address : 10; /// 1024
	
  void initialize_module(void);

public:
  void send(LoRaPackage lp);
};

void LoRaModule::initialize_module(void)
{

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////// UART ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class UART {
private:
  DigitalOut m0 = DigitalOut(22);
	DigitalOut m1 = DigitalOut(27);

  int fd;

  struct termios tty;
  
  int initialize_serial_port(void);
public:
  UART () {
    m0.off();
    m1.on();

    fd = open("/dev/ttyUSB0", O_RDWR);
    if (fd < 0)
      printf("Error occurred, while opening serial port");

    initialize_serial_port();
  }

  void write(unsigned char* data);
};
