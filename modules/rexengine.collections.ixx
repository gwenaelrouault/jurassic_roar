module;

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cstddef>
#include <typeinfo>
#include <type_traits>

export module rexengine.collections;

export {
    namespace rexengine {
        template <typename T> class RexVector {
            private:
                std::vector<std::shared_ptr<T>> elements_;

            public:
                RexVector() = default;
                RexVector(const RexVector<T>&) = delete;
                RexVector(const RexVector<T>&&) = default;

                RexVector<T>& operator=(const RexVector<T>&) = delete;
                RexVector<T>& operator=(const RexVector<T>&&) = default;

                template<typename U = T, typename... Args> std::shared_ptr<U> add(Args&&... args) {
                    static_assert(std::is_base_of_v<T, U>, "U must be derived from T");

                    auto ptr = std::make_shared<U>(std::forward<Args>(args)...);
                    elements_.push_back(std::move(ptr));
                    return ptr;
                }

                template<typename U = T> std::shared_ptr<U> get(std::size_t index) {
                    static_assert(std::is_base_of_v<T, U>, "U must be derived from T");

                    if (index >= elements_.size()) {
                        return nullptr;
                    }

                    auto base = elements_.at(index);
                    return std::dynamic_pointer_cast<U>(base);
                }

                template<typename U = T> std::shared_ptr<const U> get(std::size_t index) const {
                    static_assert(std::is_base_of_v<T, U>, "U must be derived from T");

                    if (index >= elements_.size()) {
                        return nullptr;
                    }

                    auto base = elements_.at(index);
                    return std::dynamic_pointer_cast<const U>(base);
                }

                bool del(std::size_t index) {
                    if (index >= elements_.size())
                        return false;

                    elements_.erase(elements_.begin() + index);
                    return true;
                }

                std::size_t size() const noexcept {
                    return elements_.size();
                }

                std::shared_ptr<T> operator[](size_t index) {
                    return get(index);
                }

                auto begin()    noexcept        { return elements_.begin(); }
                auto end()      noexcept        { return elements_.end(); }
                auto begin()    const noexcept  { return elements_.begin(); }
                auto end()      const noexcept  { return elements_.end(); }

            protected:
                auto& getContainer() noexcept{
                    return elements_;
                }

                const auto& getContainer() const noexcept {
                    return elements_;
                }
        };

        template <typename T> class RexMap {
            private:
                std::map<std::string, std::shared_ptr<T>> elements_;

            public:
                RexMap() = default;
                RexMap(const RexMap<T>&) = delete;
                RexMap(const RexMap<T>&&) = default;

                RexMap<T>& operator=(const RexMap<T>&) = delete;
                RexMap<T>& operator=(const RexMap<T>&&) = default;

                template<typename U = T, typename... Args> std::shared_ptr<U> add(const std::string& key, Args&&... args) {
                    static_assert(std::is_base_of_v<T, U>, "U must be derived from T");

                    auto ptr = std::make_shared<U>(std::forward<Args>(args)...);
                    elements_[key] = ptr;
                    return ptr;
                }

                template<typename U = T> std::shared_ptr<U> get(const std::string& key) {
                    static_assert(std::is_base_of_v<T, U>, "U must be derived from T");

                    if (!elements_.contains(key))
                        return nullptr;

                    auto base = elements_.at(key);
                    return std::dynamic_pointer_cast<U>(base);
                }

                template<typename U = T> const std::shared_ptr<const U> get(const std::string& key) const {
                    static_assert(std::is_base_of_v<T, U>, "U must be derived from T");

                    if (!elements_.contains(key))
                        return nullptr;

                    auto base = elements_.at(key);
                    return std::dynamic_pointer_cast<const U>(base);
                }

                bool contains(const std::string& key) const noexcept {
                    return elements_.find(key) != elements_.end();
                }

                bool del(std::size_t index) {
                    if (index >= elements_.size())
                        return false;

                    auto it = elements_.begin();
                    std::advance(it, index);
                    elements_.erase(it);
                    return true;
                }

                bool del(const std::string& key) {
                    return elements_.erase(key) > 0;
                }

                std::size_t size() const noexcept {
                    return elements_.size();
                }

                std::shared_ptr<T> operator[](const std::string& key) {
                    return get(key);
                }

                auto begin()    noexcept        { return elements_.begin(); }
                auto end()      noexcept        { return elements_.end(); }
                auto begin()    const noexcept  { return elements_.begin(); }
                auto end()      const noexcept  { return elements_.end(); }

            protected:
                auto& getContainer() noexcept {
                    return elements_;
                }

                const auto& getContainer() const noexcept {
                    return elements_;
                }
        };
    }
}