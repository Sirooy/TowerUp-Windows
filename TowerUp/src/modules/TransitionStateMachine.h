#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>

template<typename TKey, typename T>
struct State;

template<typename TKey, typename T>
struct Transition
{
    Transition(bool (T::*isTriggered)(void), State<TKey, T>* targetState) 
        : IsTriggered(isTriggered), TargetState(targetState) { }
    
    bool (T::*IsTriggered)(void) = nullptr;
    State<TKey, T>* TargetState = nullptr;
};

template<typename TKey, typename T>
struct State
{
    TKey Key;
    void (T::*Action)(float) = nullptr;
    void (T::*OnEnterAction)(void) = nullptr;
    void (T::*OnExitAction)(void) = nullptr;
    
    std::vector<Transition<TKey, T>> Transitions {};
};

template<typename TKey, typename T>
class TransitionStateMachine
{
public:

    void Update(T* obj, float param)
    {
        Transition<TKey, T>* transition = CheckTransitions(obj);
        
        if(transition)
        {
            if(currentState->OnExitAction)
                (obj->*currentState->OnExitAction)();
                
            currentState = transition->TargetState;
            
            if(currentState->OnEnterAction)
                (obj->*currentState->OnEnterAction)();
        }
        
        if(currentState->Action)
            (obj->*currentState->Action)(param);
    }
    
    void AddState(TKey key, void (T::*action)(float), void (T::*onEnterAction)(void), void (T::*onExitAction)(void))
    {
        std::unique_ptr<State<TKey, T>> state(new State<TKey, T>{key, action, onEnterAction, onExitAction});
        currentState = state.get();
        states.emplace(std::make_pair(key, std::move(state)));
    }

    State<TKey, T>* GetState(TKey key)
    {
        return states.at(key).get();
    }

    State<TKey, T>* GetCurrentState()
    {
        return currentState;
    }
    
    void SetCurrentState(TKey key)
    {
        currentState = states.at(key).get();
    }

    void ChangeCurrentState(T* obj, TKey key)
    {
        if(currentState->OnExitAction)
            (obj->*currentState->OnExitAction)();
                
        currentState = states.at(key).get();
        
        if(currentState->OnEnterAction)
            (obj->*currentState->OnEnterAction)();
    }

    void AddTransitionToState(TKey key, State<TKey, T>* targetState, bool (T::*triggerFunc)(void))
    {
        State<TKey, T>* state = states.at(key).get();
        state->Transitions.emplace_back(triggerFunc, targetState);
    }

    void AddTransitionToState(TKey fromState, TKey toState, bool (T::*triggerFunc)(void))
    {
        AddTransitionToState(fromState, states.at(toState).get(), triggerFunc);
    }

    void AddTransitionToAllStates(TKey toState, bool (T::*triggerFunc)(void))
    {
        State<TKey, T>* targetState = states.at(toState).get();

        for(auto& it : states)
        {
            if(it.first == toState)
                continue;;
            it.second->Transitions.emplace_back(triggerFunc, targetState);
        }
    }

private:
    std::unordered_map<TKey, std::unique_ptr<State<TKey, T>>> states;
    State<TKey, T>* currentState;
    
    Transition<TKey, T>* CheckTransitions(T* obj)
    {
        for(auto& t : currentState->Transitions)
        {
            if((obj->*t.IsTriggered)())
            {
                return &t;
            }
        }
        
        return nullptr;
    }
};