#include "ImageUtilities.h"
#include "../Common/Console.h"

ImageBufferFormatLocal ImageUtility::ReadImageBuffer(const std::string& ImageLocation)
{
	std::ifstream OpenedFile(ImageLocation, std::ios::binary);
	if (!OpenedFile) { Console::SysPrint("Failed to open file: " + ImageLocation, 2); return {}; }
	ImageBufferFormatLocal NewBuffer;

	uint32_t TemporaryWidth, TemporaryHeight, TemporaryChannel;
	uint32_t DataSize;

	OpenedFile.read((char*)&TemporaryWidth, sizeof(uint32_t));
	OpenedFile.read((char*)&TemporaryHeight, sizeof(uint32_t));
	OpenedFile.read((char*)&TemporaryChannel , sizeof(uint32_t));
	OpenedFile.read((char*)&DataSize, sizeof(uint32_t));

	std::vector<unsigned char> RefinedData(DataSize);

	OpenedFile.read((char*)RefinedData.data(), DataSize);

	NewBuffer.TextureWidth = TemporaryWidth;
	NewBuffer.TextureHeight = TemporaryHeight;
	NewBuffer.TextureChannel = TemporaryChannel;
	NewBuffer.TextureData = RefinedData;

	OpenedFile.close();
	return NewBuffer;
}


unsigned int ImageUtility::LoadTextureIntoMemory(ImageBufferFormatLocal& TextureData)
{
	unsigned int TextureID;
	unsigned int ChannelFormat;

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	ChannelFormat =	(TextureData.TextureChannel == 4) ? GL_RGBA :
					(TextureData.TextureChannel == 3) ? GL_RGB :
					(TextureData.TextureChannel == 1) ? GL_RED : GL_RGB;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ChannelFormat, TextureData.TextureWidth, TextureData.TextureHeight, 0, ChannelFormat, GL_UNSIGNED_BYTE, &TextureData.TextureData[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	return TextureID;
}