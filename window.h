#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

struct WindowCharacteristics {
	sf::RenderWindow* window;
	int WIDTH, HEIGHT;

	std::string TITLE;
	std::string GUI_TITLE;
	std::string ROBOTO;
	std::string ROBOTO_MEDIUM;
	std::string ROBOTO_BOLD;
	std::map<std::string, sf::Font> FONTS;

	float GUI_TITLE_SIZE;
	sf::Color GUI_TITLE_COLOR;
	float GUI_TITLE_X;
	float GUI_TITLE_Y;

	std::string SORT_BY_TEXT;
	float SORT_BY_SIZE;
	sf::Color SORT_BY_COLOR;
	float SORT_BY_X;
	float SORT_BY_Y;

	std::string SORT_ALGOS_TEXT;
	float SORT_ALGOS_SIZE;
	sf::Color SORT_ALGOS_COLOR;
	float SORT_ALGOS_X;
	float SORT_ALGOS_Y;

	float SORT_WIDTH;
	float SORT_HEIGHT;
	sf::Color SORT_COLOR;
	sf::Color SORT_OUTLINE_COLOR;
	float SORT_OUTLINE_THICKNESS;
	float SORT_X;
	float SORT_Y;

	std::vector<std::string> SORT_CATS;
	std::vector<std::string> SORT_ALGOS;
	float BUTTON_TEXT_SIZE;
	sf::Color BUTTON_TEXT_COLOR;
	float BUTTON_TEXT_X;
	float BUTTON_TEXT_Y;

	std::string HEAP_ALGO_CHOSEN;
	std::string QUICK_ALGO_CHOSEN;
	float CHOSEN_ALGO_SIZE;
	sf::Color CHOSEN_ALGO_COLOR;
	float CHOSEN_ALGO_X;
	float CHOSEN_ALGO_Y;

	float SEP_HEIGHT;
	float SEP_X;
	float SEP_Y;
	sf::Color SEP_COLOR = sf::Color::Black;

	float GPU_TEXT_SIZE;
	sf::Color GPU_TEXT_COLOR;
	float GPU_TEXT_X;
	float GPU_TEXT_Y;
};

struct Button
{
	float x, y, width, height;
	size_t index;
	bool isOn;


	Button(float _x, float _y, float _width, float _height,
		size_t _index, bool _isOn)
		: x(_x), y(_y), width(_width), height(_height),
		index(_index), isOn(_isOn) {}
};

class GUI {
private:


	WindowCharacteristics wc;
	std::map<std::string, sf::Font> loadFonts();

public:
	GUI(int _WIDTH = 1200, int _HEIGHT = 800);
	struct WindowCharacteristics getWindowChar();
	void createButton(std::string text, float text_size, float width, float height, float x, float y);

};

GUI::GUI(int _WIDTH, int _HEIGHT) {
	this->wc.WIDTH = _WIDTH;
	this->wc.HEIGHT = _HEIGHT;
	this->wc.TITLE = "Project 3";
	this->wc.GUI_TITLE = "GUI Information";

	this->wc.window = new sf::RenderWindow(sf::VideoMode(wc.WIDTH, wc.HEIGHT), wc.TITLE);
	this->wc.ROBOTO = "fonts/roboto-regular.ttf";
	this->wc.ROBOTO_MEDIUM = "fonts/roboto-medium.ttf";
	this->wc.ROBOTO_BOLD = "fonts/roboto-bold.ttf";

	this->wc.GUI_TITLE_SIZE = 35;
	this->wc.GUI_TITLE_COLOR = sf::Color::Black;
	this->wc.GUI_TITLE_X = 120;
	this->wc.GUI_TITLE_Y = 20;

	this->wc.SORT_BY_TEXT = "Sort by: ";
	this->wc.SORT_BY_SIZE = 20;
	this->wc.SORT_BY_COLOR = sf::Color::Black;
	this->wc.SORT_BY_X = 20;
	this->wc.SORT_BY_Y = 100;

	this->wc.SORT_ALGOS_TEXT = "Sorting algorithms: ";
	this->wc.SORT_ALGOS_SIZE = 20;
	this->wc.SORT_ALGOS_COLOR = sf::Color::Black;
	this->wc.SORT_ALGOS_X = 20;
	this->wc.SORT_ALGOS_Y = 140;

	this->wc.SORT_WIDTH = 130;
	this->wc.SORT_HEIGHT = 25;
	this->wc.SORT_COLOR = sf::Color(240, 240, 240);
	this->wc.SORT_OUTLINE_COLOR = sf::Color::Black;
	this->wc.SORT_OUTLINE_THICKNESS = 2;
	this->wc.SORT_X = 105;
	this->wc.SORT_Y = 100;

	this->wc.SORT_CATS = { "Name", "Best Resolution", "Memory", "Memory Type", "Manufacturer" };
	this->wc.SORT_ALGOS = { "Heap", "Quick" };
	this->wc.BUTTON_TEXT_SIZE = 15;
	this->wc.BUTTON_TEXT_COLOR = sf::Color::Black;
	this->wc.BUTTON_TEXT_X = this->wc.SORT_X;
	this->wc.BUTTON_TEXT_Y = this->wc.SORT_Y + 3;

	this->wc.SEP_HEIGHT = 1;
	this->wc.SEP_X = 0;
	this->wc.SEP_Y = 175;
	this->wc.SEP_COLOR = sf::Color::Black;

	this->wc.GPU_TEXT_SIZE = 14;
	this->wc.GPU_TEXT_COLOR = sf::Color::Black;
	this->wc.GPU_TEXT_X = 20;
	this->wc.GPU_TEXT_Y = 190;

	this->wc.FONTS = loadFonts();

}

std::map<std::string, sf::Font> GUI::loadFonts() {
	sf::Font roboto, robotoMedium, robotoBold;
	roboto.loadFromFile(this->wc.ROBOTO);
	robotoMedium.loadFromFile(this->wc.ROBOTO_MEDIUM);
	robotoBold.loadFromFile(this->wc.ROBOTO_BOLD);

	this->wc.FONTS["roboto"] = roboto;
	this->wc.FONTS["roboto-medium"] = robotoMedium;
	this->wc.FONTS["roboto-bold"] = robotoBold;

	return this->wc.FONTS;
}

void GUI::createButton(std::string text, float text_size, float width, float height, float x, float y) {
	sf::RectangleShape sortButton;
	sf::Text buttonDisc(text, this->wc.FONTS["roboto"]);

	sortButton.setSize(sf::Vector2f(width, height));
	sortButton.setOutlineThickness(this->wc.SORT_OUTLINE_THICKNESS);
	sortButton.setPosition({ x, y });
	sortButton.setFillColor(this->wc.SORT_COLOR);
	sortButton.setOutlineColor(this->wc.SORT_OUTLINE_COLOR);

	int xTextPadding = 10;
	int yTextPadding = 3;

	buttonDisc.setCharacterSize(text_size);
	buttonDisc.setFillColor(this->wc.BUTTON_TEXT_COLOR);
	buttonDisc.setPosition({ x + xTextPadding, y + yTextPadding });

	this->wc.window->draw(sortButton);
	this->wc.window->draw(buttonDisc);
}

struct WindowCharacteristics GUI::getWindowChar() {
	return this->wc;
}