Para trabajar con las escenas primero tenemos que crear una clase Scene que herede de una de las plantillas con la siguiente header.
```
//Header de las escenas ejemplo de la plantilla para las rooms
#include "SceneTemplate.h"
class SceneRoomTemplate: public SceneTemplate
{
	private:
	//variables de la sala (inventario...)
	public:

	SceneRoomTemplate();
	~SceneRoomTemplate();
	void refresh() override;
	void init() override;
	void unload() override;

};
```
La clase Scene tiene dos métodos virtuales el init() para iniciar los objetos de nuestra escena(meterlos en el EntityManager e iniciar el dialogueManager), de la misma manera para el unload() para descargar los objetos de memoria, el refesh() para refrescar el estado de la escena al volver de una a otra. 
Dentro del ccp debemos incluir los componentes que vamos a utilizar y meter los objetos en la escena en su init() NO EN SU CONSTRUCTORA (se explica en el SceneManager). Los objetos de la escena se crean he introducen en el EntityManager en el init().
```
//Ejemplo del cpp de la SceneRoomTemplate
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

//iniciamos la SceneTemplate en la constructora
SceneRoomTemplate::SceneRoomTemplate(): SceneTemplate()
{
}

SceneRoomTemplate::~SceneRoomTemplate()
{
	unload();
}

void SceneRoomTemplate::init()
{
	auto _fighter = entityManager->addEntity();
	auto _fighterTransform = entityManager->addComponent<Transform>(_fighter, Vector2D(10,10), Vector2D(0, 0), 500, 500, 0);
	entityManager->addComponent<Image>(_fighter, &sdlutils().images().at("prueba"));
}

void SceneRoomTemplate::unload()
{
	
}
```
## Como funciona el SceneManager
El SceneManager se crea en el game. Este contiene el vector de escenas scene[] y la lista de las escenas que se estan ejecutando en simultaneo currentScene. Ambos guardan punteros a escenas.
```
class SceneTemplate;

using namespace ecs;
class SceneManager
{
private:
	std::vector<SceneTemplate*> scenes;
	std::list<SceneTemplate*> currentscenes;
protected:
public:
	SceneManager();
	 void popScene();
	 void loadScene(int index, entity_t& extradata);
	 //pushback
	 void loadScene(int index);
	 //unload of the ram de currentscene and pop it
	 void unloadScene();
	 void render();
	 //refrest the currect scene 
	 void refrest();
	 //update of the currentscene
	 void update();
	 //delete of enitities
	~SceneManager();

};

```
scenes contiene un puntero a todas las escenas del juego, pero estas no están inicializadas (init()) solo creadas. Esto nos permite tener acceso a todas las escenas y para cargar y descargar escenas simplemente las iniciamos con init() y liberamos sus datos de memoria con unload(). 
currentscene es una lista que vamos a utilizar como el stack de tpv1, pero prefiero tener una estructura que nos de un poco mas de opciones.
Para poder cambiar entre escenas tendremos que llamar al singleton de game y hacer un get del sceneManager desde el objeto donde estemos. Para cambiar de escena podemos apilar o desapilar escenas. 
	-Para desapilar simplemente llamos a popScene() si no queremos descargar los objetos de la escena de memoria o unLoadScene si si queremos descargarla.
	-Para apilar una escena llamamos a loadScene que puede recibir un gameObject (inventario) y el indice de la escena a apilar.

Durante ejecución el sceneManager durante su update() y render() simplemente llamara al render y update de la última escena de la lista (currentScenes.end()).