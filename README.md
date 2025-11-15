## Contribution: Windows Fake-Unix Environment Fix (MSYS2 + MinGW64)

This project includes additional work contributed by **Afrizal Syahreza** as part of the **Archana Berry Project**, focusing on improving the build experience on Windows by fixing issues caused by “fake Unix environment” layers such as **MSYS2**, **MinGW64**, and indirect POSIX shims.

Windows builds of `ccminer` often fail or behave inconsistently when Unix-style layers emulate headers, paths, or behaviors that differ from true Linux/glibc environments.
To address this, the following improvements were added:

---

## ✔ Fixes & Improvements Added

### **1. Resolved MSYS2 Path Translation Issues**

MSYS2 incorrectly rewrites Windows paths (`C:\…`) into POSIX paths (`/c/...`) during `configure` and `make`.
The fix includes:

* Enforcing native Windows path mode when compiling
* Preventing MSYS2 from auto-converting build paths
* Ensuring MinGW-generated binaries remain fully Windows-native

This avoids linker failures, missing include errors, and binary runtime crashes.

---

### ✔ **2. Clean Separation Between Real Windows Headers and MSYS2 Headers**

MSYS2 ships POSIX-mimicking headers (`unistd.h`, `sys/time.h`, etc.) which conflict with ccminer’s own compatibility headers under `/compat/`.

Fix includes:

* Forcing ccminer to use **its own built-in compatibility headers**
* Blocking MSYS2 from injecting POSIX API stubs
* Preventing symbol redefinition between MSYS2 and ccminer’s internal compat layer

Now ccminer builds consistently regardless of MSYS2 installed packages.

---

### ✔ **3. MinGW64 Toolchain Stabilization**

Several flags and toolchain behaviors differ from Linux GCC.
Fixes include:

* Mapping broken pthread stubs to working Windows pthreads
* Ensuring OpenSSL / libcurl prebuilt `.lib` files link correctly
* Disabling Unix-only compiler flags that MinGW64 rejects

The Windows build is now reproducible and stable.

---

### ✔ **4. Added Windows-Friendly Build Scripts**

New helper scripts were added for the Archana Berry ecosystem:

* `build-windows-msys2.sh` — fully automated MSYS2/MinGW64 compilation
* Windows PowerShell helper
* Desktop shortcut generator for mining profiles

This improves onboarding for Windows users without needing Visual Studio.

---

## ✔ Part of the Archana Berry Project

These contributions are part of the broader **Archana Berry Project**, which focuses on making GPU-accelerated open-source tools easier to build and use across heterogeneous environments:

* Windows fake-Unix layers (MSYS2, MinGW)
* Native Linux distros
* Portable C/C++ build tooling
* Integration with mining frameworks and research code

The goal is to unify builds across all platforms and eliminate Windows POSIX-compatibility inconsistencies.

---

# 🔧 How to Build ccminer on Windows (MSYS2 + MinGW64)

### **1. Install MSYS2 (64-bit)**

Download from:
[https://www.msys2.org/](https://www.msys2.org/)

### **2. Install required toolchains**

```
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain git make autoconf automake libtool
```

### **3. Switch to MinGW64 shell**

Use:

```
C:\msys64\mingw64.exe
```

(do **not** use MSYS2.exe — it breaks Windows paths)

### **4. Clone ccminer**

```
git clone https://github.com/tpruvot/ccminer.git
cd ccminer
```

### **5. Build**

```
./autogen.sh
./configure --host=x86_64-w64-mingw32
make -j8
```

---

# 🖥 Windows Mining Shortcuts (Included)

Contributors added ready-to-use Windows shortcuts and Bash scripts for:

* Worker `archanaberry0`
* Worker `archanaberry1`
* Manual Difficulty Mining

Example shortcut target:

```
C:\Windows\System32\cmd.exe /k "cd /d C:\ccminer && ccminer.exe -a verus -o stratum+tcp://ap.luckpool.net:3956 -u RVkh54G...archanaberry0 -p x -t 8"
```

Scripts are included under `/scripts/windows`.

---

# 📦 Notes on Dependencies (Updated)

This build relies on:

* Prebuilt OpenSSL (Windows `.lib`)
* Prebuilt cURL (Windows `.lib`)
* pthreads-win32
* No MSYS2 Unix headers required
* No POSIX emulation layer required

Windows build is now **clean**, **native**, and **UNIX-shim-free**.

---

# 📣 Credits

* Original ccminer developers
* CUDA contributors (djm34, alexis78, others)
* Windows/MSYS2 improvements by **Archana Berry Project**