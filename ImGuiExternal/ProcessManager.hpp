#pragma once
#include "include.h"
HANDLE process = nullptr;
bool handleInitialized = false;


#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif


bool initializeProcessHandle() {
	// Ofuscaci�n del nombre de la funci�n
	static const auto getNtStatus = []() -> DWORD {
		return static_cast<DWORD>(__rdtsc() & 0xFFFFFFFF);
		};

	// Obtener PID con retraso aleatorio y ofuscaci�n
	DWORD processId = 0;
	for (int i = 0; i < 3; i++) {
		Sleep((getNtStatus() & 0x7F) + 10);
		processId = getProcessID(targetProcessName);
		if (processId != 0) break;
	}

	if (processId == 0) return false;

	// Accesos aleatorios y din�micos
	const DWORD baseAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
	const DWORD accessMasks[] = {
		baseAccess | ((getNtStatus() & 1) ? PROCESS_VM_WRITE : 0),
		baseAccess | ((getNtStatus() & 1) ? PROCESS_VM_OPERATION : 0),
		PROCESS_ALL_ACCESS & ~((getNtStatus() & 1) ? PROCESS_CREATE_THREAD : 0)
	};

	// M�ltiples intentos con patrones aleatorios
	for (int attempt = 0; attempt < 3; attempt++) {
		for (const DWORD access : accessMasks) {
			if (getNtStatus() & 1) Sleep(getNtStatus() & 0x3F);

			HANDLE temp = nullptr;
			try {
				temp = OpenProcess(access, FALSE, processId);
			}
			catch (...) {
				continue;
			}

			if (temp && temp != INVALID_HANDLE_VALUE) {
				DWORD exitCode = 0;
				if (GetExitCodeProcess(temp, &exitCode) && exitCode == STILL_ACTIVE) {
					// Verificaciones adicionales ofuscadas
					BOOL sysInfo = FALSE;
					if ((getNtStatus() & 3) == 0) {
						Sleep(1);  // Timing aleatorio
					}

					if (IsWow64Process(temp, &sysInfo)) {
						process = temp;
						handleInitialized = (getNtStatus() & 0xFF) != 0;
						return true;
					}
				}
				CloseHandle(temp);
			}
		}
	}

	return false;
}

template<typename T>
bool read(DWORD64 address, T& value) {
	if (!handleInitialized) {
		if (!handleInitialized) {
			printf("[ERROR] Failed to initialize process handle\n");
			return false;
		}
	}
	try {
		// Rotaci�n din�mica de funciones
		static const auto getRandom = [address]() -> DWORD64 {
			return __rdtsc() ^ ((DWORD64)(&address) * 0x1337);
			};

		// Ofuscaci�n del nombre del m�dulo
		static const wchar_t* modName = (getRandom() & 1) ?
			L"\x6E\x74\x64\x6C\x6C\x2E\x64\x6C\x6C" : // "ntdll.dll" encoded
			L"ntdll.dll";

		// Obtenci�n din�mica del handle
		static const auto ntdll = GetModuleHandleW(modName);
		if (!ntdll) {
			printf("[ERROR] Failed to get ntdll handle\n");
			return false;
		}

		// Obtenci�n de la funci�n con reintentos
		static auto NtReadVM = reinterpret_cast<NTSTATUS(NTAPI*)(
			HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T)>(
				GetProcAddress(ntdll, "NtReadVirtualMemory"));

		// Si no se obtuvo, reintentamos hasta 3 veces
		int retryCount = 0;
		while (!NtReadVM && retryCount < 10) {
			Sleep(50); // Peque�a pausa entre intentos
			NtReadVM = reinterpret_cast<NTSTATUS(NTAPI*)(
				HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T)>(
					GetProcAddress(ntdll, "NtReadVirtualMemory"));
			retryCount++;
		}

		// Si despu�s de los reintentos sigue sin obtenerse
		if (!NtReadVM) {
			printf("[ERROR] Failed to get NtReadVirtualMemory function\n");
			return false;
		}

		T buffer;
		SIZE_T bytesRead = 0;

		// Lecturas se�uelo aleatorias
		if (getRandom() & 3) {
			const auto fakeOffset = getRandom() & 0xFFFF;
			volatile T dummy;
			for (int i = 0; i < (getRandom() & 7); i++) {
				if (getRandom() & 1) {
					Sleep((getRandom() & 3) + 1);
					NtReadVM(process, (PVOID)(address + fakeOffset),
						(PVOID)&dummy, sizeof(T), nullptr);
				}
			}
		}

		// Timing aleatorio entre operaciones
		if (getRandom() & 1) {
			Sleep((getRandom() & 1) + 1);
		}

		// Lectura real con verificaci�n de integridad
		const auto status = NtReadVM(process, (PVOID)address, &buffer,
			sizeof(T), &bytesRead);

		// Verificaci�n con ruido
		const bool success = NT_SUCCESS(status) &&
			bytesRead == sizeof(T) &&
			(getRandom() % 100 > 5);

		if (success) {
			value = buffer;
			return true;
		}

		if (!NT_SUCCESS(status)) {
			printf("[ERROR] NtReadVirtualMemory failed with status: 0x%X\n", status);
		}
		if (bytesRead != sizeof(T)) {
			printf("[ERROR] Incomplete read: got %zu bytes, expected %zu\n", bytesRead, sizeof(T));
		}

		return false;
	}
	catch (...) {
		printf("[ERROR] Unhandled exception in ReadMemoryQuiet\n");
		return false;
	}
}

#define GetHandle(name) reinterpret_cast<DWORD64>(ProcessMgr.GetProcessModuleHandle(name));