# DLLinjector
A specialized Windows-based penetration testing and security research tool designed to demonstrate, test, and audit DLL Hijacking vulnerabilities and DLL Injection techniques. This utility allows security practitioners to simulate post-exploitation vectors, analyze process memory behavior, and validate Endpoint Detection and Response (EDR) telemetry against native API manipulations.

    ⚠️ Disclaimer: This tool is developed strictly for authorized security auditing, educational purposes, and defensive research. Do not use this software against infrastructure without prior written consent.

🛠️ Core Capabilities & Injection Mechanisms

The tool abstracts complex Windows API interactions into structured pipelines, supporting multiple memory allocation and execution techniques:
Memory Allocation & Mapping Variations

    Standard LoadLibraryA Routing: Allocates memory within the target process space exclusively for the file path string of the payload, forcing the target process to load the DLL natively from disk.

    Reflective / Manual Entry Jumping: Allocates a full memory block mapped to the raw bytes of the target DLL, manually handling base relocations and shifting execution directly to the DLL's entry point (DllMain).

Execution Thread Hijacking Techniques

    CreateRemoteThread: The classic method utilizing the Windows subsystem to spawn a worker thread in the context of the remote process.

    NtCreateThreadEx: Bypasses basic user-mode Kernel32.dll hooks by leveraging the undocumented Native API equivalent exported by ntdll.dll.

    Suspend / Inject / Resume: Hijacks an existing thread's execution flow. Suspends the remote thread, manipulates its instruction pointer (EIP/RIP) context to redirect to the shellcode/DLL loader payload, and resumes state.

    RtlCreateUserThread: An alternative Native API implementation bypass that leverages undocumented underlying session and environment thread spawning structures.
