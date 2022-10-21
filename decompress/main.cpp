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

#include "./ZipLib/utils/stream_utils.h"



void ensure_directory_exists(fs::path const& dir)
{
	if (fs::exists(dir))
	{
		std::error_code err;
		fs::create_directories(dir, err);
	}
}


void decompress(fs::path const& destination,
	fs::path const& archive,
	std::function<void(std::string_view)> reporter)
{
	ensure_directory_exists(destination);

	auto zipArchive = ZipFile::Open(archive.string());

	for (size_t i = 0; i < zipArchive->GetEntriesCount(); ++i)
	{
		auto entry = zipArchive->GetEntry(i);

		if (entry)
		{
			auto filepath = destination / fs::path{ entry->GetFullName() }.relative_path();
			if (reporter) reporter("Creating" + filepath.string());

			if (entry->IsDirectory())
			{
				ensure_directory_exists(filepath);
			}
			else
			{
				ensure_directory_exists(filepath.parent_path());

				std::ofstream destFile;
				destFile.open(filepath.string().c_str(), std::ios::binary | std::ios::trunc);
				if (!destFile.is_open())
				{
					if (reporter) reporter("Cannot create destination file!");
				}

				auto dataStream = entry->GetDecompressionStream();
				if (dataStream)
				{
					utils::stream::copy(*dataStream, destFile);
				}
			}
		}
	}
}



int main(void)
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	cout << "Test Zip." << endl;

	decompress(".\\", ".\\test.zip",
		[](std::string_view message) { std::cout << message << std::endl; });


	cout << "End Zip." << endl;
	
	return 0;
}