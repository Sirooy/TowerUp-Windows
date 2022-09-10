#pragma once
#include <vector>
#include <unordered_map>

template<typename TKey, typename T>
class ActionQueue
{
public:    
    void AddAction(TKey key, bool (T::*action)(float))
    {
        actions.emplace(std::make_pair(key, action));
    }
    
    void EnqueueAction(TKey key)
    {
        actionQueue.push_back(actions.at(key));
        currentAction = actionQueue[0];
    }
    
    void Update(T* obj, float param, float& actionTimer)
    {
        bool hasFinished = true;
        while(currentAction && hasFinished) //Mientra haya acciones que se terminen en 1 frame se siguen ejecutando
        {
            hasFinished = (obj->*currentAction)(param);
            
            if(hasFinished)
            {
                actionTimer = 0.0f;
                actionQueue.erase(actionQueue.begin());

                if(actionQueue.size() > 0)
                    currentAction = actionQueue[0];
                else
                    currentAction = nullptr;
            }
            
        }
    }
    
    bool IsEmpty()
    {
        return actionQueue.empty();
    }

    void ClearQueue()
    {
        actionQueue.clear();
    }

protected:
    std::unordered_map<TKey, bool (T::*)(float)> actions {};
    std::vector<bool (T::*)(float)> actionQueue {};
    bool (T::*currentAction)(float) { nullptr };
};

/*
template<typename TKey, typename T>
class ActionQueue
{
public:
    struct Action
    {
       bool (T::*Execute)(float);
       void (T::*OnEnter)(void);
       void (T::*OnExit)(void); 
       
       Action(bool (T::*execute)(float) = nullptr, void (T::*onEnter)(void) = nullptr, void (T::*onExit)(void) = nullptr) : 
           Execute(execute), OnEnter(onEnter), OnExit(onExit) { }
    };
    
    void AddAction(TKey key, bool (T::*execute)(float))
    {
        std::unique_ptr<Action> action(new Action{ execute });
        actions.emplace(std::make_pair(key, std::move(action)));
    }
    
    void AddAction(TKey key, bool (T::*execute)(float), void (T::*onEnter)(void), void (T::*onExit)(void) = nullptr)
    {
        std::unique_ptr<Action> action(new Action{ execute, onEnter, onExit });
        actions.emplace(std::make_pair(key, std::move(action)));
    }
    
    void EnqueueAction(TKey key)
    {
        actionQueue.push_back(actions.at(key).get());
    }
    
    void Update(T* obj, float param)
    {
        if(actionQueue.size() > 0)
        {
            if(currentAction == nullptr) //Cambio de accion en la iteracion anterior
            {
                currentAction = actionQueue[0];
                if(currentAction->OnEnter)
                    (obj->*currentAction->OnEnter)();
            }
                
            bool result = (obj->*currentAction->Execute)(param);
            
            if(result)
            {
                if(currentAction->OnExit)
                    (obj->*currentAction->OnExit)();
                    
                actionQueue.erase(actionQueue.begin());
                currentAction = nullptr;
            }
        }
    }
    
    bool IsEmpty()
    {
        return actionQueue.empty();
    }

    void ClearQueue()
    {
        actionQueue.clear();
    }

protected:
    std::unordered_map<TKey, std::unique_ptr<Action>> actions {};
    std::vector<Action*> actionQueue {};
    Action* currentAction {};
};
*/