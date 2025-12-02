module;

#include <functional>
#include <string>
#include <unordered_map>
#include <memory>

export module rexengine.sysmon;

import rexengine.core;
import rexengine.containers;
import rexengine.ui;
import rexengine.resman;

export {
    namespace rexengine {
        class SysMonView;

        class SystemMonitor: public System, public ViewModel<SysMonView> {
            private:
                float   dt_;

                std::unordered_map<std::string, float> frametimes_;
                std::unordered_map<std::string, float> flushedFrametimes_;


            public:
                SystemMonitor(Host& host, SystemFlags flags);

                void watch(const std::string& name, std::function<void()> func);
                void update(float dt) override;
                void flush();

                float getFrametime(const std::string& key = "");
        };

        class SysMonView: public View<SysMonView, SystemMonitor> {
            private:
                SystemMonitor* sysmon_;
                std::shared_ptr<ResMan>   resmgr_;
                std::shared_ptr<AutoText> txt_global_dt_;
                std::shared_ptr<AutoText> txt_render_dt_;
                std::shared_ptr<AutoText> txt_logic_dt_;
                std::shared_ptr<AutoText> txt_fps_;
                std::shared_ptr<AutoText> txt_resmgr_tex_;
                std::shared_ptr<AutoText> txt_resmgr_snd_;
                std::shared_ptr<AutoText> txt_resmgr_ttf_;

            public:
                SysMonView(SystemMonitor& sysmon);
        };
    }
}