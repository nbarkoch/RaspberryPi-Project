#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string>
#include <stdio.h>



using std::string;

#pragma comment(lib,"ws2_32.lib")

class HttpCreateAlert
{
private:
	const int PORT = 80;
	HINSTANCE hInst;
	WSADATA wsaData;

	void mParseUrl(string mUrl, string& serverName, string& filepath, string& filename);
	SOCKET connectToServer(char* szServerName, WORD portNum);
	int getHeaderLength(char* content);
	char* readUrl2(string szUrl, long& bytesReturnedOut, char** headerOut);
public:
	int run(std::string baseURL, 
		std::string wrapper, 
		std::string stationID, 
		std::string cameraID, 
		std::string fileID);
	void operator()(std::string baseURL,
		std::string wrapper,
		std::string stationID,
		std::string cameraID,
		std::string fileID) {
		run(baseURL, wrapper, stationID, cameraID, fileID);
	}
};

