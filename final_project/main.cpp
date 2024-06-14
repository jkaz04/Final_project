#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

class Rope : public sf::Sprite {
private:
    sf::Texture rope_texture_;

public:
    Rope(const std::string& path) {
        if (!rope_texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture " << path << std::endl;
        }
        setTexture(rope_texture_);
    }
};

class FirstBlock : public sf::Sprite {
private:
    sf::Texture block_texture_;

public:
    FirstBlock(const std::string& path) {
        if (!block_texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture " << path << std::endl;
        }
        setTexture(block_texture_);
    }

    void f_block_position(float x) {
        setPosition(x, getPosition().y);
    }
};

class Block : public sf::Sprite {
private:
    float swing_speed;
    int falling_speed;
    sf::Texture block_texture_;
    float l_bound_;
    float r_bound_;
    bool moving_right_;
    bool is_falling_;
    Rope* attached_rope_;

public:
    Block(const std::string& path) {
        if (!block_texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture " << path << std::endl;
        }
        setTexture(block_texture_);
        moving_right_ = true;
        is_falling_ = false;
    }

    void set_swing_speed(const float& speed, const int& fall_speed) {
        swing_speed = speed;
        falling_speed = fall_speed;
    }

    void increment_speed() {
        swing_speed += 0.3;
    }

    void setBounds(const float& l_bound, const float& r_bound) {
        l_bound_ = l_bound;
        r_bound_ = r_bound;
    }

    void attachRope(Rope* rope) {
        attached_rope_ = rope;
    }

    void swing() {
        if (is_falling_) return;

        sf::FloatRect block_bounds = getGlobalBounds();

        if (block_bounds.left <= l_bound_) {
            moving_right_ = true;
        }
        if (block_bounds.left + block_bounds.width >= r_bound_) {
            moving_right_ = false;
        }

        if (moving_right_) {
            move(swing_speed, 0);
        } else {
            move(-swing_speed, 0);
        }

        if (attached_rope_ != nullptr) {
            sf::FloatRect rope_bounds = attached_rope_->getGlobalBounds();
            attached_rope_->setPosition(getPosition().x + (block_bounds.width / 2) - (rope_bounds.width / 2), 0);
        }
    }

    void falling() {
        if (is_falling_) {
            move(0, falling_speed);
        }
    }

    void startFalling() {
        is_falling_ = true;
    }

    void stopFalling() {
        is_falling_ = false;
    }

    bool isFalling() const {
        return is_falling_;
    }

    void reset() {
        setPosition(430, 20);
        moving_right_ = true;
        is_falling_ = false;
    }

    void reset_speed(){
        swing_speed = 0.3;
    }
};

class Background : public sf::Sprite {
private:
    sf::Texture background_texture_;
    float move_speed_;

public:
    Background(const std::string& path, float move_speed) {
        if (!background_texture_.loadFromFile(path)) {
            std::cerr << "Could not load background texture " << path << std::endl;
        }
        setTexture(background_texture_);
        setPosition(0, -360);
        move_speed_ = move_speed;
    }

    void moveBackground() {
        move(0, move_speed_);
    }

    void reset() {
        setPosition(0, -360);
    }
};

class Cloud : public sf::Sprite {
private:
    sf::Texture cloud_texture_;
    float move_speed_;

public:
    Cloud(const std::string& path, float move_speed) {
        if (!cloud_texture_.loadFromFile(path)) {
            std::cerr << "Could not load cloud texture " << path << std::endl;
        }
        setTexture(cloud_texture_);
        setPosition(960, 260);
        move_speed_ = move_speed;
    }

    void moveCloud() {
        move(-move_speed_, 0);
        if (getPosition().x + getGlobalBounds().width < 0) {
            setPosition(960, 260);
        }
    }
};


