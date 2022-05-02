#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>
#include <glob.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pigpio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include <pigpio.h>

#include "LoRaModule.h"

void intHandler(int sig);
void cleanUp();

using namespace std;
using namespace serial;

/* GW Purpose (dutch)
   
   De bedoeling is, dat als we de eerste (root) gateway aanzetten. Hij
   een bericht verstuurd en even wacht. Indien hij naar een korte
   periode niets waarneemt. Benoemt hij zich zelf dat root van de
   gateways (de eerste gateway). Dit betekend niet dat het een master/slave netwerk wordt.
   
   Het is nog steeds peer-to-peer maar dit is belangrijk voor traversal.
*/

int main() {
	signal(SIGINT, intHandler);

	if (gpioInitialise() < 0) {
#ifdef __DEBUG__
		printf("pigpio initialisation failed.");
#endif
	}
	
	Serial* serial = new Serial();
	LoRaModule* loramod = new LoRaModule();


	gpioSetMode(M0, PI_OUTPUT);
	gpioSetMode(M1, PI_OUTPUT);

	loramod->SetHatMode(Configuration);

	while (1)
	{
	
	}
	
	cleanUp();
}

void intHandler(int sig) {
	char c;

	signal(sig, SIG_IGN);

	printf("Do you really want to quit? [y/n] ");

	c = getchar();
	if (c == 'y' || c == 'Y')
		cleanUp();
	else
		signal(SIGINT, intHandler);
	getchar();
	exit(0);
}

void cleanUp() {
	gpioTerminate();
}