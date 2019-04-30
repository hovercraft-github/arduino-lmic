#include <virtual_main.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
//#include <sys/filio.h>
#include <sys/ioctl.h> // For FIONREAD
#include <termios.h>
#include <stdbool.h>

int kbhit(void)
{
    static bool initflag = false;
    static const int STDIN = 0;

    if (!initflag) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initflag = true;
    }

    int nbbytes;
    ioctl(STDIN, FIONREAD, &nbbytes);  // 0 is STDIN
    return nbbytes;
}

void ctrlc_handler(int s){
  printf("Caught signal %d\n",s);
  uninit();
  exit(0);
}

int main(void)
{
  /*struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = ctrlc_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);*/
  signal (SIGINT, ctrlc_handler);

	clock_gettime(CLOCK_MONOTONIC, &prog_start_time);
	init();
	setup();
	for (;;) {
    //if (kbhit()) break;
		loop();
	}

  uninit();
	return 0;
}
