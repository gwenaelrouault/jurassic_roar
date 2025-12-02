module;

#include "SFML/Audio/SoundBuffer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>

export module rexengine.resman;

import rexengine.core;

export {
    namespace rexengine {
        class ResMan: public System {
            private:
                std::map<std::string, sf::Font*>         fonts_;
                std::map<std::string, sf::Texture*>      textures_;
                std::map<std::string, sf::Sound*>        sounds_;
                std::map<std::string, sf::SoundBuffer>   soundbuffers_;

                void loadFont(const std::string& fileName, const std::string& id);
                void loadTexture(const std::string& fileName, const std::string& id);
                void loadSoundBuffer(const std::string& fileName, const std::string& id);
                
            public:
                ResMan(Host& host, SystemFlags flags = NONE);
                ~ResMan();

                void init() override;
                void update(float dt) override {}

                sf::Font*        getFont(const std::string& id);
                sf::Texture*     getTexture(const std::string& id);
                sf::SoundBuffer* getSoundBuffer(const std::string& id);

                unsigned int getTexturesCount();
                unsigned int getFontsCount();
                unsigned int getSoundsCount();

                unsigned long getTexturesVRAM();
        };
    }
}