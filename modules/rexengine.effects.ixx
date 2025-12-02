module;

#include <SFML/Graphics.hpp>
#include <variant>

export module rexengine.effects;

import rexengine.animator;
import rexengine.properties;

export {
    namespace rexengine {
        class FadeIn: public Animation {
            private:
                using TargetType = std::variant<sf::Text*, sf::Shape*>;

                TargetType target_;
                sf::Color color_;
                unsigned int max_;

            public:
                template <typename T> FadeIn(T& target, unsigned int max = 255, float speed = 1.0)
                    :   Animation(Animation::Type::FADE_IN, static_cast<void*>(&target)), target_(&target), max_(max), color_(target.getFillColor())
                {
                    conflicts(Animation::Type::FADE_OUT);
                    Animation::setSpeed(speed);
                    setLooping(true);
                }

                void play() override;
                bool done() override;
        };

        class FadeOut: public Animation {
            private:
                using TargetType = std::variant<sf::Text*, sf::Shape*>;

                TargetType target_;
                sf::Color color_;
                unsigned int min_;

            public:
                template <typename T> FadeOut(T& target, unsigned int min = 255, float speed = 1.0)
                    : Animation(Animation::Type::FADE_IN, static_cast<void*>(&target)), target_(&target), min_(min), color_(target.getFillColor())
                {
                    conflicts(Animation::Type::FADE_IN);
                    Animation::setSpeed(speed);
                    setLooping(true);
                }

                void play() override;
                bool done() override;
        };

        class SlideUp: public Animation {
            private:
                Positionable* target_;
                sf::Vector2f position_;
                int min_;

            public:
                SlideUp(Positionable& target, int min = 0, float speed = 1.0);
                
                void play() override;
                bool done() override;
        };

        class SlideDown: public Animation {
            private:
                Positionable* target_;
                sf::Vector2f position_;
                int max_;

            public:
                SlideDown(Positionable& target, int max = 255, float speed = 1.0);
                
                void play() override;
                bool done() override;
        };
    }
}