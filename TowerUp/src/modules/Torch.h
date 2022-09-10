#pragma once

#include "Engine2D/Sprite.h"
#include "Animation.h"

namespace E2D
{

class Light;

}

class Torch
{
public:
    Torch(E2D::Light* light);

    void Update(float dt);
    void Draw();

protected:
    E2D::Sprite sprite;
    Animation animation;
};