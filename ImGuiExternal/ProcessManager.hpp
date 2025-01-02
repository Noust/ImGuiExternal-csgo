#pragma once
#include "include.h"
#define _is_invalid_ptr(v, ret) do { if((v) == NULL) return (ret); } while(0)
#define _is_invalid_val(v, val, ret) do { if((v) == (val)) return (ret); } while(0)


enum StatusCode
{
	SUCCEED,
	FAILE_PROCESSID, 
	FAILE_HPROCESS,
	FAILE_MODULE,
};



class ProcessManager
{
private:
	bool   Attached = false;
	
	typedef HANDLE(WINAPI* pOpenProcess)(DWORD, BOOL, DWORD);
	typedef BOOL(WINAPI* pCloseHandle)(HANDLE);
	typedef BOOL(WINAPI* pReadProcessMemory)(HANDLE, LPCVOID, LPVOID, SIZE_T, SIZE_T*);
	typedef BOOL(WINAPI* pWriteProcessMemory)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
	typedef BOOL(WINAPI* pGetExitCodeProcess)(HANDLE, LPDWORD);
	typedef HANDLE(WINAPI* pCreateToolhelp32Snapshot)(DWORD, DWORD);
	typedef BOOL(WINAPI* pProcess32First)(HANDLE, LPPROCESSENTRY32W);
	typedef BOOL(WINAPI* pProcess32Next)(HANDLE, LPPROCESSENTRY32W);
	typedef BOOL(WINAPI* pModule32First)(HANDLE, LPMODULEENTRY32);
	typedef BOOL(WINAPI* pModule32Next)(HANDLE, LPMODULEENTRY32);

	pOpenProcess NtOpenProcess;
	pCloseHandle NtCloseHandle; 
	pReadProcessMemory NtReadProcessMemory;
	pWriteProcessMemory NtWriteProcessMemory;
	pGetExitCodeProcess NtGetExitCodeProcess;
	pCreateToolhelp32Snapshot NtCreateToolhelp32Snapshot;
	pProcess32First NtProcess32First;
	pProcess32Next NtProcess32Next;
	pModule32First NtModule32First;
	pModule32Next NtModule32Next;

	void InitializeFunctions() {
		HMODULE k32 = GetModuleHandleA("kernel32.dll");
		NtOpenProcess = (pOpenProcess)GetProcAddress(k32, "OpenProcess");
		NtCloseHandle = (pCloseHandle)GetProcAddress(k32, "CloseHandle");
		NtReadProcessMemory = (pReadProcessMemory)GetProcAddress(k32, "ReadProcessMemory");
		NtWriteProcessMemory = (pWriteProcessMemory)GetProcAddress(k32, "WriteProcessMemory");
		NtGetExitCodeProcess = (pGetExitCodeProcess)GetProcAddress(k32, "GetExitCodeProcess");
		NtCreateToolhelp32Snapshot = (pCreateToolhelp32Snapshot)GetProcAddress(k32, "CreateToolhelp32Snapshot");
		NtProcess32First = (pProcess32First)GetProcAddress(k32, "Process32FirstW");
		NtProcess32Next = (pProcess32Next)GetProcAddress(k32, "Process32NextW");
		NtModule32First = (pModule32First)GetProcAddress(k32, "Module32First");
		NtModule32Next = (pModule32Next)GetProcAddress(k32, "Module32Next");
	}

public:
	HANDLE hProcess = 0;
	DWORD  ProcessID = 0;
	DWORD64  ModuleAddress = 0;

public:
	ProcessManager() {
		InitializeFunctions();
	}

	~ProcessManager()
	{
		if(hProcess) NtCloseHandle(hProcess);
	}

	StatusCode Attach(std::string ProcessName)
	{
		ProcessID = this->GetProcessID(ProcessName);
		_is_invalid_val(ProcessID, 0, FAILE_PROCESSID);

		hProcess = NtOpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ProcessID);
		_is_invalid_ptr(hProcess, FAILE_HPROCESS);

		ModuleAddress = reinterpret_cast<DWORD64>(this->GetProcessModuleHandle(ProcessName));
		_is_invalid_val(ModuleAddress, 0, FAILE_MODULE);

