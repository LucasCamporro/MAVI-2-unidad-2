#include "Box2DHelper.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <tchar.h>

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2World* phyWorld;

	//tiempo de frame
	float frameTime;
	int fps;

	// Cuerpo de box2d
	b2Body* cuboBody;
	b2Body* groundBody;


public:

	// Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void CreateEnemy(int x, int y);
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};

Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();

}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{
	// Dibujamos el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	groundShape.setPosition(0, 0);
	wnd->draw(groundShape);

	sf::RectangleShape groundShape2(sf::Vector2f(5, 500));
	groundShape2.setFillColor(sf::Color::Red);
	groundShape2.setPosition(95, 0);
	wnd->draw(groundShape2);

	groundShape2.setPosition(0, 0);
	wnd->draw(groundShape2);

	// Dibujamos el cuerpo de control (c�rculo)
	sf::RectangleShape cuboShape(sf::Vector2f(10, 10));
	cuboShape.setFillColor(sf::Color::Magenta);
	cuboShape.setPosition(cuboBody->GetPosition().x - 5, cuboBody->GetPosition().y - 5);
	wnd->draw(cuboShape);

}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close();
			break;
		}
	}

	// Movemos el cuerpo
	cuboBody->SetAwake(true);
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		cuboBody->SetLinearVelocity(b2Vec2(-50.0f, 0.0f));
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		cuboBody->SetLinearVelocity(b2Vec2(50.0f, 0.0f));
	}
}


// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Creamos un piso y paredes

	b2Body* groundBody2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
	groundBody2->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Body* groundBody3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody3->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Body* groundBody4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody4->SetTransform(b2Vec2(100.0f, 0.0f), 0.0f);

	// Creamos un cuerpo definido por el usuario
	b2BodyDef groundDefinicion;
	groundDefinicion.type = b2_staticBody;
	groundDefinicion.position.Set(0.0f, 95.0f); // Posici�n del cuerpo
	groundBody = phyWorld->CreateBody(&groundDefinicion); // Creamos el cuerpo din�mico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape groundShape;
	groundShape.SetAsBox(95.0f, 0.0f); // Tama�o del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixturegroundDef;
	fixturegroundDef.shape = &groundShape; // Forma asociada al cuerpo
	fixturegroundDef.density = 1.0f; // Densidad
	fixturegroundDef.friction = 0.50f; // Fricci�n
	fixturegroundDef.restitution = 0.0f; // Restituci�n (rebote)
	// A�adimos la forma al cuerpo
	groundBody->CreateFixture(&fixturegroundDef);

	// Creamos un cuerpo definido por el usuario
	b2BodyDef cuboDefinicion;
	cuboDefinicion.type = b2_dynamicBody;
	cuboDefinicion.position.Set(50.0f, 90.0f); // Posici�n del cuerpo
	cuboBody = phyWorld->CreateBody(&cuboDefinicion); // Creamos el cuerpo din�mico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape cuboShape;
	cuboShape.SetAsBox(5.0f, 5.0f); // Tama�o del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &cuboShape; // Forma asociada al cuerpo
	fixtureDef.density = 1.0f; // Densidad
	fixtureDef.friction = 0.50f; // Fricci�n
	fixtureDef.restitution = 0.0f; // Restituci�n (rebote)
	// A�adimos la forma al cuerpo
	cuboBody->CreateFixture(&fixtureDef);
}

Game::~Game(void)
{ }

using namespace sf;

int _tmain(int argc, _TCHAR* argv[])
{

	Game* Juego;
	Juego = new Game(800, 600, "ventana eje 1");
	Juego->Loop();

	return 0;
}