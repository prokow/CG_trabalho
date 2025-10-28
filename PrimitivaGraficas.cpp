#include "PrimitivaGraficas.h"

// O Construtor usa uma lista de inicialização para configurar tudo
PrimitivaGraficas::PrimitivaGraficas(sf::Font& font) :
    titleText(font), // Passa a fonte para o construtor do sf::Text
    ponto1({1.f, 1.f}),
    ponto2({1.f, 1.f}),
    linhaMeio({160, 2}),
    bordaCima({600, 2}),
    bordaEsq({2, 450}),
    bordaDir({2, 450}),
    bordaBaixo({600, 2}),
    retangulo1({150, 110}),
    retangulo2({150, 110})
{

    // Definição das cores
    const sf::Color azul = sf::Color::Blue;
    const sf::Color vermelho = sf::Color::Red;
    const sf::Color verde = sf::Color::Green;
    const sf::Color amarelo = sf::Color::Yellow;
    const sf::Color branco = sf::Color::White;
    
    // Pontos
    ponto1.setPosition(sf::Vector2f(250, 250));
    ponto1.setFillColor(vermelho);
    ponto2.setPosition(sf::Vector2f(550, 250));
    ponto2.setFillColor(vermelho);

    // Linhas
    linhaMeio.setPosition(sf::Vector2f(320, 250));
    linhaMeio.setFillColor(azul);
    bordaCima.setPosition(sf::Vector2f(100, 100));
    bordaCima.setFillColor(azul);
    bordaEsq.setPosition(sf::Vector2f(100, 100));
    bordaEsq.setFillColor(azul);
    bordaDir.setPosition(sf::Vector2f(700, 100));
    bordaDir.setFillColor(azul);
    bordaBaixo.setPosition(sf::Vector2f(100, 550));
    bordaBaixo.setFillColor(azul);

    // Círculos
    circulo1.setRadius(50);
    circulo1.setOrigin(sf::Vector2f(50, 50));
    circulo1.setPosition(sf::Vector2f(250, 250));
    circulo1.setFillColor(sf::Color::Transparent);
    circulo1.setOutlineColor(verde);
    circulo1.setOutlineThickness(2);

    circulo2.setRadius(50);
    circulo2.setOrigin(sf::Vector2f(50, 50));
    circulo2.setPosition(sf::Vector2f(550, 250));
    circulo2.setFillColor(sf::Color::Transparent);
    circulo2.setOutlineColor(verde);
    circulo2.setOutlineThickness(2);

    // Retângulos
    retangulo1.setPosition(sf::Vector2f(175, 200));
    retangulo1.setFillColor(sf::Color::Transparent);
    retangulo1.setOutlineColor(amarelo);
    retangulo1.setOutlineThickness(2);

    retangulo2.setPosition(sf::Vector2f(475, 200));
    retangulo2.setFillColor(sf::Color::Transparent);
    retangulo2.setOutlineColor(amarelo);
    retangulo2.setOutlineThickness(2);

    // Polígonos/Triângulos
    triangulo1.setPointCount(3);
    triangulo1.setPoint(0, sf::Vector2f(300, 500));
    triangulo1.setPoint(1, sf::Vector2f(550, 500));
    triangulo1.setPoint(2, sf::Vector2f(400, 400));
    triangulo1.setFillColor(sf::Color::Transparent);
    triangulo1.setOutlineColor(vermelho);
    triangulo1.setOutlineThickness(2);

    triangulo2.setPointCount(3);
    triangulo2.setPoint(0, sf::Vector2f(315, 99));
    triangulo2.setPoint(1, sf::Vector2f(475, 99));
    triangulo2.setPoint(2, sf::Vector2f(400, 35));
    triangulo2.setFillColor(sf::Color::Transparent);
    triangulo2.setOutlineColor(amarelo);
    triangulo2.setOutlineThickness(2);
}


// Método Draw 
void PrimitivaGraficas::draw(sf::RenderWindow& window)
{
    // Desenha todos os objetos da cena
    window.draw(titleText);
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
