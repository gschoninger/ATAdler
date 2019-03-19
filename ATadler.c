/*========================================================================================================================================== //
|-----------.        .---.			                                                                              .---.        .-----------
|  ------    \  __  /     \			Federal Institute of Santa Catarina			2015/2                           /     \  __  /    ------
|    -----    \(  )/     \ \		Discipline: Microcontrolers					Professor: Mayara de Sousa		/ /     \(  )/    -----
|       ---   ` \/     \\\\\\		Students: Gabriel Schoninger					Class: 822				   //////   ' \/ `   ---
|         --- :    : \\ \ \\\\				  Johann Schmitdinger											  //// / // :    : ---
|           --'    `\  \   \ \\		Project: Hand Controller					File: ATadler.c				 // /   /  /`    '--
|             //..\\          \\																			//          //..\\
|        ====UU====UU====			                                                                               ====UU====UU====
|            '//||\\`				                                                                                   '//||\\`
|              ''``					                                                                                     ''``
/ ========================================================================================================================================== */

// LIBRARIES =============================================================================================================================== //
#include "adler.h"

int main(void)
{
	alrInit(MODE_SUPER);
	alrAutoModeEnable();
	while(EWIG)
	{
		switch(stage){
		case STAGE_IDLE:
			alrUpdatePose();
			break;
		case STAGE_UPDATE_OUTPUT:
			alrUpdatePwm();
			stage = STAGE_IDLE;
			break;
		case STAGE_UPDATE_INPUT:
			alrUpdateAdc();
			stage = STAGE_IDLE;
			break;
		default:
			break;
		}
		alrUsartTransmit('c');
	}
	
	return SABABA;
}
