## 📂 DLNA Container ID Discovery Script

This PowerShell script helps you **discover directory (container) IDs** on a DLNA server.  
These IDs are required to configure DLNA browsing correctly in **yoRadio**.

The script has been tested primarily with **Synology NAS**, but should work with most DLNA servers.

---

## 🛠 Prerequisites

- Windows with PowerShell
- Network access to your DLNA server
- DLNA service enabled on the server

---

## ▶️ Usage (Step by Step)

### 1. Copy the script
Copy the `dlna-list.ps1` PowerShell script into any folder of your choice.

---

### 2. Open PowerShell in that folder
Open a PowerShell window in the script’s folder:

> **SHIFT + Right click** → *Open PowerShell window here*

---

### 3. Temporarily disable execution policy
Allow the script to run for the current session only:

Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass

### 4. (Optional) Enable UTF-8 output

If your DLNA library contains accented or non-ASCII characters, switch the code page:

chcp 65001

### 5. List the DLNA root structure

Run the script using your DLNA server’s IP address and start from the root (ObjectId = 0):

.\dlna-list.ps1 -DlnaHost your.dlnahost.ip -ObjectId 0

### 6. Locate the Music folder

From the output, find the Music (or equivalent) container and note its ID.

Run the script again using that ID:

.\dlna-list.ps1 -DlnaHost your.dlnahost.ip -ObjectId yourMusicDirID

### 7. Browse deeper (optional)

Repeat the process with deeper container IDs until you reach the actual audio tracks.

If you can browse your DLNA server successfully using this script,
yoRadio DLNA support will most likely work as well.

📄 Example Output

PS C:\Users\<username>\Downloads> .\dlna-list.ps1 -DlnaHost 192.168.180.122 -ObjectId 0

DLNA container list under ObjectID=0 on 192.168.180.122

----------------------------------------

21      Music

23      Pictures

25      Video

999     Video Station



