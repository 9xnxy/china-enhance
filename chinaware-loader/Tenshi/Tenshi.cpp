// Tenshi.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")

#define CURL_STATICLIB
#include "curl/curl.h"
#pragma comment (lib, "curl/libcurl_a.lib")

#define FILE_PATH "A:\\Fortnite\\Fortnite\\Engine\\Binaries\\ThirdParty\\PhysX3\\Win64\\VS2015\\nvToolsExt64_1.dll"
#define GAME_NAME "FortniteClient-Win64-Shipping.exe"
#define LOADER_NAME "chinaenhance.exe"

bool file_exists(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

DWORD get_process_id(std::string procname)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);  // clean the snapshot object
		return(FALSE);
	}

	// Now walk the snapshot of processes 
	do
	{
		std::string str(pe32.szExeFile);

		if (str == procname) // put the name of your process you want to kill
		{
			return pe32.th32ProcessID;
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return 0x0; // no process id
}

bool is_process_open(std::string process_name)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);  // clean the snapshot object
		return(FALSE);
	}

	// Now walk the snapshot of processes 
	do
	{
		std::string str(pe32.szExeFile);

		if (str == process_name) // put the name of your process you want to kill
		{
			return true;
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return false;
}

BOOL KillProcess(DWORD dwProcessId, UINT uExitCode)
{
	DWORD dwDesiredAccess = PROCESS_TERMINATE;
	BOOL  bInheritHandle = FALSE;
	HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
	if (hProcess == NULL)
		return FALSE;

	BOOL result = TerminateProcess(hProcess, uExitCode);

	CloseHandle(hProcess);

	return result;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void downloadfile() {
	CURL *curl;
	FILE *fp;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		fp = fopen("c:\\chinaware\\china.wav", "wb");
		curl_easy_setopt(curl, CURLOPT_URL, "http://chinaenhance.xyz/china.wav");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}
}

DWORD WINAPI Attach(LPVOID lpParam)
{
//	downloadfile();
//	PlaySoundA("C:\\Chinaware\\china.wav", NULL, SND_LOOP | SND_ASYNC);
	// Check if injected...
	bool injected = file_exists(FILE_PATH);
	if (injected) {
		// Wait when the loader is open.
		while (is_process_open(LOADER_NAME))
		{
			Sleep(500);
		}
		// once loader is closed, check if game is open
		while (is_process_open(GAME_NAME)) {
			// Game is open, kill the program.
			auto process_id = get_process_id(GAME_NAME);

			// Attempt to kill
			KillProcess(process_id, 1);
			// cpu
			Sleep(250);
		}

		// Once game is killed delete the cheat.
		while (file_exists(FILE_PATH))
		{
			std::remove(FILE_PATH);
			Sleep(250);
		}
	}
	return 0;
}
