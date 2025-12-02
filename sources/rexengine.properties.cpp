module;

#include <SFML/Graphics.hpp>

module rexengine.properties;

using namespace rexengine;

void Alignable::setAlignment(Alignable::Alignment alignment) {
    alignment_ = alignment;
}

Alignable::Alignment Alignable::getAlignment() {
    return alignment_;
}