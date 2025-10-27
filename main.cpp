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
    int dy = std::abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int error = dx - dy;
    int e2;
    while (true)
    {
        sf::Vertex vertex;
        vertex.position = sf::Vector2f(static_cast<float>(x1), static_cast<float>(y1));
        vertex.color = color;
        linePoints.append(vertex);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * error;
        if (e2 > -dy) { error -= dy; x1 += sx; }
        if (e2 < dx) { error += dx; y1 += sy; }
    }
    window.draw(linePoints);
}

void drawGrid(sf::RenderWindow& surface, sf::Font& font, const sf::Color& color, int step, const sf::Color& font_color)
{
    float width = static_cast<float>(surface.getSize().x);
    float height = static_cast<float>(surface.getSize().y);

    // Usa VertexArray para eficiência (desenha todas as linhas de uma vez)
    sf::VertexArray gridLines(sf::PrimitiveType::Lines);
    
    sf::Text label(font);
    label.setCharacterSize(14);
    label.setFillColor(font_color);

    // Linhas verticais e rótulos X
    for (float x = 0; x < width; x += step)
    {
        gridLines.append({sf::Vector2f(x, 0), color});
        gridLines.append({sf::Vector2f(x, height), color});
        
        label.setString(std::to_string(static_cast<int>(x)));
        label.setPosition(sf::Vector2f(x + 2, 2));
        surface.draw(label);
    }

    // Linhas horizontais e rótulos Y
    for (float y = 0; y < height; y += step)
    {
        gridLines.append({sf::Vector2f(0, y), color});
        gridLines.append({sf::Vector2f(width, y), color});

        label.setString(std::to_string(static_cast<int>(y)));
        label.setPosition(sf::Vector2f(2.f, y + 2.f));
        surface.draw(label);
    }
    
    surface.draw(gridLines);
}


