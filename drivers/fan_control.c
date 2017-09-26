#include <rtthread.h>
#include "finsh.h"
#include "board.h"
#include "math.h"

#define D_TO_A 1240.909f
#define A_to_D 0.000805f

void fan_speed_set(float value)
{
	if (value <= 4000 && value > 0)
	{
//		value = (value * 4095 / 3.3);
		DAC_SetChannel1Data(DAC_Align_12b_R, value);
		
	}
}

void cmd_fan_speed_set(int argc, char* argv[])
{
	float value;
	if (argc == 2)
	{
		value = atof(argv[1]);
		fan_speed_set(value);
	}
}
MSH_CMD_EXPORT_ALIAS(cmd_fan_speed_set, fsp, set fan motor speed);
