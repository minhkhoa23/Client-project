#include "file.h"

unordered_set <string> loadDownloadedFiles(const char* filename) {
	ifstream f;
	f.open(filename);
	unordered_set<string> downloadfiles;
	string line;
	while (getline(f, line)) {
		downloadfiles.insert(line);
	}
	f.close();
	return downloadfiles;
}

void saveDownloadedFile(const char* fileName, const string& downloadedFile) {
	ofstream file(fileName, ios::app);
	if (file.is_open()) {
		file << downloadedFile << endl;
		file.close();
	}
	else {
		cerr << "Error opening file: " << fileName << endl;
	}
}

vector <string> getNewFilesToDownload(const char* inputFileName, const unordered_set <string>& downloadedFiles) {
	ifstream f(inputFileName);
	vector<string> newFiles;
	string line;
	while (getline(f, line)) {
		if (downloadedFiles.find(line) == downloadedFiles.end()) {
			newFiles.push_back(line);
		}
	}
	f.close();
	return newFiles;
}

void downloadFile(CSocket& serverSocket, const string& filename) {
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strcpy_s(buffer, filename.c_str());
	serverSocket.Send(buffer, filename.size());

	ofstream outFile("output/" + filename, ios::binary);
	if (!outFile.is_open()) {
		cout << "Khong the tao file khi download" << endl;
		return;
	}

	int bytesReceived;
	int totalBytesReceived = 0;

	while ((bytesReceived = serverSocket.Receive(buffer, BUFFER_SIZE)) > 0) {
		if (string(buffer, bytesReceived) == "ERROR") {
			cerr << "Error: File not found on server" << endl;
			outFile.close();
			remove(("output/" + filename).c_str());
			return;
		}
		outFile.write(buffer, bytesReceived);
		totalBytesReceived += bytesReceived;

		cout << "\rDownloading " << filename << " .... " << totalBytesReceived << " bytes";
	}
	cout << endl;

	outFile.close();
	cout << "File downloaded successfully: " << filename << endl;
	saveDownloadedFile("downloaded_files.txt", filename);
}

void signalHandler(int signum) {
	cout << "Interrupt signal (" << signum << ") received.\n";
	exit(signum);
}

//an tro chuot
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}