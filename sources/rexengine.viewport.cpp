module;

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics.hpp"
#include <string>
//#include <console.h>
//#include <resmgr.h>
//#include <soundman.h>
//#include <animator.h>

module rexengine.viewport;

import rexengine.resman;
import rexengine.console;
import rexengine.animator;
import rexengine.sysmon;

using namespace rexengine;

#define DEFAULT_WIDTH	800
#define DEFAULT_HEIGHT	600
#define DEFAULT_FPS     60

Viewport::Viewport() {
    consoleLock_ = false;

    create(sf::VideoMode({1024, 768}), rexengine::appName);

    install<ResMan>("resmgr", AUTO_INIT|AUTO_KILL);
    install<Animator>("animator", AUTO_INIT|AUTO_KILL);
    install<Console>("console", AUTO_INIT|AUTO_KILL);
    install<Renderer>("renderloop", AUTO_RENDER);
    sysmon_ = install<SystemMonitor>("sysmon", AUTO_INIT|AUTO_KILL);
}

Viewport::~Viewport() {
}

void Viewport::run() {
//	create(sf::VideoMode({1024, 768}), "jurassic_roar");
    //this->setFramerateLimit(10);
    this->setVerticalSyncEnabled(true);

    get<Renderer>("renderloop")->add<ConsoleView>("Yconsoleview", *get<Console>("console"));
    get<Renderer>("renderloop")->add<SysMonView>("Zsysmonview", *get<SystemMonitor>("sysmon"));
    //get<Renderer>("renderloop").get<ConsoleView>("Zconsoleview").setBackColor(sf::Color(0,0,0,128));
    //get<Renderer>("renderloop").get<ConsoleView>("Zconsoleview").setTextColor(sf::Color(255,255,255,255));
	
	scaleFactor_.x = getSize().x / DEFAULT_WIDTH;
	scaleFactor_.y = getSize().y / DEFAULT_HEIGHT;

	logo_.setSize(sf::Vector2f(512.0f, 512.0f));
    get<ResMan>("resmgr")->getTexture("LOGO")->setSmooth(true);
	logo_.setTexture(get<ResMan>("resmgr")->getTexture("LOGO"));
	logo_.setOrigin(sf::Vector2f({logo_.getGlobalBounds().size.x / 2.0f, logo_.getGlobalBounds().size.y / 2.0f}));
	logo_.setPosition(sf::Vector2f { this->getSize() / 2u });

    //splash_ = new Splash();

    //textPage_0_ = new TextPage();
    //textPage_0_->addLine(*textLine_0_);
    //textPage_0_->addLine(*textLine_1_);
    //textPage_0_->fadeIn(255, 30.0f);
    //textLine_0_->fadeIn(255, 30.0f);

    //setRunLevel(RunLevel::SPLASH);
   
	while (isOpen())
    {
        sysmon_->watch("global", [this]() {
            clock::time_point now = clock::now();
            float dt = std::chrono::duration<float>(now - last_).count();
            last_ = now;
            // --- calcul du framerate
            elapsedTime_ = clock_.restart();
            FPS_ = 1000000.f / elapsedTime_.asMicroseconds();
            // ---

            pollEvents();
            //render();
            sysmon_->watch("logic", [this, dt](){
                Host::update(dt);
            });

            sysmon_->watch("render", [this]() {
                Host::render();
                display();
                clear();
            });
        });

        sysmon_->flush();
	}
}

void Viewport::render() {
//    clear();

    clock::time_point now = clock::now();
    float dt = std::chrono::duration<float>(now - last_).count();
    last_ = now;

    sysmon_->watch("logic", [this, dt](){
        Host::update(dt);
    });

    sysmon_->watch("render", [this]() {
        Host::render();
        display();
        clear();
    });
}

void Viewport::pollEvents() {
    while (const std::optional event = this->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            close();
                
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                if (!get<Console>("console")->isActive())
                    get<Console>("console")->show();
                else
                    get<Console>("console")->hide();
    		}
		}
			
		if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
			if (get<Console>("console")->isActive())
				get<Console>("console")->key(textEntered->unicode);
			else {
			}
		}
	}
}

void Viewport::setRunLevel(RunLevel level) {
    backupRunLevel_ = currentRunLevel_;
    currentRunLevel_ = level;
}

Viewport::RunLevel Viewport::getRunLevel() {
    return currentRunLevel_;
}

void Viewport::restoreRunLevel() {
    currentRunLevel_ = backupRunLevel_;
}

sf::Vector2f Viewport::getScaleFactor() {
	return scaleFactor_;
}

unsigned int Viewport::getFPS() {
    return FPS_;
}

float Viewport::getRateFactor() {
	return elapsedTime_.asSeconds() * DEFAULT_FPS;
}
