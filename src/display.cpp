#define MAX7219_REG_NOOP 0x0
#define MAX7219_REG_DIGIT0 0x1
#define MAX7219_REG_DIGIT1 0x2
#define MAX7219_REG_DIGIT2 0x3
#define MAX7219_REG_DIGIT3 0x4
#define MAX7219_REG_DIGIT4 0x5
#define MAX7219_REG_DIGIT5 0x6
#define MAX7219_REG_DIGIT6 0x7
#define MAX7219_REG_DIGIT7 0x8
#define MAX7219_REG_DECODEMODE 0x09
#define MAX7219_REG_INTENSITY 0xA
#define MAX7219_REG_SCANLIMIT 0xB
#define MAX7219_REG_SHUTDOWN 0xC
#define MAX7219_REG_DISPLAYTEST 0xF

#define SYSFS_GPIO_DIR "/sys/class/gpio"

#include "display.h"

using namespace SYNTHPI;

// MAX7219

MAX7219::MAX7219(){

	
	 //fd = open(settings.spiDevice.c_str(), O_RDWR);
        fd = open("/dev/spidev0.0", O_RDWR);
		//std::cout << "devices opened: " << std::endl;
	if (fd < 0){
		throw "Can't open device";
	}
	// set SPI mode
	int ret = ioctl(fd, SPI_IOC_WR_MODE, 1);
	//std::cout << "Spi mode set " << std::endl;
	if (ret == -1) {
		//throw "can't set spi mode";
	}
	//std::cout << "device setup" << std::endl;


     //Allocate digit buffer and init to 0
     digitBuffer.assign(numDigits, 0);



    //Set command regs
    command(MAX7219_REG_DECODEMODE, decodeMode);
    command(MAX7219_REG_INTENSITY, intensity);
    command(MAX7219_REG_SCANLIMIT, scanLimit);
    command(MAX7219_REG_SHUTDOWN, shutdown);
    command(MAX7219_REG_DISPLAYTEST, displayTest);

	//std::cout << "device setup" << std::endl;

    for (int j=0; j<1000; j++){
        setDigit(7, 0x5B, false);
        setDigit(6, 0x3B, false);
        setDigit(5, 0x15, false);
        setDigit(4, 0x0F, false);
        setDigit(3, 0x97, false);
        setDigit(2, 0x67, false);
        setDigit(1, 0x04, true);
        command(0xA, (j/10));
    }

    clear(true);
}


void MAX7219::command(unsigned char reg, unsigned char data) {
	//std::cout << "command " <<reg <<"  "<< data << std::endl;
    unsigned char tx[2] = {reg, data};
    spi_transfer(fd, tx, 2);
}

int MAX7219::spi_transfer(int fd, unsigned char* tx, int n) {
	//std::cout << "transfered: "<< tx << std::endl;
	struct spi_ioc_transfer tr;
	memset(&tr,0,sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)tx;
	//tr.rx_buf = (unsigned long)rx;
	tr.len = n;
	tr.delay_usecs = delay;
	tr.speed_hz = speed;
	tr.bits_per_word = bpw;	
	int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	return ret;
}


// HIGH LEVEL METHODS //

void MAX7219::flush() {
    //unsigned char tx[2]; // Holds digit address and digit value
    for (unsigned char digit = 0; digit < 8; digit ++) {
        command((digit+1), digitBuffer[digit]);
    }
}

void MAX7219::clear(bool redraw) {
    for (unsigned int digit = 0; digit < (numDigits/2); digit ++)
        digitBuffer[digit] = 0x0;
    if(redraw) flush();
}

// SETTERS //

void MAX7219::setDigit(unsigned int digit, unsigned char value, bool redraw) {
    digitBuffer[digit] = value;
    if(redraw) flush();
}

void MAX7219::setDecodeMode(unsigned char value){
    decodeMode = value;
    command(MAX7219_REG_DECODEMODE, value);
}
void MAX7219::setIntensity(unsigned char value){
    intensity = value;
    command(MAX7219_REG_INTENSITY, value);
}
void MAX7219::setScanLimit(unsigned char value){
    scanLimit = value;
    command(MAX7219_REG_SCANLIMIT, value);
}
void MAX7219::setShutdown(unsigned char value){
    shutdown = value;
    command(MAX7219_REG_SHUTDOWN, value);
}
void MAX7219::setDisplayTest(unsigned char value){
    displayTest = value;
    command(MAX7219_REG_DISPLAYTEST, value);
}

// GETTERS //

unsigned char MAX7219::getDigit(unsigned char digit) {
    return digitBuffer[digit];
}

unsigned char MAX7219::getDecodeMode(){
    return decodeMode;
}
unsigned char MAX7219::getIntensity(){
    return intensity;
}
unsigned char MAX7219::getScanLimit(){
    return scanLimit;
}
unsigned char MAX7219::getShutdown(){
    return shutdown;
}
unsigned char MAX7219::getDisplayTest(){
    return displayTest;
}
unsigned int MAX7219::getNumDigits() {
    return numDigits;
}

//End MAX7219 

// Display

// Constructor & Destructor //

Display::Display(){

}

// PRIVATE METHODS

void Display::setThreeDigit(unsigned int value) {
    unsigned char digitOne, digitTwo, digitThree;
    digitOne = decHexVals[value % 10];
    digitTwo = decHexVals[(value/10) % 10];
    digitThree = decHexVals[(value/100) % 10];

    setDigit(0, digitOne, false);
    setDigit(1, digitTwo, false);
    setDigit(2, digitThree, false);
    flush();
}

void Display::setTwoDigit(unsigned int value) {
    unsigned char digitOne, digitTwo;
    digitOne = decHexVals[value % 10];
    digitTwo = decHexVals[(value/10) % 10];

    setDigit(0, digitOne, false);
    setDigit(1, digitTwo, false);
}

void Display::setOneDigit(unsigned int value) {
    unsigned char digitOne;
    digitOne = decHexVals[value % 10];

    setDigit(0, digitOne, false);
}

void Display::setVal(unsigned int value, bool redraw) {
    clear(false);
    // Separate value into individual digits
    // 3 digits:
    if(value > 99) setThreeDigit(value);
    // 2 digits:
    else if ((value > 9) && (value <= 99)) setTwoDigit(value);
    // 1 digit:
    else if (value <= 9) setOneDigit(value);
    if (redraw) flush();
}

void Display::setParam(unsigned int param, bool redraw) {
    clear(false);
    std::vector<unsigned char> parameter = params[param];
    setDigit(7, parameter[0], false);
    setDigit(6, parameter[1], false);
    setDigit(5, parameter[2], false);
    setDigit(4, parameter[3], false);
    if (redraw) flush();
}

//Display::~Display(){

// End Display

// DisplayThread

DisplayThread::DisplayThread(Display* display):
isPlaying(true)
{
    this->display=display;
}
DisplayThread::~DisplayThread(){} //Destructor


void DisplayThread::ccToDisplay(unsigned int address, unsigned int value){
    if (currentParam != address){
        display-> setParam(address, true);
        currentParam = address;
    }
    display-> setVal(value, true);

}

void DisplayThread::run(){
    while (isPlaying==true){}
}



//}