#include "ScreenController.h"
#include "Screen.h"

ScreenController::ScreenController() : screens(), currentScreen(nullptr),  newScreenID(0), hasToChangeScreen(false)
{

}

void ScreenController::ChangeScreen(ScreenID_t id)
{
    hasToChangeScreen = true;
    newScreenID = id;
}

void ScreenController::SwapScreens()
{
    if(hasToChangeScreen) //Cambiar a una nueva pantalla si se ha llamado a change state
    {
        hasToChangeScreen = false;
        currentScreen->OnExit();
        currentScreen = screens.at(newScreenID).get();
        currentScreen->OnEnter();
    }
    else if(currentScreen)
        currentScreen->SwapScreens();
}

Screen* ScreenController::GetScreen(ScreenID_t id)
{
    return screens.at(id).get();
}

ScreenController::~ScreenController() = default;