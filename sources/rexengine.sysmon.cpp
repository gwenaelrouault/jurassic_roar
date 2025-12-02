module;

#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <chrono>
#include <tools.h>

module rexengine.sysmon;

import rexengine.core;
import rexengine.viewport;
import rexengine.resman;

using namespace rexengine;

SystemMonitor::SystemMonitor(Host& host, SystemFlags flags): System(host, flags) {
}

void SystemMonitor::update(float dt) {
    dt_ = dt;
}

void SystemMonitor::watch(const std::string& name, std::function<void()> func) {
    using clock = std::chrono::steady_clock;
    clock::time_point before = clock::now();
    func();
    clock::time_point after = clock::now();
    frametimes_[name] = std::chrono::duration<float>(after - before).count();
}

void SystemMonitor::flush() {
    flushedFrametimes_ = frametimes_;
}

float SystemMonitor::getFrametime(const std::string& key) {
    if (!key.size()) {
        return truncateN<3>(dt_ * 1000.f);
    }

    if (flushedFrametimes_.contains(key)) {
        return truncateN<3>(flushedFrametimes_.at(key) * 1000.f);
    }

    return 0.0;
}

SysMonView::SysMonView(SystemMonitor& sysmon): View<SysMonView, SystemMonitor>(&sysmon) {
    this->sysmon_ = this->getViewModel();
    this->resmgr_ = VIEWPORT().get<ResMan>("resmgr");

    this->txt_global_dt_ = this->add<AutoText>("00dt", "MSDOS",
        [this]{
            return std::string("* global     : ") + to_string3(sysmon_->getFrametime("global")) + "ms";
        }, 15);

    this->txt_render_dt_ = this->add<AutoText>("01dt", "MSDOS",
        [this]{
            return std::string("* render     : ") + to_string3(sysmon_->getFrametime("render")) + "ms"; 
        }, 15);

    this->txt_logic_dt_ = this->add<AutoText>("02dt", "MSDOS",
        [this]{
            return std::string("* logic      : ") + to_string3(sysmon_->getFrametime("logic")) + "ms"; 
        }, 15);
    
    this->txt_fps_ = this->add<AutoText>("03dt", "MSDOS",
        [this]{
            return std::string("* framerate  : ") + std::to_string(VIEWPORT().getFPS());
        }, 15);

    this->txt_resmgr_tex_ = this->add<AutoText>("04dt", "MSDOS",
        [this]{
            return std::string("* resmgr.tex : " + std::to_string(resmgr_->getTexturesCount()) + " (" + std::to_string(resmgr_->getTexturesVRAM() / 1024) + " kb)");
        }, 15);

    this->txt_resmgr_snd_ = this->add<AutoText>("05dt", "MSDOS",
        [this]{
            return std::string("* resmgr.snd : " + std::to_string(resmgr_->getSoundsCount()));
        }, 15);

    this->txt_resmgr_ttf_ = this->add<AutoText>("06dt", "MSDOS",
        [this]{
            return std::string("* resmgr.ttf : " + std::to_string(resmgr_->getFontsCount()));
        }, 15);

    this->txt_global_dt_->propagateTo(
        *txt_render_dt_,
        *txt_logic_dt_,
        *txt_fps_,
        *txt_resmgr_tex_,
        *txt_resmgr_snd_,
        *txt_resmgr_ttf_
    );   

    //this->txt_global_dt_->setPosition(sf::Vector2f(VIEWPORT().getSize().x - txt_global_dt_->getSize().x, txt_global_dt_->getPosition().y));
    this->txt_render_dt_->setPosition(sf::Vector2f(txt_global_dt_->getPosition().x, txt_global_dt_->getPosition().y + txt_global_dt_->getCharacterSize()));
    this->txt_logic_dt_->setPosition(sf::Vector2f(txt_render_dt_->getPosition().x, txt_render_dt_->getPosition().y + txt_render_dt_->getCharacterSize()));
    this->txt_fps_->setPosition(sf::Vector2f(txt_logic_dt_->getPosition().x, txt_logic_dt_->getPosition().y + txt_logic_dt_->getCharacterSize()));
    this->txt_resmgr_tex_->setPosition(sf::Vector2f(txt_fps_->getPosition().x, txt_fps_->getPosition().y + txt_fps_->getCharacterSize()));
    this->txt_resmgr_snd_->setPosition(sf::Vector2f(txt_resmgr_tex_->getPosition().x, txt_resmgr_tex_->getPosition().y + txt_resmgr_tex_->getCharacterSize()));
    this->txt_resmgr_ttf_->setPosition(sf::Vector2f(txt_resmgr_snd_->getPosition().x, txt_resmgr_snd_->getPosition().y + txt_resmgr_snd_->getCharacterSize()));
}