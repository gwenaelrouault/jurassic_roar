module;

#include <string>
#include <map>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <cstdint>

export module rexengine.core;

import rexengine.collections;
import rexengine.containers;

export {
    namespace rexengine {
        using SystemFlags = std::uint32_t;

        constexpr SystemFlags NONE = 0;
        constexpr SystemFlags AUTO_INIT = 1u << 0;
        constexpr SystemFlags AUTO_KILL = 1u << 1;
        constexpr SystemFlags AUTO_RENDER = 1u << 2;
    }
}

namespace rexengine {
    constexpr bool has_flag(SystemFlags value, SystemFlags flag) noexcept {
        return (value & flag) != 0;
    }
}

export {
    namespace rexengine {
        class Host;
        class System;
    }
}

namespace rexengine {
    template <typename T> concept DerivedFrom_Host = std::is_base_of_v<Host, T>;
    template <typename T> concept DerivedFrom_System = std::is_base_of_v<System, T>;
}

export {
    namespace rexengine {
        class System {
            friend class Host;
            
            private:
                Host& host_;
                bool autoinit_ = false;
                bool autokill_ = false;

            protected:
                explicit System(Host& host, SystemFlags flags): host_(host) {
                    autoinit_ = has_flag(flags, AUTO_INIT);
                    autokill_ = has_flag(flags, AUTO_KILL);
                }

            public:
                virtual ~System() = default;

                template<DerivedFrom_Host H>  H& getHost() {
                    return dynamic_cast<H&>(host_);
                }

                virtual void init() {}
                virtual void kill() {}
                virtual void update(float dt) {}
        };

        class Renderer: public System, public RexMap<IView> {
            friend class Host;

            private:
                bool autorender_ = false;

                void update(float dt) override {}

            protected:
                Renderer() = delete;

            public:
                Renderer(Host& host, SystemFlags flags): System(host, flags) {
                    autorender_ = has_flag(flags, AUTO_RENDER);
                }

                virtual ~Renderer() = default;

                virtual void render() {
                    for(auto& [key, value]: *this) {
                        value->draw();
                    }
                }
        };

        class Host: public Renderer {
            private:
                std::map<std::string, std::shared_ptr<System>> systems_;

                void init() override {}
                void kill() override {}

            public:
                Host(): Renderer(*this, NONE) {
                }

                virtual ~Host() = default;

                template <DerivedFrom_System S> std::shared_ptr<S> install(const std::string& id, SystemFlags flags = NONE) {
                    if (systems_.contains(id)) {
                        throw std::runtime_error("install: " + id + " already exists !");
                    }

                    auto system_ptr = std::make_shared<S>(*this, flags);
                    systems_.emplace(id, system_ptr);

                    if (system_ptr->autoinit_)
                        system_ptr->init();

                    return system_ptr;
                }

                void uninstall(const std::string& id) {
                    if (!systems_.contains(id)) {
                        throw std::runtime_error("uninstall: unknown " + id + " id !");
                    }

                    if (systems_.at(id)->autokill_)
                        systems_.at(id)->kill();
                    
                    systems_.erase(id);
                }

                template <DerivedFrom_System S> std::shared_ptr<S> get(const std::string& id) {
                    if (!systems_.contains(id)) {
                        throw std::runtime_error("[" + id + "]: unknown id !");
                    }
                    
                    auto base = systems_.at(id);
                    return std::dynamic_pointer_cast<S>(base);
                }

                virtual void render() override {
                    for(auto& [key, value]: systems_) {
                        if (auto renderer = dynamic_cast<Renderer*>(value.get())) {
                            if (renderer->autorender_)
                                renderer->render();
                        }
                    }
                }

                virtual void update(float dt) override {
                    for(auto& [key, value]: systems_) {
                        value->update(dt);
                    }
                }
        };
    }
}