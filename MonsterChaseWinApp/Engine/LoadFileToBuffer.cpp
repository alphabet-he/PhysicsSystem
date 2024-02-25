#include "LoadFileToBuffer.h"
#include <cassert>
#include <string>

std::vector<uint8_t> Engine::LoadFileToBuffer(const char* i_pFilename)
{
	assert(i_pFilename != nullptr);

	std::vector<uint8_t> Buffer;

	std::string basePath = "..\\MonsterChase\\";
	std::string newFileName = basePath + i_pFilename; // Concatenation
	const char* finalPath = newFileName.c_str();

	FILE* pFile = nullptr;

	if (fopen_s(&pFile, finalPath, "rb") == 0)
	{
		assert(pFile != nullptr);

		int FileIOError = fseek(pFile, 0, SEEK_END);
		assert(FileIOError == 0);

		long FileSize = ftell(pFile);
		assert(FileSize >= 0);

		FileIOError = fseek(pFile, 0, SEEK_SET);
		assert(FileIOError == 0);

		Buffer.reserve(FileSize);
		Buffer.resize(FileSize);

		size_t FileRead = fread(&Buffer[0], 1, FileSize, pFile);
		assert(FileRead == FileSize);

		fclose(pFile);
	}

	return Buffer;
}
