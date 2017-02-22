#include "stdafx.h"
#include "Core.h"

using namespace std;

std::vector<const char*> g_filenames =
{
	R"(.\img\00.jpg)",
	R"(.\img\01.jpg)",
	R"(.\img\10.jpg)",
	R"(.\img\11.jpg)",
	R"(.\img\20.jpg)",
	R"(.\img\21.jpg)",
	R"(.\img\30.jpg)",
	R"(.\img\31.jpg)",
	R"(.\img\40.jpg)",
	R"(.\img\41.jpg)"
};

string baseFileName(const char* filename)
{
	string base_name = filename;
	size_t pos = base_name.rfind('/');
	if (pos != string::npos)
	{
		base_name.erase(0, pos + 1);
	}
	pos = base_name.rfind('\\');
	if (pos != string::npos)
	{
		base_name.erase(0, pos + 1);
	}
	return base_name;
}