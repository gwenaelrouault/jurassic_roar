module;

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <unordered_set>

export module rexengine.properties;

export {
    namespace rexengine{
        class Animatronics {
            protected:
                Animatronics() = default;

            public:
                virtual ~Animatronics() = default;

                virtual void fadeIn(unsigned int max, float speed) {}
                virtual void fadeOut(unsigned int min, float speed) {}
                
                virtual void slideUp(int min, float speed) {}
                virtual void slideDown(int max, float speed) {}
        };

        class Alignable {
            public:
                enum class Alignment {
                    LEFT,
                    RIGHT,
                    CENTER,
                    NONE
                };

            private:
                Alignment alignment_ = Alignment::NONE;

            protected:
                Alignable() = default;
                
            public:
                ~Alignable() = default;
                void setAlignment(Alignment alignment);
                Alignment getAlignment();
        };

        class Drawable {
            private:
                bool hidden_ = false;

            protected:
                Drawable() = default;

            public:
                void show() { hidden_ = false; }
                void hide() { hidden_ = true; }
                bool isHidden() { return hidden_; }

                virtual ~Drawable() = default;
                virtual void draw() = 0;
        };

        class Positionable: public Alignable {
            private:
                std::unordered_set<Positionable*> followers_;
                sf::Transformable* transformable_;

            protected:
                Positionable() = delete;
                Positionable(sf::Transformable* t): transformable_(t) {}

            public:
                enum class Placement {
                    TOP_OF,
                    BOTTOM_OF,
                    LEFT_OF,
                    RIGHT_OF
                };

                virtual ~Positionable() = default;

                virtual void setPosition(sf::Vector2f position) {
                    sf::Vector2f offset = position - transformable_->getPosition();
                    transformable_->setPosition(position);

                    for(auto* f: followers_) {
                        f->setPosition(f->transformable_->getPosition() + offset);
                    }
                }

                virtual sf::Vector2f getPosition() const {
                    return transformable_->getPosition();
                }

                void bind(Positionable& follower) {
                    if (&follower != this && !followers_.contains(&follower)) {
                        followers_.insert(&follower);
                    }
                }

                void unbind(Positionable& follower) {
                    if (followers_.contains(&follower)) {
                        followers_.erase(&follower);
                    }
                }

                void follow(Positionable& target) {
                    if (&target != this) {
                        target.bind(*this);
                    }
                }

                void release(Positionable& target) {
                    target.unbind(*this);
                }

                virtual void setSize(sf::Vector2f size) {}
                virtual sf::Vector2f getSize() const = 0;
        };
    }
}