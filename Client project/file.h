#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <signal.h>
#include <string>
#include <cstring>
#include <afxsock.h>
#include <unordered_set>

#define BUFFER_SIZE 1024

using namespace std;
unordered_set <string> loadDownloadedFiles(const char* filename);
void saveDownloadedFile(const char* fileName, const string& downloadedFile);
vector <string> getNewFilesToDownload(const char* inputFileName, const unordered_set <string>& downloadedFiles);
void downloadFile(CSocket& serverSocket, const string& filename);
void signalHandler(int signum);
void ShowCur(bool CursorVisibility);