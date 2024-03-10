#include "pch.h"

#include "BitOperations.h"

namespace MyEngine
{
	ulong BitOperations::ShiftAndAdd_Byte_to_ULong(ulong theULong, char theByte, int bytesToShift)
	{
		// Too many bytes to shift (there are only 4 bytes in a long, so max is 3)?
		if (bytesToShift > 3)
		{
			bytesToShift = 0;
		}
		ushort bitsToShift = bytesToShift * 8;	// 8 bits / byte.
		// Now, convert the uChar to a uLong, 
		//	shift it the number of bits,
		//	then or it (add it) to the long
		uchar uByte = theByte;
		theULong = theULong | (static_cast<ulong>(uByte) << bitsToShift);

		return theULong;
	}

	ushort BitOperations::ShiftAndAdd_Byte_to_UShort(ushort theUShort, char theByte, int bytesToShift)
	{
		// Too many bytes to shift (there is only 2 bytes in a long, so max is 1)?
		if (bytesToShift > 1)
		{
			bytesToShift = 0;
		}
		ushort bitsToShift = bytesToShift * 8;	// 8 bits / byte.
		// Now, convert the uChar to a uLong, 
		//	shift it the number of bits,
		//	then or it (add it) to the long
		uchar uByte = theByte;
		theUShort = theUShort | (static_cast<ulong>(uByte) << bitsToShift);
		return theUShort;
	}

	ulong BitOperations::ReadAnUnsignedLong(std::ifstream& theStream)
	{
		ulong ulTheReturnVal = 0;
		// Read four bytes, but return them in reverse order.
		char TheByte = 0;
		theStream.get(TheByte);
		ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<uchar>(TheByte), 0);
		theStream.get(TheByte);
		ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<uchar>(TheByte), 1);
		theStream.get(TheByte);
		ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<uchar>(TheByte), 2);
		theStream.get(TheByte);
		ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<uchar>(TheByte), 3);
		return ulTheReturnVal;
	}

	ushort BitOperations::ReadAnUnsignedShort(std::ifstream& theStream)
	{
		ushort usTheReturnVal = 0;
		// Read two bytes, but reverse the order.
		char TheByte;
		theStream.get(TheByte);
		usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, TheByte, 0);
		theStream.get(TheByte);
		usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, TheByte, 1);
		return usTheReturnVal;
	}

	char BitOperations::ReadAByte(std::ifstream& theStream)
	{
		char theByte = 0;
		theStream.get(theByte);
		return theByte;
	}

	ulong BitOperations::ReadNextUnsignedLong(char* data, ulong& index)
	{
		ulong ulTheReturnVal = 0;
		// Read four bytes, but return them in reverse order.
		ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<uchar>(data[index]), 0);
		index++;
		ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<uchar>(data[index]), 1);
		index++;
		ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<uchar>(data[index]), 2);
		index++;
		ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<uchar>(data[index]), 3);
		index++;
		return ulTheReturnVal;
	}

	ushort BitOperations::ReadNextUnsignedShort(char* data, ulong& index)
	{
		ushort usTheReturnVal = 0;
		usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, static_cast<uchar>(data[index]), 0);
		index++;
		usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, static_cast<uchar>(data[index]), 1);
		index++;
		return usTheReturnVal;
	}

	char BitOperations::ReadNextChar(char* data, ulong& index)
	{
		char returnVal = static_cast<uchar>(data[index]);
		index++;
		return returnVal;
	}

	void BitOperations::WriteAsUnsignedShort(ushort value, std::ofstream& theStream)
	{
		// AB --> BA
		char TheByte[2] = { 0 };
		TheByte[1] = static_cast<char>((value & 0xFF00) >> 8);
		TheByte[0] = static_cast<char>(value & 0x00FF);

		theStream.put(TheByte[0]);
		theStream.put(TheByte[1]);
	}

	void BitOperations::WriteAsUnsignedLong(ulong value, std::ofstream& theStream)
	{
		// ABCD --> DCBA
		char TheByte[4] = { 0 };
		TheByte[3] = static_cast<char>((value & 0xFF000000) >> 24);
		TheByte[2] = static_cast<char>((value & 0x00FF0000) >> 16);
		TheByte[1] = static_cast<char>((value & 0x0000FF00) >> 8);
		TheByte[0] = static_cast<char>(value & 0x000000FF);

		theStream.put(TheByte[0]);
		theStream.put(TheByte[1]);
		theStream.put(TheByte[2]);
		theStream.put(TheByte[3]);
	}
}
