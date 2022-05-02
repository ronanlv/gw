
#include <pigpio.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

#include "LoRaModule.h"
void LoRaModule::SetHatMode(enum HatMode mode)
{
	switch (mode)
	{
	case Transmission:
		gpioWrite(M0, LOW);
		gpioWrite(M1, LOW);
		break;
	case Configuration:
		gpioWrite(M0, LOW);
		gpioWrite(M1, HIGH);
		break;
	case WOR:
		gpioWrite(M0, HIGH);
		gpioWrite(M1, HIGH);
		break;
	default:
		gpioWrite(M0, LOW);
		gpioWrite(M1, LOW);
		break;
	}
}

void LoRaModule::initialize_module(int frequency, int address, int power, int rssi)
{
	SetHatMode(Configuration);
	
	int rssi_tmp;
	if (rssi)
		rssi_tmp = 0x80;
	else
		rssi_tmp = 0;
	
	// Needs to be integer array. Some values exceed char-length
	int cfg [] = {0xc2, 0x00, 0x09, 0x00, 0x00, 0x00, 0x62, 0x00, 0x12, 0x43, 0x00, 0x00};
	
	cfg[3] = 0x01;
	cfg[4] = 0x02;
	cfg[5] = 0x03;
	cfg[6] = 0x60;
	cfg[7] = 0x04;
	cfg[8] = 0x57;
	cfg[9] = 0x43 + rssi_tmp;
	cfg[10] = 8 & 0xff;
	cfg[11] = 0 & 0xff;
	
	sp->flush();
	
	std::vector<uint8_t> data(cfg, cfg+12);
	sp->write(data);
	usleep(200);
	
	std::vector<uint8_t> ret_buff;
	if (sp->available() > 0)
	{
		sp->waitByteTimes(12);
		std::string tmp = sp->read(12);
//		sp->read(ret_buff, 12);
		
		tmp = tmp.substr(0xc1, 12);
	
		if ((ret_buff[0] == 0xc1))
		{
			std::cout << "[LoRaModule::initialize_module] Setting succeeded.\n";
		}
		else
		{
			std::cout << "Setting failed.\n";
			exit(0);
		}
	}
	
	SetHatMode(Transmission);
}

void LoRaModule::PrintSettings()
{
	SetHatMode(Configuration);
	usleep(100);
	
	std::vector<uint8_t> demand_settings = {0xc1, 0x00, 0x09};
	sp->write(demand_settings);
	
	std::string recv_settings;
	if (sp->available() > 0)
	{
		usleep(100);
		recv_settings = sp->read(12);
	} 
	
	std::vector<uint8_t> recv_cfg;
	for (auto a : recv_settings)
	{
		recv_cfg.push_back(a);
	}
	
	if (recv_cfg[0] == 0xc1 && recv_cfg[0] == 0x09)
	{
		uint8_t freqtmp = recv_cfg[8];
		uint8_t addrtmp = recv_cfg[3] + recv_cfg[4];
		uint8_t airspeedtmp = recv_cfg[6] & 0x03;
		uint8_t powertmp = recv_cfg[7] & 0x03;
		
		std::cout << "Frequency is: " << freqtmp << ".125MHz\n";
		std::cout << "Node address is: " << addrtmp << "\n";
		
		// TODO LowPriority convert human readable
		std::cout << "Air speed is: " << airspeedtmp << "bps\n";
		std::cout << "Power is: " << powertmp << "dBm\n";
	}
	else
	{
		std::cout << "Failure while reading configuration from device.";
		exit(0);
	}
	
	SetHatMode(Transmission);	
}

void LoRaModule::Send(struct LoRaPackage* lp)
{
	SetHatMode(Transmission);
	usleep(100);
	
	// TODO Priority flags, hello_msg_type
	std::string data(lp->msg_contents);
	sp->write(data);
	
	usleep(100);
}

void LoRaModule::Receive()
{
	std::string rcv;
	
	if (sp->available() > 0)
	{
		usleep(500);
		while (sp->available())
		{
			sp->read(rcv);
		}
		
		std::string msg = rcv.substr(3);
		std::cout << "Message :: " << msg;
	}
}