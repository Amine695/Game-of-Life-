﻿#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "ecosysteme.hpp"
#include "constantes.hpp"

/*
Initialisation du texte du Jeu
*/
void Ecosystem::initTextGame()
{
    // chargement police
    if(!font.loadFromFile(POLICE))
    {
        std::cerr << "Erreur, impossible de charger " << POLICE << std::endl;
        exit(EXIT_FAILURE);
    }
    play.setFont(font);
    play.setCharacterSize(50);
    play.setFillColor(COULEUR_TXT_PLAY);
    play.setString("Press ENTER to start !");
    play.setPosition(650,400);

    // initialisation texte pour les statistiques
    sheepText.setFont(font);
    sheepText.setCharacterSize(TAILLE_POLICE);
    sheepText.setFillColor(COULEUR_SHEEP);
    sheepText.setPosition(POSITION_STATISTIQUE);

    WolfText.setFont(font);
    WolfText.setCharacterSize(TAILLE_POLICE);
    WolfText.setFillColor(COULEUR_WOLF);
    WolfText.setPosition(POSITION_STATISTIQUE);

    statText.setFont(font);
    statText.setCharacterSize(TAILLE_POLICE);
    statText.setFillColor(COULEUR_TEXT);
    statText.setPosition(POSITION_STATISTIQUE);

    // initialisation texte pour les commandes
    commandText.setFont(font);
    commandText.setCharacterSize(TAILLE_POLICE);
    commandText.setFillColor(COULEUR_TEXT);
    commandText.setPosition(POSITION_COMMANDE);

}

/*
Initialisation du text et image du Menu
*/
void Ecosystem::initTextMenu()
{
    font1.loadFromFile(POLICE_MENU2);
    font2.loadFromFile(POLICE_MENU);
    button.loadFromFile(BUTTON_IMG);
    txt1.setFont(font2);
    txt2.setFont(font1);
    txt1.setString("GAME OF LIFE");
    txt2.setString(L"          Sheep vs Wolf \nWhich species will survive ?");
    txt1.setFillColor(sf::Color(243,222,170));
    txt2.setFillColor(sf::Color::Black);
    txt1.setCharacterSize(60);
    txt2.setCharacterSize(40);
    txt1.setPosition(550,10);
    txt2.setPosition(590,150);

}

/*
constructeur
*/
Ecosystem::Ecosystem():
    timer(0.0f),
    timeSpeed(1.0f),
    paused(false),
    finished(true),
    showStats(true)
{
    initTextGame();
    sheeps.init(NB_SHEEPS_DEBUT);
    wolves.init(NB_WOLVES_DEBUT);
    foods.init(NB_FOODS);

}

/*
destructeur
*/
Ecosystem::~Ecosystem() {}


/*
Mise à jour de la simulation
*/
void Ecosystem::update()
{
    // Update animals
    sheeps.update(*foods.getList(), timeSpeed);
    wolves.update(*sheeps.getList(),timeSpeed);
    // Update timer
    timer += clock.getElapsedTime().asSeconds() * timeSpeed;
}


/*
Affichage des statistiques
*/
void Ecosystem::drawStats()
{
    std::stringstream ss,ss1,ss2;
    ss << "Time: " << timer << "\n"
        << "Speed: x" << timeSpeed << "\n\n";
    ss1 << "\n\nSheeps: " << sheeps.getNb() << "\n";
    ss2 << "\n\n\nWolves: " << wolves.getNb() << "\n";
    ss  << "\nFoods eaten: " << sheeps.getNbEaten() << "\n"
        << "Sheeps killed: " << wolves.getNbEaten() << "\n"
        << "Sheeps born: " << sheeps.getNbBirths() << "\n"
        << "Wolves born: " << wolves.getNbBirths() << "\n";
    if (paused)
        ss << "Pause";
    
    sheepText.setString(ss1.str());
    WolfText.setString(ss2.str());
    statText.setString(ss.str());
    window.draw(statText);
    window.draw(sheepText);
    window.draw(WolfText);
    
}

/**
 * Afichage des commandes du jeu
 */
void Ecosystem::drawCommands()
{
    std::stringstream ss;
    ss  << "Right arrow: Speed x2\n"
        << "Left arrow: Speed x0.5\n"
        << "S: show/hide statistics\n"
        << " P: pause\n";
    commandText.setString(ss.str());
    window.draw(commandText);
}


