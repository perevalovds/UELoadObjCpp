// Fill out your copyright notice in the Description page of Project Settings.


#include "DeUtils.h"

#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>


//----------------------------------------
int DeUtils::ToInt(const std::string& intString) {
	int x = 0;
	std::istringstream cur(intString);
	cur >> x;
	return x;
}

//----------------------------------------
int DeUtils::ToUInt32(const std::string& intString) {
	int x = 0;
	std::istringstream cur(intString);
	cur >> x;
	return x;
}
//---------------------------------------
unsigned char DeUtils::ToU8(const std::string& intString) {
	unsigned char x = 0;
	std::istringstream cur(intString);
	cur >> x;
	return x;
}

//----------------------------------------
float DeUtils::ToFloat(const std::string& floatString) {
	float x = 0;
	std::istringstream cur(floatString);
	cur >> x;
	return x;
}

//----------------------------------------
double DeUtils::ToDouble(const std::string& doubleString) {
	double x = 0;
	std::istringstream cur(doubleString);
	cur >> x;
	return x;
}

//----------------------------------------
std::vector<std::string> DeUtils::SplitString(const std::string& source, const std::string& delimiter, bool ignoreEmpty) {
	std::vector<std::string> result;
	if (delimiter.empty()) {
		result.push_back(source);
		return result;
	}
	std::string::const_iterator substart = source.begin(), subend;
	while (true) {
		subend = search(substart, source.end(), delimiter.begin(), delimiter.end());
		std::string sub(substart, subend);
		if (!ignoreEmpty || !sub.empty()) {
			result.push_back(sub);
		}
		if (subend == source.end()) {
			break;
		}
		substart = subend + delimiter.size();
	}
	return result;
}

//--------------------------------------------------
void DeUtils::StringTrim(std::string& input, bool trimSpace, bool trimTab, bool trimBSlashN) {
	StringTrimLeft(input, trimSpace, trimTab, trimBSlashN);
	StringTrimRight(input, trimSpace, trimTab, trimBSlashN);
}

//--------------------------------------------------
void DeUtils::StringTrimLeft(std::string& input, bool trimSpace, bool trimTab, bool trimBSlashN) {
	while (!input.empty()) {
		char c = input[0];
		if ((trimSpace && c == ' ') || (trimTab && c == '\t') || (trimBSlashN && (c == '\n' || c == '\r'))) {
			input = input.substr(1);
		}
		else break;
	}
}

//--------------------------------------------------
void DeUtils::StringTrimRight(std::string& input, bool trimSpace, bool trimTab, bool trimBSlashN) {
	while (!input.empty()) {
		int i = int(input.length() - 1);
		char c = input[i];
		if ((trimSpace && c == ' ') || (trimTab && c == '\t') || (trimBSlashN && (c == '\n' || c == '\r'))) {
			input = input.substr(0, input.length() - 1);
		}
		else break;
	}
}

//--------------------------------------------------
bool DeUtils::StringBeginsWith(const std::string& s, const std::string& prefix) {
	return (s.substr(0, prefix.length()) == prefix);
}

//--------------------------------------------------
bool DeUtils::StringEndsWith(const std::string& s, const std::string& suffix) {
	return (s.length() >= suffix.length() && s.substr(s.length() - suffix.length(), suffix.length()) == suffix);
}

//--------------------------------------------------
char DeUtils::StringLastSymbol(const std::string& s) {
	if (!s.empty()) return s[s.length() - 1];
	else return 0;
}

//--------------------------------------------------
bool DeUtils::StringIsInt(const std::string& s) {  //является ли строка целым числом
	std::string s1 = s;
	StringTrim(s1);
	for (size_t i = 0; i < s1.length(); i++) {
		char c = s1[i];
		if (!((c >= '0' && c <= '9') || (i == 0 && (c == '+' || c == '-')))) {
			return false;
		}
	}
	return true;
}

//--------------------------------------------------
std::string DeUtils::FileRemoveExtension(std::string fileName)    //имя файла без расширения
{
	if (!fileName.empty()) {
		int n = fileName.length();
		int k = FileGetExtension(fileName).length();
		return fileName.substr(0, n - k - 1);
	}
	return "";
}

//--------------------------------------------------
std::string DeUtils::FileGetExtension(std::string fileName)         //расширение файла
{
	if (!fileName.empty()) {
		int n = fileName.size();
		for (int i = n - 1; i >= 0; i--) {
			unsigned char c = fileName[i];
			if (c == '.') {
				return fileName.substr(i + 1);
			}
			if (c == '/' || c == '\\') break;
		}
	}
	return "";
}

