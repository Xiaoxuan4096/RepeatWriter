// RepeatWriter - Repeatly write a string.
// Version: 1.1.1.7
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
		static void printStatus(std::wstring str, unsigned long long current, unsigned long long total, unsigned long long totalDigitNumber, unsigned long long currentFile, unsigned long long totalFile, unsigned long long totalFileDigitNumber) {
			std::wcout << "String = " << str << ", Current = " << std::setw(totalDigitNumber) << current + 1 << ", Total = " << total << ", SingleRate = " << std::fixed << std::setprecision(2) << std::setw(6) << (100 * (double)(current + 1) / total) << "%, CurrentFile = " << std::setw(totalFileDigitNumber) << currentFile + 1 << ", TotalFile = " << totalFile << ", TotalRate = " << std::fixed << std::setprecision(2) << std::setw(6) << (100 * (double)(currentFile * total + current + 1) / (totalFile * total)) << "%." << std::endl;
			return;
		}
		static void printStatus(unsigned long long current, unsigned long long total, unsigned long long totalDigitNumber, unsigned long long currentFile, unsigned long long totalFile, unsigned long long totalFileDigitNumber, char str) {
			std::wcout << "String = " << str << ", Current = " << std::setw(totalDigitNumber) << current + 1 << ", Total = " << total << ", SingleRate = " << std::fixed << std::setprecision(2) << std::setw(6) << (100 * (double)(current + 1) / total) << "%, CurrentFile = " << std::setw(totalFileDigitNumber) << currentFile + 1 << ", TotalFile = " << totalFile << ", TotalRate = " << std::fixed << std::setprecision(2) << std::setw(6) << (100 * (double)(currentFile * total + current + 1) / (totalFile * total)) << "%." << std::endl;
			return;
		}

		static bool randomOutput(std::wstring path, unsigned long long total, bool config = false, unsigned long long currentFile = 1, unsigned long long totalFile = 1) {
			std::wfstream fout;
			fout.open(path, std::ios::out);
			if (!fout.is_open()) {
				std::wcerr << "Failed to open file: " << path << '!' << std::endl
					<< "This program can only write to existing folders." << std::endl
					<< "Try to create the folder manually." << std::endl;
				Sleep(3000);
				return false;
			}
			unsigned long long totalDigitNumber = getDigitNumber(total), totalFileNumber = getDigitNumber(totalFile);
			for (unsigned long long i = 0; i < total; i++) {
				char tmp = getRandomChar();
				fout << tmp;
				if (fout.fail() || fout.bad()) {
					fout.close();
					std::wcerr << "Failed to write string: " << tmp << ", to file: " << path << '!' << std::endl;
					Sleep(3000);
					return false;
				}
				if (config)
					printStatus(i, total, totalDigitNumber, currentFile, totalFile, totalFileNumber, tmp);
				else
					printStatus(i, total, totalDigitNumber, tmp);
			}
			fout.close();
			if (!config) {
				std::wcout << "Successfully write random string to file: " << path << '!' << std::endl;
				Sleep(3000);
			}
			return true;
		}
		static bool defaultOutput(std::wstring path, std::wstring str, unsigned long long total, bool config = false, unsigned long long currentFile = 1, unsigned long long totalFile = 1) {
			std::wfstream fout;
			fout.open(path, std::ios::out);
			if (!fout.is_open()) {
				std::wcerr << "Failed to open file: " << path << '!' << std::endl
					<< "This program can only write to existing folders." << std::endl
					<< "Try to create the folder manually." << std::endl;
				Sleep(3000);
				return false;
			}
			unsigned long long totalDigitNumber = getDigitNumber(total), totalFileNumber = getDigitNumber(totalFile);
			for (unsigned long long i = 0; i < total; i++) {
				fout << str;
				if (fout.fail() || fout.bad()) {
					fout.close();
					std::wcerr << "Failed to write string: " << str << ", to file: " << path << '!' << std::endl;
					Sleep(3000);
					return false;
				}
				if (config)
					printStatus(str, i, total, totalDigitNumber, currentFile, totalFile, totalFileNumber);
				else
					printStatus(str, i, total, totalDigitNumber);
			}
			fout.close();
			if (!config) {
				std::wcout << "Successfully write string: " << str << ", to file: " << path << '!' << std::endl;
				Sleep(3000);
			}
			return true;
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
			bool succeed = true;
			std::cout << "Input config file path:\n>>> ";
			getline(std::wcin, configPath);
			normalisePath(configPath);
			readConfigFile(configPath, fileNum, total, folderPath, fileName, str);
			if (fileNum == 1)
				if (str == L"")
					randomOutput(folderPath + L'/' + fileName, total);
				else
					defaultOutput(folderPath + L'/' + fileName, str, total);
			else
				if (str == L"") {
					for (unsigned long long i = 0; i < fileNum; i++) {
						std::wstring number;
						std::wstringstream ss;
						ss << i + 1;
						ss >> number;
						if (!randomOutput(folderPath + L'/' + number + L" - " + fileName, total, true, i, fileNum)) {
							succeed = false;
							break;
						}
					}
					if (succeed) {
						std::wcout << "Successfully write random string to files: " << folderPath + L'/' + fileName << '!' << std::endl;
						Sleep(3000);
					}
				}
				else {
					for (unsigned long long i = 0; i < fileNum; i++) {
						std::wstring number;
						std::wstringstream ss;
						ss << i + 1;
						ss >> number;
						if (!defaultOutput(folderPath + L'/' + number + L" - " + fileName, str, total, true, i, fileNum)) {
							succeed = false;
							break;
						}
					}
					if (succeed) {
						std::wcout << "Successfully write string: " << str << ", to files: " << folderPath + L'/' + fileName << '!' << std::endl;
						Sleep(3000);
					}
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