		Attached = true;
		return SUCCEED;
	}

	void Detach()
	{
		if (hProcess)
			NtCloseHandle(hProcess);
		hProcess = 0;
		ProcessID = 0;
		ModuleAddress = 0;
		Attached = false;
	}

	bool IsActive()
	{
		if (!Attached)
			return false;
		DWORD ExitCode{};
		NtGetExitCodeProcess(hProcess, &ExitCode);
		return ExitCode == STILL_ACTIVE;
	}

	template <typename ReadType>
	bool ReadMemory(DWORD64 Address, ReadType& Value, int Size)
	{
		_is_invalid_ptr(hProcess, false);
		_is_invalid_val(ProcessID, 0, false);
		return NtReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address), &Value, Size, 0);
	}

	template <typename ReadType>
	bool ReadMemory(DWORD64 Address, ReadType& Value)
	{
		_is_invalid_ptr(hProcess, false);
		_is_invalid_val(ProcessID, 0, false);
		return NtReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Address), &Value, sizeof(ReadType), 0);
	}

	template <typename ReadType>
	bool WriteMemory(DWORD64 Address, ReadType& Value, int Size)
	{
		_is_invalid_ptr(hProcess, false);
		_is_invalid_val(ProcessID, 0, false);
		return NtWriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(Address), &Value, Size, 0);
	}

	template <typename ReadType>
	bool WriteMemory(DWORD64 Address, ReadType& Value)
	{
		_is_invalid_ptr(hProcess, false);
		_is_invalid_val(ProcessID, 0, false);
		return NtWriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(Address), &Value, sizeof(ReadType), 0);
	}

	std::vector<DWORD64> SearchMemory(const std::string& Signature, DWORD64 StartAddress, DWORD64 EndAddress, int SearchNum = 1);

	DWORD64 TraceAddress(DWORD64 BaseAddress, std::vector<DWORD> Offsets)
	{
		_is_invalid_ptr(hProcess, 0);
		_is_invalid_val(ProcessID, 0, 0);
		DWORD64 Address = 0;

		if (Offsets.empty())
			return BaseAddress;

		if (!ReadMemory<DWORD64>(BaseAddress, Address))
			return 0;

		for (size_t i = 0; i < Offsets.size() - 1; i++)
		{
			if (!ReadMemory<DWORD64>(Address + Offsets[i], Address))
				return 0;
		}
		return Address == 0 ? 0 : Address + Offsets[Offsets.size() - 1];
	}

public:
	DWORD GetProcessID(std::string ProcessName)
	{
		PROCESSENTRY32W ProcessInfoPE;
		ProcessInfoPE.dwSize = sizeof(PROCESSENTRY32W);
		HANDLE hSnapshot = NtCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		NtProcess32First(hSnapshot, &ProcessInfoPE);
		do {
			if (wcscmp(ProcessInfoPE.szExeFile, std::wstring(ProcessName.begin(), ProcessName.end()).c_str()) == 0)
			{
				NtCloseHandle(hSnapshot);
				return ProcessInfoPE.th32ProcessID;
			}
		} while (NtProcess32Next(hSnapshot, &ProcessInfoPE));
		NtCloseHandle(hSnapshot);
		return 0;
	}

	HMODULE GetProcessModuleHandle(std::string ModuleName)
	{
		MODULEENTRY32 ModuleInfoPE;
		ModuleInfoPE.dwSize = sizeof(MODULEENTRY32);
		HANDLE hSnapshot = NtCreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->ProcessID);
		
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return 0;

		std::string moduleName = ModuleName;
		
		if (NtModule32First(hSnapshot, &ModuleInfoPE))
		{
			do {
				if (_stricmp(ModuleInfoPE.szModule, moduleName.c_str()) == 0)
				{
					NtCloseHandle(hSnapshot);
					return ModuleInfoPE.hModule;
				}
			} while (NtModule32Next(hSnapshot, &ModuleInfoPE));
		}
		
		NtCloseHandle(hSnapshot);
		return 0;
	}
};

inline ProcessManager ProcessMgr;

template <typename T>
inline bool read(const DWORD64& Address, DWORD Offset, T& Data)
{
	return (Address != 0) && ProcessMgr.ReadMemory<T>(Address + Offset, Data);
}

template <typename T>
inline bool write(const DWORD64& Address, DWORD Offset, T& Data)
{
	return (Address != 0) && ProcessMgr.WriteMemory<T>(Address + Offset, Data);
}

#define GetHandle(name) reinterpret_cast<DWORD64>(ProcessMgr.GetProcessModuleHandle(name));