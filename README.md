# DLLinjector
A specialized Windows-based penetration testing and security research tool designed to demonstrate, test, and audit DLL Hijacking vulnerabilities and DLL Injection techniques. This utility allows security practitioners to simulate post-exploitation vectors, analyze process memory behavior, and validate Endpoint Detection and Response (EDR) telemetry against native API manipulations.

**⚠️ Disclaimer:** This tool is developed strictly for authorized security auditing, educational purposes, and defensive research. Do not use this software against infrastructure without prior written consent.

<img width="1920" height="1080" alt="edited" src="https://github.com/user-attachments/assets/9a280c81-09d1-45cf-a413-f99273d34962" />


## 🛠️ Core Capabilities & Injection Mechanisms

The tool abstracts complex Windows API interactions into structured pipelines, supporting multiple memory allocation and execution techniques:
Memory Allocation & Mapping Variations

1. Standard LoadLibraryA Routing: Allocates memory within the target process space exclusively for the file path string of the payload, forcing the target process to load the DLL natively from disk.
2. Reflective / Manual Entry Jumping: Allocates a full memory block mapped to the raw bytes of the target DLL, manually handling base relocations and shifting execution directly to the DLL's entry point (DllMain).


## Execution Thread Hijacking Techniques

* `CreateRemoteThread`: The classic method utilizing the Windows subsystem to spawn a worker thread in the context of the remote process.
* `NtCreateThreadEx`: Bypasses basic user-mode `Kernel32.dll` hooks by leveraging the undocumented Native API equivalent exported by `ntdll.dll`.
* Suspend / Inject / Resume: Hijacks an existing thread's execution flow. Suspends the remote thread, manipulates its instruction pointer (`EIP`/`RIP`) context to redirect to the shellcode/DLL loader payload, and resumes state.
* `RtlCreateUserThread`: An alternative Native API implementation bypass that leverages undocumented underlying session and environment thread spawning structures.

## 🏗️ Architecture & Flow

The overall execution pipeline follows a low-level process manipulation sequence:
1. Target Identification: Obtains a handle to the target process using `OpenProcess` with requisite access rights (`PROCESS_VM_WRITE` | `PROCESS_VM_OPERATION` | `PROCESS_CREATE_THREAD`).
2. Virtual Space Allocation: Reserves page regions via `VirtualAllocEx` inside the remote process container using appropriate memory protections (`PAGE_READWRITE` or `PAGE_EXECUTE_READWRITE`).
3. Payload Streaming: Commits the configuration string or raw binaries across boundaries using `WriteProcessMemory`.
4. Execution Disruption: Dispatches a thread or mutates contexts to invoke execution primitives natively inside the host target.


## 🚀 Getting Started

**Prerequisites:**

1. Operating System: Windows 10 / 11 or Windows Server (x64 / x86 native architectures).
2. Compiler: Microsoft Visual Studio (MSVC) supporting C++11 or higher.

    
**Compilation:**

build the .cpp file in vscode or in terminal

**Usage Syntax:**

    1. Open any program Let's say mspaint so open it 
    2. injector.exe <PID> <MAL DLL>
    3. Or can get the PID easily and impliment it. [ .\injector.exe (Get-process | ?{$_.ProcessName -eq "mspaint"} |Select-Object -ExpandProperty Id) <MAL DLL> ]


## ⚖️ Ethical Guidelines

**By using this tool, you agree to the following:**

1. You will not use this tool for malicious purposes.
2. You are responsible for obtaining all necessary permissions before interacting with any process.
3. The author is not responsible for any damage caused by the use of this software.

## 📝 License

Distributed under the MIT License. See LICENSE for more information.