int main(){

    sf::Vector2u windowSize(1280, 800);
    sf::RenderWindow window(sf::VideoMode(windowSize), "Trabalho 1 - Computacao Grafica");
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

    // --- MUDANÇA: Criar o retângulo de fundo do menu ---
    sf::RectangleShape menuBackground;
    menuBackground.setFillColor(sf::Color::Yellow);
    // A posição e o tamanho serão definidos dinamicamente dentro do loop

    // --- Atividade 1: Criar Primitivas Gráficas ---
  // Definindo as cores
    const sf::Color azul = sf::Color::Blue;
    const sf::Color vermelho = sf::Color::Red;
    const sf::Color verde = sf::Color::Green;
    const sf::Color amarelo = sf::Color::Yellow;
    const sf::Color branco = sf::Color::White;

    // Título (usando a fonte já carregada)
    sf::Text titleText(font);
    titleText.setString("Exercicio 1: Primitiva Grafica (como ponto, retas, circulos, poligonos, etc.)");
    titleText.setCharacterSize(20);
    titleText.setFillColor(branco);
    titleText.setPosition(sf::Vector2f(10, 10));
    
    // 1. Pontos (put_pixel) - Usando RectangleShape de 1x1
    sf::RectangleShape ponto1({1.f, 1.f});
    ponto1.setPosition(sf::Vector2f(250, 250));
    ponto1.setFillColor(vermelho);

    sf::RectangleShape ponto2({1.f, 1.f});
    ponto2.setPosition(sf::Vector2f(550, 250));
    ponto2.setFillColor(vermelho);

    // 2. Linhas (pygame.draw.line)
    sf::RectangleShape linhaMeio({160, 2}); // (480-320) = 160
    linhaMeio.setPosition(sf::Vector2f(320, 250));
    linhaMeio.setFillColor(azul);

    // As 4 linhas da borda
    sf::RectangleShape bordaCima({600, 2}); // (700-100)
    bordaCima.setPosition(sf::Vector2f(100, 100));
    bordaCima.setFillColor(azul);

    sf::RectangleShape bordaEsq({2, 450}); // (550-100)
    bordaEsq.setPosition(sf::Vector2f(100, 100));
    bordaEsq.setFillColor(azul);

    sf::RectangleShape bordaDir({2, 450}); // (550-100)
    bordaDir.setPosition(sf::Vector2f(700, 100));
    bordaDir.setFillColor(azul);

    sf::RectangleShape bordaBaixo({600, 2}); // (700-100)
    bordaBaixo.setPosition(sf::Vector2f(100, 550));
    bordaBaixo.setFillColor(azul);

    // 3. Círculos (pygame.draw.circle)
    sf::CircleShape circulo1;
    circulo1.setRadius(50);
    circulo1.setOrigin(sf::Vector2f(50, 50)); // << IMPORTANTE: move a âncora para o centro
    circulo1.setPosition(sf::Vector2f(250, 250)); // Agora (250, 250) é o centro
    circulo1.setFillColor(sf::Color::Transparent);
    circulo1.setOutlineColor(verde);
    circulo1.setOutlineThickness(2);

    sf::CircleShape circulo2;
    circulo2.setRadius(50);
    circulo2.setOrigin(sf::Vector2f(50, 50)); // << IMPORTANTE
    circulo2.setPosition(sf::Vector2f(550, 250)); // Agora (550, 250) é o centro
    circulo2.setFillColor(sf::Color::Transparent);
    circulo2.setOutlineColor(verde);
    circulo2.setOutlineThickness(2);

    // 4. Retângulos (pygame.draw.rect)
    sf::RectangleShape retangulo1({150, 110});
    retangulo1.setPosition(sf::Vector2f(175, 200));
    retangulo1.setFillColor(sf::Color::Transparent);
    retangulo1.setOutlineColor(amarelo);
    retangulo1.setOutlineThickness(2);

    sf::RectangleShape retangulo2({150, 110});
    retangulo2.setPosition(sf::Vector2f(475, 200));
    retangulo2.setFillColor(sf::Color::Transparent);
    retangulo2.setOutlineColor(amarelo);
    retangulo2.setOutlineThickness(2);

    // 5. Polígonos/Triângulos (pygame.draw.polygon)
    sf::ConvexShape triangulo1;
    triangulo1.setPointCount(3);
    triangulo1.setPoint(0, sf::Vector2f(300, 500));
    triangulo1.setPoint(1, sf::Vector2f(550, 500));
    triangulo1.setPoint(2, sf::Vector2f(400, 400));
    triangulo1.setFillColor(sf::Color::Transparent);
    triangulo1.setOutlineColor(vermelho);
    triangulo1.setOutlineThickness(2);

    sf::ConvexShape triangulo2;
    triangulo2.setPointCount(3);
    triangulo2.setPoint(0, sf::Vector2f(315, 99));
    triangulo2.setPoint(1, sf::Vector2f(475, 99));
    triangulo2.setPoint(2, sf::Vector2f(400, 35));
    triangulo2.setFillColor(sf::Color::Transparent);
    triangulo2.setOutlineColor(amarelo);
    triangulo2.setOutlineThickness(2);
    
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
        menuStream << " - pressione o numero da atividade desejada - \n";
        menuStream << "1. primitivas graficas " << (atividadeAtual == 1 ? "(X)" : "") << "\n";
        menuStream << "2. ponto medio " << (atividadeAtual == 2 ? "(X)" : "") << "\n";
        menuStream << "3. cohen sutherland " << (atividadeAtual == 3 ? "(X)" : "") << "\n";
        menuStream << "4. desenha um poligono " << (atividadeAtual == 4 ? "(X)" : "") << "\n";
        menuStream << "5. scanline" << (atividadeAtual == 5 ? "(X)" : "");
        text.setString(menuStream.str());

        // --- MUDANÇA 3: Calcular Posição Y dinamicamente ---
        float padding = 10.f; // Nosso espaçamento da borda

        // getLocalBounds nos dá o tamanho real do bloco de texto
        sf::Rect<float> localTextBounds = text.getLocalBounds(); 
        
        // A altura total do texto
        float textHeight = localTextBounds.size.y; 
        
        // O "top" do texto (fontes não começam em y=0)
        float textTopOffset = localTextBounds.position.y; 

        // CÁLCULO: (Altura da Janela) - (Altura do Texto) - (Offset do Topo) - (Padding do Fundo)
        float newY = static_cast<float>(windowSize.y) - textHeight - textTopOffset - (padding * 2);
        
        // Define a posição do texto com o Y calculado
        text.setPosition(sf::Vector2f(padding, newY));
        // --- FIM DA MUDANÇA ---


        // 2. Ajustar o fundo do menu
        // (Este código agora usa a nova posição e funciona perfeitamente)
        sf::Rect<float> textBounds = text.getGlobalBounds(); 
        menuBackground.setSize(sf::Vector2f(
            textBounds.size.x + (padding * 2),
            textBounds.size.y + (padding * 2)
        ));
        menuBackground.setPosition(sf::Vector2f(
            textBounds.position.x - padding,
            textBounds.position.y - padding
        ));

        // 2. Desenhar a atividade correta
        if (atividadeAtual == 1) {
            drawGrid(window, font, sf::Color(50, 50, 50), 50, sf::Color(180, 180, 180));
            window.draw(ponto1);
            window.draw(ponto2);
            window.draw(linhaMeio);
            window.draw(bordaCima);
            window.draw(bordaEsq);
            window.draw(bordaDir);
            window.draw(bordaBaixo);
            window.draw(circulo1);
            window.draw(circulo2);
            window.draw(retangulo1);
            window.draw(retangulo2);
            window.draw(triangulo1);
            window.draw(triangulo2);
        } 
        else if (atividadeAtual == 2) {

            drawGrid(window, font, sf::Color(50, 50, 50), 50, sf::Color(180, 180, 180));

            drawLineBresenham(window, 50, 50, 700, 550, sf::Color::Red);
            drawLineBresenham(window, 100, 500, 600, 100, sf::Color::Green);
            drawLineBresenham(window, 50, 300, 750, 300, sf::Color::Blue);    // Horizontal
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