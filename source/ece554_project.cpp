#include <stdio.h>
#include <string>
#include "ece554_project.h"

static std::string project_description =
"Embedded Audio Mixer\n"
"Author: Chris Thomas\n"
"ECE 554 - Embedded Systems\n"
"University of Michigan - Dearborn\n";

int main(void)
{
	printf("%s\n", project_description.c_str());
	return 0;
}
