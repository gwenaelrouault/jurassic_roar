module;

#include <variant>
#include <SFML/System/Vector2.hpp>

module rexengine.containers;

import rexengine.ui;

using namespace rexengine;

void TextBox::add(std::variant<Text> line) {
    if (!size()) {
        RexVector<std::variant<Text>>::add(line);
    } else {
        auto& container = RexVector<std::variant<Text>>::getContainer();
        auto& lastElement = container[container.size() - 1];

        if (std::holds_alternative<Text>(*lastElement)) {
            auto& textLine = std::get<Text>(*lastElement);

            float pyp = textLine.getPosition().y;
            float pcs = textLine.getCharacterSize();
            float pls = textLine.getLineSpacing();

            textLine.setPosition(sf::Vector2f{textLine.getPosition().x, pyp + pcs + pls});
        }

        RexVector<std::variant<Text>>::add(line);
    }
}