/*
On réaffiche la fenêtre
*/
void Ecosystem::redraw()
{
    bgGame.loadFromFile(BACKGROUNG_IMG);
    sf::Sprite s5(bgGame);
	s5.setScale(1,1.5);
    window.draw(s5);
    window.draw(play);
    if(!finished)
    {
        window.draw(sheeps);
        window.draw(wolves);
        window.draw(foods);
    }
    if(showStats)
        drawStats();  

    drawCommands();
    window.display();
}

/*
Redémarrer la simulation
*/
void Ecosystem::restart()
{
    sheeps.init(NB_SHEEPS_DEBUT);
    wolves.init(NB_WOLVES_DEBUT);
    foods.init(NB_FOODS);
    finished = false;
    showStats = true;
}


/**
 * Fonction vérifiant si le sprite contient le curseur de la souris
 * dont les coordonnées sont stockées dans v
 * @param sprite : un FloatRect
 * @param v : un Vector2f
 * @return true si sprite contient v, false sinon
*/
bool Ecosystem::checkCursor(sf::FloatRect sprite, sf::Vector2f v)
{
    if(sprite.contains(v))
        return true;
    return false;
}

/*
Fonction qui run la première fenêtre
*/
void Ecosystem::runMenu()
{

    sf::RenderWindow window(sf::VideoMode(LARGEUR_FENETRE_MENU,HAUTEUR_FENETRE_MENU),FENETRE_MENU);
    initTextMenu();
    sf::Sprite s1,s2;
    s1.setTexture(button);
    bgMenu.loadFromFile(BACKGROUNG_MENU);
    s2.setTexture(bgMenu);
    WolfMusic();
    MenuMusic();
    
    while (window.isOpen())
    {
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			if (windowEvent.type == sf::Event::Closed)
				window.close();

			if(checkCursor(s1.getGlobalBounds(), sf::Vector2f(windowEvent.mouseButton.x, windowEvent.mouseButton.y)) == true)
			{
				if(windowEvent.type == sf::Event::MouseButtonReleased &&  windowEvent.mouseButton.button == sf::Mouse::Left)
				{
                    MenuMusicStop();
                    StartMusic();
					window.close();
                    runGame();
				}
			}
		}
		
		s1.setPosition(690,620);
		s1.setScale(0.5,0.5);
        
        window.draw(s2);
        window.draw(s1);

		window.draw(txt1);
		window.draw(txt2);
		
		window.display();
    }

}



/**
 * Fonction qui exécute le script python permettant de ploter nos résultats avec matplotlib
 * Save le plot sous format png dans le dossier plot
*/
void Ecosystem::savetoPNG()
{
    char buff[FILENAME_MAX]; //buffer contenant le path
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    std::string command = "python3 ";
    command +=  current_working_dir;
    command += "/python/plot.py";
    system(command.c_str());
}



/*
Fonction qui lance le jeu
*/
void Ecosystem::runGame()
{
    //on crée la fenêtre
    window.create(sf::VideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE), FENETRE_JEU);
    window.setFramerateLimit(FREQUENCE_RAFRAICHISSEMENT_IMG);
    inGameMusic();
    // durée pour controler la vitesse d'animation
    int currentColor = 1;
    float duration = float();
    std::ofstream file("data/data.txt"); 

    while(window.isOpen())
    {
        sf::Event event;
        sf::Time dt = clock1.restart();
        duration += dt.asSeconds();
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                //window closed
                case sf::Event::Closed:
                    window.close();
                    break;

                // key pressed
                case sf::Event::KeyPressed:
                    switch(event.key.code)
                    {
                        //start
                        case sf::Keyboard::Enter:
                            if(finished)
                            {
                                play.setScale(0,0);//on enleve le texte
                                restart();
                            }
                            break;
                        //pause
                        case sf::Keyboard::P:
                            paused = !paused;
                            break;
                        //statistiques
                        case sf::Keyboard::S:
                            showStats = !showStats;
                            break;
                        // speed up
                        case sf::Keyboard::Right:
                            timeSpeed *= 2;
                            break;
                        //slow down
                        case sf::Keyboard::Left:
                            timeSpeed /=2;
                            break;

                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if(duration > 0.01f)
        {
            duration = 0;
            if(currentColor < 200)
                currentColor += 5;
            else
                currentColor = 0;
            play.setFillColor(sf::Color(0,currentColor,0));
        }

        if(!paused && !finished)
        {
            update();
            file<< timer <<"\n"
                << sheeps.getNb() << "\n"
                << wolves.getNb() << "\n";

            if(sheeps.empty() && wolves.empty())
            {
                GameSound.stop();
                finished = true;
                file.close();
                savetoPNG();
                
            }

        }
        clock.restart();
        redraw();
    }
}