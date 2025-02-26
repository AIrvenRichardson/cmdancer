#include "include/raylib.h"
#include <stdio.h>
#include <pthread.h>

void *threadfun(void* arg){
     InitAudioDevice();
    Music music = LoadMusicStream("resources/Laserpack.mp3");

    PlayMusicStream(music);
    float timePlayed = 0.0f;

    while (true)
    {
        UpdateMusicStream(music);

        if (*((char *)arg) == '1')
        {
            break;
        }
        timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);

        if (timePlayed > 1.0f) timePlayed = 1.0f;
    }

    UnloadMusicStream(music);
    CloseAudioDevice();

    return NULL;
}

int main(void)
{
    pthread_t thread;

    char input;

    pthread_create(&thread, NULL, threadfun,(void *) &input);

    while (input != '1')
    {
        scanf("%c", &input);
    }

    pthread_join(thread, NULL);

    return 0;
}