//--------------------------------------------------
std::vector<std::string> DeUtils::FileReadStrings(std::string fileName)    //считать текстовый файл в строки
{
	std::vector<std::string> lines;
	//Assert(FileExists(fileName), "FileReadStrings no file " + fileName);
	std::ifstream f(fileName.c_str(), std::ios::in | std::ios::binary);
	std::string line;
	while (getline(f, line)) {
		if (line == "") continue;
		else {
			//убираем в конце '\r' для правильного считывания windows-файлов в linux
			//и в windows также сейчас такие есть
			while (StringLastSymbol(line) == '\r') {
				line = line.substr(0, line.length() - 1);
			}
			lines.push_back(line);
		}
	}
	return lines;
}

//--------------------------------------------------
std::string DeUtils::FileReadString(std::string fileName)    //считать строку из текстового файла
{
	//Assert(FileExists(fileName), "FileReadString - file '" + fileName + "' not found");

	//TODO на linux последний символ "\r" отрезает всё строку, поэтому такое считывание сделал
	std::string value = "";
	FILE* f;
	fopen_s(&f, fileName.c_str(), "rb");
	if (f) {
		while (!feof(f)) {
			char c;
			fread(&c, 1, 1, f);
			if (feof(f)) break;
			if (c == '\r' || c == '\n') break;
			value += c;
		}
		fclose(f);
	}
	return value;
}

//--------------------------------------------------
void DeUtils::FileWriteStrings(const std::vector<std::string>& list, std::string fileName)    //записать строки в текстовый файл
{
	std::ofstream f(fileName.c_str(), std::ios::out);
	//Assert(!f.fail(), "FileWriteStrings - Error creating file " + fileName);
	for (size_t i = 0; i < list.size(); i++) {
		f << list[i] << std::endl;
	}
	//Assert(!f.fail(), "FileWriteStrings - error writing file " + fileName);
	f.close();
}

//--------------------------------------------------
void DeUtils::FileWriteString(const std::string& line, const std::string& fileName)    //записать строку в текстовый файл
{
	std::vector<std::string> file;
	file.push_back(line);
	FileWriteStrings(file, fileName);
}

//--------------------------------------------------
// Load Obj file 
// Returns array of vertices x0,y0,z0,x1,y1,z1,... and triangle indices A0,B0,C0,A1,B1,C1,...
bool DeUtils::LoadObjMesh(const std::string& fileName, std::vector<float>& XYZ, std::vector<int>& Tri)
{
	XYZ.clear();      //vertices
	Tri.clear();  //indices for triangles
	//vector<glm::vec2> tex;    //texture coords

	auto lines = FileReadStrings(fileName);
	if (lines.empty()) {
		return false;
	}

	int f[4];

	for (int i = 0; i < lines.size(); i++) {
		auto list = SplitString(lines[i], " ", true);
		int n = list.size();
		if (n > 0) {
			if (list[0] == "v" && n >= 4) {
				XYZ.push_back(ToFloat(list[1]));
				XYZ.push_back(ToFloat(list[2]));
				XYZ.push_back(ToFloat(list[3]));
			}
			/*if (useTex && list[0] == "vt" && n >= 3) {
				tex.push_back(glm::vec2(
					ofToFloat(list[1]) * texW,
					ofToFloat(list[2]) * texH
				));
			}*/
			if (list[0] == "f" && n >= 4) {
				int N = std::min(n - 1, 4);
				for (int j = 0; j < N; j++) {
					auto line = SplitString(list[j + 1], "/", true);
					if (line.size() > 0) {
						f[j] = ToInt(line[0]) - 1;
					}
				}
				Tri.push_back(f[0]);
				Tri.push_back(f[1]);
				Tri.push_back(f[2]);
				//4-угольная грань
				if (n >= 5) {
					Tri.push_back(f[0]);
					Tri.push_back(f[2]);
					Tri.push_back(f[3]);
				}
			}

		}
	}

	//нормализация в куб [-1,1]x[-1,1]x[-1,1]
	/*if (normalize) {
		glm::vec3 p0 = ofPoint(0);
		glm::vec3 p1 = p0;
		if (v.size() > 0) {
			p0 = v[0];
			p1 = p0;
			for (int i = 0; i < v.size(); i++) {
				glm::vec3& p = v[i];
				p0.x = min(p0.x, p.x);
				p0.y = min(p0.y, p.y);
				p0.z = min(p0.z, p.z);
				p1.x = max(p1.x, p.x);
				p1.y = max(p1.y, p.y);
				p1.z = max(p1.z, p.z);
			}
		}
		glm::vec3 c = (p0 + p1) * 0.5;
		glm::vec3 delta = p1 - p0;
		float scl = delta.x;
		scl = max(scl, delta.y);
		scl = max(scl, delta.z);
		if (scl > 0) {
			scl = 1.0 / scl;
		}
		for (int j = 0; j < v.size(); j++) {
			glm::vec3& p = v[j];
			p = (p - c) * scl;
		}
	}*/

	//normals
	// if (setupNormals) { ofxKuSetNormals(mesh); }
	return true;
}


//--------------------------------------------------
