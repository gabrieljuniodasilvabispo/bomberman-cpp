#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace bomberman {

    class Explosion;

    class Bomb {
        public:
            Bomb(sf::Vector2i position, int range);

            sf::Vector2i getPosition() const;
            int getRange() const;
            bool isExploded() const;

            void toUpdateBombTime(sf::Time time);
            void toExplode();

        private:
            sf::Vector2i position_;
            int range_;
            sf::Time time_;
            bool exploded_;
    };

}
