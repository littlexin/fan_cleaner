#include <rtthread.h>
#include <board.h>
#include "lcd_r61529.h"

/* screen width and height in pixels */
static rt_uint16_t lcd_width  = 480;
static rt_uint16_t lcd_height = 320;


/* LCD is connected to the FSMC_Bank1_NOR/SRAM1 and NE1 is used as ship select signal */
/* RS <==> A16 */
#define LCD_REG         ((volatile rt_uint16_t *) 0x60000000) /* RS = 0 */
#define LCD_RAM         ((volatile rt_uint16_t *) 0x60020000) /* RS = 1 */
#define LCD_PIXEL 153600

rt_inline void lcd_write_cmd(rt_uint16_t cmd)
{
	*(volatile rt_uint16_t *)LCD_REG = cmd;
}

rt_inline void lcd_write_data(rt_uint16_t data)
{
	*(volatile rt_uint16_t *)LCD_RAM = data;
}

rt_inline rt_uint16_t lcd_read_data(void)
{
	return *(volatile rt_uint16_t *) LCD_RAM;
}

rt_inline void lcd_mdelay(rt_uint32_t ms)
{
	if(ms < 10)
		rt_thread_delay(1);
	else
		rt_thread_delay(ms * RT_TICK_PER_SECOND /1000);
}

/******************************************************************************
                            Static Functions
*******************************************************************************/
static void r61529_power_on(void)
{
    lcd_write_cmd(LCD_SLEEP_OUT);
    lcd_mdelay(120);
    lcd_write_cmd(LCD_DISPLAY_ON);
}

static void r61529_power_off(void)
{
    lcd_write_cmd(LCD_DISPLAY_OFF);
    lcd_write_cmd(LCD_SPLIN);
}

static void r61529_set_window(rt_uint16_t x, rt_uint16_t y,
															rt_uint16_t width, rt_uint16_t height)
{
	rt_uint16_t end_x = x + width -1;
	rt_uint16_t end_y = y + height - 1;
	
	lcd_write_cmd(0x2A);
	lcd_write_data(x >> 8);
	lcd_write_data(x & 0xFF);
	lcd_write_data(end_x >> 8);
	lcd_write_data(end_x & 0xFF);
	
	lcd_write_cmd(0x2B);
	lcd_write_data(y >> 8);
	lcd_write_data(y & 0xFF);
	lcd_write_data(end_y >> 8);
	lcd_write_data(end_y & 0xFF);
	
}

static void r61529_set_pixel(const char *pixel, int x, int y)
{
	rt_uint16_t p = *(rt_uint16_t *)pixel;
	
	/*Set X position*/
	lcd_write_cmd(0x2A);
	lcd_write_data(x >> 8);
	lcd_write_data(x & 0xFF);
	lcd_write_data(x >> 8);
	lcd_write_data(x & 0xFF);
	
	/*Set Y position*/
	lcd_write_cmd(0x2B);
	lcd_write_data(y >> 8);
	lcd_write_data(y & 0xFF);
	lcd_write_data(y >> 8);
	lcd_write_data(y & 0xFF);
	
	lcd_write_cmd(0x2C);
	lcd_write_data(p);
}

static void r61529_draw_image(rt_uint16_t x, rt_uint16_t y, 
															rt_uint16_t width, rt_uint16_t height,const rt_uint8_t *data)
{
		rt_size_t size;
		rt_uint32_t i;
		
		size = width * height;
		
		r61529_set_window(x, y, width, height);
		
		lcd_write_cmd(0x2c);
		for(i = 0; i < size; i++)
		{
			lcd_write_data(*(rt_uint16_t *)data);
			data = data + 2;
		}
}

static void r61529_clear(void)
{
	rt_uint32_t i;
	
	r61529_set_window(0, 0, lcd_width, lcd_height);
	
	lcd_write_cmd(0x2C);
	for(i = 0;i < LCD_PIXEL; i++)
	lcd_write_data(LCD_COLOR_WHITE);
	
}

