module;

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

module rexengine.resman;

import rexengine.core;

using namespace rexengine;

ResMan::ResMan(Host& host, SystemFlags flags): System(host, flags) {
}

ResMan::~ResMan() {
}

void ResMan::loadFont(const std::string &filename, const std::string &id) {
    sf::Font* f = new sf::Font();
	
	//CONSOLE->writeln("Loading font [" + id + "]: " + filename);
    
    if (f->openFromFile(filename))
        ResMan::fonts_[id] = f;
    else
        delete f;
}

void ResMan::loadTexture(const std::string &filename, const std::string &id) {
    sf::Texture* t = new sf::Texture();

    //CONSOLE->writeln("Loading texture [" + id + "]: " + filename);
    
    if (t->loadFromFile(filename))
        ResMan::textures_[id] = t;
    else
        delete t;
}

void ResMan::loadSoundBuffer(const std::string& fileName, const std::string& id) {
    if (!soundbuffers_.contains(id)) {
        //CONSOLE->writeln("Loading sound [" + id + "]: " + fileName);

        if (!soundbuffers_[id].loadFromFile(fileName)) {
            soundbuffers_.erase(id);
        }
    }
}

void ResMan::init() {
    std::cout << "ok" << std::endl;

    ResMan::loadFont("res/DooM.ttf", "DOOM");
	ResMan::loadFont("res/PixeloidMono.ttf", "PIX_MONO");
	ResMan::loadFont("res/roboto_mono_regular.ttf", "RMR");
	ResMan::loadFont("res/msdos.ttf", "MSDOS");
    ResMan::loadFont("res/JurassicPark-BL48.ttf", "JP");
	
    ResMan::loadTexture("res/marble.jpg", "CONSOLE_BG");
	ResMan::loadTexture("res/rexengine_logo.png", "LOGO");

    ResMan::loadSoundBuffer("res/beep_s.wav", "BEEP_S");
    ResMan::loadSoundBuffer("res/beep_l.wav", "BEEP_L");
    ResMan::loadSoundBuffer("res/magicword.wav", "MAGIC_WORD");
}

sf::Font* ResMan::getFont(const std::string &id) {
    auto item = ResMan::fonts_.find(id);
    
    if (item != ResMan::fonts_.end())
        return item->second;
        
    return nullptr;
}

sf::Texture* ResMan::getTexture(const std::string &id) {
    auto item = ResMan::textures_.find(id);
    
    if (item != ResMan::textures_.end())
        return item->second;
        
    return nullptr;
}

sf::SoundBuffer* ResMan::getSoundBuffer(const std::string& id) {
    if (soundbuffers_.contains(id)) {
        return &soundbuffers_[id];
    }

    return nullptr;
}

unsigned int ResMan::getTexturesCount() {
    return textures_.size();
}

unsigned int ResMan::getSoundsCount() {
    return sounds_.size();
}

unsigned int ResMan::getFontsCount() {
    return fonts_.size();
}

unsigned long ResMan::getTexturesVRAM() {
    unsigned long size = 0;

    for(auto& [key, value]: textures_) {
        size += value->getSize().x * value->getSize().y * 4;
    }

    return size;
}