#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace bomberman {

    class Map;

    class Explosion {
        public:
            Explosion(sf::Vector2i origin, int range);

            sf::Vector2i getOrigin() const;
            bool isActive() const;

            void toCreate(const Map& map);
            void toUpdate(sf::Time time);

            const std::vector<sf::Vector2i>& getPositions() const;

        private:
            sf::Vector2i origin_;
            int range_;
            sf::Time time_;
            bool active_;
            std::vector<sf::Vector2i> positions_;
    };

}
