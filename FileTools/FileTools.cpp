#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int key;
string rawPath, rawKey, path, secPath;

void fCipher(string file, int code);

int main(int argc, char* argv[])
{
	rawPath = { argv[argc - 1] };
	cout << "RAW FILE PATH: " << rawPath << endl;

	cout << "KEY: ", cin >> rawKey;
	key = stoi(rawKey);
	cout << "KEY: " << key << endl;
	fCipher(rawPath, key);
	return 0;
}

void fCipher(string file, int code) {
	ifstream fin(file, ios::binary);
	char rawTemp;
	int intTemp;

	if (file.substr(file.find_last_of("\\") + 1, file.length()).find("encrypted_") != file.npos)
	{
		string outPath = file.substr(0, file.find_last_of("\\") + 1) + file.substr(file.find_last_of("\\") + 1, file.length()).replace(file.substr(file.find_last_of("\\") + 1, file.length()).find("encrypted_"), 10, "");
		cout << outPath << endl;
		system("pause");
		ofstream fout(outPath, ios::binary);
		while (fin.get(rawTemp))
		{
			intTemp = rawTemp;
			cout << intTemp << endl;
			intTemp += code;
			cout << (char)intTemp;
			fout.put((char)intTemp);
		}
		fout.close();

	}
	else
	{
		string outPath = file.substr(0, file.find_last_of("\\") + 1) + "encrypted_" + file.substr(file.find_last_of("\\") + 1, file.length());
		cout << outPath << endl;
		system("pause");
		ofstream fout(outPath, ios::binary);
		while (fin.get(rawTemp))
		{
			intTemp = rawTemp;
			cout << intTemp << endl;
			intTemp -= code;
			cout << (char)intTemp;
			fout.put((char)intTemp);
		}
		fout.close();

	}

	fin.close();
	system("pause");
}