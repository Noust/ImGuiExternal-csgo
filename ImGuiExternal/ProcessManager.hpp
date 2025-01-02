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
    if (!process) {
        printf("[ERROR] Process handle is invalid\n");
        return false;
    }
    if (!handleInitialized) {
        printf("[ERROR] Process handle not initialized\n");
        return false;
    }
    if (address == 0) {
        printf("[ERROR] Invalid memory address (0x0)\n");
        return false;
    }

    __try {
        SIZE_T bytesRead = 0;
        if constexpr (std::is_array_v<T>) {
            // Para arrays, leemos directamente en el buffer proporcionado
            if (!ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), &value, sizeof(T), &bytesRead)) {
                printf("[ERROR] ReadProcessMemory failed for array at address 0x%llx (Error: %lu)\n", 
                       address, GetLastError());
                return false;
            }
        } else {
            // Para tipos normales, usamos un buffer temporal
            T buffer = T();
            if (!ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead)) {
                DWORD error = GetLastError();
                printf("[ERROR] ReadProcessMemory failed at address 0x%llx (Error: %lu - %s)\n", 
                       address, error, (error == 299 ? "Partial Copy" : "Unknown Error"));
                return false;
            }
            value = buffer;
        }

        if (bytesRead != sizeof(T)) {
            printf("[ERROR] Incomplete read at address 0x%llx (Read %zu bytes of %zu)\n", 
                   address, bytesRead, sizeof(T));
            return false;
        }

        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        printf("[ERROR] Exception occurred while reading memory at address 0x%llx\n", address);
        return false;
    }
}

// Sobrecarga específica para arrays multidimensionales usando std::array
template<typename T, size_t Rows, size_t Cols>
bool read(DWORD64 address, std::array<std::array<T, Cols>, Rows>& value) {
    if (!process) {
        printf("[ERROR] Process handle is invalid\n");
        return false;
    }
    if (!handleInitialized) {
        printf("[ERROR] Process handle not initialized\n");
        return false;
    }
    if (address == 0) {
        printf("[ERROR] Invalid memory address (0x0)\n");
        return false;
    }

    __try {
        SIZE_T bytesRead = 0;
        const size_t totalSize = sizeof(T) * Rows * Cols;
        
        if (!ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), 
                             value.data(), totalSize, &bytesRead)) {
            printf("[ERROR] ReadProcessMemory failed for array[%zu][%zu] at address 0x%llx (Error: %lu)\n", 
                   Rows, Cols, address, GetLastError());
            return false;
        }
        
        if (bytesRead != totalSize) {
            printf("[ERROR] Incomplete read for array[%zu][%zu] at address 0x%llx (Read %zu bytes of %zu)\n", 
                   Rows, Cols, address, bytesRead, totalSize);
            return false;
        }
        
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        printf("[ERROR] Exception occurred while reading array[%zu][%zu] at address 0x%llx\n", 
               Rows, Cols, address);
        return false;
    }
}

HMODULE GetProcessModuleHandle(std::string ModuleName)
{
	MODULEENTRY32 ModuleInfoPE;
	ModuleInfoPE.dwSize = sizeof(MODULEENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, getProcessID(targetProcessName));
	Module32First(hSnapshot, &ModuleInfoPE);
	do {
		if (strcmp(ModuleInfoPE.szModule, ModuleName.c_str()) == 0)
		{
			CloseHandle(hSnapshot);
			return ModuleInfoPE.hModule;
		}
	} while (Module32Next(hSnapshot, &ModuleInfoPE));
	CloseHandle(hSnapshot);
	return 0;
}

#define GetHandle(name) reinterpret_cast<DWORD64>(GetProcessModuleHandle(name));