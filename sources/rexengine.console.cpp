module;

#include <algorithm>
#include <math.h>
#include <tools.h>
#include <SFML/Graphics.hpp>
#include <stringvector.h>

module rexengine.console;

import rexengine.viewport;
import rexengine.resman;
import rexengine.animator;

using namespace rexengine;

Console::Console(Host& host, SystemFlags flags): System(host, flags) {
}

Console::~Console() {
}

void Console::init() {
    isVisible_ = false;
	isActive_ 	= false;
	isHidden_	= true;
    speed_ 	= 2;
	fontSize_	= 24;
	isInit_	= false;
	historyChanged_ = false;
	//isFullyDeployed_ = false;
	//isFullyRetracted_ = true;
	//isDeploying_ = false;
	//isRetracting_ = false;

	arnoldMode_ = false;
	ahahah_ = false;
	//fatboy_ = AnimatedGIF("res/magicword.gif");
	
	//initFonts();
	
	outputLines_ = (24 / fontSize_) * 24;
		

	prompt_ 	= "]";
	cursor_ 	= "_";
	cmd_		= "";
	
	for(unsigned int i = 0; i < outputLines_; ++i)
		history_ += '\n';
		
	history_ +=  rexengine::getAppName() + " CMD";
	historyChanged_ = true;
	cmdline_ = prompt_ + cmd_ + cursor_;
	isInit_ = true;
}

void Console::resetOutput_(History mode) {
	switch (mode) {
		case History::BACKUP_HISTORY:
			historyBackup_ = history_;
			break;

		case History::RESTOR_HISTORY:
			history_ = historyBackup_;
			historyChanged_ = true;
			return;

		case History::FORGET_HISTORY:
		default: break;
	}

	history_ = "";
	
	for(unsigned int i = 0; i < outputLines_; ++i)
		history_ += '\n';
		
	if (!arnoldMode_)
		history_ += "JURASSIC_ROAR CMD";
}

void Console::show(unsigned int speed) {
    isVisible_ = true;
	isHidden_ = false;
	isActive_ = true;
	this->getView()->show();
}

void Console::hide() {
    isVisible_ = false;
	isHidden_ = true;
	isActive_ = false;
	this->getView()->hide();
}

void Console::update(float dt) {
	if (!isHidden_) {
		if (historyChanged_) {
			historyChanged_ = false;
		}

//		if (ahahah_) {
//			nedry_ = fatboy_.update();
//			nedry_->setPosition(sf::Vector2f({(float)(getHost().getSize().x - fatboy_.getSize().x), 0}));
			//getHost().draw(*nedry_);
//            getHost().draw(*nedry_);
//		}
		
		renderCursor_();			
	}
}

void Console::renderCursor_() {
	if (clock_.getElapsedTime().asMilliseconds() <= 500) {
		cmdline_ = prompt_ + cmd_ + cursor_;
	} else
		if (clock_.getElapsedTime().asMilliseconds() <= 1000) {
			cmdline_ = prompt_ + cmd_;
		} else
			clock_.restart();
}

bool Console::isActive() {
	return isActive_;
}

void Console::key(char32_t unicode) {
	switch(unicode) {
		case 8:												// BACKSPACE
			cmd_ = cmd_.substr(0, cmd_.size() - 1);
			break;
				
		case 13:											// RETURN
			execute_(cmd_);
			cmd_ = "";
			break;
				
		case 27:											// ESCAPE
			break;
				
		default:
			cmd_ += static_cast<char>(unicode);
	}
}

