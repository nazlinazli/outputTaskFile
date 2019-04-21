#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio>  
#include <chrono>    
#include <thread>

using namespace std;

void CalcInput(double, double);
char getOperationByValue(int);
double CalculateOperation(double, double, int);
string OpenBinaryFileGetByPath(string);

inline bool exist(const std::string&);

double totalCount = 0.0;
double lastValue = 0;
double lastOperationValue = 0, currentOperationValue = 0;

int main(int argc, char* argv[])
{
	//progress bar 
	int step = 1;
	int displayNext = step;
	int percent = 0;

	int seekCount;
	int count = 0;
	int progressBarCount = 0;

	int nList = 0;
	int nIndex = 0;
	int nFiles = 0;
	char operationSymbol;

	string taskListPath, taskIndexPath, taskFilePath, OutputFilePath;

	ifstream taskList, taskIndex, taskFile;
	ofstream ofile;

	//TaskList
	taskListPath = OpenBinaryFileGetByPath("TaskList");
	taskList.open(taskListPath, ios::in | ios::binary);

	// tasklistProgress Bar attb.
	taskList.seekg(0, taskList.end);
	int taskListLengt = taskList.tellg();
	taskList.seekg(0, taskList.beg);


	//TaskIndex
	taskIndexPath = OpenBinaryFileGetByPath("TaskIndex");
	taskIndex.open(taskIndexPath, ios::in | ios::binary);

	//TaskFile
	taskFilePath = OpenBinaryFileGetByPath("TaskFile");
	taskFile.open(taskFilePath, ios::in | ios::binary);

	//Output dosyas� 
	do {
		cout << "Enter the extract File Path : ";
		cin >> OutputFilePath;

		if (exist(OutputFilePath)) {
			if (remove(OutputFilePath.c_str()) != 0)
				cout << "Failed to delete file.";
			else
				cout << "File deleted.";
		}

		ofile.open(OutputFilePath, ios::app);
		cout << endl << "File is created on " + OutputFilePath + " Path.";

		if (!ofile) {
			cout << endl << "**************************************";
			cout << endl << "EROR !!! Failed to create file. Please enter a valid file path. ";
			cout << "**************************************" << endl;

		}
	} while (!ofile);

	cout << endl << "**************************************";
	cout << endl << "Processing " << taskListLengt << " byte..." << endl;
	cout << endl << "**************************************" << endl;

	while (taskList >> nList)
	{

		//Indexte ki okunacak byte bulunuyor. ilk d�rt byte bizim nList de�erini d�nd�rd��� i�in son 4 byte l�k de�eri okuyoruz.
		seekCount = nList * 8 + 4;

		//TaskList'te ki s�raya g�re index tablosundan ilgili sonucu al�yoruz.
		taskIndex.clear();
		taskIndex.seekg(seekCount, ios::beg);
		taskIndex.read(reinterpret_cast<char*>(&nIndex), sizeof(4));

		//Sonucun File dosyas�ndaki kar��l���n� al�yoruz.
		taskFile.clear();
		taskFile.seekg(nIndex, ios::beg);
		taskFile.read(reinterpret_cast<char*>(&nFiles), sizeof(4));

		count++;
		//Operat�rler 2. okumada al�n�yor.
		if (count % 2 == 0) {

			progressBarCount += 2;

			operationSymbol = getOperationByValue(nFiles);
			//cout << operationSymbol << ' ';
			ofile << operationSymbol << " ";
			//current Operation = nfiles
			currentOperationValue = nFiles;
		}
		else {
			progressBarCount += 4;

			//tekli okumalar her zaman 4 bytel�k say�lar�m�z
			//cout << nFiles << ' ';
			ofile << nFiles << " ";
			//ilk okuma i�in �n�nde parametre olmad��� i�in bir i�lem.
			if (totalCount == 0) {
				totalCount = nFiles;
			}
			else {
				//okunan de�eri ve i�lemi hesaplamak i�in ilgili fonksiyonumuzu �al��t�r�yoruz.
				CalcInput(currentOperationValue, nFiles);
			}
		}

		percent = (double)(100 * (progressBarCount + (taskListLengt * 0.01))) / taskListLengt;
		if (percent >= displayNext)
		{
			cout << "\r" << "[" << std::string(percent / 5, (char)254u) << std::string(100 / 5 - percent / 5, ' ') << "]";
			cout << percent << "%" << " [Progressing: " << progressBarCount << " of " << taskListLengt << "]";
			std::cout.flush();
			displayNext += step;
		}
	}


	//Sonucu ekrana ve sonu� dosyas�na yazd�r�yoruz.
	cout << endl << totalCount << ' ';
	ofile << endl << totalCount << " ";

	// a��lan dosyalar kapan�yor.
	ofile.close();
	taskList.close();
	taskIndex.close();
	taskFile.close();

	cout << endl;

	system("pause");

	return 0;
}



//Dosyay� a�mak i�in
string OpenBinaryFileGetByPath(string OpenFileName) {
	ifstream fileStream;
	string inputPath;
	//D��ar�dan pathleri s�ras� ile al�yoruz.
	do {
		cout << "Enter " + OpenFileName + " Path: ";
		cin >> inputPath;
		fileStream.open(inputPath, ios::in | ios::binary | ios::ate);
		if (!fileStream) {
			cout << endl << "**************************************";
			cout << endl << "File Failed. Please enter a valid file path.";
			cout << endl << "**************************************" << endl;

		}
		else {
			fileStream.close();
		}
	} while (!fileStream);
	return inputPath;
}

// Hesaplama yap�lacak operat�r ve say� g�nderiliyor.
void CalcInput(double operatorCount, double value)
{
	//E�er i�lemimiz �arpma ise matematiksel i�lemlerin �nceli�i i�in 
	if ((operatorCount == -3 || operatorCount == -4) && (lastOperationValue == -1 || lastOperationValue == -2))
	{
		totalCount = (double)CalculateOperation(totalCount, lastValue, lastOperationValue == -1 ? -2 : -1);
		totalCount = (double)CalculateOperation(totalCount, CalculateOperation(lastValue, value, operatorCount), lastOperationValue);
	}
	else {
		totalCount = (double)CalculateOperation(totalCount, value, operatorCount);
	}

	lastOperationValue = operatorCount;
	lastValue = value;
}


//+ -1, - -2, * -3 , /-4 olacak �ekilde value1 ile value2 yi i�leme sokan fonksiyon
double CalculateOperation(double value1, double value2, int operatorCount)
{
	double result = 0;
	if (operatorCount == -1) {
		result = value1 + value2;
	}
	else if (operatorCount == -2)
	{
		result = value1 - value2;
	}
	else if (operatorCount == -3)
	{
		result = value1 * value2;
	}
	else if (operatorCount == -4)
	{
		result = value1 / value2;
	}
	return result;
}

//ilgili tipe g�re karakter olarak sonucu d�nd�r�yor
char getOperationByValue(int value) {
	if (value == -1) {
		return '+';
	}
	else if (value == -2) {
		return '-';
	}
	else if (value == -3)
	{
		return '*';
	}
	else if (value == -4)
	{
		return '/';
	}
	return '*';
}

//dosyan�n olup olmad���n� inceliyor
inline bool exist(const std::string & name)
{
	ifstream file(name);
	if (!file)            // If the file was not found, then file is 0, i.e. !file=1 or true.
		return false;    // The file was not found.
	else                 // If the file was found, then file is non-0.
		return true;     // The file was found.
}