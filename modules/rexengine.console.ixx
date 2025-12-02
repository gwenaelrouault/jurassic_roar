module;

#include <SFML/Graphics.hpp>
#include <string>
#include <stack>
#include <iostream>
#include <memory>

export module rexengine.console;

import rexengine.viewport;
import rexengine.core;
import rexengine.properties;
import rexengine.containers;
import rexengine.ui;
import rexengine.resman;
import rexengine.animator;
//import rexengine.animations;
import rexengine.effects;

export {
	namespace rexengine {
		class ConsoleView;

		class Console: public System, public Animatronics, public ViewModel<ConsoleView> {
			public:
					enum class History {
						BACKUP_HISTORY = 0,
						FORGET_HISTORY = 1,
						RESTOR_HISTORY = 2
					};

			private:
				bool                isVisible_;
				bool				isActive_;
				bool				isHidden_;
				bool				isInit_;
				bool				historyChanged_;
				float        		speed_;
				bool				ahahah_;
				bool				arnoldMode_;
				
				//sf::SoundBuffer		tetaclaq_;
				//sf::Sound			youdidntsaythemagicword_;
				//sf::Sprite			nedry_;

				std::stack<sf::Color> fillcolor_bak_;
				std::stack<sf::Color> textcolor_bak_;
				
				//AnimatedGIF			fatboy_;
				
				std::string			prompt_;
				std::string			cmd_;
				std::string			cursor_;
				std::string			history_;
				std::string			historyBackup_;
				std::string			cmdline_;
				
				sf::Clock			clock_;
				
				unsigned int		outputLines_;
				unsigned int		fontSize_;
				
				void expand_();
				void retract_();
				void execute_(std::string cmd);
				void resetOutput_(History mode = History::FORGET_HISTORY);
				void renderCursor_();

			public:
				Console(Host& host, SystemFlags flags);
				~Console();
				
				bool isActive();

				void update(float dt) override;

				void init() override;
				void show(unsigned int speed = 200);
				void hide();
				void draw();
				void key(char32_t unicode);
				void write(const std::string &str);
				void writeln(const std::string &str);

				const std::string& getOutput() const {
					return history_;
				}

				const std::string& getCmdline() const {
					return cmdline_;
				}
		};

		class ConsoleView: public View<ConsoleView, Console> {
			private:
				Console*					console_;
				std::shared_ptr<Surface>	backgnd_;
				std::shared_ptr<AutoText> 	output_;
				std::shared_ptr<AutoText> 	cmdline_;

				sf::Color	bak_backcolor_;
				sf::Color	bak_histcolor_;
				sf::Color	bak_cmdlcolor_;

				float 		height_;

			public:
				ConsoleView(Console& console, float height = 0.8);

				void setBackColor(sf::Color color = sf::Color(0,0,255,127));
				void setHistoryColor(sf::Color color = sf::Color(127,127,127,255));
				void setCmdLineColor(sf::Color color = sf::Color(0,255,255,255));
				void setTextColor(sf::Color color);
				void restoreConsoleColor();

				void show();
				void hide();
		};
	}
}