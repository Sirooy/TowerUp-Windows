#pragma once

#include <memory>
#include <unordered_map>

using ScreenID_t = uint32_t;

class ScreenIDHolder
{
    template<typename T>
    friend class ScreenIDGenerator;

    inline static ScreenID_t CurrentID { 0 };
};

template<typename T>
class ScreenIDGenerator
{
public:
    static ScreenID_t GetID()
    {
        static ScreenID_t id = ScreenIDHolder::CurrentID++;
        return id;
    }
};

class Screen;

class ScreenController
{
public:
    ScreenController();

    void ChangeScreen(ScreenID_t id);
    void SwapScreens();
    Screen* GetScreen(ScreenID_t id);
    
    ~ScreenController();
protected:
    std::unordered_map<ScreenID_t, std::unique_ptr<Screen>> screens;
    Screen* currentScreen;
    ScreenID_t newScreenID;
    bool hasToChangeScreen;
};