#include <iostream>
#include <SFML/Graphics.hpp> //Biblioteca Gráfica SFML
#include <variant>
#include <sstream>
#include <string>

// Arquivos das Atividades
#include "GraficaAlgoritmos.h"
#include "PrimitivaGraficas.h"
#include "Poligono.h" 
using namespace std;


int main(){
    
    // Configuração da Janela
    sf::Vector2u windowSize(950, 800);
    sf::RenderWindow window(sf::VideoMode(windowSize), "Trabalho 1 - Computacao Grafica");
    window.setFramerateLimit(60);

    // Carregamento da Fonte 
    sf::Font font;
    if (!font.openFromFile("./Fontes/data-latin.ttf")) {
        std::cout << "Erro ao carregar a fonte." << std::endl;
        return -1;
    }

    // Configuração do Menu no canto inferior esquerdo da tela
    sf::RectangleShape menuBackground;
    menuBackground.setFillColor(sf::Color::Yellow);
    sf::Text text(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);


    // -- Desenho combinando primitivas gráficas -- 
    // Instância a classe da atividade 1, passando a fonte carregada
    PrimitivaGraficas atividade1(font);


    sf::RenderTexture canvas;
    //canvas.create(windowSize.x, windowSize.y);
    sf::Color backgroundColor = sf::Color(64, 64, 64);
    sf::ConvexShape polygonToFill; // (Configure seu polígono aqui)
    sf::Vector2i seedPoint(400, 450); // (Configure seu ponto semente aqui)
    sf::Color fillColor = sf::Color::Red;
    //sf::Sprite canvasSprite;
    
    int atividadeAtual = 1; // váriavel para controlar a atividade atual 

    // Obtém o handle da instância do Windows (necessário para as Atividades 4 e 5)
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    // Loop Principal da aplicação, que mantém a janela aberta e responde a eventos
    while (window.isOpen())
    {
        // Processa todos os eventos da janela (mouse, teclado, etc.)
        while (auto opt_event = window.pollEvent())
        {
            sf::Event event = *opt_event;
            
            // Condição para fechar a janela 
            if(const auto* closed = event.getIf<sf::Event::Closed>())
            {
                window.close();
            }

            // Condição para cada tecla numérica pressionada
            else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Num1) { atividadeAtual = 1; }
                else if (keyPressed->code == sf::Keyboard::Key::Num2) { atividadeAtual = 2; }
                else if (keyPressed->code == sf::Keyboard::Key::Num3) { atividadeAtual = 3; }
                else if (keyPressed->code == sf::Keyboard::Key::Num4) { atividadeAtual = 4; }
                else if (keyPressed->code == sf::Keyboard::Key::Num5) { atividadeAtual = 5; }
            }
        }
    

        // Limpa a janela com a cor de fundo
        window.clear(backgroundColor); 

        // Lógica do Menu no canto inferior esquerdo da tela, para mudar entre atividades
        std::ostringstream menuStream;
        menuStream << " pressione um numero para escolher:\n";
        menuStream << "1. primitivas graficas " << (atividadeAtual == 1 ? "(X)" : "") << "\n";
        menuStream << "2. ponto medio para retas " << (atividadeAtual == 2 ? "(X)" : "") << "\n";
        menuStream << "3. recorte cohen-Sutherland " << (atividadeAtual == 3 ? "(X)" : "") << "\n";
        menuStream << "4. desenha poligono (opengl) " << (atividadeAtual == 4 ? "(X)" : "") << "\n";
        menuStream << "5. preenchimento poligono (opengl) " << (atividadeAtual == 5 ? "(X)" : "");
        text.setString(menuStream.str()); // Atualiza o texto do menu

        // Cálculo Dinâmico da Posição do Menu (para ficar sempre no canto inferior esquerdo)--
        float padding = 10.f; // Nosso espaçamento da borda

        // getLocalBounds nos dá o tamanho real do bloco de texto
        sf::Rect<float> localTextBounds = text.getLocalBounds(); 
        
        // A altura total do texto
        float textHeight = localTextBounds.size.y; 
        
        // O "top" do texto (fontes não começam em y=0)
        float textTopOffset = localTextBounds.position.y; 

        // (Altura da Janela) - (Altura do Texto) - (Offset do Topo) - (Padding do Fundo)
        float newY = static_cast<float>(windowSize.y) - textHeight - textTopOffset - (padding * 2);
        
        // Define a posição do texto com o Y calculado
        text.setPosition(sf::Vector2f(padding, newY));
        

        // Ajusta o fundo (menuBackground) para se ajustar ao tamanho e posição do texto        
        sf::Rect<float> textBounds = text.getGlobalBounds(); 
        menuBackground.setSize(sf::Vector2f(
            textBounds.size.x + (padding * 2),
            textBounds.size.y + (padding * 2)
        ));
        menuBackground.setPosition(sf::Vector2f(
            textBounds.position.x - padding,
            textBounds.position.y - padding
        ));



        // Condições de cada atividade, cada atividade selecionada será chamada e mostrado na tela
        if (atividadeAtual == 1) {  // Atividade 1: Primitivas Gráficas (SFML)
            drawGrid(window, font, sf::Color(50, 50, 50), 50, sf::Color(180, 180, 180));
            atividade1.draw(window); 
        } 
        else if (atividadeAtual == 2) { // Atividade 2: Algoritmo de Reta (Bresenham)
            drawGrid(window, font, sf::Color(50, 50, 50), 50, sf::Color(180, 180, 180));
            drawLineBresenham(window, font, 50, 50, 700, 550, sf::Color::Red, true);
            drawLineBresenham(window, font, 100, 500, 600, 100, sf::Color::Green, true);
            drawLineBresenham(window, font, 50, 300, 750, 300, sf::Color::Blue, true);
            drawLineBresenham(window, font, 400, 50, 400, 550, sf::Color::Yellow, true);
        }
        else if (atividadeAtual == 3){  // Atividade 3: Recorte de Reta (Cohen-Sutherland)
            
            
            drawGrid(window, font, sf::Color(50, 50, 50), 50, sf::Color(180, 180, 180));
            
            // Define a janela de recorte (contorno branco)
            const double clipXMin = 200, clipYMin = 150;
            const double clipXMax = 600, clipYMax = 450;
            sf::RectangleShape clipWindow(sf::Vector2f(clipXMax - clipXMin, clipYMax - clipYMin));
            clipWindow.setPosition(sf::Vector2f(clipXMin, clipYMin));
            clipWindow.setFillColor(sf::Color::Transparent);
            clipWindow.setOutlineColor(sf::Color::White);
            clipWindow.setOutlineThickness(2.f);
            window.draw(clipWindow);    // Desenha a janela de recorte
           
            // Com o recorte, podemos chamar a função do nosso arquivo GraficaAlgoritmos.h
            // Linha 1: Totalmente dentro (Aceite Trivial)
            cohenSutherland(window, font, 250, 200, 550, 400, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Green);
            
            // Linha 2: Totalmente fora, acima (Rejeição Trivial)
            cohenSutherland(window, font, 250, 50, 550, 100, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Blue);
            
            // Linha 3: Totalmente fora, à esquerda (Rejeição Trivial)
            cohenSutherland(window, font, 50, 200, 150, 400, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Red);
            
            // Linha 4: Cruzando (Recorte Necessário)
            cohenSutherland(window, font, 100, 100, 700, 500, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Yellow);
            
            // Linha 5: Cruzando (Recorte Necessário)
            cohenSutherland(window, font, 300, 500, 500, 100, clipXMin, clipYMin, clipXMax, clipYMax, sf::Color::Magenta);

        }
        else if(atividadeAtual == 4){   // Atividade 4: Polígono 3D Wireframe (OpenGL)
            // Quando selecionada esta atividade, chama o executável externo
            // e acaba fechando a aplicação por completo quando clicamos em fechar esta janela
            return Poligono(hInstance, nullptr, nullptr, SW_SHOWNORMAL);
        }
        else if(atividadeAtual == 5){   // Atividade 5: Polígono 3D Preenchido (OpenGL)
            // Quando selecionada esta atividade, chama o executável externo
            // e acaba fechando a aplicação por completo quando clicamos em fechar esta janela
            return WinMain(hInstance, nullptr, nullptr, SW_SHOWNORMAL);
        }

        // Desenhar o menu (por cima de tudo)
        window.draw(menuBackground); 
        window.draw(text);           
        
        window.display();
    }

    return 0;
}