#ifndef GG_TIMER_H
#define GG_TIMER_H

#include <SDL2/SDL.h>

class GG_Timer {
public:
    GG_Timer();

    //timer acctions
    void start();
    void stop();
    void pause();
    void unpause();

    //return timers time
    Uint32 getTicks();

    bool isStarted();
    bool isPaused();
private:
    //Start clock time
    Uint32 mStartTicks;
    Uint32 mPausedTicks;

    //timer status
    bool isMPaused;
    bool isMStarted;
};

#endif

GG_Timer::GG_Timer() {
    mStartTicks = 0;
    mPausedTicks = 0;

    isMPaused = false;
    isMStarted = false;
}

void GG_Timer::start() {
    isMStarted = true;
    isMPaused = false;

    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void GG_Timer::stop() {
    isMStarted = false;
    isMPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void GG_Timer::pause() {
    if (isMStarted && !isMPaused) {
        isMStarted = false;
        isMPaused = true;

        mPausedTicks = SDL_GetTicks() - mPausedTicks;
        mStartTicks = 0;
    }
}

void GG_Timer::unpause() {
    if (isMPaused && isMStarted) { // timer runs while was paused
        isMStarted = true;
        isMPaused = false;

        mStartTicks = SDL_GetTicks() - mPausedTicks;
        mPausedTicks = 0;
    }
}

Uint32 GG_Timer::getTicks() {
    Uint32 time = 0;

    if( isMStarted ) {
        if( isMPaused ) {
            time = mPausedTicks;
        }
        else {
            time = SDL_GetTicks() - mStartTicks;
        }
    }
    return time;
}

bool GG_Timer::isStarted() {
    return isMStarted;
}

bool GG_Timer::isPaused() {
    return ( isMStarted && isMPaused );
}