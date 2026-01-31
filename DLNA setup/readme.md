This build adds **DLNA playlist browsing and playback** support to yoRadio,

tested primarily with **Synology NAS**.

## 1. Build Configuration (`myoptions.h`)

Enable DLNA support and configure your DLNA server:

#define USE_DLNA

#define dlnaHost "192.168.180.122"   // DLNA server IP address

#define dlnaIDX  21                 // Root MUSIC container ID (Synology default is usually 21)
<img width="796" height="128" alt="2026-01-31_19-50-16" src="https://github.com/user-attachments/assets/24db8029-913e-41ec-bd78-a5b1f9f67a81" />


## 2. DLNA Browser Overview

The DLNA browser works in two levels:

📁 Category List (Containers)

Displays DLNA folders / categories (e.g. Artist, Album, Genre)

<img width="1205" height="589" alt="2026-01-31_20-16-44" src="https://github.com/user-attachments/assets/c3359638-2a13-4de7-9a93-b1890d5baee4" />

Selecting a category loads its contents

Containers without playable tracks are handled gracefully

🎵 Item List (Tracks)

Displays playable audio items inside the selected container

<img width="1268" height="698" alt="2026-01-31_20-18-19" src="https://github.com/user-attachments/assets/a338e6dd-8fa6-447d-a336-ec901c0d9f0e" />

#### Playlist Build Rules

- **Build** and **Append** actions are available **only if the current container actually contains tracks**
  
- Containers with **subfolders only** (no direct tracks) cannot be built or appended
  
- This prevents creating empty or invalid playlists

#### Pagination & Large Containers

- The item list is loaded in **pages of 200 entries**
  
- If a container contains more than 200 tracks or sub-containers:
  
  - A **Load more** option appears at the bottom of the list
    
  - Loading the next page **appends** new items to the existing list
    
  - Previously loaded items remain visible and selectable
    
<img width="1232" height="613" alt="2026-01-31_20-24-52" src="https://github.com/user-attachments/assets/29a5b873-6e36-4010-9df4-422e374dcbc2" />

## 3. Building a DLNA Playlist

Build DLNA Playlist

Builds a new playlist from the selected container

The playlist is stored internally and replaces the previous DLNA playlist

After build, the playlist index is reset to track 1

Append DLNA Playlist

Appends items from the selected container to the existing DLNA playlist

Current playback position is preserved

Build operations run asynchronously and do not freeze the UI.

## 4. Playlist Source Selection

Two explicit buttons control the active playlist source:

▶️ Use DLNA Playlist

Activates the DLNA playlist as the current source

Playback resumes automatically only if audio was playing before


▶️ Use WEB Playlist

Switches back to the regular WEB playlist

WEB playlist index and state are preserved independently from DLNA

👉 DLNA and WEB playlists are fully independent:
switching sources does not reset the other playlist.

## 5. Current Status

✅ Stable playback switching (WEB ↔ DLNA)

✅ Independent playlist indices

✅ Safe asynchronous build / append

⚠️ Considered experimental – feedback and testing welcome
