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

static rt_err_t r61529_init(void)
{
    /* Configure LCD */
    lcd_write_cmd(LCD_SLEEP_OUT);
    lcd_mdelay(120);
    
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

    lcd_write_cmd(0xC8);//Gamma
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

    lcd_write_cmd(0xC9);//Gamma
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

    lcd_write_cmd(0xB6);
    lcd_write_data(0x00);

    lcd_write_cmd(0xCA);//Gamma
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

    lcd_write_cmd(0xD0);
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

    lcd_write_cmd(0x36);
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

    lcd_write_cmd(LCD_DISPLAY_ON);

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

