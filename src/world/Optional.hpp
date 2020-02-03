#pragma once
#ifndef WORLD_OPTIONAL_HPP
#define WORLD_OPTIONAL_HPP
#include <stdexcept>

namespace ofxPlanet {
/**
 * Optional is type for can store empty value.
 */
template <typename T>
class Optional {
       public:
        explicit Optional();
        explicit Optional(T value);

		/**
		 * set a value.
		 * @param value
		 */
        void setValue(T value);
		/**
		 * returns value.
		 * @return
		 */
        T& getValue();
		/**
		 * return default value if empty.
		 * in else, returns value.
		 * @param defaultValue
		 * @return
		 */
        T getOrDefault(T defaultValue) const;
		/**
		 * returns value.
		 * @return
		 */
        const T& getValue() const;
		/**
		 * return true if empty.
		 * @return
		 */
        bool isEmpty() const;
		/**
		 * return false if empty.
		 * @return
		 */
        bool hasValue() const;

       private:
        T value;
        bool empty;
};

template <typename T>
inline Optional<T>::Optional() : value(), empty(true) {}
template <typename T>
inline Optional<T>::Optional(T value) : value(value), empty(false) {}
template <typename T>
inline void Optional<T>::setValue(T value) {
        this->value = value;
        this->empty = false;
}
template <typename T>
inline T& Optional<T>::getValue() {
        if (empty) {
                throw std::logic_error("value is missing");
        }
        return value;
}
template <typename T>
inline T Optional<T>::getOrDefault(T defaultValue) const {
        if (empty) {
                return defaultValuevalue;
        }
        return value;
}
template <typename T>
inline const T& Optional<T>::getValue() const {
        if (empty) {
                throw std::logic_error("value is missing");
        }
        return value;
}
template <typename T>
inline bool Optional<T>::isEmpty() const {
        return empty;
}
template <typename T>
inline bool Optional<T>::hasValue() const {
        return !isEmpty();
}
}  // namespace ofxPlanet
#endif