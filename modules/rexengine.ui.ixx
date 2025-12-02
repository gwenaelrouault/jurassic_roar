module;

#include <SFML/Graphics.hpp>
#include <functional>

export module rexengine.ui;

import rexengine.properties;

export {
    namespace rexengine {
        class Surface;
        class AutoText;

        class Text: public sf::Text, public Drawable, public Animatronics, public Positionable {
            friend class AutoText;

            public:
                Text() = delete;
                Text(const std::string& font, const std::string& text = "", unsigned int charSize = 30);

                void fadeIn(unsigned int max = 255, float speed = 1.0f) override;
                void fadeOut(unsigned int min = 0, float speed = 1.0f) override;

                void slideUp(int min, float speed = 1.0f) override;
                void slideDown(int max, float speed = 1.0f) override;

                void setAlignment(Alignment alignment);
                void setPosition(sf::Vector2f position) override;
                sf::Vector2f getPosition() const override;
                sf::Vector2f getSize() const override;

                void inheritsFrom(const Text& text);

                template <typename... Args> requires (std::derived_from<std::decay_t<Args>, Text> && ...)
                void propagateTo(Args&&... args) {
                    (args.inheritsFrom(*this), ...);
                }

                void draw() override;
        };

        class AutoText: public Text {
            private:
                std::function<std::string()> source_;

            public:
                AutoText() = delete;
                AutoText(const std::string& font, const std::function<std::string()>& source, unsigned int charSize = 30);

                void draw() override;
        };

        class Surface: public sf::RectangleShape, public Drawable, public Animatronics, public Positionable {
            public:
                Surface(sf::Vector2f size);
                Surface(const std::string& texture, sf::Vector2f size);

                void fadeIn(unsigned int max = 255, float speed = 1.0f) override;
                void fadeOut(unsigned int min = 0, float speed = 1.0f) override;

                void slideUp(int min, float speed = 1.0f) override;
                void slideDown(int max, float speed = 1.0f) override;

                void setAlignment(Alignment alignment);
                void setPosition(sf::Vector2f position) override;
                sf::Vector2f getPosition() const override;
                sf::Vector2f getSize() const override;

                void draw() override;
        };
    }
}