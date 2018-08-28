#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "RichText.h"

// 11 12 15 17 19 21 24 26 28 30 32 34 36 39 41 43 45 47 49 52 54 58 65 69 78 84
enum FONT_SIZE {
    FONT_SMALL=13,
    FONT_MEDIUM=14,
    FONT_BIG=18
};





inline bool compareChar(char a, char b) {
   return(toupper(a) == toupper(b));
}

static bool compareStr(const std::string& s1, const std::string& s2) {
   return((s1.size() == s2.size()) && equal(s1.begin(), s1.end(), s2.begin(), compareChar));
}





class Dictionary {
    public:
        Dictionary() {
            _dictionary = loadDictionary("english");
        }

        std::string getRandomWord() {
            return _dictionary[rand() % _dictionary.size()];
        }

        std::vector<std::string> getRandomWords(int count) {
            std::vector<std::string> words;

            for(int i = 0; i < count; ++i)
                words.push_back(getRandomWord());

            return words;
        }

    private:
        std::vector<std::string> loadDictionary(std::string language) {
            std::ifstream in("data/assets/words/" + language + ".txt");
            if(!in) throw EXIT_FAILURE;

            std::vector<std::string> words;

            std::string word;
            while(std::getline(in, word))
                if(word.size() > 0) words.push_back(word);

            return words;
        }

        std::vector<std::string> _dictionary;
};

class Word {
    public:
        Word() : type_index(0) {}
        Word(const std::string& word) : _str(word), type_index(0) {}

        std::string get() {
            return _str;
        }

        char getNextLetter() {
            return _str[type_index];
        }

        std::string getDisplayText() {
            std::string display;

            for(int i = 0; i < type_index; ++i) display += ' ';

            display += _str.substr(type_index);

            return display;
        }

        void typeLetter() {
            ++type_index;
        }

        bool done() {
            return type_index >= _str.size();
        }

        operator==(const Word& w) {
            return _str == w._str;
        }

    private:
        std::string _str;
        size_t type_index;
};

class Typing {
    public:
        Typing() {}

        bool submit() {
            if(inputMatches()) {
                _input.clear();

                if(_active_word_idx < _words.size() - 1) {
                    ++_active_word_idx;
                }
                else {

                }

                return true;
            }

            return false;
        }

        void type(char letter, bool ctrl = false) {
            if(letter == '\b') {
                if(!_input.empty()) {
                    if(ctrl) _input.clear();
                    else _input.erase(_input.size()-1, 1);
                }
            }
            else {
                _input += letter;
            }
        }

        void setWords(std::vector<std::string> words) {
            for(auto& word : words) {
                _words.push_back(Word(word));
            }
        }

        RichText getInput(const sf::Font& font, unsigned size) {
            RichText text(font);
            text.setCharacterSize(size);

            text << sf::Text::Bold << sf::Color::White << _input;

            return text;
        }

        RichText getDisplay(const sf::Font& font, unsigned size) {
            RichText text(font);
            text.setCharacterSize(size);

            int word_count = 0;

            for(int i = 0; i < _active_word_idx; ++i) {
                text << sf::Text::Regular << sf::Color::Green << _words[i].getDisplayText() + ' ';
                if(++word_count >= line_length) { text << '\n'; word_count = 0; }
            }

            text << sf::Text::Regular << (wrongInput() ? sf::Color::Yellow : sf::Color::Red) << activeWord().getDisplayText() + ' ';
            if(++word_count >= line_length) { text << '\n'; word_count = 0; }


            for(int i = _active_word_idx + 1; i < _words.size(); ++i) {
                text << sf::Text::Regular << sf::Color::White << _words[i].getDisplayText() + ' ';
                if(++word_count >= line_length) { text << '\n'; word_count = 0; }
            }

            return text;
        }

    private:
        int line_length = 10;

        std::vector<Word> _words;
        int _active_word_idx = 0;

        Word& activeWord() {
            return _words[_active_word_idx];
        }

        std::string _input;

        bool inputMatches() {
            return compareStr(_input, activeWord().get());
        }

        bool wrongInput() {
            return compareStr(_input, activeWord().get().substr(0, _input.size()));
        }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Typing", sf::Style::Default);

    // Load font
    int size = FONT_SMALL;
    sf::Font font;
    if(!font.loadFromFile("data/assets/fonts/Ubuntu-C.ttf")) return EXIT_FAILURE;

    Dictionary dc;
    Typing typ;
    typ.setWords(dc.getRandomWords(30));

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Down) --size;
                    else if(event.key.code == sf::Keyboard::Up) ++size;
                    else if(event.key.code == sf::Keyboard::BackSpace)
                            typ.type('\b', event.key.control);
                    else if(event.key.code == sf::Keyboard::Space ||
                            event.key.code == sf::Keyboard::Return)
                            typ.submit();
                    break;

                case sf::Event::TextEntered:
                    // ENTER
                    if(event.text.unicode == ' ') {

                    }
                    else if(event.text.unicode == '\r') {

                    }
                    else if(event.text.unicode == '\b') {

                    }
                    else if(event.text.unicode >= 32 && event.text.unicode < 127) {
                        typ.type(event.text.unicode);
                    }

                    break;
            }
        }

        window.clear();
        {
            RichText text = typ.getDisplay(font, size);
            text.setPosition(50, 20);
            window.draw(text);
        }

        {
            RichText text = typ.getInput(font, size);
            text.setPosition(50, 100);
            window.draw(text);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
