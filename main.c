#include "include/raylib.h"
#include <ncurses.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_NAME_SIZE 30

int main(int argc, char *argv[])
{
    //Ncurses Setup
    initscr();
    timeout(1);
    noecho();
    char input;
    const char np[] = "Now Playing: ";
    char msg[50];

    //Get the file to be played, if none were provided, go to default.
    char resourcepath[] = "resources/";
    char musicTitle[30];
    if (argc == 2)
    {
        strncpy(musicTitle,argv[1],MAX_NAME_SIZE);
    }
    else
    {
        strcpy(musicTitle, "Laserpack.mp3");
    }
    strcat(resourcepath,musicTitle);

    //Setup the music finally
    InitAudioDevice();
    Music music = LoadMusicStream(resourcepath);
    PlayMusicStream(music);
    float timePlayed = 0.0f;

    while (true)
    {
        //Write the currently playing song on the screen
        msg[0] = '\0';
        strcpy(msg, np);
        strcat(msg,musicTitle);
        mvaddstr(0,0,msg);
        //Show the audio data
        //Keep playing the music
        UpdateMusicStream(music);
        //Check for input, stop the program if it's 1
        input = getch();
        if (input == '1')
        {
            break;
        }
        
        timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
        if (timePlayed > 1.0f) timePlayed = 1.0f;
        refresh();
    }
    //De-init everything
    endwin();
    UnloadMusicStream(music);
    CloseAudioDevice();
    return 0;
}
