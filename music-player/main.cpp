#include <iostream>
#include "DBLLinkedList.h" //Own custom datatype

#include <locale> //For printing hungarian accents correctly
#include <fstream>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

void InitializeSDL()
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        exit(1);
    }
}

void CloseSDL()
{
    Mix_CloseAudio();
    SDL_Quit();
}

void PrintMenu()
{
    cout << "******************" << endl;
    cout << "** Music player **" << endl;
    cout << "******************" << endl;
    cout << "1. Show playlist" << endl;
    cout << "2. Play" << endl;
    cout << "3. Next" << endl;
    cout << "4. Previous" << endl;
    cout << "5. Select other" << endl;
    cout << "6. Insert" << endl;
    cout << "7. Delete" << endl;
    cout << "8. Shuffle" << endl;
    cout << "9. Sort playlist" << endl;
    cout << "10. Exit" << endl;
    cout << "Select an option: " << endl;
}

void ClearScreen()
{
    cout << "\033[2J\033[H";
}

void PlayMusic(DBLLinkedList::Node::Song song)
{
    //Create a window (just for event handling)
    SDL_Window* window = SDL_CreateWindow(("Music player ~ " + song.path).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 425, 100, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        cerr << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        cerr << "Renderer creation failed. SDL Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //RGBA for black
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    //Load music
    Mix_Music* music = Mix_LoadMUS(song.path.c_str());
    if (music == nullptr)
    {
        cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << endl;
        exit(1);
    }

    //Play the music
    if (Mix_PlayMusic(music, 1) == -1)
    {
        cerr << "Failed to play music! SDL_mixer Error: " << Mix_GetError() << endl;
        exit(1);
    }

    //Event loop
    SDL_Event event;
    bool stopMusic = false;

    while (!stopMusic)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            //If the SDL window is closed
            if (event.type == SDL_QUIT)
            {
                stopMusic = true;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) //Or escape is hit
            {
                stopMusic = true;
            }
        }

        //Check if music has ended
        if (!Mix_PlayingMusic())
        {
            stopMusic = true;
        }

        //Add a small delay to reduce CPU usage
        SDL_Delay(50);
    }

    //Stop the music
    Mix_HaltMusic();

    //Free resources
    Mix_FreeMusic(music);
    SDL_DestroyWindow(window);
}

bool IsNumberInRange(int number, int upperBound)
{
    if (number < 1 || number > upperBound)
    {
        cerr << "Invalid number..." << endl;
        return false;
    }

    return true;
}

void SelectNext(DBLLinkedList::Node*& currentSong)
{
    if (currentSong->next)
    {
        currentSong = currentSong->next;
    }
    else
    {
        cerr << "There is no next song..." << endl;
    }
}

void SelectPrevious(DBLLinkedList::Node*& currentSong)
{
    if (currentSong->previous)
    {
        currentSong = currentSong->previous;
    }
    else
    {
        cerr << "There is no previous song..." << endl;
    }
}

void SelectOther(DBLLinkedList& playlist, DBLLinkedList::Node*& currentSong)
{
    cout << "Type the number of the song you want to select: ";
    int number;
    cin >> number;
    if (IsNumberInRange(number, playlist.GetSize()))
    {
        currentSong = playlist.GetNode(number - 1);
    }
}

void InsertSong(DBLLinkedList& playlist, DBLLinkedList::Node*& currentSong)
{
    cout << "Type the index where you want to insert the new song: ";
    int number;
    cin >> number;
    cin.get(); //Read into buffer

    if (playlist.IsEmpty() || IsNumberInRange(number, playlist.GetSize()))
    {
        DBLLinkedList::Node::Song songToInsert;

        cout << "Title: ";
        getline(wcin, songToInsert.title);
        cout << "Artist: ";
        getline(wcin, songToInsert.artist);
        cout << "File name: ";
        getline(cin, songToInsert.path);

        if (!playlist.IsEmpty())
        {
            playlist.InsertValue(number - 1, songToInsert); //Indexing by 1
        }
        else
        {
            playlist.InsertValue(0, songToInsert); //Inserting to the first place
            currentSong = playlist.GetNode(0);
        }
    }
}

void DeleteSong(DBLLinkedList& playlist, DBLLinkedList::Node*& currentSong)
{
    if (!playlist.IsEmpty())
    {
        cout << "Type the number of the song you want to delete: ";
        int number;
        cin >> number;
        if (IsNumberInRange(number, playlist.GetSize()))
        {
            int currentIndex = playlist.GetIndex(currentSong);

            if (currentIndex == number == 1) //If the node to be deleted is the first node
            {
                if (currentSong->previous)
                {
                    currentSong = currentSong->previous;
                }
                else if (currentSong->next)
                {
                    currentSong = currentSong->next;
                }
            }
            else if (currentIndex == number == playlist.GetSize()) // If the node to be deleted is the last node
            {
                if (currentSong->next)
                {
                    currentSong = currentSong->next;
                }
                else if (currentSong->previous)
                {
                    currentSong = currentSong->previous;
                }
            }
            //else it remains the same
            playlist.DeleteNode(number - 1); //Indexing by 1
        }
    }
    else
    {
        cerr << "There are no more songs to delete..." << endl;
    }
}

void SortSongs(DBLLinkedList& playlist)
{
    cout << "Type '<' or '>': ";
    string angleBracket;
    cin >> angleBracket;
    if (angleBracket != "<" && angleBracket != ">")
    {
        cerr << "Invalid symbol..." << endl;
    }
    else
    {
        playlist.SortList(angleBracket);
    }
}

void Menu(DBLLinkedList& playlist)
{
    int choice = 0, number = 0;
    DBLLinkedList::Node* currentSong = playlist.headNode;
    playlist.PrintListCursor(currentSong);
    do
    {
        PrintMenu();
        cin >> choice;
        ClearScreen();
        switch (choice)
        {
        case 1:
            //Show playlist
            playlist.PrintList();
            break;
        case 2:
            //Play current song
            PlayMusic(currentSong->data);
        case 3:
            //Next
            SelectNext(currentSong);
            break;
        case 4:
            //Previous
            SelectPrevious(currentSong);
            break;
        case 5:
            //Select other
            SelectOther(playlist, currentSong);
            break;
        case 6:
            //Insert
            InsertSong(playlist, currentSong);
            break;
        case 7:
            //Delete
            DeleteSong(playlist, currentSong);
            break;
        case 8:
            //Shuffle
            playlist.ShuffleList();
            break;
        case 9:
            //Sort
            SortSongs(playlist);
            break;
        case 10:
            //Exit
            cout << "Goodbye!" << endl;
            break;
        default:
            cerr << "Not a valid choice..." << endl;
            break;
        }

        if (choice != 1 && choice != 10)
        {
            playlist.PrintListCursor(currentSong);
        }
    } while (choice != 10);
}

int main()
{
    InitializeSDL();
    wifstream inputFile("playlist.txt");
    DBLLinkedList playlist(inputFile);
    ClearScreen();

    Menu(playlist);

    CloseSDL();

    return 0;
}