#include "afxsock.h"
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "file.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]) {
    ShowCur(false);
    signal(SIGINT, signalHandler);
    int nRetCode = 0;
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
        _tprintf(_T("Fatal Error: MFC initialization failed\n"));
        nRetCode = 1;
    }
    else {
        if (AfxSocketInit() == false) {
            cout << "Khong the khoi tao Socket Library " << endl;
            return 1;
        }

        CSocket clientSocket;
        if (!clientSocket.Create()) {
            cout << "Khong the tao socket " << endl;
            return 1;
        }

        if (clientSocket.Connect(_T("127.0.0.1"), 1234) != 0) {
            cout << "Ket noi toi server thanh cong !!" << endl << endl;

            char buffer[BUFFER_SIZE];
            memset(buffer, 0, BUFFER_SIZE);
            clientSocket.Receive(buffer, BUFFER_SIZE);

            cout << "Available files from server: \n" << buffer << endl;

            unordered_set<string> downloadedFiles = loadDownloadedFiles("downloaded_files.txt");

            while (true) {
                vector<string> newFilesToDownload = getNewFilesToDownload("input.txt", downloadedFiles);

                for (const auto& filename : newFilesToDownload) {
                    downloadFile(clientSocket, filename);
                    downloadedFiles.insert(filename);

                    newFilesToDownload = getNewFilesToDownload("input.txt", downloadedFiles);
                }
         
            }
        }
        else {
            cout << "Khong the ket noi duoc voi server " << endl;
        }

        clientSocket.Close();
        cout << "Client da dong !" << endl;
    }
    return nRetCode;
}