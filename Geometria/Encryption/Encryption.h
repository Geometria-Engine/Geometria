#pragma once

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

struct Encryption
{
	struct Base64
	{
		static std::string Encode(std::string t);
		static std::string Encode(std::string t, bool url);
		static std::string Decode(std::string t);
		static std::string Decode(std::string t, bool url);
	};

	struct Xor
	{
		static std::string Run(std::string t, int key);
	};

	struct LZMA
	{
		static std::string Compress(std::string t);
		static std::string UnCompress(std::string t);
	};

	struct ZLib
	{
		enum class Format
		{
			DEFLATE,
			ZLIB,
			GZIP
		};

		static std::string Deflate(std::string t);
		static std::string Inflate(std::string t, ZLib::Format format);
	};
};