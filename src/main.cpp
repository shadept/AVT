#include <iostream>
#include <fstream>
#include <string>

#include "App.h"

std::string readFile(const std::string& filename)
{
	std::string contents;
	std::ifstream file(filename, std::ios::in);
	if (file)
	{
		file.seekg(0, std::ios::end);
		contents.resize(file.tellg()); // wild efficient memory allocation appears
		file.seekg(0, std::ios::beg);
		file.read(&contents[0], contents.size());
		file.close();
	}
	return contents;
}

int main(int argc, char* argv[])
{
	App app;
	app.MainLoop();

	exit(EXIT_SUCCESS);
}
