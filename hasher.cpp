//#include "stdafx.h"			Include this for compatibility with projects with precompiled headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SETTINGS_MAX_LINE_LENGTH 500


// TODO: Keeping settings list in strings will make the settings update method
// of HasherSettings class dependent on a specific string's number in array.
// The most organic solution should be making a single setting class containing
// its name and value, and create an array of these classes in HasherSettings class.
const char* settings_filename = "hset.ini";
const char* settings_list[] = {	"HasherSetModulo=",
								"HasherSetSalt=",		
								"HasherSetMode="	};




// This is different from checking whether source contains substring, in that we only want to
// know if the string at the start is what we're looking for. This implementation is simpler
// and more secure.

bool extract_substring(const char* source, const char* substring) {
	if(source && substring) {
		if(strcmp(source, substring) < 0)
			return false;
		for(unsigned int i = 0; i < SETTINGS_MAX_LINE_LENGTH && source[i] != '\0' && substring[i] != '\0'; i++)
			if(source[i] != substring[i])
				return false;	// If at least two characters don't match, substring cannot be extracted.
		return true;	// So far we don't care what comes next.
		}
	return false;	// If at least one pointer to string is NULL
}

char* extract_remainder(const char* source) {
	char* remainder = (char*)malloc(sizeof(char) * SETTINGS_MAX_LINE_LENGTH);
	unsigned int i, j = 0;
	for(i = 0; source[i] != '='; i++)
		;
	i++;
	for(j = 0; source[i] != '\n' && source[i] != '\0'; i++, j++)
		remainder[j] = source[i];
	remainder[j] = '\0';
	return remainder;
}


class HasherSettings {
private:
	unsigned short modulo;
	int salt;

public:
	int ReadSettingsFromFile() {
		FILE* _fptr;
		char* settings_str = (char*)malloc(sizeof(char) * SETTINGS_MAX_LINE_LENGTH);
		if(!(_fptr = fopen(settings_filename, "r"))) {		// If pointer to file is NULL
			printf("Error while opening hset.ini\n"
					"Make sure that the settings file is properly placed and named.\n");
			exit(EXIT_FAILURE);
		}
		while(!feof(_fptr)) {		// Print lines out until it's end of file.
			printf("%s", settings_str = fgets(settings_str, SETTINGS_MAX_LINE_LENGTH, _fptr));
			for(unsigned int i = 0; i < 3; i++)
				if(extract_substring(settings_str, settings_list[i]))
					printf("\nRemainder: %s\n\n", extract_remainder(settings_str));
		}
		printf("\n");
		return 0;
	}
};




int main(int argc, char* argv[]) {
	HasherSettings _hs;
	_hs.ReadSettingsFromFile();
	return 0;
}