void Console::execute_(std::string cmd_line) {
	StringVector argList = StringVector(cmd_line, ' ');
	std::string	cmd = argList.getFirstElement();
	std::string arg1, arg2, arg3;
	
	if (cmd != NULL_STR) {
		if (cmd == "clear") {
			if (argList.getSize() == 1)
				resetOutput_();
			else
				writeln("Usage: clear");
				
			return;
		}
		
		if (cmd == "exit") {
			if (argList.getSize() == 1)
				getHost<Viewport>().close();
			else
				writeln("Usage: exit");
				
			return;
		}
/*		
		if (cmd == "set") {
			if (argList.getSize() == 3) {
				arg1 = argList.getNextElement();
				arg2 = argList.getNextElement();
				
				if (arg1 == "fps") {
					if (Tools::isNumber(arg2)) {
						getHost().setFramerateLimit(Tools::fromString<int>(arg2));
						writeln("FPS set at " + arg2);
					}
					else
						writeln("set: invalid argument " + arg2);
				} else
					Dataref::set(arg1, arg2);
			} else
				writeln("Usage: set param value");
				
			return;
		}
*/
/*		
		if (cmd == "get") {
			if (argList.getSize() > 1)
				while(argList.isNextElement())
					//arg1 = argList.getNextElement();
					Dataref::get(argList.getNextElement());
			else
				writeln("Usage: get param1 param2...");
			
			return;
		}
*/		
		if (cmd == "arnold_on" && !arnoldMode_) {
			arnoldMode_ = true;
			prompt_ = ">";
			//background_.setFillColor(sf::Color(0, 0, 255, 255));
			getView()->setBackColor(sf::Color(0,0,255,255));
			getView()->setTextColor(sf::Color(127,127,127,255));
			resetOutput_(History::BACKUP_HISTORY);
						
			
			
			writeln("Jurassic Park, System Security Interface");
			writeln("Version 4.0.5, Alpha E");
			writeln("Ready...");
			
			return;
		}
		
		if (cmd == "arnold_off" && arnoldMode_) {
			arnoldMode_ = false;
			ahahah_ = false;
//			soundman_->stop("MAGIC_WORD");
			prompt_ = "]";
			//background_.setFillColor(sf::Color(128, 0, 128, 192));
			getView()->restoreConsoleColor();
			resetOutput_(History::RESTOR_HISTORY);
			
			return;
		}
		
		if (arnoldMode_) {
			writeln(prompt_ + cmd_line);
			
			if (cmd == "access") {
				if (argList.getSize() == 4) {
					arg1 = argList.getNextElement();
					arg2 = argList.getNextElement();
					arg3 = argList.getNextElement();
					
					if (arg1 == "main" && arg2 == "security" && arg3 == "grid") {
						//beep_snd_->play();
//						soundman_->play("BEEP_S");
//						getHost().renderWhile([&](){ return soundman_->isPlaying("BEEP_S"); });

						writeln("access: PERMISSION DENIED....and...");

						for(int i = 0; i < 10; ++i) {
//							soundman_->play("BEEP_L");

//							getHost<Viewport>().renderWhile([&]{ 
//								writeln("YOU DIDN'T SAY THE MAGIC WORD!");
//								return soundman_->isPlaying("BEEP_L");
//							});
						}

						ahahah_ = true;
//						soundman_->play("MAGIC_WORD", true);
						
						return;
					}
				}
			}
				
				writeln("access: PERMISSION DENIED.");
//				soundman_->play("BEEP_S");
				
				return;
		}
		
		writeln(cmd + ": unknown command");
	}
}

void Console::write(const std::string &str) {
	history_ += str;
	historyChanged_ = true;
}

void Console::writeln(const std::string &str) {
	//std::cout << str << "\n";
	
	write("\n" + str);
	
	int eol = 0;
	int eolnum = std::count(history_.begin(), history_.end(), '\n');
	int i 	= 0;
	
	while(eol < eolnum - outputLines_ + 1) {
		if (history_[i] == '\n')
			eol++;
			
		i++;
	}
	
	history_ = history_.substr(i - 1, history_.size() - 1);
}

ConsoleView::ConsoleView(Console& console, float height): View<ConsoleView, Console>(&console) {
	height_ 	= height;
	console_	= this->getViewModel();
	backgnd_ 	= add<Surface>("00background", sf::Vector2f(VIEWPORT().getSize().x, (int)(height * VIEWPORT().getSize().y)));
	output_		= add<AutoText>("01output", "MSDOS", [this]{ return console_->getOutput(); }, 30);
	cmdline_	= add<AutoText>("02cmdline", "MSDOS", [this]{ return console_->getCmdline(); }, 30);

	backgnd_->setPosition(sf::Vector2f(0, -backgnd_->getSize().y));

	cmdline_->setOrigin(sf::Vector2f(0, cmdline_->getSize().y));
	cmdline_->setPosition(sf::Vector2f(backgnd_->getPosition().x, backgnd_->getSize().y + backgnd_->getPosition().y - 0.2 * cmdline_->getCharacterSize()));

	output_->setOrigin(sf::Vector2f(0, output_->getSize().y));
	output_->setPosition(sf::Vector2f(backgnd_->getPosition().x, backgnd_->getSize().y + backgnd_->getPosition().y - cmdline_->getCharacterSize() - 0.2 * cmdline_->getCharacterSize()));

	output_->follow(*backgnd_);
	cmdline_->follow(*backgnd_);

	setBackColor();
	setHistoryColor();
	setCmdLineColor();
}

void ConsoleView::setBackColor(sf::Color color) {
  bak_backcolor_ = backgnd_->getFillColor();
  backgnd_->setFillColor(color);
}

void ConsoleView::setHistoryColor(sf::Color color) {
  bak_histcolor_ = output_->getFillColor();
  output_->setFillColor(color);
}

void ConsoleView::setCmdLineColor(sf::Color color) {
  bak_cmdlcolor_ = cmdline_->getFillColor();
  cmdline_->setFillColor(color);
}

void ConsoleView::setTextColor(sf::Color color) {
  setHistoryColor(color);
  setCmdLineColor(color);
}

void ConsoleView::restoreConsoleColor() {
	setBackColor(bak_backcolor_);
	setHistoryColor(bak_histcolor_);
	setCmdLineColor(bak_cmdlcolor_);
}

void ConsoleView::show() { backgnd_->slideDown(0, 1500.f); }

void ConsoleView::hide() { backgnd_->slideUp(-backgnd_->getSize().y, 1500.0f); }