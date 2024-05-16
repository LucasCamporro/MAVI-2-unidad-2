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
	b2Body* obstaculoBody;
	b2Body* obstaculo2Body;


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

	// Dibujamos el cuerpo de control (círculo)
	sf::RectangleShape cuboShape(sf::Vector2f(10, 10));
	cuboShape.setFillColor(sf::Color::Magenta);
	cuboShape.setPosition(cuboBody->GetPosition().x - 5, cuboBody->GetPosition().y - 5);
	wnd->draw(cuboShape);

	sf::RectangleShape ObstaculoShape(sf::Vector2f(11, 11)); // Ajusta el tamaño según lo necesites
	ObstaculoShape.setFillColor(sf::Color::Blue);
	ObstaculoShape.setPosition(obstaculoBody->GetPosition().x - 5, obstaculoBody->GetPosition().y - 5);
	wnd->draw(ObstaculoShape);

	sf::RectangleShape Obstaculo2Shape(sf::Vector2f(11, 11)); // Ajusta el tamaño según lo necesites
	Obstaculo2Shape.setFillColor(sf::Color::Green);
	Obstaculo2Shape.setPosition(obstaculo2Body->GetPosition().x - 5, obstaculo2Body->GetPosition().y - 5);
	wnd->draw(Obstaculo2Shape);

	

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
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* groundBody2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
	groundBody2->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Body* groundBody3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody3->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Body* groundBody4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody4->SetTransform(b2Vec2(100.0f, 0.0f), 0.0f);

	// Creamos un cuerpo definido por el usuario
	b2BodyDef cuboDefinicion;
	cuboDefinicion.type = b2_dynamicBody;
	cuboDefinicion.position.Set(50.0f, 30.0f); // Posición del cuerpo
	cuboBody = phyWorld->CreateBody(&cuboDefinicion); // Creamos el cuerpo dinámico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape cuboShape;
	cuboShape.SetAsBox(5.0f, 5.0f); // Tamaño del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &cuboShape; // Forma asociada al cuerpo
	fixtureDef.density = 1.0f; // Densidad
	fixtureDef.friction = 0.5f; // Fricción
	fixtureDef.restitution = 1.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
	cuboBody->CreateFixture(&fixtureDef);

	// Creamos un cuerpo definido por el usuario
	b2BodyDef ObstaculoDefinicion;
	ObstaculoDefinicion.type = b2_staticBody;
	ObstaculoDefinicion.position.Set(30.0f, 30.0f); // Posición del cuerpo
	obstaculoBody = phyWorld->CreateBody(&ObstaculoDefinicion); // Creamos el cuerpo dinámico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape ObstaculoShape;
	ObstaculoShape.SetAsBox(5.5f, 5.5f); // Tamaño del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureObstaculoDef;
	fixtureObstaculoDef.shape = &ObstaculoShape; // Forma asociada al cuerpo
	fixtureObstaculoDef.density = 1.0f; // Densidad
	fixtureObstaculoDef.friction = 0.5f; // Fricción
	fixtureObstaculoDef.restitution = 1.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
	obstaculoBody->CreateFixture(&fixtureObstaculoDef);

	// Creamos un cuerpo definido por el usuario
	b2BodyDef Obstaculo2Definicion;
	Obstaculo2Definicion.type = b2_staticBody;
	Obstaculo2Definicion.position.Set(80.0f, 80.0f); // Posición del cuerpo
	obstaculo2Body = phyWorld->CreateBody(&Obstaculo2Definicion); // Creamos el cuerpo dinámico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape Obstaculo2Shape;
	Obstaculo2Shape.SetAsBox(5.5f, 5.5f); // Tamaño del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureObstaculo2Def;
	fixtureObstaculo2Def.shape = &Obstaculo2Shape; // Forma asociada al cuerpo
	fixtureObstaculo2Def.density = 1.0f; // Densidad
	fixtureObstaculo2Def.friction = 0.5f; // Fricción
	fixtureObstaculo2Def.restitution = 1.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
	obstaculo2Body->CreateFixture(&fixtureObstaculo2Def);


	// Asignamos una velocidad inicial al cuerpo
	float velocidadX = -25.0f; // Velocidad horizontal inicial
	float velocidadY = -5.0f; // Velocidad vertical inicial (hacia arriba)
	cuboBody->SetLinearVelocity(b2Vec2(velocidadX, velocidadY));
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