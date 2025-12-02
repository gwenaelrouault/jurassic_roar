module;

#include <variant>
#include <vector>
#include <utility>
#include "SFML/System/Vector2.hpp"
#include <stdexcept>

export module rexengine.containers;

import rexengine.properties;
import rexengine.ui;
import rexengine.collections;

export {
    namespace rexengine {
        class TextBox: public RexVector<std::variant<Text>>, public Drawable, public Alignable, public Animatronics {
            public:
                void add(std::variant<Text> element);

                void fadeIn(unsigned int max = 255, float speed = 1.0f) override {
                    for(auto& line_ptr: *this) {
                        std::visit([&](Animatronics& a) {
                            a.fadeIn(max, speed);
                        }, *line_ptr);
                    }
                }

                void fadeOut(unsigned int min = 0, float speed = 1.0f) override {
                    for(auto& line_ptr: *this) {
                        std::visit([&](Animatronics& a) {
                            a.fadeOut(min, speed);
                        }, *line_ptr);
                    }
                }

                void draw() override {
                    for(auto& line_ptr: *this) {
                        std::visit([](Drawable& d) {
                            d.draw();
                        }, *line_ptr);
                    }
                }
        };

        class Screens: public RexVector<TextBox>, public Drawable, public Alignable, public Animatronics {
            public:
                void setCurrentPage(unsigned int index);
        };

        class IView;

        class IViewModel {
            public:
                //virtual void setView(IView* view) = 0;
                virtual IView* getViewBase() = 0;
        };

        class IView: public Drawable {
            public:
                //virtual void setViewModel(IViewModel* model) = 0;
                virtual IViewModel* getViewModelBase() = 0;
                virtual void draw() = 0;
        };

        template <typename D, typename M> class View: public RexMap<Drawable>, public IView {
            private:
                M* model_ = nullptr;

            public:
                View(M* model): model_(model) {
                    if (model) {
                        model->setView(static_cast<D*>(this));
                    }
                }
                
                M* getViewModel() { return model_; }
                IViewModel* getViewModelBase() override { return model_; }
                void setViewModel(M* model) { model_ = model; }

                void draw() override {
                    for(auto& [key, value]: *this) {
                        value->draw();
                    }
                }
        };

        template <typename V> class ViewModel: public IViewModel {
            protected:
                V* view_ = nullptr;

            public:
                V* getView() {
                    return view_;
                }
                
                IView* getViewBase() override {
                    if (view_) {
                        return view_;
                    }

                    throw std::logic_error("Tried to access view, but returns nullptr");
                }
                
                void setView(V* view) {
                    view_ = view;
                }
        };
    }
}