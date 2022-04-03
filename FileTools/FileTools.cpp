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
//long float progress(0);

void fCipher(string file, int code);
void showBar();

int main(int argc, char* argv[])
{
	rawPath = { argv[argc - 1] };
	cout << "INPUT FILE PATH: " << rawPath << endl;

	cout << "KEY: ", cin >> rawKey;
	key = stoi(rawKey);
	//cout << "KEY: " << key << endl;
	thread t1(fCipher, rawPath, key);
	showBar();
	t1.join();
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
		cout << outPath << endl;
		ofstream fout(outPath, ios::binary);
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
		cout << outPath << endl;
		ofstream fout(outPath, ios::binary);
		ready = true;
		while (fin.get(rawTemp))
		{
			intTemp = rawTemp;
			//cout << intTemp << endl;
			intTemp -= code;
			//cout << (char)intTemp;
			fout.put((char)intTemp);
			progress = progress + 1;
		}
		fout.close();
	}
	fin.close();
	//system("pause");
	done = true;
}

void showBar() {
	while (!ready)
	{

	}
	while (!done)
	{
		encrypt.update(progress);
		this_thread::sleep_for(chrono::milliseconds(250));
	}
}