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
	b2Body* inclinategroundBody;


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
	sf::RectangleShape groundShape(sf::Vector2f(70, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(75, 25);
	groundShape.setRotation(120);
	wnd->draw(groundShape);

	sf::RectangleShape groundShape12(sf::Vector2f(500, 5));
	groundShape12.setFillColor(sf::Color::Red);
	groundShape12.setPosition(0, 95);
	wnd->draw(groundShape12);

	sf::RectangleShape groundShape1(sf::Vector2f(500, 5));
	groundShape1.setFillColor(sf::Color::Red);
	groundShape1.setPosition(0, 0);
	wnd->draw(groundShape1);

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

	b2Body* groundBody12 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
	groundBody12->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* groundBody2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
	groundBody2->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Body* groundBody3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody3->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

	b2Body* groundBody4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
	groundBody4->SetTransform(b2Vec2(100.0f, 0.0f), 0.0f);

	// Creamos un cuerpo definido por el usuario
	b2BodyDef inclinateGroundDefinicion;
	inclinateGroundDefinicion.type = b2_staticBody;
	inclinateGroundDefinicion.position.Set(20.0f, 45.0f); // Posición del cuerpo
	inclinategroundBody = phyWorld->CreateBody(&inclinateGroundDefinicion); // Creamos el cuerpo dinámico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape inclinateGroundShape;
	float width = 80.0f;
	float height = 90.0f;
	inclinateGroundShape.SetAsBox(width / 1.0f, height / 1.0f, b2Vec2(width / 1.0f, height / 1.0f), b2_pi / 6.0f); // Tamaño del cubo, posición y rotación
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureinclinateGroundDef;
	fixtureinclinateGroundDef.shape = &inclinateGroundShape; // Forma asociada al cuerpo
	fixtureinclinateGroundDef.density = 1.0f; // Densidad
	fixtureinclinateGroundDef.friction = 0.50f; // Fricción
	fixtureinclinateGroundDef.restitution = 0.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
	inclinategroundBody->CreateFixture(&fixtureinclinateGroundDef);

	// Creamos un cuerpo definido por el usuario
	b2BodyDef cuboDefinicion;
	cuboDefinicion.type = b2_dynamicBody;
	cuboDefinicion.position.Set(55.0f, 10.0f); // Posición del cuerpo
	cuboBody = phyWorld->CreateBody(&cuboDefinicion); // Creamos el cuerpo dinámico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape cuboShape;
	cuboShape.SetAsBox(5.0f, 5.0f); // Tamaño del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &cuboShape; // Forma asociada al cuerpo
	fixtureDef.density = 1.0f; // Densidad
	fixtureDef.friction = 0.50f; // Fricción
	fixtureDef.restitution = 0.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
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