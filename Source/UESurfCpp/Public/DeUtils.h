// DeUtils, UE5-utilities, adopted from openFrameworks sources and ofxKu addon by Denis Perevalov

#pragma once

#include "CoreMinimal.h"


#include <string>
#include <vector>

class UESURFCPP_API DeUtils
{
public:
	static int ToInt(const std::string& intString);
	
	static int ToUInt32(const std::string& intString);
	
	static unsigned char ToU8(const std::string& intString);
	
	static float ToFloat(const std::string& floatString);
	
	static double ToDouble(const std::string& doubleString);

	static std::vector<std::string> SplitString(const std::string& source, const std::string& delimiter, bool ignoreEmpty = false);

	static void StringTrim(std::string& input, bool trimSpace = true, bool trimTab = true, bool trimBSlashN = true);

	static void StringTrimLeft(std::string& input, bool trimSpace = true, bool trimTab = true, bool trimBSlashN = true);

	static void StringTrimRight(std::string& input, bool trimSpace = true, bool trimTab = true, bool trimBSlashN = true);

	static bool StringBeginsWith(const std::string& s, const std::string& prefix);

	static bool StringEndsWith(const std::string& s, const std::string& suffix);

	static char StringLastSymbol(const std::string& s);

	static bool StringIsInt(const std::string& s);  //является ли строка целым числом

	static std::string FileRemoveExtension(std::string fileName);    //имя файла без расширения

	static std::string FileGetExtension(std::string fileName);			//расширение файла

	static std::vector<std::string> FileReadStrings(std::string fileName);    //считать текстовый файл в строки
	
	static std::string FileReadString(std::string fileName);    //считать строку из текстового файла
	
	static void FileWriteStrings(const std::vector<std::string>& list, std::string fileName);    //записать строки в текстовый файл
	
	static void FileWriteString(const std::string& line, const std::string& fileName);    //записать строку в текстовый файл
	
	// Load Obj file 
	// Returns array of vertices x0,y0,z0,x1,y1,z1,... and triangle indices A0,B0,C0,A1,B1,C1,...
	static bool LoadObjMesh(const std::string& fileName, std::vector<float>& XYZ, std::vector<int>& Tri);
};
