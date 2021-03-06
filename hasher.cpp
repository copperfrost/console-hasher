//#include "stdafx.h"			Include this for compatibility with projects with precompiled headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SETTINGS_MAX_LINE_LENGTH 500
#define SETTINGS_NUM 2

const char* settings_filename = "hset.ini";
const char* settings_list[SETTINGS_NUM] = { "HasherSetModulo", "HasherSetSalt" };

class HasherSettings {
private:

	unsigned short modulo;
	int salt;
	char* settings[SETTINGS_NUM];

	int GetSettingIndex(const char* setting_name) {
		for (int i = 0; i < SETTINGS_NUM; i++)
			if (AreStringsEqual(setting_name, settings_list[i]))
				return i;
		return -1;
	}

public:

	HasherSettings() {
		for (unsigned int i = 0; i < SETTINGS_NUM; settings[i] = '\0', i++)
			;
	}

	bool AreStringsEqual(const char* str1, const char* str2) {
		if (abs(strncmp(str1, str2, SETTINGS_MAX_LINE_LENGTH)))
			return false;
		for (unsigned int i = 0; str1[i] != '\0' && str2[i] != '\0' && i < SETTINGS_MAX_LINE_LENGTH; i++)
			if (str1[i] != str2[i])
				return false;
		return true;
	}

	char* ExtractSubstring(const char* str) {
		char* substr = (char*)malloc(sizeof(char)*SETTINGS_MAX_LINE_LENGTH);
		unsigned int i;
		for (i = 0; str[i] != '=' && str[i] != '\0' && str[i] != '\n' && i < SETTINGS_MAX_LINE_LENGTH; i++)
			substr[i] = str[i];
		substr[i] = '\0';
		return substr;
	}

	char* ExtractRemainder(const char* str) {
		char* substr = (char*)malloc(sizeof(char)*SETTINGS_MAX_LINE_LENGTH);
		unsigned int i, j;
		for (i = 0; str[i] != '=' && i < SETTINGS_MAX_LINE_LENGTH; i++)
			;
		i++;
		if (i > SETTINGS_MAX_LINE_LENGTH)
			return NULL;
		for (j = 0; j < SETTINGS_MAX_LINE_LENGTH && (substr[j] = str[i]) != '\0' && str[i] != '\n'; i++, j++)
			;
		substr[j] = '\0';
		return substr;
	}

	int ReadSettingsFromFile() {
		FILE* _fptr;
		unsigned int setting_index;
		char* settings_str = (char*)malloc(sizeof(char) * SETTINGS_MAX_LINE_LENGTH);
		if(!(_fptr = fopen(settings_filename, "r"))) {		// If pointer to file is NULL
			printf(	"Error while opening hset.ini\n"
					"Make sure that the settings file is properly placed and named.\n");
			exit(EXIT_FAILURE);
		}
		while(!feof(_fptr)) {
			settings_str = fgets(settings_str, SETTINGS_MAX_LINE_LENGTH, _fptr);
			setting_index = GetSettingIndex(ExtractSubstring(settings_str));
			if(setting_index >= 0)
				settings[setting_index] = ExtractRemainder(settings_str);
		}
		printf("\n");
		return 0;
	}

};


int main(int argc, char* argv[]) {
	HasherSettings hs;
	hs.ReadSettingsFromFile();
	return 0;
}