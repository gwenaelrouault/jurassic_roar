module;

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <variant>
#include <iostream>

module rexengine.effects;

import rexengine.properties;

using namespace rexengine;

void FadeIn::play() {
    this->color_.a++;

    std::visit([&](auto* obj) {
        obj->setFillColor(this->color_);
    }, this->target_);
}

bool FadeIn::done() {
    return this->color_.a == max_;
}

void FadeOut::play() {
    this->color_.a--;

    std::visit([&](auto* obj) {
        obj->setFillColor(this->color_);
    }, this->target_);
}

bool FadeOut::done() {
    return this->color_.a == min_;
}

SlideUp::SlideUp(Positionable& target, int min, float speed)
    : Animation(Animation::Type::SLIDE_UP, static_cast<void*>(&target)), target_(&target), position_(target.getPosition()), min_(min)
{
    conflicts(Animation::Type::SLIDE_DOWN);
    Animation::setSpeed(speed);
    setLooping(true);
}

void SlideUp::play() {
    this->position_.y--;
    this->target_->setPosition(this->position_);
}

bool SlideUp::done() {
    return this->position_.y == min_;
}

SlideDown::SlideDown(Positionable& target, int max, float speed)
    : Animation(Animation::Type::SLIDE_DOWN, static_cast<void*>(&target)), target_(&target), position_(target.getPosition()), max_(max)
{
    conflicts(Animation::Type::SLIDE_UP);
    Animation::setSpeed(speed);
    setLooping(true);
}

void SlideDown::play() {
    this->position_.y++;
    this->target_->setPosition(this->position_);
}

bool SlideDown::done() {
    return this->position_.y == max_;
}