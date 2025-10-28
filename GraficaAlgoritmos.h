#pragma once
#include <SFML/Graphics.hpp>
#include <string> 

// Desenha uma reta pixel a pixel usando o algoritmo de Bresenham (Atividade 2).
// Pré-condição: Uma janela SFML válida, fonte, coordenadas (x1, y1) e (x2, y2).
// Pós-condição: A reta é desenhada na janela.
void drawLineBresenham(sf::RenderWindow& window, sf::Font& font, int x1, int y1, int x2, int y2, const sf::Color& color, bool drawLabels = false);

// Desenha uma grade de fundo com rótulos de coordenadas.
// Pré-condição: Uma superfície SFML (janela), fonte, cor e espaçamento (step).
// Pós-condição: A grade é desenhada na superfície.
void drawGrid(sf::RenderWindow& surface, sf::Font& font, const sf::Color& color, int step, const sf::Color& font_color);

// Declaração da sua função cohenSutherland (Atividade 3).
// Recorta uma reta 2D contra uma janela de recorte retangular.
// Pré-condição: Janela, fonte, coordenadas da reta (x1,y1,x2,y2) e da janela (xmin,ymin,xmax,ymax).
// Pós-condição: A porção da reta (se houver) que está dentro da janela de recorte é desenhada.
void cohenSutherland(sf::RenderWindow& window, sf::Font& font, double x1, double y1, double x2, double y2, 
                                double xmin, double ymin, double xmax, double ymax,const sf::Color& color);