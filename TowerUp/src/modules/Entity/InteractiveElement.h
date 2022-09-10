#pragma once

class InteractiveElement
{
public:
    virtual void Update(float dt) = 0;
    virtual void Draw(float tickPercent) = 0;

    virtual ~InteractiveElement() = default;
};