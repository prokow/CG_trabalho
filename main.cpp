#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector> // Necessário para sf::VertexArray
#include <cmath>  // Necessário para std::abs (valor absoluto)
#include <fstream> // Para ler arquivos de fonte como fallback
#include <iterator> // Para std::istreambuf_iterator
#include <variant>
#include <sstream> // Necessário para construir o menu
#include <string>

#include "Poligono.cpp"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

// --- Atividade 2: Implementação do Algoritmo de Reta (Bresenham) ---
void drawLineBresenham(sf::RenderWindow& window, int x1, int y1, int x2, int y2, const sf::Color& color)
{
    // --- MUDANÇA: Salva as coordenadas originais ---
    const int x1_original = x1;
    const int y1_original = y1;
    // (x2 e y2 não são modificados, então estão seguros)

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
        if (e2 >= dy)
        {
            error += dy;
            x1 += sx; // <-- x1 é modificado aqui
        }
        if (e2 <= dx)
        {
            error += dx;
            y1 += sy; // <-- y1 é modificado aqui
        }
    }

    // 1. Desenha a linha (os pixels) de uma só vez
    window.draw(linePoints);

    // --- Bloco dos Marcadores (agora usa as coordenadas salvas) ---
    
    float markerRadius = 4.f; 
    sf::Color markerFillColor = sf::Color::White;
    
    // 3. Cria e desenha o marcador de INÍCIO
    sf::CircleShape startMarker(markerRadius);
    startMarker.setOrigin(sf::Vector2f(markerRadius, markerRadius)); 
    startMarker.setPosition(sf::Vector2f(static_cast<float>(x1_original), static_cast<float>(y1_original))); // <-- Usa as originais
    startMarker.setFillColor(markerFillColor);
    startMarker.setOutlineColor(color); 
    startMarker.setOutlineThickness(1.f);
    window.draw(startMarker);

    // 4. Cria e desenha o marcador de FIM
    sf::CircleShape endMarker(markerRadius);
    endMarker.setOrigin(sf::Vector2f(markerRadius, markerRadius)); 
    endMarker.setPosition(sf::Vector2f(static_cast<float>(x2), static_cast<float>(y2))); // y2 e x2 nunca mudaram
    endMarker.setFillColor(markerFillColor);
    endMarker.setOutlineColor(color);
    endMarker.setOutlineThickness(1.f);
    window.draw(endMarker);
}


// --- Função Auxiliar: Desenha uma grade com rótulos ---
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




// --- Constantes para a Atividade 3 (Cohen-Sutherland) ---
// Usamos bits para definir as 4 regiões
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// --- Atividade 3: Função Auxiliar para calcular o "OutCode" ---
// Esta função descobre em qual região um ponto (x, y) está.
// Note que em SFML, Y cresce para baixo (y_min é o topo, y_max é o fundo)
int computeOutCode(double x, double y, double xmin, double ymin, double xmax, double ymax)
{
    int code = INSIDE; // Começa assumindo que está dentro

    if (x < xmin)      // À esquerda da janela
        code |= LEFT;
    else if (x > xmax) // À direita da janela
        code |= RIGHT;
    if (y < ymin)      // Acima da janela
        code |= TOP;
    else if (y > ymax) // Abaixo da janela
        code |= BOTTOM;

    return code;
}

