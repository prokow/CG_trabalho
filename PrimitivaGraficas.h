#pragma once
#include <SFML/Graphics.hpp>

// Esta classe encapsula todos as primitivas gráficas necessárias para a Atividade 1
// Armazena as formas como membros privados e fornece um construtor para configurá-las 
// e um método 'draw' para desenhá-las.
class PrimitivaGraficas
{
public:
    // Explicação: Carrega e configura todas as formas (posições, cores, tamanhos).
    // Pré-condição: 'font' deve ser uma referência a um objeto sf::Font válido e já carregado.
    // Pós-condição: Um objeto PrimitivaGraficas é criado, com todas as formas
    // internas (pontos, linhas, círculos, etc.) inicializadas e prontas para desenhar.
    PrimitivaGraficas(sf::Font& font);      // Construtor 
    
    // Desenha todas as formas encapsuladas nesta classe em uma janela.
    // Pré-condição: 'window' deve ser uma sf::RenderWindow aberta e válida.
    // O objeto PrimitivaGraficas deve ter sido construído corretamente.
    // Pós-condição: Todas as formas da Atividade 1 são renderizadas na 'window'.
    void draw(sf::RenderWindow& window);

private:
    // Atributos privados que compõem a atividade.
    sf::Text titleText;
    sf::RectangleShape ponto1, ponto2;
    sf::RectangleShape linhaMeio, bordaCima, bordaEsq, bordaDir, bordaBaixo;
    sf::CircleShape circulo1, circulo2;
    sf::RectangleShape retangulo1, retangulo2;
    sf::ConvexShape triangulo1, triangulo2;
};