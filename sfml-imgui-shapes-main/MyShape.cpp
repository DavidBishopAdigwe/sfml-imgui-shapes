#include <imgui-SFML.h>
#include "project/MyShape.h"


MyShape::MyShape(const ShapeType& type, const std::string& name,
                 int colour1, int colour2, int colour3,
                 float radius,
                 float width,
                 float height,
                 float velocityX,
                 float velocityY)
    : m_type(type),
      m_name(name),
      m_colour(sf::Color(colour1, colour2, colour3)),
      m_radius(radius),
      m_rectangle_size(width, height),
      m_velocity(velocityX, velocityY),
      m_og_radius(radius),
      m_og_rect_size(width, height)
{
}

MyShape::MyShape() = default;

void MyShape::changeName(std::string name)
{
    this->m_name = name;
}



ShapeType MyShape::getType() const
{
    return m_type;
}

float MyShape::getRadius() const
{
    return m_radius;
}

sf::Vector2<float> MyShape::getRectSize() const
{
    return m_rectangle_size;
}


sf::Vector2<float> MyShape::getVelocity() const
{
    return m_velocity;
}

sf::Color MyShape::getColour() const
{
    return m_colour;
}

void MyShape::setColour(float c1, float c2, const float c3)
{
    m_colour.r = c1;
    m_colour.g = c2;
    m_colour.b = c3;
    switch (m_type)
    {
    case Circle:
        m_circle.setFillColor(m_colour);
        break;
    case Rectangle:
        m_rectangle.setFillColor(m_colour);
        break;
    }
}

sf::Text MyShape::getNameText()
{
    return m_name_text;
}

void MyShape::setRadius(float radius)
{
    m_radius = radius;
    m_circle.setRadius(radius);
    m_name_text.setScale((radius/m_og_radius), (radius/m_og_radius));
}

void MyShape::setRectSize(float width, float height)
{
    m_rectangle_size = sf::Vector2<float>(width, height);
    m_rectangle.setSize(sf::Vector2<float>(width, height));
    m_name_text.setScale((width/m_og_rect_size.x), (height/m_og_rect_size.y));
}

void MyShape::setVelocity(float velo_x, float velo_y)
{
    m_velocity.x = velo_x;
    m_velocity.y = velo_y;
}

std::string MyShape::getName() const
{
    return m_name;
}

const char* MyShape::getNameChar() const
{
    return m_name.c_str();
}

void MyShape::setName(const std::string& name)
{
    m_name_text.setString(name);
}


void MyShape::initializeShape(const sf::Font& font)
{
    m_can_draw = true;
    m_name_text = sf::Text(m_name, font, 15); // Arbitary char size
    if (m_type == Circle)
    {
        m_circle = sf::CircleShape(m_radius, 32);
        m_circle.setPosition(10.0f, 10.0f);
        m_circle.setFillColor(m_colour);
    }
    else if (m_type == Rectangle)
    {
        sf::Vector2f size = sf::Vector2f(m_rectangle_size.x, m_rectangle_size.y);
        m_rectangle = sf::RectangleShape(size);
        m_rectangle.setPosition(10.0f, 10.0f);
        m_rectangle.setFillColor(m_colour);
    }
    centerText();
}

void MyShape::resetShape()
{
    m_can_draw = true;
    m_circle.setPosition(10.0f, 10.0f);
    m_rectangle.setPosition(10.0f, 10.0f);
}

void MyShape::drawShape(bool toggle)
{
    m_can_draw = toggle;
}

void MyShape::draw(sf::RenderWindow& window)
{
    m_window_size_x = static_cast<float>(window.getSize().x);
    m_window_size_y = static_cast<float>(window.getSize().y);
    if (!m_can_draw) return;
    switch (m_type)
    {
    case Circle:
        window.draw(m_circle);
        break;
    case Rectangle:
        window.draw(m_rectangle);
        break;
    }
    window.draw(m_name_text);
}


void MyShape::setFill(const sf::Color& colour)
{
    switch (m_type)
    {
    case Circle:
        m_circle.setFillColor(colour);
        break;
    case Rectangle:
        m_rectangle.setFillColor(colour);
        break;
    }
}

bool MyShape::canShapeDraw() const
{
    return m_can_draw;
}


void MyShape::move()
{
    m_circle.setPosition(m_circle.getPosition().x + m_velocity.x, m_circle.getPosition().y + m_velocity.y);
    m_rectangle.setPosition(m_rectangle.getPosition().x + m_velocity.x, m_rectangle.getPosition().y + m_velocity.y);
    detectCollisions();
    centerText();
}

void MyShape::centerText()
{
    auto bounds = m_name_text.getLocalBounds();

    m_name_text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

    if (m_type == Circle)
    {
        sf::Vector2f center = m_circle.getPosition() + sf::Vector2f(m_radius, m_radius);
        m_name_text.setPosition(center);
    }
    else
    {
        sf::Vector2f center = m_rectangle.getPosition() + m_rectangle_size / 2.0f;
        m_name_text.setPosition(center);
    }
}


void MyShape::detectCollisions()
{
    if (m_circle.getGlobalBounds().getPosition().x + m_circle.getGlobalBounds().getSize().x + m_velocity.x >
        m_window_size_x || m_circle.getGlobalBounds().getPosition().x + m_velocity.x < 0)
    {
        m_velocity.x = -m_velocity.x;
    }
    if (m_circle.getGlobalBounds().getPosition().y + m_circle.getGlobalBounds().getSize().y + m_velocity.y >
        m_window_size_y || m_circle.getGlobalBounds().getPosition().y + m_velocity.y < 0)
    {
        m_velocity.y = -m_velocity.y;
    }
    if (m_rectangle.getGlobalBounds().getPosition().x + m_rectangle.getGlobalBounds().getSize().x + m_velocity.x >
        m_window_size_x || m_rectangle.getGlobalBounds().getPosition().x + m_velocity.x < 0)
    {
        m_velocity.x = -m_velocity.x;
    }
    if (m_rectangle.getGlobalBounds().getPosition().y + m_rectangle.getGlobalBounds().getSize().y + m_velocity.y >
        m_window_size_y || m_rectangle.getGlobalBounds().getPosition().y + m_velocity.y < 0)
    {
        m_velocity.y = -m_velocity.y;
    }
}
