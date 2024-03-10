#pragma once

#include "Engine/Core/types.h"

#include <fstream>

namespace MyEngine
{
	class BitOperations
	{
	public:
		// These functions shift numbers by one, two, and three bytes.
		static ulong ShiftAndAdd_Byte_to_ULong(ulong theULong, char theByte, int bytesToShift);
		static ushort ShiftAndAdd_Byte_to_UShort(ushort theUShort, char theByte, int bytesToShift);
		
		// These are used to read the data from a file (LoadBMP method).
		static ulong ReadAnUnsignedLong(std::ifstream& theStream);
		static ushort ReadAnUnsignedShort(std::ifstream& theStream);
		static char ReadAByte(std::ifstream& theStream);
		
		// These are used to read the data from a file (LoadBMP2 method).
		// * index is incremented appropriate amount (to next char)
		// * no bounds checking for array
		static ulong ReadNextUnsignedLong(char* data, ulong& index);
		static ushort ReadNextUnsignedShort(char* data, ulong& index);
		static char ReadNextChar(char* data, ulong& index);
		//
		static void WriteAsUnsignedShort(ushort value, std::ofstream& theStream);
		static void WriteAsUnsignedLong(ulong value, std::ofstream& theStream);
	};
}
