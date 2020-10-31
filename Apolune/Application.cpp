#include "Application.h"

Application defaultApplication;
Zp<Application> application = &defaultApplication;

#include "Audio.h"
void Application::Startup() {    // Called when traditionally we have played the startup sound.
 audio.manager.CueAndPlay("data/sounds/UI/dingtone.wav", 1.0f, 0.5f, false, true);
}