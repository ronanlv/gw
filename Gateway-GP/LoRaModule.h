#pragma once

#include <pigpio.h>

#include "serial.h"

#define M0 22
#define M1 27

#define LOW 0
#define HIGH 1

using namespace serial;

enum HatMode {Transmission, Configuration, WOR, DeepSleep};

struct LoRaPackage
{
	unsigned char priority_flg : 1;
	unsigned char hello_msg_type : 1;
	const char* msg_contents;
};

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
	Serial* sp;
	void initialize_module(int frequency, int address, int power, int rssi);
	
public:
	
	LoRaModule()
	{
		sp = new Serial("/dev/ttyS0");

		initialize_module(433, 0, NULL, NULL);
	}
	
	void Send(struct LoRaPackage* lp);
	void Receive();
	void SetHatMode(enum HatMode mode);
	
	void PrintSettings();
};