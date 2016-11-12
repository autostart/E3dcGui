/*
gcc -g -o screenSave  screenSave.c
*/

#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include "parameter.h"
#include "parameterHM.h"
#include "Frame/touch.h"
#include "Frame/touch.c"
#include "Frame/framebuffer.c"
#include "Frame/DrawCorner.c"
#include "funktion.h"

int main()
{
	signal(SIGINT, INThandler);

	int  xres,yres;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	int screenChange;

	system("fbset -fb /dev/fb0 -depth 16");    //Umschalten auf 16Bit Display
	system("setterm -cursor off");             //Courser Abschalten

	int screenShutdown = ShutdownRun;

	char Value[20];

	screenOn();
	writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
	if(E3DC_S10 ==1){
		writeData("/mnt/RAMDisk/ScreenChange.txt", "1\n");
	}
	else{
		writeData("/mnt/RAMDisk/ScreenChange.txt", "12\n");
	}
	writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
	writeData("/mnt/RAMDisk/ScreenShutdown.txt", "5\n");
	writeData("/mnt/RAMDisk/LegendeSOC.txt", "true\n");
	writeData("/mnt/RAMDisk/LegendeSolar.txt", "true\n");
	writeData("/mnt/RAMDisk/LegendeADD.txt", "false\n");
	writeData("/mnt/RAMDisk/LegendeHome.txt", "true\n");
	writeData("/mnt/RAMDisk/LegendeNetIn.txt", "true\n");
	writeData("/mnt/RAMDisk/LegendeNetOut.txt", "true\n");
	writeData("/mnt/RAMDisk/LegendeBatIn.txt", "false\n");
	writeData("/mnt/RAMDisk/LegendeBatOut.txt", "false\n");

	if (openTouchScreen() == 1)
		perror("error opening touch screen");

	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

	framebufferInitialize(&xres,&yres);

	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	printf ("X Scale Factor = %f\n", scaleXvalue);
	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	printf ("Y Scale Factor = %f\n", scaleYvalue);

	int buttonCordsSave[4] = {5,110,790,280};
	int	buttonSave= BUTTON_OFF;
	int buttonTimerSave = mymillis();
	int buttonCordsSaveHalb[4] = {5,110,400,280};
	int	buttonSaveHalb= BUTTON_OFF;
	int buttonTimerSaveHalb = mymillis();
	int buttonCordsAktuell[4] = {270,10,80,90};
	int buttonTimerAktuell = mymillis();
	int buttonCordsLangzeit[4] = {360,10,80,90};
	int buttonTimerLangzeit = mymillis();
	int buttonCordsSetup[4] = {180,10,80,90};
	int buttonTimerSetup = mymillis();
	int buttonCordsMonitor[4] = {450,10,80,90};
	int buttonTimerMonitor = mymillis();
	int buttonCordsHM[4] = {540,10,80,90};
	int buttonTimerHM = mymillis();
	int buttonCordsHMrefresh[4] = {760,440,20,20};
	int buttonTimerHMrefresh = mymillis();

	int buttonCordsSD[4] = {120,200,180,30};
	int buttonTimerSD = mymillis();
	int buttonCordsSDN[4] = {120,200,180,30};
	int buttonTimerSDN = mymillis();
	int buttonCordsSRS[4] = {120,250,180,30};
	int buttonTimerSRS = mymillis();
	int buttonCordsHRS[4] = {120,300,180,30};
	int buttonTimerHRS = mymillis();
	int buttonCordstime_zone[4] = {470,200,180,30};
	int buttonTimertime_zone = mymillis();

	int buttonCordsLeSOC[4] = {364,438,50,30};
	int buttonTimerLeSOC = mymillis();
	int buttonCordsLeSolar[4] = {417,438,50,30};
	int buttonTimerLeSolar = mymillis();
	int buttonCordsLeHome[4] = {470,438,50,30};
	int buttonTimerLeHome = mymillis();
	int buttonCordsLeNetIn[4] = {523,438,50,30};
	int buttonTimerLeNetIn = mymillis();
	int buttonCordsLeNetOut[4] = {576,438,50,30};
	int buttonTimerLeNetOut = mymillis();
	int buttonCordsLeBatIn[4] = {629,438,50,30};
	int buttonTimerLeBatIn = mymillis();
	int buttonCordsLeBatOut[4] = {682,438,50,30};
	int buttonTimerLeBatOut = mymillis();
	int buttonCordsLeADD[4] = {735,438,50,30};
	int buttonTimerLeADD = mymillis();

	int buttonCordsParty[4] = {S7-3,R2,Fw+6,21+3};
	int	buttonParty= BUTTON_OFF;
	int buttonTimerParty = mymillis();
	int buttonCordsKueche[4] = {(S6-3),(R2),(Fw+6),(21+3)};
	int	buttonKueche= BUTTON_OFF;
	int buttonTimerKueche = mymillis();
	int buttonCordsBrunnen[4] = {(S8-3),(R4),(Fw+6),(21+3)};
	int	buttonBrunnen= BUTTON_OFF;
	int buttonTimerBrunnen = mymillis();
	int buttonCordsMac[4] = {(S8-3),(R2),(Fw+6),(21+3)};
	int	buttonMac= BUTTON_OFF;
	int buttonTimerMac = mymillis();
	int buttonCordsJOGWST[4] = {(S5-3),(R2),(Fw+6),(21+3)};
	int	buttonJOGWST= BUTTON_OFF;
	int buttonTimerJOGWST = mymillis();

	while(1){
		getTouchSample(&rawX, &rawY, &rawPressure);
		scaledX = rawX/scaleXvalue;
		scaledY = rawY/scaleYvalue;

		readData("ScreenChange", 1, Value);
		screenChange = atoi(Value);

		if(E3DC_S10 ==1){
			if((scaledX  > buttonCordsAktuell[X] && scaledX < (buttonCordsAktuell[X]+buttonCordsAktuell[W])) && (scaledY > buttonCordsAktuell[Y] && scaledY < (buttonCordsAktuell[Y]+buttonCordsAktuell[H]))){
				if (mymillis() - buttonTimerAktuell > 500){
					buttonTimerAktuell = mymillis();
					writeData("/mnt/RAMDisk/ScreenChange.txt", "1\n");
					writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					writeData("/mnt/RAMDisk/ScreenShutdown.txt", "5\n");
				}
			}
			if((scaledX  > buttonCordsLangzeit[X] && scaledX < (buttonCordsLangzeit[X]+buttonCordsLangzeit[W])) && (scaledY > buttonCordsLangzeit[Y] && scaledY < (buttonCordsLangzeit[Y]+buttonCordsLangzeit[H]))){
				if (mymillis() - buttonTimerLangzeit > 500){
					buttonTimerLangzeit = mymillis();
					writeData("/mnt/RAMDisk/ScreenChange.txt", "2\n");
					writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					writeData("/mnt/RAMDisk/ScreenShutdown.txt", "5\n");
				}
			}
		}
		if((scaledX  > buttonCordsSetup[X] && scaledX < (buttonCordsSetup[X]+buttonCordsSetup[W])) && (scaledY > buttonCordsSetup[Y] && scaledY < (buttonCordsSetup[Y]+buttonCordsSetup[H]))){
			if (mymillis() - buttonTimerSetup > 500){
				buttonTimerSetup = mymillis();
				writeData("/mnt/RAMDisk/ScreenChange.txt", "3\n");
				writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
				writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
				writeData("/mnt/RAMDisk/ScreenShutdown.txt", "5\n");
			}
		}
		if((scaledX  > buttonCordsMonitor[X] && scaledX < (buttonCordsMonitor[X]+buttonCordsMonitor[W])) && (scaledY > buttonCordsMonitor[Y] && scaledY < (buttonCordsMonitor[Y]+buttonCordsMonitor[H]))){
			if (mymillis() - buttonTimerMonitor > 500){
				buttonTimerMonitor = mymillis();
				writeData("/mnt/RAMDisk/ScreenChange.txt", "11\n");
				writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
				writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
				writeData("/mnt/RAMDisk/ScreenShutdown.txt", "5\n");
			}
		}

		if(Homematic_GUI == 1){
			if((scaledX  > buttonCordsHM[X] && scaledX < (buttonCordsHM[X]+buttonCordsHM[W])) && (scaledY > buttonCordsHM[Y] && scaledY < (buttonCordsHM[Y]+buttonCordsHM[H]))){
				if (mymillis() - buttonTimerHM > 500){
					buttonTimerHM = mymillis();
					writeData("/mnt/RAMDisk/ScreenChange.txt", "12\n");
					writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					writeData("/mnt/RAMDisk/ScreenShutdown.txt", "5\n");
				}
			}
		}
		switch(screenChange){
			case ScreenAktuell:{
				if((scaledX  > buttonCordsSave[X] && scaledX < (buttonCordsSave[X]+buttonCordsSave[W])) && (scaledY > buttonCordsSave[Y] && scaledY < (buttonCordsSave[Y]+buttonCordsSave[H]))){
					if (mymillis() - buttonTimerSave > 500)
					if(buttonSave){
						screenOff();
						buttonSave= BUTTON_OFF;
						buttonTimerSave = mymillis();
					}
					else{
						screenOn();
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						buttonSave= BUTTON_ON;
						buttonTimerSave = mymillis();
					}
				}
				break; // ScreenAktuell
			}
			case ScreenLangzeit:{
				if((scaledX  > buttonCordsSave[X] && scaledX < (buttonCordsSave[X]+buttonCordsSave[W])) && (scaledY > buttonCordsSave[Y] && scaledY < (buttonCordsSave[Y]+buttonCordsSave[H]))){
					if (mymillis() - buttonTimerSave > 500)
					if(buttonSave){
						screenOff();
						buttonSave= BUTTON_OFF;
						buttonTimerSave = mymillis();
					}
					else{
						screenOn();
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						buttonSave= BUTTON_ON;
						buttonTimerSave = mymillis();
					}
				}
				if((scaledX  > buttonCordsLeSOC[X] && scaledX < (buttonCordsLeSOC[X]+buttonCordsLeSOC[W])) && (scaledY > buttonCordsLeSOC[Y] && scaledY < (buttonCordsLeSOC[Y]+buttonCordsLeSOC[H]))){
					if (mymillis() - buttonTimerLeSOC > 300){
						buttonTimerLeSOC = mymillis();
						ChangeState("LegendeSOC");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					}
				}
				if((scaledX  > buttonCordsLeSolar[X] && scaledX < (buttonCordsLeSolar[X]+buttonCordsLeSolar[W])) && (scaledY > buttonCordsLeSolar[Y] && scaledY < (buttonCordsLeSolar[Y]+buttonCordsLeSolar[H]))){
					if (mymillis() - buttonTimerLeSolar > 300){
						buttonTimerLeSolar = mymillis();
						ChangeState("LegendeSolar");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					}
				}
				if((scaledX  > buttonCordsLeHome[X] && scaledX < (buttonCordsLeHome[X]+buttonCordsLeHome[W])) && (scaledY > buttonCordsLeHome[Y] && scaledY < (buttonCordsLeHome[Y]+buttonCordsLeHome[H]))){
					if (mymillis() - buttonTimerLeHome > 300){
						buttonTimerLeHome = mymillis();
						ChangeState("LegendeHome");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					}
				}
				if((scaledX  > buttonCordsLeNetIn[X] && scaledX < (buttonCordsLeNetIn[X]+buttonCordsLeNetIn[W])) && (scaledY > buttonCordsLeNetIn[Y] && scaledY < (buttonCordsLeNetIn[Y]+buttonCordsLeNetIn[H]))){
					if (mymillis() - buttonTimerLeNetIn > 300){
						buttonTimerLeNetIn = mymillis();
						ChangeState("LegendeNetIn");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					}
				}
				if((scaledX  > buttonCordsLeNetOut[X] && scaledX < (buttonCordsLeNetOut[X]+buttonCordsLeNetOut[W])) && (scaledY > buttonCordsLeNetOut[Y] && scaledY < (buttonCordsLeNetOut[Y]+buttonCordsLeNetOut[H]))){
					if (mymillis() - buttonTimerLeNetOut > 300){
						buttonTimerLeNetOut = mymillis();
						ChangeState("LegendeNetOut");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					}
				}
				if((scaledX  > buttonCordsLeBatIn[X] && scaledX < (buttonCordsLeBatIn[X]+buttonCordsLeBatIn[W])) && (scaledY > buttonCordsLeBatIn[Y] && scaledY < (buttonCordsLeBatIn[Y]+buttonCordsLeBatIn[H]))){
					if (mymillis() - buttonTimerLeBatIn > 300){
						buttonTimerLeBatIn = mymillis();
						ChangeState("LegendeBatIn");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					}
				}
				if((scaledX  > buttonCordsLeBatOut[X] && scaledX < (buttonCordsLeBatOut[X]+buttonCordsLeBatOut[W])) && (scaledY > buttonCordsLeBatOut[Y] && scaledY < (buttonCordsLeBatOut[Y]+buttonCordsLeBatOut[H]))){
					if (mymillis() - buttonTimerLeBatOut > 300){
						buttonTimerLeBatOut = mymillis();
						ChangeState("LegendeBatOut");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
					}
				}
				if(Additional == 1){
					if((scaledX  > buttonCordsLeADD[X] && scaledX < (buttonCordsLeADD[X]+buttonCordsLeADD[W])) && (scaledY > buttonCordsLeADD[Y] && scaledY < (buttonCordsLeADD[Y]+buttonCordsLeADD[H]))){
						if (mymillis() - buttonTimerLeADD > 300){
							buttonTimerLeADD = mymillis();
							ChangeState("LegendeADD");
							writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
							writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
						}
					}
				}
				break; // ScreenLangzeit
			}
			case ScreenSetup:{
				if(screenShutdown = ShutdownRun){
					if((scaledX  > buttonCordsSD[X] && scaledX < (buttonCordsSD[X]+buttonCordsSD[W])) && (scaledY > buttonCordsSD[Y] && scaledY < (buttonCordsSD[Y]+buttonCordsSD[H]))){
						if (mymillis() - buttonTimerSD > 600){
							buttonTimerSD = mymillis();
							writeData("/mnt/RAMDisk/ScreenShutdown.txt", "6\n");
							writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
							screenShutdown = ShutdownSD;
						}
					}
				} // if ScreenShutdownRun
				if((scaledX  > buttonCordstime_zone[X] && scaledX < (buttonCordstime_zone[X]+buttonCordstime_zone[W])) && (scaledY > buttonCordstime_zone[Y] && scaledY < (buttonCordstime_zone[Y]+buttonCordstime_zone[H]))){
					if (mymillis() - buttonTimertime_zone > 600){
						buttonTimertime_zone = mymillis();
						char file_Path [100],file_read [100];
						FILE *fp;
						snprintf (file_Path, (size_t)100, "/home/pi/E3dcGui/Data/Timezone.txt");
						fp = fopen(file_Path, "r");
						if(fp == NULL) {
							printf("Datei konnte NICHT geoeffnet werden.\n");
							snprintf (file_read, (size_t)20, "Summertime");
						}
						else {
							fgets(file_read,20,fp);
							strtok(file_read, "\n");
							fclose(fp);
						}
						if (strcmp ("Wintertime",file_read) == 0){
							writeData("/home/pi/E3dcGui/Data/Timezone.txt", "Summertime");
							drawSquare(450,200,180,30,GREY);
							drawCorner(450,200,180,30, WHITE);
							put_string(470,208, "Sommerzeit", GREEN);

						}
						else{
							writeData("/home/pi/E3dcGui/Data/Timezone.txt", "Wintertime");
							drawSquare(450,200,180,30,GREY);
							drawCorner(450,200,180,30, WHITE);
							put_string(470,208, "Winterzeit", GREEN);
						}
						writeData("/mnt/RAMDisk/ScreenShutdown.txt", "7\n");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						sleep (2);
						pkill();
						system("/home/pi/E3dcGui/start &");
						drawSquare(2,2,800,480,LTGREY);
						system("pkill screenSave");
						return 0;
					}
				}
				if((scaledX  > buttonCordsSRS[X] && scaledX < (buttonCordsSRS[X]+buttonCordsSRS[W])) && (scaledY > buttonCordsSRS[Y] && scaledY < (buttonCordsSRS[Y]+buttonCordsSRS[H]))){
					if (mymillis() - buttonTimerSRS > 3000){
						buttonTimerSRS = mymillis();
						writeData("/mnt/RAMDisk/ScreenShutdown.txt", "7\n");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						sleep (2);
						pkill();
						system("/home/pi/E3dcGui/start &");
						drawSquare(2,2,800,480,LTGREY);
						system("pkill screenSave");
						return 0;
					}
				}
				if((scaledX  > buttonCordsHRS[X] && scaledX < (buttonCordsHRS[X]+buttonCordsHRS[W])) && (scaledY > buttonCordsHRS[Y] && scaledY < (buttonCordsHRS[Y]+buttonCordsHRS[H]))){
					if (mymillis() - buttonTimerHRS > 3000){
						buttonTimerHRS = mymillis();
						writeData("/mnt/RAMDisk/ScreenShutdown.txt", "8\n");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						sleep (2);
						pkill();
						drawSquare(2,2,800,480,BLACK);
						system("sudo reboot");
						system("pkill screenSave");
						return 0;
					}
				}
				if(screenShutdown == ShutdownSD){
					if((scaledX  > buttonCordsSDN[X] && scaledX < (buttonCordsSDN[X]+buttonCordsSDN[W])) && (scaledY > buttonCordsSDN[Y] && scaledY < (buttonCordsSDN[Y]+buttonCordsSDN[H]))){
						if (mymillis() - buttonTimerSDN > 3000){
							buttonTimerSDN = mymillis();
							writeData("/mnt/RAMDisk/ScreenShutdown.txt", "9\n");
							writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
							pkill();
							sleep (2);
							drawSquare(2,2,800,480,BLACK);
							system("sudo shutdown -h now");
							system("pkill screenSave");
							return 0;
						}
					}
				} // if screenShutdownSD
				break; // ScreenSetup
			}
			case ScreenMonitor:{
				if((scaledX  > buttonCordsSave[X] && scaledX < (buttonCordsSave[X]+buttonCordsSave[W])) && (scaledY > buttonCordsSave[Y] && scaledY < (buttonCordsSave[Y]+buttonCordsSave[H]))){
					if (mymillis() - buttonTimerSave > 500)
					if(buttonSave){
						screenOff();
						buttonSave= BUTTON_OFF;
						buttonTimerSave = mymillis();
					}
					else{
						screenOn();
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						buttonSave= BUTTON_ON;
						buttonTimerSave = mymillis();
					}
				}
				break; // ScreenMonitor
			}
			case ScreenHM:{
				if((scaledX  > buttonCordsSaveHalb[X] && scaledX < (buttonCordsSaveHalb[X]+buttonCordsSaveHalb[W])) && (scaledY > buttonCordsSaveHalb[Y] && scaledY < (buttonCordsSaveHalb[Y]+buttonCordsSaveHalb[H]))){
					if (mymillis() - buttonTimerSaveHalb > 500)
					if(buttonSaveHalb){
						screenOff();
						buttonSaveHalb= BUTTON_OFF;
						buttonTimerSaveHalb = mymillis();
					}
					else{
						screenOn();
						writeData("/mnt/RAMDisk/ScreenSaver.txt", "0");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
						buttonSaveHalb= BUTTON_ON;
						buttonTimerSaveHalb = mymillis();
					}
				}
				if((scaledX  > buttonCordsHMrefresh[X] && scaledX < (buttonCordsHMrefresh[X]+buttonCordsHMrefresh[W])) && (scaledY > buttonCordsHMrefresh[Y] && scaledY < (buttonCordsHMrefresh[Y]+buttonCordsHMrefresh[H]))){
					if (mymillis() - buttonTimerHMrefresh > 500){
						buttonTimerHMrefresh = mymillis();
						drawSquare(760,440,20,20,LIGHT_GREEN);
						drawCorner(760,440,20,20,WHITE);
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					}
				}
				if((scaledX  > buttonCordsParty[X] && scaledX < (buttonCordsParty[X]+buttonCordsParty[W])) && (scaledY > buttonCordsParty[Y] && scaledY < (buttonCordsParty[Y]+buttonCordsParty[H]))){
					if (mymillis() - buttonTimerParty > 500)
					if(buttonParty){
						buttonParty= BUTTON_OFF;
						buttonTimerParty = mymillis();
						drawSquare(S7-3,R2,Fw+6,21+3,LTGREY);
						printsend(ISE_OGParty, "false");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					}
					else{
						buttonParty= BUTTON_ON;
						buttonTimerParty = mymillis();
						drawSquare(S7-3,R2,Fw+6,21+3,LIGHT_GREEN);
						printsend(ISE_OGParty, "true");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					}
				}
				if((scaledX  > buttonCordsKueche[X] && scaledX < (buttonCordsKueche[X]+buttonCordsKueche[W])) && (scaledY > buttonCordsKueche[Y] && scaledY < (buttonCordsKueche[Y]+buttonCordsKueche[H]))){
					if (mymillis() - buttonTimerKueche > 500)
					if(buttonKueche){
						buttonKueche= BUTTON_OFF;
						buttonTimerKueche = mymillis();
						drawSquare(S6-3,R2,Fw+6,21+3,LTGREY);
					}
					else{
						buttonKueche= BUTTON_ON;
						buttonTimerKueche = mymillis();
						drawSquare(S6-3,R2,Fw+6,21+3,LIGHT_GREEN);
						printsend(ISE_Send_OGKSpul, "true");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					}
				}
				if((scaledX  > buttonCordsBrunnen[X] && scaledX < (buttonCordsBrunnen[X]+buttonCordsBrunnen[W])) && (scaledY > buttonCordsBrunnen[Y] && scaledY < (buttonCordsBrunnen[Y]+buttonCordsBrunnen[H]))){
					if (mymillis() - buttonTimerBrunnen > 500)
					if(buttonBrunnen){
						buttonBrunnen= BUTTON_OFF;
						buttonTimerBrunnen = mymillis();
						drawSquare(S8-3,R4,Fw+6,21+3,LTGREY);
						printsend(ISE_Brunnen, "false");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					}
					else{
						buttonBrunnen= BUTTON_ON;
						buttonTimerBrunnen = mymillis();
						drawSquare(S8-3,R4,Fw+6,21+3,LIGHT_BLUE);
						printsend(ISE_Brunnen, "true");
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					}
				}
				if((scaledX  > buttonCordsMac[X] && scaledX < (buttonCordsMac[X]+buttonCordsMac[W])) && (scaledY > buttonCordsMac[Y] && scaledY < (buttonCordsMac[Y]+buttonCordsMac[H]))){
					if (mymillis() - buttonTimerMac > 500){
						buttonTimerMac = mymillis();
						char MacSchrank[20];
						read_HM(ISE_MacSchrankValue, 4, MacSchrank);
						if (strcmp ("true",MacSchrank) == 0){
							drawSquare(S8-3,R2,Fw+6,21+3,LIGHT_GREEN);
							printsend(ISE_MacSchrank, "false");
						}
						else if (strcmp ("fals",MacSchrank) == 0){
							drawSquare(S8-3,R2,Fw+6,21+3,LIGHT_RED);
							printsend(ISE_MacSchrank, "true");
						}
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					}
				}
				if((scaledX  > buttonCordsJOGWST[X] && scaledX < (buttonCordsJOGWST[X]+buttonCordsJOGWST[W])) && (scaledY > buttonCordsJOGWST[Y] && scaledY < (buttonCordsJOGWST[Y]+buttonCordsJOGWST[H]))){
					if (mymillis() - buttonTimerJOGWST > 500){
						buttonTimerJOGWST = mymillis();
						char OGWJalSt[20];
						readJalou_HM(ISE_OGWJalSt, OGWJalSt);																//Jalousie Wert aus der Homematic Abfrage
						if (strcmp ("100",OGWJalSt) == 0){
							drawSquare(S5-3,R2,Fw+6,21+3,GREEN);
							printsend(ISE_OGWJalSt, "0.50");
						}
						else {
							drawSquare(S5-3,R2,Fw+6,21+3,GREY);
							printsend(ISE_OGWJalSt, "1.00");
						}
						writeData("/mnt/RAMDisk/ScreenCounter.txt", "0");
					}
				}
				break; // ScreenHM
			}
			default:{
				writeData("/mnt/RAMDisk/ScreenChange.txt", "1\n");
			}
		} // Switch screenChange
	}  //while
} //main
