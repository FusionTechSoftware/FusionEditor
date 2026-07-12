#ifndef __IMAGE_UTILITIES_HEADER__
#define __IMAGE_UTILITIES_HEADER__

#include <string>
#include <vector>
#include <fstream>

#include "../../Resources/glad/glad.h"

struct ImageBufferFormatLocal
{
	unsigned int				TextureWidth;
	unsigned int				TextureHeight;
	unsigned int				TextureChannel;
	std::vector<unsigned char>	TextureData;
};

class ImageUtility
{
	public:
		static ImageBufferFormatLocal	ReadImageBuffer(const std::string& ImageLocation);
		static unsigned int				LoadTextureIntoMemory(ImageBufferFormatLocal& TextureData);
};

#endif	