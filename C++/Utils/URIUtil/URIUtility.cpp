#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*		EXIT CODES DEFINITION		*/
#define	INVALID_ARGUMENT_COUNT		2
#define INVALID_OPTION_ARGUMENT		3 
#define INVALID_INPUT_FILE				4
#define OUTPUT_FILE_EXISTS				5
#define INPUT_FILE_NOT_READABLE		6
#define OUTPUT_FILE_NOT_WRITABLE	7

namespace UTF8URIUtil
{
	const char HEX2DEC[256] = 
	{
			/*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
			/* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,
			
			/* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			
			/* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			
			/* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
			/* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
	};

	// Only alphanum is safe.
	const char SAFE[256] =
	{
			/*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
			/* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,
			
			/* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
			/* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
			/* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
			/* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
			
			/* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			
			/* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
			/* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
	};

	std::string URIDecode(const std::string & sSrc)
	{
		// Note from RFC1630:  "Sequences which start with a percent sign
		// but are not followed by two hexadecimal characters (0-9, A-F) are reserved
		// for future extension"
			
		const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
		const int SRC_LEN = sSrc.length();
		const unsigned char * const SRC_END = pSrc + SRC_LEN;
		const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   // last decodable '%' 

		char * const pStart = new char[SRC_LEN];
		char * pEnd = pStart;

		while (pSrc < SRC_LAST_DEC)
		{
			if (*pSrc == '%')
			{
				char dec1, dec2;
				if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)]) && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
				{
					*pEnd++ = (dec1 << 4) + dec2;
					pSrc += 3;
					continue;
				}
			}
			*pEnd++ = *pSrc++;
		}

		// the last 2- chars
		while (pSrc < SRC_END) {
			*pEnd++ = *pSrc++;
		}

		std::string sResult(pStart, pEnd);
		delete [] pStart;
		return sResult;
	}

	std::string URIEncode(const std::string & sSrc)
	{
		const char DEC2HEX[16 + 1] = "0123456789abcdef";
		const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
		const int SRC_LEN = sSrc.length();
		unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
		unsigned char * pEnd = pStart;
		const unsigned char * const SRC_END = pSrc + SRC_LEN;

		for (; pSrc < SRC_END; ++pSrc)
		{
			if (SAFE[*pSrc]) {
				*pEnd++ = *pSrc;
			}	else if (*pSrc == ' ' || *pSrc == '-' || *pSrc == '_' || *pSrc == '.' || *pSrc == '!' || *pSrc == '~' || *pSrc == '*' || *pSrc == '\'' || *pSrc == '(' || *pSrc == ')' || *pSrc == ':' || *pSrc == '/'|| *pSrc == '\\') {
				*pEnd++ = *pSrc;
			} else {
				// escape this char
				*pEnd++ = '%';
				*pEnd++ = DEC2HEX[*pSrc >> 4];
				*pEnd++ = DEC2HEX[*pSrc & 0x0F];
			}
		}

		std::string sResult((char *)pStart, (char *)pEnd);
		delete [] pStart;
		return sResult;
	}

	int ConvertFile(std::string option,std::string inputFile,std::string outputFile)
	{
		if( option.compare("encode") == 0 )
		{

			return 0;
		}
		else if (option.compare("decode") == 0 )
		{
			std::string inputFileLine;
			std::ifstream _inputFileStream;
			_inputFileStream.open(inputFile.c_str());

			if(!_inputFileStream.good())
			{
				return INPUT_FILE_NOT_READABLE;
			}

			std::ofstream _outputFileStream(outputFile.c_str());
			bool canOpenAndWrite = _outputFileStream.is_open();
			if(!canOpenAndWrite)
			{
				return OUTPUT_FILE_NOT_WRITABLE;
			}

			while (std::getline(_inputFileStream, inputFileLine))
			{
				std::istringstream iss(inputFileLine);
				std::string fileNameToDecode = inputFileLine;
				std::string decodedFileName = URIDecode(fileNameToDecode);
				_outputFileStream<<decodedFileName<<"\n";
				//std::cout<<decodedFileName<<"\n";
			}
			_inputFileStream.close();
			_outputFileStream.close();
			return 0;
		}
		else
		{
			return INVALID_OPTION_ARGUMENT; 
		}
	}
}

namespace FSUtil
{
	bool IsFileExists(const std::string& fileName)
	{
		struct stat statBuf;
		int retVal = stat(fileName.c_str(),&statBuf);
		if(retVal == 0 )	// Existence is Confirmed
		{
			//Now Check if the stat belongs to a file
			if (S_ISREG (statBuf.st_mode))
			{
				return true;
			}
			else
			{
				std::cout << "###	"<<fileName<<" is not a regular File ###\n";
				return false;
			}
			//if (S_ISDIR (st_buf.st_mode))	DIR CHECK
		}
		else
		{
			std::cout << "###	"<<fileName<<" doesn't exists ###\n";
			return false;
		}
	}
}

namespace CommonUtil
{
	int ProcessArguments(std::string option,std::string inputFile,std::string outputFile)
	{
		if( (option.compare("encode") != 0) && (option.compare("decode") != 0))
		{
			std::cout << "###	Invalid Option Argument ### \n";
			return INVALID_OPTION_ARGUMENT;
		}
		if(!FSUtil::IsFileExists(inputFile))
		{
			std::cout << "###	Input File doesn't Exists ### \n";
			return INVALID_INPUT_FILE;
		}
		if(FSUtil::IsFileExists(outputFile))
		{
			std::cout << "###	Output File Exists Already ###\n";
			return OUTPUT_FILE_EXISTS;
		}
		return 0;
	}
}


int main(int argc, char *argv[])
{
	// Start Processing Arguments
	if( argc == 4)
	{
		std::cout << "###	Parsing Arguments		###\n";
		std::string option			= argv[1];
		std::string inputFile		= argv[2];
		std::string outputFile	= argv[3];

		int retVal = CommonUtil::ProcessArguments(option,inputFile,outputFile);
		if(retVal != 0)	// Invalid Args
		{
			return retVal;
		} // ELSE SHOULD CONTINUE

		retVal = 0;
		retVal = UTF8URIUtil::ConvertFile(option,inputFile,outputFile);
		if(retVal != 0)
		{
			return retVal;
		}
	}
	else
	{
		std::cout << "### Invalid Argument Count :"<<argc << ":	Required Argument Count : 3	###\n";
		std::cout << "### Argument Pattern :	###	\n";
		std::cout << "URLEncoder encode fileToEncode.txt outputEncodedFile.txt				\n";
		std::cout << "URLEncoder decode fileToDecode.txt outputDecodedFile.txt				\n";
		return INVALID_ARGUMENT_COUNT;
	}
	std::cout << "###			CONVERSION SUCCESS		###\n";
	return 0;
}
