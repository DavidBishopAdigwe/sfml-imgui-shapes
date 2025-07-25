#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_internal.h"
#include "MyShape.h"




int main(int argc, char* argv[]) {
	std::vector<MyShape> my_shapes{ };

	std::vector<const char*> shapeNames{};

	 int w_width{};
	 int w_height{};
	sf::Font myFont;
	std::string str_type{};
	std::string name = " ";
	std::string font_path = "";
	int c1 = 0;
	int c2 = 0;
	int c3 = 0;
	float radius = 0;
	float width = 0;
	float height = 0;
	float velocity_x = 0;
	float velocity_y = 0;
	int font_size;
	ShapeType shape_type{};
	MyShape shape{};

	sf::Text main_text{};
	std::ifstream config_stream("project\\config.txt");
	
		std::cout << "File opened";
		std::string line;
		while (std::getline(config_stream, line))
		{
			std::stringstream ss(line);
			while (ss >> str_type)
			{
				std::cout << str_type;
				if (str_type == "Circle")
				{
					ss  >> name >> c1 >> c2 >> c3 >> radius >> velocity_x >> velocity_y;
					shape_type = Circle;
					shape = MyShape(shape_type, name, c1, c2, c3, radius, 0, 0, velocity_x, velocity_y);
					my_shapes.push_back(shape);
				}
				else if (str_type == "Rectangle")
				{
					ss >> name >> c1 >> c2 >> c3 >> width >> height >> velocity_x >> velocity_y;
					shape_type = Rectangle;
					shape = MyShape(shape_type, name, c1, c2, c3,0, width, height, velocity_x, velocity_y);
					my_shapes.push_back(shape);
				}
				else if (str_type == "Window")
				{
					ss >> w_width >> w_height;
					std::cout << w_height;
				}
				else if (str_type == "Font")
				{
					ss >> font_path >> font_size >> c1 >> c2 >> c3;
					if (!myFont.loadFromFile(font_path))
					{
						std::cerr << "Could not load font \n";
						exit (-1);
					}
					main_text = sf::Text("Assignment Text", myFont, font_size);
					main_text.setFillColor(sf::Color((uint8_t)c1, (uint8_t)c2, (uint8_t)c3));
				}
			}
		}

	
	sf::RenderWindow window(sf::VideoMode(w_width, w_height), "Assignment");

	main_text.setPosition(0, (float)w_height - (float)main_text.getCharacterSize());

	window.setFramerateLimit(60);


	ImGui::SFML::Init(window);
	sf::Clock deltaClock;

	ImGui::GetStyle().ScaleAllSizes(2.0f);
	ImGui::GetIO().FontGlobalScale = 2.0f;

	float c[3] = { 0.0f, 1.0f, 1.0f };

	char buffer[255] = "SetText";



	
	for (auto& s : my_shapes)
	{
		s.initializeShape(myFont);
		shapeNames.push_back(s.getNameChar());
	}

	float velocity[2]{};
	float rectSize[2]{};
	float colour[3]{};
	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				std::cout << "Key pressed with code = " << event.key.code << "\n";
				
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		ImGui::Begin("Window Title");
		ImGui::Text("Window text!");
		static int item_index = 0;
		ImGui::Combo("Combo", &item_index, shapeNames.data(), static_cast<int>(shapeNames.size()));
		size_t index = static_cast<size_t>(item_index);
		
		if (item_index >= 0) {
			auto* current_shape = &my_shapes[index];
			bool can_draw = current_shape->canShapeDraw();
			float radius = current_shape->getRadius();
			sf::Vector2<float> shape_velocity = current_shape->getVelocity();
			sf::Vector2<float> rect_size = current_shape->getRectSize();
			
			ImGui::Checkbox("Draw", &can_draw);
			current_shape->drawShape(can_draw);
			
			if (my_shapes[item_index].getType() == Circle)
			{
				ImGui::SliderFloat("Radius", &radius, 0, 300);
				current_shape->setRadius(radius);
			}
			else if (my_shapes[item_index].getType() == Rectangle)
			{
				rectSize[0] = rect_size.x;
				rectSize[1] = rect_size.y;
				ImGui::SliderFloat2("Size", rectSize, 0, 1000);
				current_shape->setRectSize(rectSize[0], rectSize[1]);
			}
			
			velocity[0] = shape_velocity.x;
			velocity[1] = shape_velocity.y;
			ImGui::SliderFloat2("Velocity", velocity, -10, 10);
			current_shape->setVelocity(velocity[0], velocity[1]);
			ImGui::InputText("Name", buffer, 255);
			
			if (ImGui::Button("Set Name"))
			{
				current_shape->setName(buffer);
				shapeNames[item_index] = buffer;
			}

			colour[0] = static_cast<float> (current_shape->getColour().r)/255.0f;
			colour[1] = static_cast<float>(current_shape->getColour().g)/255.0f;
			colour[2] = static_cast<float>(current_shape->getColour().b)/255.0f;
			ImGui::ColorEdit3("Shape Colour", colour);
			current_shape->setColour(colour[0]*255, colour[1]*255, colour[2]*255);
		
			
		}


		ImGui::End();

		for (auto& s : my_shapes) {
			s.move();
		}

		
		window.clear();
		
		for (auto& s : my_shapes) {
			s.draw(window);
		}
		window.draw(main_text);

		
		ImGui::SFML::Render(window);
		window.display();
	}
}





