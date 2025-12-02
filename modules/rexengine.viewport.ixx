module;

#include <SFML/Graphics.hpp>
#include <memory>

export module rexengine.viewport;

import rexengine.core;
import rexengine.sysmon;

namespace rexengine {
	std::string appName = "REXENGINE_DEFAULT_VIEWPORT";
}

export {
	namespace rexengine {
		class Viewport: public Host, public sf::RenderWindow {
			public:
				enum class RunLevel {
					MENU,
					GAME,
					SPLASH
				};

			private:
				sf::Vector2f    	scaleFactor_;
				sf::Clock       	clock_;
				sf::Time        	elapsedTime_;
				sf::RectangleShape	logo_;
				float           	FPS_;
				bool            	consoleLock_;
				RunLevel			currentRunLevel_ = RunLevel::MENU;
				RunLevel			backupRunLevel_;
				std::shared_ptr<SystemMonitor>		sysmon_;


				using clock = std::chrono::steady_clock;
				clock::time_point 	last_ = clock::now();
				
				Viewport();

			public:
				Viewport(const Viewport&) = delete;
				~Viewport();

				Viewport& operator=(const Viewport&) = delete;
				
				static Viewport& getInstance() {
					static Viewport instance;
					return instance;
				}

				void update(float dt) override {}
				void run();
				void render() override;
				void pollEvents();
				void setRunLevel(RunLevel level);
				RunLevel getRunLevel();
				void restoreRunLevel();

				template <typename Condition> void renderWhile(Condition condition) {
					renderWhile(condition, [this](){ this->render(); });
				}

				template <typename Condition, typename RenderFct> void renderWhile(Condition condition, RenderFct render) {
					while(isOpen() && condition()) {
						pollEvents();
						render();
					}
				}
				
				sf::Vector2f 	getScaleFactor();
				unsigned int 	getFPS();
				float 			getRateFactor();
		};

		Viewport& VIEWPORT() {
			return Viewport::getInstance();
		}

		void setAppName(const std::string& name) {
			appName = name;
			VIEWPORT().setTitle(name);
		}

		const std::string& getAppName() {
			return appName;
		}
	}
}
