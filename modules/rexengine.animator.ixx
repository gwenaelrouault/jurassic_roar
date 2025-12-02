module;

#include <vector>
#include <memory>
#include <utility>
#include <functional>

export module rexengine.animator;

import rexengine.core;

export {
    namespace rexengine {
        class Animator;

        class Animation {
            friend class Animator;

            protected:
                enum class Type {
                    FADE_IN,
                    FADE_OUT,
                    SLIDE_UP,
                    SLIDE_DOWN
                };

            private:
                void* target_;
                Animation::Type type_;
                std::vector<Animation::Type> conflicted_;
                std::function<void()> action_ = nullptr;
                std::function<bool()> stopCondition_ = nullptr;
                std::function<void()> onFinished_ = nullptr;
                bool  looping_ = false;
                bool  playing_ = false;
                float speed_ = 1.0f;
                float timer_ = 0.0f;

            protected:
                template <typename... Args> void conflicts(Args&&... args) {
                    (conflicted_.emplace_back(std::forward<Args>(args)), ...);
                }                

            public:
                Animation() = delete;
                Animation(Animation::Type type, void* target);
                virtual ~Animation() = default;

                void setLooping(bool looping);

                bool& isLooping();
                bool& isPlaying();

                void setOnFinished(std::function<void()> action);

                virtual void play() = 0;
                virtual bool done() = 0;

                std::function<void()>& getAction();
                std::function<bool()>& getStopCondition();
                std::function<void()>& getOnFinished();

                float getSpeed();
                float& getTimer();
                void setSpeed(float speed);
                void setTimer(float timer);
        };

        class Animator: public System {
            private:
                std::vector<std::unique_ptr<Animation>> animations_;

            public:
                Animator(Host& host, SystemFlags flags);
                ~Animator() = default;

                void play(std::unique_ptr<Animation> animation);

                template <typename T, class... Args>
                void play(Args&&... args) {
                    static_assert(std::is_base_of_v<Animation, T>, "T must derive from Animation");

                    auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
                    play(std::move(ptr));
                    //ptr->playing_ = true;
                    //animations_.emplace_back(std::move(ptr));
                }

                void update(float dt) override;
        };
    }
}