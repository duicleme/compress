#include <stdio.h>
#include <iostream>


#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif



#include "../Source/ZipLib/ZipFile.h"
#include "../Source/ZipLib/streams/memstream.h"

#include "../Source/ZipLib/methods/Bzip2Method.h"



#include <fstream>
#include <functional>
#include <string>
#include <string_view>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>




#pragma comment (lib,"bzip2.lib")
#pragma comment (lib,"lzma.lib")
#pragma comment (lib,"ZipLib.lib")
#pragma comment (lib,"zlib.lib")


using std::cout;
using std::endl;


namespace fs = std::experimental::filesystem;

void compress(fs::path const& source,
	fs::path const& archive,
	std::function<void(std::string_view)> reporter)
{
	if (fs::is_regular_file(source))
	{
		if (reporter) reporter("Compressing " + source.string());
		ZipFile::AddFile(archive.string(), source.string(), LzmaMethod::Create());
	}
	else
	{
		puts("is not a regular file .");
	}
}


int main(void)
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	cout << "Test Zip." << endl;

	compress(".\\in1.jpg", ".\\test.zip",
		[](std::string_view message) { std::cout << message << std::endl; });

	cout << "End Zip." << endl;
	
	return 0;
}