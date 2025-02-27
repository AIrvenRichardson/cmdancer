#include "include/raylib.h"
#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAX_NAME_SIZE 30

void ProcessAudio(void *buffer, unsigned int frames)
{
    clear();
    int16_t *samples = (int16_t *)buffer;
    int fpc = (int)frames / COLS;

    for (unsigned int frame = 0; frame < frames; frame+=fpc)
    {
        int16_t *l = &samples[frame * 2 + 0], *r = &samples[frame * 2 + 1];
        int16_t comp = *l;
        float scaled = (float)comp/(float)INT16_MAX;
        comp = (int16_t)(scaled * ((float)(LINES-2)/2));

        int start = 0;
        while (start != comp)
        {
            mvaddch((LINES/2)+start,frame/fpc, '|');
            if (start > comp) start--;
            else start++;
        }
        
    }
    refresh();
}


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
    AttachAudioMixedProcessor(ProcessAudio);
    Music music = LoadMusicStream(resourcepath);
    PlayMusicStream(music);
    float timePlayed = 0.0f;
    bool paused = false;

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
        else if (input == 'p')
        {
            paused = !paused;
            if (paused) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }
        
        timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
        if (timePlayed > 1.0f) timePlayed = 1.0f;
        //clear();
        //refresh();
    }
    //De-init everything
    endwin();
    UnloadMusicStream(music);
    DetachAudioMixedProcessor(ProcessAudio);
    CloseAudioDevice();
    return 0;
}
