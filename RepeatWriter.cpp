// RepeatWriter - Repeatly write a string.
// Version: 1.1.0.0
// Written by Xiaoxuan4096.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

#include <Windows.h>

namespace Xiaoxuan4096 {
	namespace Public {
		static unsigned long long getDigitNumber(unsigned long long x) {
			unsigned long long sum = 0;
			for (; x > 0; x /= 10, sum++);
			return sum;
		}
		static char getRandomChar() {
			const char chars[96] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
			std::random_device rd;
			std::uniform_int_distribution<> uid{0, 94};
			return chars[uid(rd)];
		}
		static void normalisePath(std::wstring& path) { // Delete character \"
			if (path[0] == '\"') {
				path.erase(path.begin());
				path.erase(path.end() - 1);
			}
			return;
		}
		static void readConfigFile(std::wstring configPath, unsigned long long& fileNum, unsigned long long& total, std::wstring& folderPath, std::wstring& fileName, std::wstring& str) {
			std::wfstream fin;
			std::wstring tmp;
			fin.open(configPath, std::ios::in);
			if (!fin.is_open()) {
				std::wcerr << "Failed to open config file: " << configPath << '!' << std::endl;
				Sleep(3000);
				return;
			}
			while (getline(fin, tmp)) {
				if (fin.fail() || fin.bad()) {
					fin.close();
					std::wcerr << "Failed to read config file: " << configPath << '!' << std::endl;
					Sleep(3000);
					return;
				}
				if (tmp == L"" || tmp[0] == '#')
					continue;
				std::wstring tkey = L"", tvalue = L"";
				size_t i;
				for (i = 0; tmp[i] != ' ' && i < tmp.size(); i++)
					tkey += tmp[i];
				for (; tmp[i] != '='; i++);
				for (; tmp[i] != '\"'; i++);
				for (i += 1; i < tmp.size(); i++)
					tvalue += tmp[i];
				tvalue.erase(tvalue.end() - 1); // Remove \".
				if (tkey == L"FileNumber") {
					std::wstringstream ss;
					ss << tvalue;
					ss >> fileNum;
				}
				if (tkey == L"FolderPath")
					folderPath = tvalue;
				if (tkey == L"FileName")
					fileName = tvalue;
				if (tkey == L"String")
					str = tvalue;
				if (tkey == L"Total") {
					std::wstringstream ss;
					ss << tvalue;
					ss >> total;
				}
			}
			return;
		}

		static void printStatus(std::wstring str, unsigned long long current, unsigned long long total, unsigned long long totalDigitNumber) {
			std::wcout << "String = " << str << ", Current = " << std::setw(totalDigitNumber) << current + 1 << ", Total = " << total << ", Rate = " << std::fixed << std::setprecision(2) << std::setw(6) << (100 * (double)(current + 1) / total) << "%." << std::endl;
			return;
		}
		static void printStatus(unsigned long long current, unsigned long long total, unsigned long long totalDigitNumber, char str) {
			std::wcout << "String = " << str << ", Current = " << std::setw(totalDigitNumber) << current + 1 << ", Total = " << total << ", Rate = " << std::fixed << std::setprecision(2) << std::setw(6) << (100 * (double)(current + 1) / total) << "%." << std::endl;
			return;
		}

		static void randomOutput(std::wstring path, unsigned long long total) {
			std::wfstream fout;
			fout.open(path, std::ios::out);
			if (!fout.is_open()) {
				std::wcerr << "Failed to open file: " << path << '!' << std::endl
					<< "This program can only write to existing folders." << std::endl
					<< "Try to create the folder manually." << std::endl;
				Sleep(3000);
				return;
			}
			unsigned long long totalDigitNumber = getDigitNumber(total);
			for (unsigned long long i = 0; i < total; i++) {
				char tmp = getRandomChar();
				fout << tmp;
				if (fout.fail() || fout.bad()) {
					fout.close();
					std::wcerr << "Failed to write string: " << tmp << ",to file: " << path << '!' << std::endl;
					Sleep(3000);
					return;
				}
				printStatus(i, total, totalDigitNumber, tmp);
			}
			fout.close();
			std::wcout << "Successfully write random string to file: " << path << '!' << std::endl;
			Sleep(3000);
			return;
		}
		static void defaultOutput(std::wstring path, std::wstring str, unsigned long long total) {
			std::wfstream fout;
			fout.open(path, std::ios::out);
			if (!fout.is_open()) {
				std::wcerr << "Failed to open file: " << path << '!' << std::endl
					<< "This program can only write to existing folders." << std::endl
					<< "Try to create the folder manually." << std::endl;
				Sleep(3000);
				return;
			}
			unsigned long long totalDigitNumber = getDigitNumber(total);
			for (unsigned long long i = 0; i < total; i++) {
				fout << str;
				if (fout.fail() || fout.bad()) {
					fout.close();
					std::wcerr << "Failed to write string: " << str << ",to file: " << path << '!' << std::endl;
					Sleep(3000);
					return;
				}
				printStatus(str, i, total, totalDigitNumber);
			}
			fout.close();
			std::wcout << "Successfully write string: " << str << ",to file: " << path << '!' << std::endl;
			Sleep(3000);
			return;
		}

		static void useInterface(unsigned long long total) {
			std::wstring str, path;
			std::cout << "Input string(leave blank for random characters):\n>>> ";
			getline(std::wcin, str);
			std::cout << "Input path:\n>>> ";
			getline(std::wcin, path);
			normalisePath(path);
			if (str == L"")
				randomOutput(path, total);
			else
				defaultOutput(path, str, total);
			return;
		}
		static void useConfigFile() {
			std::wstring configPath, folderPath, fileName, str;
			unsigned long long total, fileNum;
			std::cout << "Input config file path(leave blank to use configExample.ini):\n>>> ";
			getline(std::wcin, configPath);
			if (configPath == L"")
				configPath = L"configExample.ini";
			readConfigFile(configPath, fileNum, total, folderPath, fileName, str);
			if (fileNum == 1)
				if (str == L"")
					randomOutput(folderPath + L'/' + fileName, total);
				else
					defaultOutput(folderPath + L'/' + fileName, str, total);
			else
				if (str == L"")
					for (unsigned long long i = 0; i < fileNum; i++) {
						std::wstring number;
						std::wstringstream ss;
						ss << i + 1;
						ss >> number;
						randomOutput(number + L" - " + folderPath + fileName, total);
					}
				else
					for (unsigned long long i = 0; i < fileNum; i++) {
						std::wstring number;
						std::wstringstream ss;
						ss << i + 1;
						ss >> number;
						defaultOutput(number + L" - " + folderPath + fileName, str, total);
					}
			return;
		}

		void main() {
			unsigned long long total;
			std::cout << "Input total(input 0 to read config file):\n>>> ";
			std::cin >> total;
			getchar();
			if (total == 0)
				useConfigFile();
			else
				useInterface(total);
			return;
		}
	}
}
int main() {
	Xiaoxuan4096::Public::main();
	return 0;
}