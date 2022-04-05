#include <iostream>
#include <string>
#include <fstream>
#include <Thread>
#include <chrono>
#include <atomic>
#include "C:\Users\jiaji\source\repos\ProgressBarLib\ProgressBarLib\ProgressBarLib.h"
progressBar encrypt;
using namespace std;

int key;
string rawPath, rawKey;
bool done(false);
atomic<long float> progress(0);
atomic<bool> ready(false);
atomic<chrono::steady_clock::time_point> startT;
chrono::steady_clock::time_point nowT;
//long float progress(0);

void fCipher(string file, int code);
void showBar();
void hideCursor();

int main(int argc, char* argv[])
{
	rawPath = { argv[argc - 1] };
	cout << "INPUT FILE PATH: " << rawPath << endl;

	cout << "KEY: ", cin >> rawKey;
	key = stoi(rawKey);
	cout << "KEY: " << key << endl;
	thread t1(fCipher, rawPath, key);
	showBar();
	t1.join();
	cout << endl << "complete!" << ends;
	Sleep(3000);
	return 0;
}

void fCipher(string file, int code) {
	ifstream fin(file, ios::binary);
	fin.seekg(0, fin.end);
	encrypt.max = fin.tellg();
	fin.seekg(0, fin.beg);

	char rawTemp;
	int intTemp;

	if (file.substr(file.find_last_of("\\") + 1, file.length()).find("encrypted_") != file.npos)
	{
		string outPath = file.substr(0, file.find_last_of("\\") + 1) + file.substr(file.find_last_of("\\") + 1, file.length()).replace(file.substr(file.find_last_of("\\") + 1, file.length()).find("encrypted_"), 10, "");
		cout << "OUTPUT FILE PATH: " << outPath << endl << endl;
		hideCursor();
		ofstream fout(outPath, ios::binary);
		startT = chrono::steady_clock::now();
		ready = true;
		while (fin.get(rawTemp))
		{
			intTemp = rawTemp;
			//cout << intTemp << endl;
			intTemp += code;
			//cout << (char)intTemp;
			fout.put((char)intTemp);
			progress = progress + 1;
		}
		fout.close();
		done = true;
	}
	else
	{
		string outPath = file.substr(0, file.find_last_of("\\") + 1) + "encrypted_" + file.substr(file.find_last_of("\\") + 1, file.length());
		cout << "OUTPUT FILE PATH: " << outPath << endl << endl;
		ofstream fout(outPath, ios::binary);
		hideCursor();
		startT = chrono::steady_clock::now();
		ready = true;
		while (fin.get(rawTemp))
		{
			intTemp = rawTemp;
			intTemp -= code;
			fout.put((char)intTemp);
			progress = progress + 1;
		}
		fout.close();
	}
	fin.close();
	done = true;
}

void showBar() {
	while (!ready)
	{

	}
	while (!done)
	{
		encrypt.update(progress);
		cout << endl;
		nowT = chrono::steady_clock::now();
		chrono::steady_clock::time_point beginT = startT;
		chrono::steady_clock::duration timeSpan = nowT - beginT;

		auto timeVal = chrono::duration_cast<chrono::seconds>(nowT - beginT).count();
		auto rate = progress / timeVal;
		auto tLeft = (encrypt.max - progress) / rate;
		int hLeft = tLeft / 3600;
		auto mLeft = ((long)tLeft % 3600) / 60;
		auto sLeft = ((long)tLeft % 3600) % 60;
		cout << "TIME REMAINING: " << hLeft << "hrs " << mLeft << "mins " << sLeft << "secs" << " | SPEED: " << setprecision(1) << fixed << rate / 1000 << "KBps" << setw(1);
		COORD cPos{ 0, 5 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
		this_thread::sleep_for(chrono::milliseconds(125));
	}
}

void hideCursor() {
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	cursor.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}