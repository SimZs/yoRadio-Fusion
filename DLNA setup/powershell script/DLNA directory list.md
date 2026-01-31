A szkript a DLNA könyvtárak ID kinyeréséhez készült.
Használat:
1. másold be a dlna-list.ps1 PowerShell szkriptet egy tetszőleges mappába
2. ebben a mappában nyiss egy PowerShell parancssort (SHIFT + jobb egér -> PowerShell ablak megnyitása itt.)
3. kapcsold ki a Policy-t ideiglenesen: 
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
a kérdésre válaszd az A opciót (Yes to All)
4. (opcionális) kapcsold át a kódlapot, hogy az ékezetek is olvashatóak legyenek:
chcp 65001
5. futtasd az alábbi szkriptet úgy, hogy az IP cím helyén a DLNA szervered IP címe legyen:
.\dlna-list.ps1 -DlnaHost your.dlnahost.ip -ObjectId 0
Az ObjectId először legyen 0, hogy lásd a DLNA gyökér szerkezetét, és a hozzájuk tartozó ID-ket
6. A listázott adatok közül válaszd ki a Zene mappa ID-jét, és ezzel is futtasd újra szkriptet
.\dlna-list.ps1 -DlnaHost your.dlnahost.ip -ObjectId yourMusicDirID
7. Egészen addig tudod ismételni a folyamatot (mindig az egyel mélyebben lévő ID-kel), amíg a valós track-ekig elérsz
Ha így végig tudod tallózni a DLNA szervered, akkor nagy valószínűséggel a yoRadio firmware-ben is működni fog.

Példa, hogy mit kell látnod:
PS C:\Users\tvara\Downloads> .\dlna-list.ps1 -DlnaHost 192.168.1.200 -ObjectId 0

DLNA container list under ObjectID=0 on 192.168.1.200
----------------------------------------
21      Zene
23      Fénykép
25      Videó
999     Video Station

PS C:\Users\tvara\Downloads> .\dlna-list.ps1 -DlnaHost 192.168.180.122 -ObjectId 21

A yoRadio myoptions.h fájlban ezután így tudod beállítani a DLNA funkciót:
#define USE_DLNA
#define dlnaHost "192.168.180.122" //cseréld le az a IP címet a saját DLNA szervered IP címére
#define dlnaIDX  21 //cseréld le az indexet a saját DLNA szervered Zene mappájának ID.jére




This script is designed to extract directory IDs from a DLNA server.
Usage:
Copy the dlna-list.ps1 PowerShell script into any folder of your choice.
Open a PowerShell command window in that folder
(SHIFT + right-click → “Open PowerShell window here”).
Temporarily disable the execution policy:
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
When prompted, choose option A (Yes to All).

Optional: only needed if your DLNA library contains non-ASCII characters such as accented or non-Latin letters 
Switch the code page so accented characters are displayed correctly:
chcp 65001

Run the script using your DLNA server’s IP address:
.\dlna-list.ps1 -DlnaHost your.dlnahost.ip -ObjectId 0

First, use ObjectId = 0 to view the DLNA root structure and the corresponding IDs.

From the listed results, select the Music folder ID and run the script again:

.\dlna-list.ps1 -DlnaHost your.dlnahost.ip -ObjectId yourMusicDirID

Repeat this process (always using the next, deeper-level ID) until you reach the actual audio tracks.
If you can successfully browse your DLNA server this way, it is very likely that it will also work correctly in the yoRadio firmware.

Example output:
PS C:\Users\<username>\Downloads> .\dlna-list.ps1 -DlnaHost 192.168.180.122 -ObjectId 0

DLNA container list under ObjectID=0 on 192.168.1.200
----------------------------------------
21      Music
23      Pictures
25      Video
999     Video Station

PS C:\Users\<username>\Downloads> .\dlna-list.ps1 -DlnaHost 192.168.1.200 -ObjectId 21

DLNA configuration in yoRadio

After that, you can configure DLNA in the yoRadio myoptions.h file as follows:

#define USE_DLNA
#define dlnaHost "192.168.180.122"   // replace with your DLNA server’s IP address
#define dlnaIDX  21                  // replace with the ID of your DLNA server’s Music folder