// --- Atividade 3: Função "Gerente" (Calcula e Desenha) ---
// Esta função implementa o algoritmo de Cohen-Sutherland.
// Ela REUTILIZA sua função drawLineBresenham para o desenho final.
void cohenSutherlandClipAndDraw(
    sf::RenderWindow& window, sf::Font& font,
    double x1, double y1, double x2, double y2, 
    double xmin, double ymin, double xmax, double ymax,
    const sf::Color& color)
{
    // 1. Calcula os outcodes dos dois pontos
    int outcode1 = computeOutCode(x1, y1, xmin, ymin, xmax, ymax);
    int outcode2 = computeOutCode(x2, y2, xmin, ymin, xmax, ymax);
    bool accept = false;

    while (true) {
        // Caso 1: Rejeição Trivial (Ambos os pontos na mesma região externa)
        // (ex: ambos acima, ou ambos à esquerda)
        if ((outcode1 & outcode2) != 0) {
            break; // A linha está totalmente fora. Não desenha nada.
        }
        
        // Caso 2: Aceite Trivial (Ambos os pontos dentro da janela)
        if ((outcode1 | outcode2) == 0) {
            accept = true;
            break; // A linha está totalmente dentro.
        }

        // Caso 3: Recorte Necessário
        // Pelo menos um ponto está fora.
        double x = 0, y = 0;

        // Escolhe o ponto que está fora
        int outcodeOut = (outcode1 != 0) ? outcode1 : outcode2;

        // Calcula o ponto de interseção
        // (m = (y2-y1) / (x2-x1))
        if (outcodeOut & TOP) {           // Ponto está acima
            x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
            y = ymin;
        } else if (outcodeOut & BOTTOM) { // Ponto está abaixo
            x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
            y = ymax;
        } else if (outcodeOut & RIGHT) {  // Ponto está à direita
            y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
            x = xmax;
        } else if (outcodeOut & LEFT) {   // Ponto está à esquerda
            y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
            x = xmin;
        }

        // Agora movemos o ponto "externo" para o ponto de interseção
        if (outcodeOut == outcode1) {
            x1 = x;
            y1 = y;
            outcode1 = computeOutCode(x1, y1, xmin, ymin, xmax, ymax);
        } else {
            x2 = x;
            y2 = y;
            outcode2 = computeOutCode(x2, y2, xmin, ymin, xmax, ymax);
        }
    }

    // 2. Se a linha foi aceita (total ou parcialmente)...
    if (accept) {
        // 3. REUTILIZA sua função original para desenhar o resultado!
        // (Convertemos de volta para int, pois Bresenham espera inteiros)
        // (Não passamos 'true' para os rótulos, para não poluir a tela)
        drawLineBresenham(window, static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2), color);
    }
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

    HINSTANCE hInstance = GetModuleHandle(nullptr);

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
        else if (atividadeAtual == 3){
            // 1. REUTILIZA sua função de grade
            drawGrid(window, font, sf::Color(50, 50, 50), 50, sf::Color(180, 180, 180));

            // 2. Define e desenha a janela de recorte (um retângulo branco)
            const double clipXMin = 200, clipYMin = 150;
            const double clipXMax = 600, clipYMax = 450;
            
            sf::RectangleShape clipWindow;
            clipWindow.setPosition(sf::Vector2f(clipXMin, clipYMin));
            clipWindow.setSize(sf::Vector2f(clipXMax - clipXMin, clipYMax - clipYMin));
            clipWindow.setFillColor(sf::Color::Transparent);
            clipWindow.setOutlineColor(sf::Color::White);
            clipWindow.setOutlineThickness(2.f);
            window.draw(clipWindow);

            // 3. Chama a função de recorte para várias linhas de teste
            
            // Linha 1: Totalmente dentro (Aceite Trivial)
            cohenSutherlandClipAndDraw(window, font, 250, 200, 550, 400, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Green);
            
            // Linha 2: Totalmente fora, acima (Rejeição Trivial)
            cohenSutherlandClipAndDraw(window, font, 250, 50, 550, 100, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Red);
            
            // Linha 3: Totalmente fora, à esquerda (Rejeição Trivial)
            cohenSutherlandClipAndDraw(window, font, 50, 200, 150, 400, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Red);
            
            // Linha 4: Cruzando (Recorte Necessário)
            cohenSutherlandClipAndDraw(window, font, 100, 100, 700, 500, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Yellow);
            
            // Linha 5: Cruzando (Recorte Necessário)
            cohenSutherlandClipAndDraw(window, font, 300, 500, 500, 100, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Magenta);
        }
        else if(atividadeAtual == 4){
            return Poligono(hInstance, nullptr, nullptr, SW_SHOWNORMAL);
        }
        else if(atividadeAtual == 5){
            return WinMain(hInstance, nullptr, nullptr, SW_SHOWNORMAL);
        }

        // --- MUDANÇA: Desenhar o fundo e DEPOIS o texto ---
        window.draw(menuBackground); // 3. Desenha o retângulo PRIMEIRO
        window.draw(text);           // 4. Desenha o texto por CIMA
        
        window.display();
    }

    return 0;
}