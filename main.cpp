#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector> // Necessário para sf::VertexArray
#include <cmath>  // Necessário para std::abs (valor absoluto)
#include <fstream> // Para ler arquivos de fonte como fallback
#include <iterator> // Para std::istreambuf_iterator
#include <variant>
#include <sstream> // Necessário para construir o menu

using namespace std;

// --- Atividade 2: Implementação do Algoritmo de Reta (Bresenham) ---
void drawLineBresenham(sf::RenderWindow& window, int x1, int y1, int x2, int y2, const sf::Color& color)
{
    // (Seu código de Bresenham - sem alterações)
    sf::VertexArray linePoints(sf::PrimitiveType::Points);
    int dx =  std::abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int error = dx + dy;
    int e2;
    while (true)
    {
        sf::Vertex vertex;
        vertex.position = sf::Vector2f(static_cast<float>(x1), static_cast<float>(y1));
        vertex.color = color;
        linePoints.append(vertex);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * error;
        if (e2 >= dy) { error += dy; x1 += sx; }
        if (e2 <= dx) { error += dx; y1 += sy; }
    }
    window.draw(linePoints);
}


int main(){

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Trabalho 1 - Computacao Grafica");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("./Fontes/data-unifon.ttf")) {
        std::cout << "Erro ao carregar a fonte." << std::endl;
        return -1;
    }

    sf::Text text(font);
    text.setCharacterSize(20);
    // --- MUDANÇA: Texto preto para contrastar com o fundo amarelo ---
    text.setFillColor(sf::Color::Black); 
    // Posição ajustada para caber o menu de 5 linhas
    text.setPosition(sf::Vector2f(10.f, 460.f)); 

    // --- MUDANÇA: Criar o retângulo de fundo do menu ---
    sf::RectangleShape menuBackground;
    menuBackground.setFillColor(sf::Color::Yellow);
    // A posição e o tamanho serão definidos dinamicamente dentro do loop

    // --- Atividade 1: Criar Primitivas Gráficas ---
    // (Seu código de criação de primitivas - sem alterações)
    sf::CircleShape point(2); 
    point.setFillColor(sf::Color::Red);
    point.setPosition(sf::Vector2f(10, 10));
    sf::RectangleShape line(sf::Vector2f(100, 2)); 
    line.setFillColor(sf::Color::Green);
    line.setPosition(sf::Vector2f(10, 30));
    sf::CircleShape circle(50); 
    circle.setFillColor(sf::Color::Blue);
    circle.setOutlineThickness(5);
    circle.setOutlineColor(sf::Color(250, 150, 100));
    circle.setPosition(sf::Vector2f(200, 200));
    sf::RectangleShape rectangle(sf::Vector2f(120, 80)); 
    rectangle.setFillColor(sf::Color::Magenta);
    rectangle.setPosition(sf::Vector2f(400, 150));
    sf::CircleShape triangle(60, 3); 
    triangle.setFillColor(sf::Color::Yellow);
    triangle.setOutlineThickness(3);
    triangle.setOutlineColor(sf::Color::Black);
    triangle.setPosition(sf::Vector2f(550, 300));
    sf::ConvexShape convexPolygon;
    convexPolygon.setPointCount(5); 
    convexPolygon.setPoint(0, sf::Vector2f(600, 400));
    convexPolygon.setPoint(1, sf::Vector2f(650, 380));
    convexPolygon.setPoint(2, sf::Vector2f(680, 420));
    convexPolygon.setPoint(3, sf::Vector2f(640, 470));
    convexPolygon.setPoint(4, sf::Vector2f(590, 450));
    convexPolygon.setFillColor(sf::Color(100, 200, 50));
    convexPolygon.setOutlineThickness(2);
    convexPolygon.setOutlineColor(sf::Color::White);
    
    int atividadeAtual = 1; 

    while (window.isOpen())
    {
        // (Seu loop de eventos - sem alterações)
        while (auto opt_event = window.pollEvent())
        {
            sf::Event event = *opt_event;
            if(const auto* closed = event.getIf<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Num1) {
                    atividadeAtual = 1;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num2) {
                    atividadeAtual = 2;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num3) {
                    atividadeAtual = 3;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num4) {
                    atividadeAtual = 4;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num5) {
                    atividadeAtual = 5;
                }
            }
        }
    
        window.clear(sf::Color(64, 64, 64)); 

        // --- Bloco de Texto e Desenho ---

        // 1. Construir a string do menu
        std::ostringstream menuStream;
        menuStream << " pressione o numero da atividade desejada:\n";
        menuStream << "1. Primitivas Graficas " << (atividadeAtual == 1 ? "(atual)" : "") << "\n";
        menuStream << "2. Ponto Medio " << (atividadeAtual == 2 ? "(atual)" : "") << "\n";
        menuStream << "3. Cohen Sutherland " << (atividadeAtual == 3 ? "(atual)" : "") << "\n";
        menuStream << "4. Desenha um Poligono " << (atividadeAtual == 4 ? "(atual)" : "") << "\n";
        menuStream << "5. Scanline" << (atividadeAtual == 5 ? "(atual)" : "");
        text.setString(menuStream.str());

        // --- MUDANÇA: Ajustar o fundo ao texto ---
        // Pegamos o tamanho exato do texto e adicionamos um "padding" (espaçamento)
        sf::Rect<float> textBounds = text.getGlobalBounds(); 
        float padding = 10.f; // 10 pixels de espaçamento
        
        menuBackground.setSize(sf::Vector2f(textBounds.size.x + (padding * 2), textBounds.size.y + (padding * 2)));

        // Posicionamos o fundo 10 pixels acima e à esquerda do texto
        menuBackground.setPosition(sf::Vector2f(text.getPosition().x - padding, text.getPosition().y - padding));
        // Fim do ajuste do fundo

        // 2. Desenhar a atividade correta
        if (atividadeAtual == 1) {
            window.draw(point);
            window.draw(line);
            window.draw(circle);
            window.draw(rectangle);
            window.draw(triangle);
            window.draw(convexPolygon);
        } 
        else if (atividadeAtual == 2) {
            drawLineBresenham(window, 50, 50, 700, 550, sf::Color::Red);
            drawLineBresenham(window, 100, 500, 600, 100, sf::Color::Green);
            drawLineBresenham(window, 50, 300, 750, 300, sf::Color::Blue); 
            drawLineBresenham(window, 400, 50, 400, 550, sf::Color::Yellow);
        }
        // ... (outros 'else if' para atividades 3, 4, 5) ...

        // --- MUDANÇA: Desenhar o fundo e DEPOIS o texto ---
        window.draw(menuBackground); // 3. Desenha o retângulo PRIMEIRO
        window.draw(text);           // 4. Desenha o texto por CIMA
        
        window.display();
    }

    return 0;
}