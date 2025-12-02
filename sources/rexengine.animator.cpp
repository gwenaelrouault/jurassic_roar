module;

#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

module rexengine.animator;

using namespace rexengine;

Animation::Animation(Animation::Type type, void* target) {
    type_ = type;
    target_ = target;
    action_ = [this](){this->play();};
    stopCondition_ = [this](){return this->done();};
}

void Animation::setOnFinished(std::function<void()> action) {
    onFinished_ = std::move(action);
}

void Animation::setLooping(bool looping) {
    this->looping_ = looping;
}

void Animation::setSpeed(float speed) {
    this->speed_ = speed;
}

bool& Animation::isLooping() {
    return looping_;
}

bool& Animation::isPlaying() {
    return playing_;
}

std::function<void()>& Animation::getAction() {
    return action_;
}

std::function<bool()>& Animation::getStopCondition() {
    return stopCondition_;
}

std::function<void()>& Animation::getOnFinished() {
    return onFinished_;
}

float Animation::getSpeed() {
    return speed_;
}

float& Animation::getTimer() {
    return timer_;
}

void Animation::setTimer(float timer) {
    timer_ = timer;
}

Animator::Animator(Host& host, SystemFlags flags): System(host, flags) {
}

void Animator::play(std::unique_ptr<Animation> animation) {
    animations_.erase(
        std::remove_if(
            animations_.begin(),
            animations_.end(),
            [&](const std::unique_ptr<Animation>& anim) {
                if (anim->target_ != animation->target_) {
                    return false;
                }

                return std::find(
                    animation->conflicted_.begin(),
                    animation->conflicted_.end(),
                    anim->type_
                ) != animation->conflicted_.end();
            }
        ), animations_.end()
    );

    animation->playing_ = true;
    animations_.push_back(std::move(animation));
}

void Animator::update(float dt) {
    std::vector<std::function<void()>> afterCallbacks;

    for (auto& animation : animations_) {
        if (!animation->isPlaying())
            continue;

        // speed <= 0 : on considère que c'est en pause
        if (animation->getSpeed() <= 0.0f)
            continue;

        animation->getTimer() += dt;

        float interval = 1.f / animation->getSpeed(); // temps entre deux exécutions

        // Si beaucoup de temps a passé, on peut rattraper plusieurs "ticks"
        while (animation->getTimer() >= interval && animation->isPlaying()) {
            animation->getTimer() -= interval;

            try {
                animation->getAction()();
            }
            catch (const std::bad_function_call& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                animation->isPlaying() = false;
                break;
            }

            // condition d'arrêt
            if (animation->getStopCondition() && animation->getStopCondition()()) {
                animation->isPlaying() = false;  
                break;
            }

            // si pas en loop, on s'arrête après un appel
            if (!animation->isLooping()) {
                animation->isPlaying() = false;
                break;
            }
        }

        if (!animation->isPlaying() && animation->getOnFinished() != nullptr) {
            afterCallbacks.push_back(animation->getOnFinished());
        }
    }

    for(const auto& callback: afterCallbacks) {
        callback();
    }

    animations_.erase(
        std::remove_if(
            animations_.begin(),
            animations_.end(),
            [](auto& anim) {
                return !anim->isPlaying();   // ou anim->isPlaying()
            }
        ),
        animations_.end()
    );
}