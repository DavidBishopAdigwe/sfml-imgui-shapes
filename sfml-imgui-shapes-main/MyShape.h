#pragma once

#include <SFML/Graphics.hpp>
#include <string>

enum ShapeType
{
    Circle, Rectangle
};

class MyShape {
public:
    MyShape(const ShapeType& type, const std::string& name,
        int colour1, int colour2, int colour3,
        float radius,
        float width,
        float height,
        float velocityX,
        float velocityY);

    MyShape();


    
    

    void changeName(std::string name);
    ShapeType getType() const;
    float getRadius() const;
    const char* getNameChar() const;
    sf::Vector2<float> getRectSize() const;
    std::string getName() const;
    sf::Vector2<float> getVelocity() const;
    sf::Color getColour() const;
    void setColour( float c1,  float c2, const float c3);
    sf::Text getNameText();
    void setRadius( float radius);
    void setRectSize( float width,  float height);
    void setVelocity(float velo_x, float velo_y);
    void setName(const std::string& name);
    void draw(sf::RenderWindow& window);
    void setFill(const sf::Color& colour);
    void move();
    void centerText();
    void initializeShape(const sf::Font& font);
    void resetShape();
    void drawShape(bool toggle);
    bool canShapeDraw() const;

private:
    ShapeType m_type;
    std::string m_name;
    sf::Color m_colour;
    float m_radius;
    sf::Vector2<float> m_rectangle_size;
    sf::Vector2<float> m_velocity;
    bool m_can_draw;
    float m_window_size_x;
    float m_window_size_y;
    sf::Text m_name_text = sf::Text();
    sf::CircleShape m_circle;
    sf::RectangleShape m_rectangle;
    float m_og_radius;
    sf::Vector2<float> m_og_rect_size;

    void detectCollisions();
};