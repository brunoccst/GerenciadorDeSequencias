#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <string.h>

using namespace std;
 
#define IN  0
#define OUT 1
 
#define LOW  0
#define HIGH 1

// Definicao dos botoes
#define B1 25
#define B2 24
#define B3 23
 

int fd;
char a[4];
char b;

int config_serial(char * device, unsigned int baudrate){
	 struct termios options;
	 int fd;

	 fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY );
	 if (fd < 0)
	 {
	 /*
	 * Could not open the port.
	 */

	 perror("config_serial: Não pode abrir a serial - ");
	 return -1;
	 }
	 fcntl(fd, F_SETFL, 0);
	 /*
	 * Get the current options for the port...
	 */
	 tcgetattr(fd, &options);

	 /* sets the terminal to something like the "raw" mode */
	 cfmakeraw(&options);

	 /*
	 * Set the baudrate...
	 */
	 cfsetispeed(&options, baudrate);
	 cfsetospeed(&options, baudrate);
	 /*
	 * Enable the receiver and set local mode...
	 */
	 options.c_cflag |= (CLOCAL | CREAD);

	 /*
	 * No parit, 1 stop bit, size 8
	 */
	 options.c_cflag &= ~PARENB;
	 options.c_cflag &= ~CSTOPB;
	 options.c_cflag &= ~CSIZE;
	 options.c_cflag |= CS8;

	 /*
	 * Clear old settings
	 */
	 options.c_cflag &= ~CRTSCTS;
	 options.c_iflag &= ~(IXON | IXOFF | IXANY);

	 /* non-caninical mode */
	 options.c_lflag &= ~ICANON;
	 /*
	 * Set the new options for the port...
	 */
	 tcsetattr(fd, TCSANOW, &options);

	 /* configura a tty para escritas e leituras não bloqueantes */
	 //fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

	 return fd;
}

static int GPIOExport(int pin)
{
        #define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
static int GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
static int GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";
 
        #define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;
 
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}
 
	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 
static int GPIORead(int pin)
{
        #define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}
 
	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}
 
	close(fd);
 
	return(atoi(value_str));
}
 
static int GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}
 
	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}

/*
* Enable GPIO pins
*/
bool enableButtons()
{
	if (-1 == GPIOExport(B1))
		return true;	
	if (-1 == GPIOExport(B2))
		return true;	
	if (-1 == GPIOExport(B3))
		return true;	
	return false;	
}

/*
* Disable GPIO pins
*/
bool disableButtons()
{
	if (-1 == GPIOUnexport(B1))
		return true;		
	if (-1 == GPIOUnexport(B2))
		return true;		
	if (-1 == GPIOUnexport(B3))
		return true;	
	return false;	
}

bool writeButton(int button)
{
	/*
     * Set GPIO directions
     */
    if (-1 == GPIODirection(button, OUT))
      return true;

    /*
     * Write GPIO value
     */
    if (-1 == GPIOWrite(button, 1))
		return true;
 
    /*
     * Set GPIO directions
     */
    if (-1 == GPIODirection(button, IN))
      return true;
}

void sequenciator()
{
	if (!GPIORead(B1))
	{
		a[0] = '1';
		a[1] = '1';
		a[2] = '1';
		a[3] = '1';
		b = '1';
		write(fd,&b,1);
		printf("%d\n",1);
		//nextSequence();
		usleep(1000000);
	}
	else if (!GPIORead(B2))
	{
		a[0] = '2';
		a[1] = '2';
		a[2] = '2';
		a[3] = '2';
		b = '2';
		write(fd,&b,1);
		printf("%d\n",2);
		//previousSequence();	
		usleep(1000000);
	}
	else if (!GPIORead(B3))
	{
		a[0] = '3';
		a[1] = '3';
		a[2] = '3';
		a[3] = '3';
		b = '3';
		write(fd,&b,1);
		read(fd,&a,4);
		printf("Uso de cada sequencia:\n",3);
		printf("sequencia 1: %s\n",a[0]);
		printf("sequencia 2: %s\n",a[1]);
		printf("sequencia 3: %s\n",a[2]);
		printf("sequencia 4: %s\n",a[3]);
		//printSequences();	
		usleep(1000000);
	}
}
 
int main(int argc, char *argv[])
{
	fd = config_serial("/dev/ttyAMA0", B9600);
	if(fd<0){
		return 0;
	}

	int repeat = 10000;

	if (enableButtons())
		return(1);

	do {

		if(writeButton(B1) || writeButton(B2) || writeButton(B3))
			return(2);

		sequenciator();
	}
	while (1);

	/*
	* Disable GPIO pins
	*/

	if(disableButtons())
		return(4);

	return(0);
}
