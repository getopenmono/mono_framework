
extern "C" {
#include <project.h>
}

#include <hx8340.h>

using namespace mono::display;

HX8340::HX8340() : IDisplayController(176,220), scrSize(176,220)
{
    cursor = 0;
}


void HX8340::setWindow(int x, int y, int w, int h)
{
    LCD_Write_COM_DATA(0x02,0x0000);
    LCD_Write_COM_DATA(0x03,x);
    LCD_Write_COM_DATA(0x04,0x0000);
    LCD_Write_COM_DATA(0x05,w>0?x+w-1:x+w); // somehow the window width is implicit 1, if you pass 0
    LCD_Write_COM_DATA(0x06,0x0000);
    LCD_Write_COM_DATA(0x07,y);
    LCD_Write_COM_DATA(0x08,0x0000);
    LCD_Write_COM_DATA(0x09,y+h);
    LCD_Write_COM(0x22);
}

uint16_t HX8340::ScreenWidth() const
{
    return this->scrSize.Width();
}

uint16_t HX8340::ScreenHeight() const
{
    return this->scrSize.Height();
}

void HX8340::setCursor(int x, int y)
{
    setWindow(x,y,x+1,y+1);
}

int HX8340::getCursorX()
{
    return 0;
}

int HX8340::getCursorY()
{
    return 0;
}

void HX8340::write(Color pixelColor)
{
    LCD_Write_DATA(pixelColor.value);
}

uint16_t HX8340::read()
{
    return 0;
}


void HX8340::init()
{
    
    TFT_Start();
    
	LCD_Write_COM_DATA(0x26,0x0084); //PT=10,GON=0, DTE=0, D=0100
    CyDelay(40);
	LCD_Write_COM_DATA(0x26,0x00B8); //PT=10,GON=1, DTE=1, D=1000
	CyDelay(40);
	LCD_Write_COM_DATA(0x26,0x00BC); //PT=10,GON=1, DTE=1, D=1100
	CyDelay(20);
	// LCD_Write_COM_DATA(0x0001,0x0000);     // PTL='1' Enter Partail mode

	//Driving ability Setting
	LCD_Write_COM_DATA(0x60,0x0000);
	LCD_Write_COM_DATA(0x61,0x0006);
	LCD_Write_COM_DATA(0x62,0x0000);
	LCD_Write_COM_DATA(0x63,0x00C8);
	CyDelay(20);

	//Gamma Setting
	LCD_Write_COM_DATA(0x73,0x0070);
	LCD_Write_COM_DATA(0x40,0x0000);
	LCD_Write_COM_DATA(0x41,0x0040);
	LCD_Write_COM_DATA(0x42,0x0045);
	LCD_Write_COM_DATA(0x43,0x0001);
	LCD_Write_COM_DATA(0x44,0x0060);
	LCD_Write_COM_DATA(0x45,0x0005);
	LCD_Write_COM_DATA(0x46,0x000C);
	LCD_Write_COM_DATA(0x47,0x00D1);
	LCD_Write_COM_DATA(0x48,0x0005);

	LCD_Write_COM_DATA(0x50,0x0075);
	LCD_Write_COM_DATA(0x51,0x0001);
	LCD_Write_COM_DATA(0x52,0x0067);
	LCD_Write_COM_DATA(0x53,0x0014);
	LCD_Write_COM_DATA(0x54,0x00F2);
	LCD_Write_COM_DATA(0x55,0x0007);
	LCD_Write_COM_DATA(0x56,0x0003);
	LCD_Write_COM_DATA(0x57,0x0049);
	CyDelay(20);

	//Power Setting
	LCD_Write_COM_DATA(0x1F,0x0003); //VRH=4.65V     VREG1?GAMMA? 00~1E  080421    
	LCD_Write_COM_DATA(0x20,0x0000); //BT (VGH~15V,VGL~-12V,DDVDH~5V)
	LCD_Write_COM_DATA(0x24,0x0024); //VCOMH(VCOM High voltage3.2V)     0024/12    080421    11~40
	LCD_Write_COM_DATA(0x25,0x0034); //VCOML(VCOM Low voltage -1.2V)    0034/4A    080421    29~3F 
	//****VCOM offset**///
	LCD_Write_COM_DATA(0x23,0x002F); //VMF(no offset)                            
	CyDelay(20);

	//##################################################################
	// Power Supply Setting
	LCD_Write_COM_DATA(0x18,0x0044); //I/P_RADJ,N/P_RADJ Noraml mode 60Hz
	LCD_Write_COM_DATA(0x21,0x0001); //OSC_EN='1' start osc
	LCD_Write_COM_DATA(0x01,0x0000); //SLP='0' out sleep
	LCD_Write_COM_DATA(0x1C,0x0003); //AP=011
	LCD_Write_COM_DATA(0x19,0x0006); // VOMG=1,PON=1, DK=0,
	CyDelay(20);

	//##################################################################
	// Display ON Setting
	LCD_Write_COM_DATA(0x26,0x0084); //PT=10,GON=0, DTE=0, D=0100
	CyDelay(40);
	LCD_Write_COM_DATA(0x26,0x00B8); //PT=10,GON=1, DTE=1, D=1000
	CyDelay(40);
	LCD_Write_COM_DATA(0x26,0x00BC); //PT=10,GON=1, DTE=1, D=1100
	CyDelay(20);

	//SET GRAM AREA
	LCD_Write_COM_DATA(0x02,0x0000); 
	LCD_Write_COM_DATA(0x03,0x0000); 
	LCD_Write_COM_DATA(0x04,0x0000);
	LCD_Write_COM_DATA(0x05,0x00AF);
	LCD_Write_COM_DATA(0x06,0x0000);
	LCD_Write_COM_DATA(0x07,0x0000);
	LCD_Write_COM_DATA(0x08,0x0000);
	LCD_Write_COM_DATA(0x09,0x00DB);
	CyDelay(20);
	LCD_Write_COM_DATA(0x16,0x0008);  //MV MX MY ML SET  0028
	LCD_Write_COM_DATA(0x17,0x0005);//COLMOD Control Register (R17h)
	LCD_Write_COM(0x21);
	LCD_Write_COM(0x22);
    
    for (int i=0; i<176*220; i++) {
        this->write(BlackColor);
    }
    
    PWM_Start();
    //PWM_WriteCompare2(0);
    PWM_WriteCompare1(128);
}

void HX8340::LCD_Write_COM(uint16_t comm)
{
    TFT_Write16(COMMAND_SELECT, comm);
}

void HX8340::LCD_Write_DATA(uint16_t data)
{
    TFT_Write16(DATA_SELECT, data);
}

void HX8340::LCD_Write_COM_DATA(uint16_t cmd, uint16_t data)
{
    LCD_Write_COM(cmd);
    LCD_Write_DATA(data);
}
