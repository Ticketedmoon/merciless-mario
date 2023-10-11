#include "c_sprite_group.h"

CSpriteGroup::CSpriteGroup(sf::RectangleShape& shape) : m_shapes({shape})
{

}

CSpriteGroup::CSpriteGroup(std::vector<sf::RectangleShape>& shapes) : m_shapes(shapes)
{

}

sf::RectangleShape& CSpriteGroup::getSprite()
{
    return m_shapes.front();
}

std::vector<sf::RectangleShape>& CSpriteGroup::getSprites()
{
    return m_shapes;
}