static rt_err_t r61529_init(void)
{
    /* Configure LCD */
    lcd_write_cmd(LCD_SLEEP_OUT);
    lcd_mdelay(50);
	
    lcd_write_cmd(0x3A);
    lcd_write_data(0x55);

    lcd_write_cmd(0xB0);
    lcd_write_data(0x00);
    
    lcd_write_cmd(0xB4);    //Frequence
    lcd_write_data(0x00); 

    lcd_write_cmd(0xC0);
    lcd_write_data(0x03);    //0013
    lcd_write_data(0xDF);    //480
    lcd_write_data(0x40);
    lcd_write_data(0x12);
    lcd_write_data(0x00);
    lcd_write_data(0x01);
    lcd_write_data(0x00);
    lcd_write_data(0x43);

    lcd_write_cmd(0xC1);//frame frequency
    lcd_write_data(0x05);//BCn,DIVn[1:0
    lcd_write_data(0x2f);//RTNn[4:0] 
    lcd_write_data(0x08);// BPn[7:0]
    lcd_write_data(0x08);// FPn[7:0]
    lcd_write_data(0x00);

    lcd_write_cmd(0xC4);
    lcd_write_data(0x63);
    lcd_write_data(0x00);
    lcd_write_data(0x08);
    lcd_write_data(0x08);

    lcd_write_cmd(0xC8);//A_Gamma
    lcd_write_data(0x06);
    lcd_write_data(0x0c);
    lcd_write_data(0x16);
    lcd_write_data(0x24);//26
    lcd_write_data(0x30);//32 
    lcd_write_data(0x48);
    lcd_write_data(0x3d);
    lcd_write_data(0x28);
    lcd_write_data(0x20);
    lcd_write_data(0x14);
    lcd_write_data(0x0c);
    lcd_write_data(0x04);

    lcd_write_data(0x06);
    lcd_write_data(0x0c);
    lcd_write_data(0x16);
    lcd_write_data(0x24);
    lcd_write_data(0x30);
    lcd_write_data(0x48);
    lcd_write_data(0x3d);
    lcd_write_data(0x28);
    lcd_write_data(0x20);
    lcd_write_data(0x14);
    lcd_write_data(0x0c);
    lcd_write_data(0x04);

    lcd_write_cmd(0xC9);//B_Gamma
    lcd_write_data(0x06);
    lcd_write_data(0x0c);
    lcd_write_data(0x16);
    lcd_write_data(0x24);//26
    lcd_write_data(0x30);//32 
    lcd_write_data(0x48);
    lcd_write_data(0x3d);
    lcd_write_data(0x28);
    lcd_write_data(0x20);
    lcd_write_data(0x14);
    lcd_write_data(0x0c);
    lcd_write_data(0x04);

    lcd_write_data(0x06);
    lcd_write_data(0x0c);
    lcd_write_data(0x16);
    lcd_write_data(0x24);
    lcd_write_data(0x30);
    lcd_write_data(0x48);
    lcd_write_data(0x3d);
    lcd_write_data(0x28);
    lcd_write_data(0x20);
    lcd_write_data(0x14);
    lcd_write_data(0x0c);
    lcd_write_data(0x04);

    lcd_write_cmd(0xCA);//C_Gamma
    lcd_write_data(0x06);
    lcd_write_data(0x0c);
    lcd_write_data(0x16);
    lcd_write_data(0x24);//26
    lcd_write_data(0x30);//32 
    lcd_write_data(0x48);
    lcd_write_data(0x3d);
    lcd_write_data(0x28);
    lcd_write_data(0x20);
    lcd_write_data(0x14);
    lcd_write_data(0x0c);
    lcd_write_data(0x04);

    lcd_write_data(0x06);
    lcd_write_data(0x0c);
    lcd_write_data(0x16);
    lcd_write_data(0x24);
    lcd_write_data(0x30);
    lcd_write_data(0x48);
    lcd_write_data(0x3d);
    lcd_write_data(0x28);
    lcd_write_data(0x20);
    lcd_write_data(0x14);
    lcd_write_data(0x0c);
    lcd_write_data(0x04);

    lcd_write_cmd(0xB6); //DSI
    lcd_write_data(0x00);

    lcd_write_cmd(0xD0); //Power control
    lcd_write_data(0x95);
    lcd_write_data(0x06);
    lcd_write_data(0x08);
    lcd_write_data(0x10);
    lcd_write_data(0x3f);

    lcd_write_cmd(0xD1);
    lcd_write_data(0x02);
    lcd_write_data(0x28);
    lcd_write_data(0x28);
    lcd_write_data(0x40);

    lcd_write_cmd(0xE1);
    lcd_write_data(0x00);
    lcd_write_data(0x00);
    lcd_write_data(0x00);
    lcd_write_data(0x00);
    lcd_write_data(0x00);
    lcd_write_data(0x00);

    lcd_write_cmd(0xE2);
    lcd_write_data(0x80);

    
		
		/*
		扫描方向、页地址、行地址
		*/
		lcd_write_cmd(0x36);  //set address mode
    lcd_write_data(0xA0);
    /* Colomn address set */
    lcd_write_cmd(LCD_COLUMN_ADDR);
    lcd_write_data(0 >> 8);
    lcd_write_data(0 & 0xFF);
    lcd_write_data((lcd_width - 1) >> 8);
    lcd_write_data((lcd_width - 1) & 0xFF);

    /* Page address set */
    lcd_write_cmd(LCD_PAGE_ADDR); 
    lcd_write_data(0 >> 8);
    lcd_write_data(0 & 0xFF);
    lcd_write_data((lcd_height - 1) >> 8);
    lcd_write_data((lcd_height - 1) & 0xFF);
		r61529_clear();
		
		lcd_mdelay(100);
    lcd_write_cmd(LCD_DISPLAY_ON);
		lcd_mdelay(100);
		
		r61529_draw_image(0, 0, 480, 320, gImage_s);
    return RT_EOK;
}

rt_uint16_t r61529_read_id(void)
{
	rt_uint16_t id;
	rt_size_t size = LCD_READ_ID4_SIZE;
	
	r61529_init();
	
	lcd_write_cmd(0xB0);
	lcd_write_data(0x04);
	
	lcd_write_cmd(0xBF);
	id = lcd_read_data();
	while(size --)
	{
		id <<= 8;
		id |= lcd_read_data();
	}
	return id;
}




