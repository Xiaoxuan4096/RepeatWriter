// RepeatWriter - Repeatly write a string.
// Version: 1.0.0.7
// Written by Xiaoxuan4096.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <random>

#include <Windows.h>

namespace Xiaoxuan4096 {
	namespace Public {
		unsigned long long getDigitNumber(unsigned long long x) {
			unsigned long long sum = 0;
			for (; x > 0; x /= 10, sum++);
			return sum;
		}
		char getRandomChar() {
			const char chars[96] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
			std::random_device rd;
			std::uniform_int_distribution<> uid{0, 94};
			return chars[uid(rd)];
		}
		void normalisePath(std::wstring& path) { // Delete character \"
			if (path[0] == '\"') {
				path.erase(path.begin());
				path.erase(path.end() - 1);
			}
			return;
		}

		void printStatus(std::wstring str, unsigned long long current, unsigned long long total, unsigned long long totalDigitNumber) {
			std::wcout << "String = " << str << ", Current = " << std::setw(totalDigitNumber) << current + 1 << ", Total = " << total << ", Rate = " << std::fixed << std::setprecision(2) << std::setw(6) << (100 * (double)(current + 1) / total) << "%." << std::endl;
			return;
		}
		void printStatus(unsigned long long current, unsigned long long total, unsigned long long totalDigitNumber, char str) {
			std::wcout << "String = " << str << ", Current = " << std::setw(totalDigitNumber) << current + 1 << ", Total = " << total << ", Rate = " << std::fixed << std::setprecision(2) << std::setw(6) << (100 * (double)(current + 1) / total) << "%." << std::endl;
			return;
		}

		void randomOutput(std::wstring path, unsigned long long total) {
			std::wfstream fout;
			fout.open(path, 'w');
			if (!fout.is_open()) {
				std::wcout << "Failed to open file: " << path << '!' << std::endl;
				Sleep(3000);
				return;
			}
			unsigned long long totalDigitNumber = getDigitNumber(total);
			for (unsigned long long i = 0; i < total; i++) {
				char tmp = getRandomChar();
				fout << tmp;
				if (fout.fail() || fout.bad()) {
					fout.close();
					std::wcout << "Failed to write string: " << tmp << ",to file: " << path << '!' << std::endl;
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
		void defaultOutput(std::wstring path, std::wstring str, unsigned long long total) {
			std::wfstream fout;
			fout.open(path, 'w');
			if (!fout.is_open()) {
				std::wcout << "Failed to open file: " << path << '!' << std::endl;
				Sleep(3000);
				return;
			}
			unsigned long long totalDigitNumber = getDigitNumber(total);
			for (unsigned long long i = 0; i < total; i++) {
				fout << str;
				if (fout.fail() || fout.bad()) {
					fout.close();
					std::wcout << "Failed to write string: " << str << ",to file: " << path << '!' << std::endl;
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

		void main() {
			unsigned long long total;
			std::wstring str, path;
			std::wcout << "Input total:\n>>> ";
			std::cin >> total;
			getchar(); // Avoid inputing a blank character.
			std::wcout << "Input string(leave blank for random characters):\n>>> ";
			getline(std::wcin, str);
			std::wcout << "Input path:\n>>> ";
			getline(std::wcin, path);
			normalisePath(path);
			if (str == L"")
				randomOutput(path, total);
			else
				defaultOutput(path, str, total);
			return;
		}
	}
}
int main() {
	Xiaoxuan4096::Public::main();
	return 0;
}