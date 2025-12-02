module;

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

module rexengine.ui;

import rexengine.viewport;
import rexengine.resman;
import rexengine.animator;
import rexengine.effects;

using namespace rexengine;

Surface::Surface(sf::Vector2f size): sf::RectangleShape(size), Positionable(this) {
}

Surface::Surface(const std::string& texture, sf::Vector2f size): sf::RectangleShape(size), Positionable(this) {
    this->setTexture(VIEWPORT().get<ResMan>("resmgr")->getTexture(texture));
}

void Surface::fadeIn(unsigned int max, float speed) {
    VIEWPORT().get<Animator>("animator")->play<FadeIn>(*this, max, speed);
}

void Surface::fadeOut(unsigned int min, float speed) {
    VIEWPORT().get<Animator>("animator")->play<FadeOut>(*this, min, speed);
}

void Surface::slideUp(int min, float speed) {
    VIEWPORT().get<Animator>("animator")->play<SlideUp>(*this, min, speed);
}

void Surface::slideDown(int max, float speed) {
    VIEWPORT().get<Animator>("animator")->play<SlideDown>(*this, max, speed);
}

void Surface::draw() {
    VIEWPORT().draw(*this);
}

void Surface::setPosition(sf::Vector2f position) {
    Positionable::setPosition(position);
}

sf::Vector2f Surface::getPosition() const {
    return Positionable::getPosition();
}

sf::Vector2f Surface::getSize() const {
    return sf::RectangleShape::getSize();
}

Text::Text(const std::string& font, const std::string& text, unsigned int charSize)
    : sf::Text(*VIEWPORT().get<ResMan>("resmgr")->getFont(font), text, charSize), Positionable(this)
{
}

void Text::fadeIn(unsigned int max, float speed) {
    VIEWPORT().get<Animator>("animator")->play<FadeIn>(*this, max, speed);
}

void Text::fadeOut(unsigned int min, float speed) {
    VIEWPORT().get<Animator>("animator")->play<FadeOut>(*this, min, speed);
}

void Text::slideUp(int min, float speed) {
    VIEWPORT().get<Animator>("animator")->play<SlideUp>(*this, min, speed);
}

void Text::slideDown(int max, float speed) {
    VIEWPORT().get<Animator>("animator")->play<SlideDown>(*this, max, speed);
}

void Text::draw() {
    VIEWPORT().draw(*this);
}

void Text::setAlignment(Alignable::Alignment alignment) {/*
    Alignable::setAlignment(alignment);

    switch(alignment) {
        case Alignable::Alignment::LEFT:
            break;

        case Alignable::Alignment::CENTER:
            sf::Text::setPosition(sf::Vector2f{(VIEWPORT().getSize().x - getLocalBounds().size.x) / 2, getLocalBounds().position.y});
            break;

        case Alignable::Alignment::RIGHT:
            sf::Text::setPosition(sf::Vector2f{VIEWPORT().getSize().x - getLocalBounds().size.x, getLocalBounds().position.y});
            break;

        case Alignable::Alignment::NONE:
            break;

        default:
            break;
    }*/
}

void Text::setPosition(sf::Vector2f position) {
    Positionable::setPosition(position);
}

sf::Vector2f Text::getSize() const {
    return this->getGlobalBounds().size + this->getGlobalBounds().position;
}

sf::Vector2f Text::getPosition() const {
    return Positionable::getPosition();
}

void Text::inheritsFrom(const Text& text) {
    setFillColor(text.getFillColor());
    setOutlineColor(text.getOutlineColor());
    setCharacterSize(text.getCharacterSize());
    setFont(text.getFont());
}

AutoText::AutoText(const std::string& font, const std::function<std::string()>& source, unsigned int charSize)
    : Text(font, source(), charSize)
{
    this->source_ = source;
}

void AutoText::draw() {
    this->setString(source_());
    Text::draw();
}