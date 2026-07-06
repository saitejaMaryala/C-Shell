# 🐚 C-Shell — Custom Unix Shell

> A mini-project for the **Operating Systems and Networks (OSN)** course.  
> A custom shell built from scratch in C, mimicking the core behaviour of a Unix/Linux Bash shell.

---

## 📖 What is C-Shell?

C-Shell is a lightweight, interactive command-line shell written entirely in C. It replicates several fundamental features of a standard Unix shell — including directory navigation, file listing, command history, process inspection, background/foreground process management, and recursive file searching — all implemented without relying on shell built-ins.

The shell starts in the directory where the executable is launched (treated as the **home** directory), displays a dynamic prompt showing the current user, hostname, and relative path, and supports running multiple semicolon-separated commands in a single line.

---

## ✨ Features

### 1. 🖥️ Interactive Prompt (`display`)
- Shows `<username>@<hostname>:<relative-path>~$` at all times.
- Displays the **time taken** by the previous foreground command if it exceeded **2 seconds**.
- Uses `getpwuid`, `gethostname`, and `getcwd` to dynamically build the prompt.

---

### 2. ⌨️ Input & Multi-Command Support
- Accepts multiple commands separated by **semicolons (`;`)** and executes them sequentially.
- Supports running commands in the **background** by appending `&`.
- Strips extra whitespace from input before processing.

---

### 3. 📁 `hop` — Change Directory
A custom `cd` equivalent.

| Usage | Behaviour |
|---|---|
| `hop ~` | Go to the home directory |
| `hop -` | Go to the previous directory |
| `hop <path>` | Go to the specified absolute or relative path |
| `hop <dir1> <dir2> ...` | Hop through multiple directories in sequence |

---

### 4. 📂 `reveal` — List Directory Contents
A custom `ls` equivalent with colour-coded output.

| Flag | Description |
|---|---|
| `-a` | Show hidden files (dotfiles) |
| `-l` | Show detailed info (permissions, size, owner, timestamps) |
| `-la` / `-al` | Combine both flags |

- **Blue** → directories  
- **Green** → executable files  
- **White** → regular files  
- Works on a specific file, a given path, `~`, `-` (previous dir), or the current directory by default.

---

### 5. 📜 `log` — Command History
A persistent command history stored in `history1.txt` across sessions (up to **15 entries**; duplicates are not stored consecutively).

| Command | Description |
|---|---|
| `log` | Display the last 15 commands |
| `log purge` | Clear the entire history |
| `log execute <N>` | Re-execute the N-th most recent command |

---

### 6. ⚙️ System Commands (Foreground & Background)
- **Foreground**: Commands run directly; the shell waits for them to finish.
- **Background** (`&`): Commands are forked into a new process group with stdio redirected to `/dev/null`. The PID is printed immediately, and an exit notification appears when the process finishes.
- Interactive background processes (like `vim`) terminate with exit status 1 (due to `/dev/null` redirection).

---

### 7. 🔍 `proclore` — Process Information
Displays information about a process by reading from `/proc/<pid>/`.

| Command | Description |
|---|---|
| `proclore` | Show info for the current shell process |
| `proclore <pid>` | Show info for any given PID |

**Output includes:**
- PID
- Process State (`R`, `S`, `Z`, etc.; appends `+` if it's a foreground process)
- Process Group ID
- Virtual Memory size (KB)
- Executable path (shown relative to home)

---

### 8. 🔎 `seek` — Recursive File/Directory Search
Recursively searches for a file or directory by name (prefix match) under a given path.

| Flag | Description |
|---|---|
| `-d` | Search only for directories |
| `-f` | Search only for files |
| `-e` | Execute: if exactly one result, `cd` into it (dir) or print its contents (file) |

**Usage:**
```
seek [-flags] <name> [search_directory]
```

- Directories are printed in **blue**, files in **green**.
- Uses relative paths from the search root.
- `~` resolves to home; defaults to current directory if no path given.
- Prints `NO Match found!!` if nothing is found.

---

## 📁 Project Structure

```
C-Shell/
├── README.md
└── src/
    ├── main.c           # Entry point; shell loop
    ├── Makefile         # Build configuration
    ├── display.c/h      # Prompt rendering and time display
    ├── execute_cmd.c/h  # Command dispatcher, tokenizer, fg/bg logic
    ├── exec_basic.c/h   # execvp wrapper for system commands
    ├── hop.c/h          # hop (cd) implementation
    ├── reveal.c/h       # reveal (ls) implementation
    ├── log.c/h          # Command history management
    ├── proclore.c/h     # Process information via /proc
    ├── seek.c/h         # Recursive file/dir search
    ├── prompt.c/h       # CWD and old-CWD tracking
    ├── color.c/h        # Terminal colour helpers
    ├── function.h       # Shared utility declarations
    └── headers.h        # Common system headers
```

---

## 🚀 How to Run

### Prerequisites
- A Linux system (uses `/proc` filesystem)
- `gcc` compiler
- `make`

### Build

```bash
cd src
make
```

This compiles all `.c` files and produces the `shell` executable inside `src/`.

### Run

```bash
./shell
```

Or use the Makefile shortcut:

```bash
make run
```

### Clean Build Artifacts

```bash
make clean
```

---

## 💡 Usage Examples

```bash
# Navigate directories
hop ~/Documents
hop -
hop ~ /etc /tmp

# List files
reveal
reveal -la
reveal -a ~/Documents

# Command history
log
log execute 3
log purge

# Process info
proclore
proclore 1234

# Background process
sleep 10 &
firefox &

# Recursive search
seek myfile ~/Documents
seek -f config /etc
seek -d -e logs ~/projects

# Multiple commands in one line
hop ~ ; reveal -l ; log
```

---

## ⚠️ Assumptions & Notes

- The **home directory** is set to the directory from which `./shell` is run.
- History is saved persistently in `history1.txt` (created in the shell's home directory).
- Path lengths are assumed to be at most **4096** characters.
- Flags must be space-separated (e.g., `-a -l` is valid; `-a-l` is not).
- Background processes have their `stdin`/`stdout`/`stderr` redirected to `/dev/null`; interactive programs (like `vim`) will therefore exit with status 1.
- The `-e` flag for `seek` has no effect when used without `-f` or `-d`.

---

## 📚 Course

**Operating Systems and Networks (OSN)**  
Mini Project 1 — Custom C Shell
