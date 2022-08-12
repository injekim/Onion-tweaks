#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

#include <sys/stat.h>
#include <sys/ioctl.h>

#include "cJSON.h"

void write_json(char* output) {
	FILE *f = fopen("/mnt/SDCARD/favourite_temp.json", "a");

	char temp[2048];
	sprintf(temp, "%s%s", output, "\n");
	fputs(temp, f);
	fclose(f);
}

bool file_exists (char *filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

char *load_file (char const *path) {
	char *buffer = 0;
	long length = 0;
	FILE *f = fopen(path, "rb");

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char *) malloc((length + 1) *sizeof(char));
		if (buffer) {
			fread(buffer, sizeof(char), length, f);
		}
		fclose(f);
	}
	buffer[length] = '\0';
	return buffer; 
}

void generate_imgpath(cJSON *rompath, char *imgpath) {
	char console_dir[256];
	char rom_name[128];
	
	strcpy(console_dir, cJSON_GetStringValue(rompath));
	
	// Separate rom_name
	strcpy(rom_name, strrchr(console_dir, '/') + 1);
	rom_name[strrchr(rom_name, '.') - rom_name] = '\0';
	
	// Clean up console_dir
	console_dir[strrchr(console_dir, '/') - console_dir + 1] = '\0';
	
	// Generate imgpath
	sprintf(imgpath, "%sImgs/%s.png", console_dir, rom_name);
}

void copy_json_string(cJSON *origin, cJSON *dest, char *key) {
	cJSON *item = NULL;
	char string_value[256];
	
	item = cJSON_GetObjectItem(origin, key);
	if (item != NULL) {
		strcpy(string_value, cJSON_GetStringValue(item));
		cJSON_AddItemToObject(dest, key, cJSON_CreateString(string_value));
	}
}

void copy_json_number(cJSON *origin, cJSON *dest, char *key) {
	cJSON *item = NULL;
	int number_value;
	
	item = cJSON_GetObjectItem(origin, key);
	if (item != NULL) {
		number_value = cJSON_GetNumberValue(item);
		cJSON_AddItemToObject(dest, key, cJSON_CreateNumber(number_value));
	}
}

int main(void) {
	cJSON *request_json = NULL;
	
	if (file_exists("/mnt/SDCARD/Roms/favourite.json")) {
		char *request_body = load_file("/mnt/SDCARD/Roms/favourite.json");
			
		if (request_body != NULL) {
			cJSON *rompath = NULL;
			char imgpath[256 + 32];
			
			// First run
			request_json = cJSON_Parse(request_body);
			rompath = cJSON_GetObjectItem(request_json, "rompath");
			
			cJSON *json_new = cJSON_CreateObject();
			copy_json_string(request_json, json_new, "label");
			copy_json_number(request_json, json_new, "type");
			copy_json_string(request_json, json_new, "launch");
			copy_json_string(request_json, json_new, "rompath");
			
			if (rompath != NULL) {
				generate_imgpath(rompath, imgpath);
				cJSON_AddItemToObject(json_new, "imgpath", cJSON_CreateString(imgpath));
			}
			
			write_json(cJSON_PrintUnformatted(json_new));
			
			while (strchr(request_body, '}') != NULL) {
				// Move pointer
				request_body = strchr(request_body, '}') + 2;
				request_json = cJSON_Parse(request_body);
				rompath = cJSON_GetObjectItem(request_json, "rompath");
				
				json_new = cJSON_CreateObject();
				copy_json_string(request_json, json_new, "label");
				copy_json_number(request_json, json_new, "type");
				copy_json_string(request_json, json_new, "launch");
				copy_json_string(request_json, json_new, "rompath");
				
				if (rompath != NULL) {
					generate_imgpath(rompath, imgpath);
					cJSON_AddItemToObject(json_new, "imgpath", cJSON_CreateString(imgpath));
				}
				
				write_json(cJSON_PrintUnformatted(json_new));
				
				if (strchr(request_body, '}') == strrchr(request_body, '}')) break;
			}
			// Change files
			remove("/mnt/SDCARD/Roms/favourite.json");
			system("cp /mnt/SDCARD/favourite_temp.json /mnt/SDCARD/Roms/favourite.json");
			remove("/mnt/SDCARD/favourite_temp.json");
		}
	}
	
	return EXIT_SUCCESS;
}