int main() {
    sf::RenderWindow window(sf::VideoMode(960, 720), "My window");
    sf::Clock clock;
    int move_count = 0;
    bool game_over = false;
    bool game_started = false;

    Block block("C:/Users/kazmi/OneDrive/Dokumenty/GitHub/Final_project/final_project/game_graphs/block.jpg");
    block.setPosition(430, 20);
    block.set_swing_speed(0.3, 1);
    block.setBounds(0, window.getSize().x);

    Rope rope("C:/Users/kazmi/OneDrive/Dokumenty/GitHub/Final_project/final_project/game_graphs/rope.jpg");
    rope.setPosition(478, 0);

    block.attachRope(&rope);

    FirstBlock f_block("C:/Users/kazmi/OneDrive/Dokumenty/GitHub/Final_project/final_project/game_graphs/block.jpg");
    f_block.setPosition(430, 560);

    FirstBlock s_block("C:/Users/kazmi/OneDrive/Dokumenty/GitHub/Final_project/final_project/game_graphs/block.jpg");
    s_block.setPosition(430, 660);

    Background background("C:/Users/kazmi/OneDrive/Dokumenty/GitHub/Final_project/final_project/game_graphs/background.jpg", 0.3);
    Cloud cloud("C:/Users/kazmi/OneDrive/Dokumenty/GitHub/Final_project/final_project/game_graphs/cloud.jpg", 0.3);


    sf::Font font;
    if (!font.loadFromFile("C:/Users/kazmi/OneDrive/Dokumenty/GitHub/Final_project/final_project/game_graphs/Blox2.ttf")) {
        std::cerr << "Could not load font" << std::endl;
        return -1;
    }


    sf::Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(48);
    score_text.setFillColor(sf::Color::Black);
    score_text.setPosition(10, 10);


    sf::Text game_over_text;
    game_over_text.setFont(font);
    game_over_text.setCharacterSize(100);
    game_over_text.setFillColor(sf::Color::Black);
    game_over_text.setString("GAME OVER");
    game_over_text.setPosition(480 - game_over_text.getGlobalBounds().width / 2, 360 - game_over_text.getGlobalBounds().height / 2);

    sf::Text start_text;
    start_text.setFont(font);
    start_text.setCharacterSize(48);
    start_text.setFillColor(sf::Color::Black);
    start_text.setString("Press SPACE to begin");
    start_text.setPosition(480 - start_text.getGlobalBounds().width / 2, 360 - start_text.getGlobalBounds().height / 2);

    sf::RectangleShape restart_button(sf::Vector2f(200, 80));
    restart_button.setFillColor(sf::Color(100, 100, 100));
    restart_button.setOutlineThickness(2);
    restart_button.setOutlineColor(sf::Color::Black);
    restart_button.setPosition(380, 500);

    sf::Text restart_text;
    restart_text.setFont(font);
    restart_text.setCharacterSize(36);
    restart_text.setFillColor(sf::Color::White);
    restart_text.setString("Restart");
    restart_text.setPosition(restart_button.getPosition().x + 50, restart_button.getPosition().y + 20);

    int score = 0;

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (game_over && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                if (restart_button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))) {

                    game_over = false;
                    score = 0;
                    move_count = 0;
                    block.reset();
                    block.reset_speed();
                    background.reset();

                    f_block.setPosition(430, 560);
                    s_block.setPosition(430, 660);
                    score_text.setString("Score: 0");
                }
            }

            if (!game_started && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                game_started = true;
            }
        }

        if (game_started && !game_over) {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                block.startFalling();
            }
        }

        if (!game_over) {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                block.startFalling();
            }

            block.swing();
            block.falling();

            if (block.isFalling() && block.getGlobalBounds().intersects(f_block.getGlobalBounds())) {
                block.stopFalling();
                block.increment_speed();
                s_block.f_block_position(f_block.getPosition().x);
                f_block.f_block_position(block.getPosition().x);
                block.reset();
                move_count++;
                score++;
                score_text.setString("Score: " + std::to_string(score));
            }


            if (block.isFalling() && block.getPosition().y > window.getSize().y) {
                game_over = true;
            }

            if (move_count < 2 && block.isFalling()) {
                background.moveBackground();
            }

            cloud.moveCloud();
        }

        window.clear();
        window.draw(background);
        window.draw(cloud);
        window.draw(f_block);
        window.draw(s_block);
        window.draw(block);
        window.draw(rope);
        window.draw(score_text);

        if (!game_started) {
            window.draw(start_text);
        }

        if (game_over) {
            window.draw(game_over_text);
            window.draw(restart_button);
            window.draw(restart_text);
        }

        window.display();
    }

    return 0;
}


