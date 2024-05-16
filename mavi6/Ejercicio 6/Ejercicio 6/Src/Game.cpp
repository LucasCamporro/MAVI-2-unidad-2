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

	float timeElapsed;
	float spawnInterval;

	// Cuerpo de box2d
	b2Body* cuboBody;
	b2Body* bolaBody;


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
	void SpawnAndShootBall();
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
	timeElapsed = 0.0f;
	spawnInterval = 4.0f;
}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		UpdatePhysics();
		DrawGame();

		// Incrementa el tiempo transcurrido en cada iteración
		timeElapsed += frameTime;

		// Si el tiempo transcurrido es mayor o igual al intervalo de aparición, 
		// spawn y dispara una nueva bola
		if (timeElapsed >= spawnInterval)
		{
			SpawnAndShootBall();
			timeElapsed = 0.0f; // Reinicia el tiempo
		}

		wnd->display();
	}
}

void Game::SpawnAndShootBall()
{
	// Definimos la posición inicial de la bola (fuera de la pantalla)
	b2Vec2 initialPosition(cuboBody->GetPosition().x + 5, cuboBody->GetPosition().y); // Por ejemplo

	// Definimos la velocidad de la bola para que salga disparada hacia la derecha
	b2Vec2 initialVelocity(50.0f, 0.0f); // Por ejemplo, velocidad en dirección x positiva

	// Establecemos la posición y velocidad de la bola
	bolaBody->SetTransform(initialPosition, 0.0f); // La orientación es 0 para mantener la bola alineada
	bolaBody->SetLinearVelocity(initialVelocity);
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

	sf::CircleShape controlShape(5);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(bolaBody->GetPosition().x - 5, bolaBody->GetPosition().y - 5);
	wnd->draw(controlShape);

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

	cuboBody->SetAwake(true);
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		cuboBody->SetLinearVelocity(b2Vec2(0.0f, -20.0f));
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		cuboBody->SetLinearVelocity(b2Vec2(0.0f, 20.0f));
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
	cuboDefinicion.position.Set(10.0f, 10.0f); // Posición del cuerpo
	cuboBody = phyWorld->CreateBody(&cuboDefinicion); // Creamos el cuerpo dinámico
	// Creamos una forma (rectangular en este caso) para el cuerpo
	b2PolygonShape cuboShape;
	cuboShape.SetAsBox(5.0f, 5.0f); // Tamaño del cubo
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &cuboShape; // Forma asociada al cuerpo
	fixtureDef.density = 1.0f; // Densidad
	fixtureDef.friction = 0.0f; // Fricción
	fixtureDef.restitution = 0.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
	cuboBody->CreateFixture(&fixtureDef);

	b2BodyDef bolaDefinicion;
	bolaDefinicion.type = b2_dynamicBody;
	bolaDefinicion.position.Set(120.0f, 120.0f); // Posición del cuerpo
	bolaBody = phyWorld->CreateBody(&bolaDefinicion); // Creamos el cuerpo dinámico
	// Creamos una forma (esfera en este caso) para el cuerpo
	b2CircleShape bolaShape;
	bolaShape.m_radius = 5.0f; // Radio de la esfera
	// Definimos las propiedades del cuerpo
	b2FixtureDef fixtureBolaDef;
	fixtureBolaDef.shape = &bolaShape; // Forma asociada al cuerpo
	fixtureBolaDef.density = 1.0f; // Densidad
	fixtureBolaDef.friction = 0.0f; // Fricción
	fixtureBolaDef.restitution = 1.0f; // Restitución (rebote)
	// Añadimos la forma al cuerpo
	bolaBody->CreateFixture(&fixtureBolaDef);
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