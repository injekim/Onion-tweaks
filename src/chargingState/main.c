#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#include "sys/ioctl.h"
#include "cJSON.h"

#define MAX_IMAGE_NO 30

bool file_exists (char *filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}


char* load_file (char const* path)
{
	char* buffer = 0;
	long length = 0;
	FILE * f = fopen(path, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char*)malloc((length+1)*sizeof(char));
		if (buffer)
		{
			fread(buffer, sizeof(char), length, f);
		}
		fclose(f);
	}
	buffer[length] = '\0';

	return buffer; 
}

static int is_charging = 0;
void checkCharging(void) {
    int i = 0;
    FILE *file = fopen("/sys/devices/gpiochip0/gpio/gpio59/value", "r");
    if (file!=NULL) {
        fscanf(file, "%i", &i);
        fclose(file);
    }
    is_charging = i;
}

void logMessage(char* Message) {
	FILE *file = fopen("/mnt/SDCARD/.tmp_update/log_charging_Message.txt", "a");
	/*char tempMess[] = "\r\n";
    strcat(Message,tempMess);
    */
    char valLog[200];
    sprintf(valLog, "%s %s", Message, "\n");
    fputs(valLog, file);
	fclose(file); 
}

void SetRawBrightness(int val) {  // val = 0-100
    int fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    if (fd>=0) {
        dprintf(fd,"%d",val);
        close(fd);
    }
}

void SetBrightness(int value) {  // value = 0-10
    SetRawBrightness(value*10);
}

int main(void) {
	cJSON* request_json = NULL;
	
	cJSON* frame_count;
	cJSON* frame_delay;
	
	int n_frame_count = 6;
	int n_frame_delay = 50000;
	
	if (file_exists("/mnt/SDCARD/.tmp_update/chargingState.json") == 1) {
		char *request_body = load_file("/mnt/SDCARD/.tmp_update/chargingState.json");
		
		if (request_body != NULL) {
			request_json = cJSON_Parse(request_body);
					
			frame_count = cJSON_GetObjectItem(request_json, "frame_count");
			frame_delay = cJSON_GetObjectItem(request_json, "frame_delay");
			
			n_frame_count = (int) cJSON_GetNumberValue(frame_count);
			n_frame_delay = (int) cJSON_GetNumberValue(frame_delay);
			
			// Log json data
			// char tmp[64];
			// sprintf(tmp, "%d", n_frame_count);
			// logMessage(tmp);
		}
	}
	

	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);
	   
	// 640x480, HW to HW blit, 1:1 crisp pixels, rightside up
	SDL_Surface* video = SDL_SetVideoMode(640,480, 32, SDL_HWSURFACE);
	SDL_Surface* screen = SDL_CreateRGBSurface(SDL_HWSURFACE, 640,480, 32, 0,0,0,0);
	
	// SDL_Surface* image0 = IMG_Load("chargingState0.png");
	// SDL_Surface* image1 = IMG_Load("chargingState1.png");
	// SDL_Surface* image2 = IMG_Load("chargingState2.png");
	// SDL_Surface* image3 = IMG_Load("chargingState3.png");
	// SDL_Surface* image4 = IMG_Load("chargingState4.png");
	// SDL_Surface* image5 = IMG_Load("chargingState5.png");
	
	SDL_Surface *images[MAX_IMAGE_NO];
	
	for (int i = 0; i < n_frame_count; i++) {
		char temp[32];
		sprintf(temp, "chargingState%d.png", i);
		images[i] = IMG_Load(temp);
	}
	
	SetBrightness(8);
	int run = 1;

	SDL_Event event;
		
	// for (int i = 1; i < 21; ++i){
	// 	SDL_BlitSurface(image0, NULL, screen, NULL);
	// 	SDL_BlitSurface(screen, NULL, video, NULL); 
	// 	SDL_Flip(video);
	// 	usleep(50000);
	// 	
	// 	SDL_BlitSurface(image1, NULL, screen, NULL);
	// 	SDL_BlitSurface(screen, NULL, video, NULL); 
	// 	SDL_Flip(video);
	// 	usleep(50000);	
	// 	
	// 	SDL_BlitSurface(image2, NULL, screen, NULL);
	// 	SDL_BlitSurface(screen, NULL, video, NULL); 
	// 	SDL_Flip(video);
	// 	usleep(50000);	
	// 	
	// 	SDL_BlitSurface(image3, NULL, screen, NULL);
	// 	SDL_BlitSurface(screen, NULL, video, NULL); 
	// 	SDL_Flip(video);
	// 	usleep(50000);	
	// 	
	// 	SDL_BlitSurface(image4, NULL, screen, NULL);
	// 	SDL_BlitSurface(screen, NULL, video, NULL); 
	// 	SDL_Flip(video);
	// 	usleep(50000);	
	// 	
	// 	SDL_BlitSurface(image5, NULL, screen, NULL);
	// 	SDL_BlitSurface(screen, NULL, video, NULL); 
	// 	SDL_Flip(video);
	// 	usleep(50000);	
	// 	
	// 	checkCharging();
	// 	if (is_charging == 0){
	// 		break;
	// 	}
	// 	
	// 	if(SDL_PollEvent(&event)){
	// 		if ((event.type==SDL_KEYDOWN)&&(event.key.keysym.sym == 0)){
	// 			run = 0;
	// 			break;
	// 		}
	// 	}
	// }
	
	for (int i = 0; i < n_frame_count * 3; i++) {
		for (int i = 0; i < n_frame_count; i++) {
			SDL_BlitSurface(images[i], NULL, screen, NULL);
			SDL_BlitSurface(screen, NULL, video, NULL); 
			SDL_Flip(video);
			usleep(n_frame_delay);	
		}
		
		checkCharging();
		if (is_charging == 0){
			break;
		}
		
		if(SDL_PollEvent(&event)){
			if ((event.type==SDL_KEYDOWN)&&(event.key.keysym.sym == 0)){
				run = 0;
				break;
			}
		}
	}

	
	//usleep(5000000);	//5s
	screen = SDL_CreateRGBSurface(SDL_HWSURFACE, 640,480, 32, 0,0,0,0);
	SDL_BlitSurface(screen, NULL, video, NULL); 
	SDL_Flip(video);
	if (run ==1){
		SetBrightness(0);
	}

	char valeur[100];
	while ((SDL_PollEvent(&event))||(run==1)) {
		SetBrightness(0);
		if (event.type==SDL_KEYDOWN) {
			if (event.key.keysym.sym == 0){	    
				SetBrightness(8);
				run = 0;
			}	
		}	
		//sprintf(valeur,"%d",event.key.keysym.sym);
		//logMessage(valeur);
		checkCharging();
		if (is_charging == 0){
			system("reboot");	
		}
		usleep(500000);
	}
	
   	SDL_FreeSurface(screen);
  
   	SDL_FreeSurface(video);
    SDL_Quit();
	
    return EXIT_SUCCESS;
}
