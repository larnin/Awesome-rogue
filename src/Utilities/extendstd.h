#ifndef EXTENDSTD
#define EXTENDSTD

#include <SFML/System/Vector2.hpp>

namespace std
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template <>
    struct hash<sf::Vector2i>
    {
        std::size_t operator()(const sf::Vector2i& k) const
        {
            return hash<int>()(k.x) ^ (hash<int>()(k.y) << 1);
        }
    };
}
#endif // EXTENDSTD

