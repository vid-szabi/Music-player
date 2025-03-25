# Music player application using custom data type and SDL2 libraries

## Overview
This C++ music player application reads a playlist from a text file and stores it in a doubly linked list. The application provides various options for managing and playing music tracks.

## Features
- Load a playlist from `playlist.txt`
- Play music using SDL2 and SDL2-mixer
- Navigate through tracks (Next, Previous, Select specific track)
- Insert and delete tracks dynamically
- Shuffle and sort the playlist
- Exit the application cleanly

## Dependencies
The application uses the following external libraries:
- **SDL2 (Simple DirectMedia Layer, 2.30.1)**
- **SDL2-mixer (2.8.0)**

These libraries are included in the project.

## Setup & Configuration
### 1. Add SDL2 Libraries
Place the SDL2 and SDL2-mixer directories in your project folder.

### 2. Configure in Visual Studio (Community 2022)
#### Include and Library Directories
1. **Properties -> VC++ Directories -> Include Directories -> Edit -> New Line -> Browse**  
   - Navigate to `<project_directory>/SDL2-2.30.1/include`
   - Navigate to `<project_directory>/SDL2_mixer-2.8.0/include`

2. **Library Directories**  
   - Navigate to `<project_directory>/SDL2-2.30.1/lib/x64`
   - Navigate to `<project_directory>/SDL2_mixer-2.8.0/lib/x64`

#### Linker Settings
1. **Properties -> Linker -> Input -> Additional Dependencies -> Edit**  
   Add the following:
   ```
   SDL2.lib
   SDL2main.lib
   SDL2_mixer.lib
   ```

### 3. Set Environment Variables (Windows 11)
If you encounter `SDL2.dll/SDL2_mixer.dll was not found` errors, add the following paths to the system environment variables:
1. **Settings -> System -> Advanced system settings -> Environment Variables -> System variables -> Path -> Edit -> Browse**
   - Add `<project_directory>/SDL2-2.30.1/lib/x64`
   - Add `<project_directory>/SDL2_mixer-2.8.0/lib/x64`

2. Restart Visual Studio to apply changes.

## Input Format (playlist.txt)
The playlist file should follow this format:
```
<song_name>|<artist_name>|<file_path>
```
- `<song_name>` and `<artist_name>` can contain accented characters.
- `<file_path>` should only contain ASCII characters.
- The `|` character is used as a separator, allowing multi-word names.

## Menu & Options
Upon running the application, the following menu is displayed:
```
******************
** Music player **
******************
1. Show playlist
2. Play
3. Next
4. Previous
5. Select other
6. Insert
7. Delete
8. Shuffle
9. Sort playlist
10. Exit
Select an option:
```
### Functionality:
- **1. Show playlist** → Displays the current playlist.
- **2. Play** → Plays the current song. If the file path is invalid, an error message appears.
- **3. Next** → Moves to the next track.
- **4. Previous** → Moves to the previous track.
- **5. Select other** → Prompts for an index and jumps to the selected track.
- **6. Insert** → Inserts a new song at a specified index.
- **7. Delete** → Removes a song at a specified index.
- **8. Shuffle** → Randomizes the order of the playlist.
- **9. Sort playlist** → Sorts the playlist by file path (ascending or descending).
- **10. Exit** → Closes the application.

## How to Run
1. Ensure the required libraries are set up.
2. Place `playlist.txt` in the project directory.
3. Run the executable.
4. Follow on-screen instructions.

## Implementation
The application is implemented using a doubly linked list written by me. The specification for the custom data type can be found in the pdf file.
