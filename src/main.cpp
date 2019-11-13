

#include <mutex>

#include <sys/resource.h>
#include <resolver.h>

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <math.h>

void displayGraphique(IBoard* board){
	

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");
    sf::RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    
	
	sf::View view(sf::FloatRect(0,0,600,600));
	window.setView(view);

	sf::Text val;
	sf::Font font;
	font.loadFromFile("../src/Font/arial.ttf");
	val.setFont(font);
	val.setCharacterSize(20);
	val.setStyle(sf::Text::Bold);
	val.setColor(sf::Color::Black);
	//val.setFillColor(sf::Color::Black);
    while(window.isOpen()){
		window.clear(sf::Color(50,50,50));
		for(long int y = 0;y < board->getHeight();y++){
			for(long int x = 0;x < board->getLength();x++){
				rectangle.setPosition(x*100,y*100);

				rectangle.setFillColor((((x+y)&1) == 1? sf::Color(131,142,131) : sf::Color(100,64,62)));

				window.draw(rectangle);

				IPiece* t = board->getPiece(y,x);
				val.setString(std::to_string(t->getTop()));
				val.setPosition(x*100+45,y*100+10);
				window.draw(val);

				val.setString(std::to_string(t->getBottom()));
				val.setPosition(x*100+45,y*100+67);
				window.draw(val);

				val.setString(std::to_string(t->getLeft()));
				val.setPosition(x*100+13,y*100+40);
				window.draw(val);

				val.setString(std::to_string(t->getRight()));
				val.setPosition(x*100+73,y*100+40);
				window.draw(val);

			}
		}
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if(event.type == sf::Event::KeyPressed){
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
					view.move(sf::Vector2f(-10.f,0.f));
					
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) view.move(10,0);
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) view.move(0,10);
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) view.move(0,-10);
				//sf::View view(sf::FloatRect(pos.x,pos.y,pos.x+600,pos.y+600));
				window.setView(view);
			}
        }

        
        window.display();
    }

}



void displayGraphiqueOpti(IBoard* board){
	

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");
    sf::RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    
	
	sf::View view(sf::FloatRect(0,0,600,600));
	window.setView(view);

	sf::Text val;
	sf::Font font;
	font.loadFromFile("../src/Font/arial.ttf");
	val.setFont(font);
	val.setCharacterSize(20);
	val.setStyle(sf::Text::Bold);
	val.setColor(sf::Color::Black);
	//val.setFillColor(sf::Color::Black);
	int yP = 0, xP = 0;
    while(window.isOpen()){
		window.clear(sf::Color(50,50,50));
		
		for(long int y = (yP > 0?floor(yP/100):0) ;y < floor(yP/100)+7 && y < board->getHeight();y++){
			for(long int x = (xP > 0 ?floor(xP/100):0);x <floor(xP/100) + 7 && x <board->getLength();x++){
				rectangle.setPosition(x*100,y*100);

				rectangle.setFillColor((((x+y)&1) == 1? sf::Color(131,142,131) : sf::Color(100,64,62)));

				window.draw(rectangle);

				IPiece* t = board->getPiece(y,x);
				val.setString(std::to_string(t->getTop()));
				val.setPosition(x*100+45,y*100+10);
				window.draw(val);

				val.setString(std::to_string(t->getBottom()));
				val.setPosition(x*100+45,y*100+67);
				window.draw(val);

				val.setString(std::to_string(t->getLeft()));
				val.setPosition(x*100+13,y*100+40);
				window.draw(val);

				val.setString(std::to_string(t->getRight()));
				val.setPosition(x*100+73,y*100+40);
				window.draw(val);

			}
			
		}
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if(event.type == sf::Event::KeyPressed){
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
					xP -= 10;
					view.move(sf::Vector2f(-10.f,0.f));
					
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
					xP += 10;
					view.move(10,0);
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
					yP += 10;
					view.move(0,10);
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
					yP -= 10;
					view.move(0,-10);
				}
				//sf::View view(sf::FloatRect(pos.x,pos.y,pos.x+600,pos.y+600));
				window.setView(view);
			}
        }

        
        window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

}









int main(int argc, char const *argv[])
{

	
    const rlim_t kStackSize = 100 * 1024 * 1024;   // min stack size = 100 MB
    struct rlimit rl;
	
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

	std::string nameFile = "";
	std::ifstream file;
	do{
		std::cout<<"Fichier à utiliser : ";
		std::cin>>nameFile;
		file = std::ifstream(nameFile, std::ios::in);
		if(!file) std::cout<<"Erreur fichier inexistant"<<std::endl;
	
	}while(!file);
	

	file.close();

	std::string moreSoluce;
	do{

		std::cout<<"Trouver toutes les solution ? (y/n) ";
		std::cin>>moreSoluce;
	}while(moreSoluce != "y" && moreSoluce != "n");
	
	
	std::cout<<"Méthode à utiliser :"<<std::endl;
	std::cout<<"\t1 - resolveSequencielRec"<<std::endl;
	std::cout<<"\t2 - resolveSequencielIt"<<std::endl;
	std::cout<<"\t3 - resolveParrallele1"<<std::endl;
	std::cout<<"\t4 - resolveParrallele2"<<std::endl;
	std::cout<<"\t5 - resolveParrallele3"<<std::endl;
	std::cout<<"\t6 - resolveParrallele4Rec"<<std::endl;
	std::cout<<"\t7 - resolveParrallele4It"<<std::endl;
	std::cout<<"\t8 - All"<<std::endl;
	std::cout<<"\t9 - All without resolveParrallele2 and resolveParrallele3"<<std::endl;
	

	std::string numFunction;
	do{
		std::cout<<"Votre choix : ";
		std::cin>>numFunction;
	}while(numFunction!="1" && numFunction!="2" && numFunction!="3" && numFunction!="4" && numFunction!="5" && numFunction!="6" && numFunction!="7" && numFunction!="8" && numFunction!="9");

	Resolver* theResolver =  new Resolver();
	theResolver->load(nameFile);
	std::vector<IBoard*> list;

	if(numFunction=="1" || numFunction=="8" || numFunction=="9"){
		list = theResolver->resolveSequencielRec(moreSoluce == "n");
	}
	if(numFunction=="2" || numFunction=="8" || numFunction=="9"){
		list = theResolver->resolveSequencielIt(moreSoluce == "n");
	}
	if(numFunction=="3" || numFunction=="8" || numFunction=="9"){
		list = theResolver->resolveParrallele1(moreSoluce == "n");
	}
	if(numFunction=="4" || numFunction=="8"){
		list = theResolver->resolveParrallele2(moreSoluce == "n");
	}
	if(numFunction=="5" || numFunction=="8"){
		list = theResolver->resolveParrallele3(moreSoluce == "n");
	}
	if(numFunction=="6" || numFunction=="8" || numFunction=="9"){
		list = theResolver->resolveParrallele4Rec(moreSoluce == "n");
	}
	if(numFunction=="7" || numFunction=="8" || numFunction=="9"){
		list = theResolver->resolveParrallele4It(moreSoluce == "n");
	}

	
	for (unsigned int i = 0; i < list.size(); i++) displayGraphiqueOpti(list[i]);
	
    return 0;
}