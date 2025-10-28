#include "GraficaAlgoritmos.h"
#include <cmath> 
#include <vector> 
#include <string> 

// Implementação do Algoritmo de Reta (Bresenham
void drawLineBresenham(sf::RenderWindow& window, sf::Font& font, int x1, int y1, int x2, int y2, const sf::Color& color, bool drawLabels){
    // Salve as coordenadas originais para os marcadores
    const int x1_original = x1;
    const int y1_original = y1;
    // (x2 e y2 não são modificados)


    // Definido as devidas vertices para a linha
    sf::VertexArray linePoints(sf::PrimitiveType::Points);

    // Algoritmo de Bresenham
    int dx =  std::abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1; 
    int error = dx + dy;
    int e2;

    // Loop principal do algoritmo
    while (true)
    {
        // Adiciona o ponto atual à lista de vértices
        sf::Vertex vertex;
        vertex.position = sf::Vector2f(static_cast<float>(x1), static_cast<float>(y1));
        vertex.color = color;
        linePoints.append(vertex);

        // Condição de parada para quando chegamos ao ponto final
        if (x1 == x2 && y1 == y2) break;
        
        // Lógica de decisão de Bresenham
        e2 = 2 * error;
        if (e2 >= dy)       // e2 >= dy  --> move em x
        {
            error += dy;    // Atualiza o erro
            x1 += sx;       // Move em x
        }
        if (e2 <= dx)       // e2 <= dx  --> move em y
        {
            error += dx;    // Atualiza o erro
            y1 += sy;       // Move em y
        }
    }

    // Desenha a linha os pontos de uma vez
    window.draw(linePoints);

    // Este proximo bloco é onde será criado os marcadores de início e fim de cada linha
    float markerRadius = 4.f; 
    sf::Color markerFillColor = sf::Color::White;
    
    // Cria e desenha o marcador de INÍCIO
    sf::CircleShape startMarker(markerRadius);
    startMarker.setOrigin(sf::Vector2f(markerRadius, markerRadius)); 
    startMarker.setPosition(sf::Vector2f(static_cast<float>(x1_original), static_cast<float>(y1_original))); // <-- Usa as originais
    startMarker.setFillColor(markerFillColor);
    startMarker.setOutlineColor(color); 
    startMarker.setOutlineThickness(1.f);
    window.draw(startMarker);

    // Cria e desenha o marcador de FIM
    sf::CircleShape endMarker(markerRadius);
    endMarker.setOrigin(sf::Vector2f(markerRadius, markerRadius)); 
    endMarker.setPosition(sf::Vector2f(static_cast<float>(x2), static_cast<float>(y2))); // y2 e x2 nunca mudaram
    endMarker.setFillColor(markerFillColor);
    endMarker.setOutlineColor(color);
    endMarker.setOutlineThickness(1.f);
    window.draw(endMarker);
}


// Função Auxiliar onde desenha uma grade com rótulos
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




// Constantes para a Atividade 3 (Cohen-Sutherland) 
// É Usado bits para definir as 4 regiões
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000


// Calcula um código binário (OutCode) de 4 bits que identifica
//         em qual região o ponto (x, y) está em relação à janela de recorte.
// Pré-condição: Coordenadas do ponto (x, y) e limites da janela (xmin, ymin, xmax, ymax).
// Pós-condição: Retorna um inteiro (bitmask) onde cada bit representa
// uma região (TOP, BOTTOM, RIGHT, LEFT). Retorna 0 (INSIDE) se o ponto estiver dentro da janela.
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

// Implementa o algoritmo de recorte de linha Cohen-Sutherland.
// Ele determina qual (se houver) segmento da reta (x1,y1)-(x2,y2) é visível dentro da área de recorte (xmin,ymin)-(xmax,ymax).
// Se um segmento for visível, ele chama 'drawLineBresenham' para desenhá-lo.
// Pré-condição: 'window' e 'font' válidas. Coordenadas da reta (x1, y1, x2, y2) e limites da janela de recorte (xmin, ymin, xmax, ymax).
// Pós-condição: Se a linha for 'aceita' (total ou parcialmente), o segmento visível é desenhado na 'window'.
void cohenSutherland(
    sf::RenderWindow& window, sf::Font& font,
    double x1, double y1, double x2, double y2, 
    double xmin, double ymin, double xmax, double ymax,
    const sf::Color& color)
{
    // Calcula os outcodes dos dois pontos
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

    // Se a linha foi aceita (total ou parcialmente)...
    if (accept) {
        // 3. REUTILIZA sua função original para desenhar o resultado!
        // (Convertemos de volta para int, pois Bresenham espera inteiros)
        // (Não passamos 'true' para os rótulos, para não poluir a tela)
        drawLineBresenham(window, font, static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2), color);
    }
}