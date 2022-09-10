#pragma once
#include <cstdint>

template<typename TType, int32_t TNumStates>  
class StateMachine
{
public:
    inline void SetState(int32_t index, void (TType::*func)(float))
    {
        states[index] = func;
        currentState = func;
        currentStateIndex = index;
    }

    inline void ChangeState(int32_t index)
    {
        currentState = states[index];
        currentStateIndex = index;
    }

    inline void Update(TType* ref, float dt)
    {
        (ref->*currentState)(dt);
    }

    inline int32_t GetState()
    {
        return currentStateIndex;
    }

private:
    void (TType::*states[TNumStates])(float) {};
    void (TType::*currentState)(float) { nullptr };
    int32_t currentStateIndex;
};