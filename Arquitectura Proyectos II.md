CLASE VECTOR2D:
 Clase que almacena las posiciones en X e Y de un objeto
 
 - Propiedades:
    - Float X
    - Float Y
    
 - Métodos:
    - Vector2D(float x, float y)
    - float GetX() const
    - float GetY() const
    - void Set(float x, float y)
    - void SetX(float x)
    - void SetY(float y)

CLASE TRANSFORM:
 Clase que almacena y gestiona la posición, la rotación , velocidad y escala.

 - Propiedades:
	-  Vector2D Position
	-  Vector2D Rotation
	- Vector2D Scale
	- Vector2D Speed
	
 - Métodos:
	- Vector2D GetPos() const
	- Vector2D GetRot() const
	- Vector2D GetScale() const
	- Vector2D GetSpeed() const
	- void SetPos()
	- void SetRot()
	- void SetScale()
	- void SetSpeed()
		
CLAE GAMEOBJECT:
 Clase abstracta de la que heredan todas las entidades del juego.

 - Propiedades:
	- Transform: (Funcionamiento como el de Unity).
	
 - Métodos:
	- virtual void render() = 0  Se encarga del renderizado de objetos
	- virtual void update() = 0  Se encarga de actualizar el objeto cada frame

CLASE GAME:
 Clase única encargada del control y funcionamiento del juego.

 - Propiedades:
    - SceneManager
    - int vivos
    - //Cualquier cosa que se deba pasar entre escenas
 - Métodos:
    - int GetAlive() const;
    - void Death() --vivos;

CLASE SCENEMANAGER:
 Singleton encargado del funcionamiento de las escenas
 - Propiedades:
    - SceneManager ScnMngr //Intancia del SceneManager
 - Métodos:
    - LoadScene(Scene,obj) //Se encarga de hacer el push y el pop de las escenas correspondientes

TEMPLATES:
 - SCENETEMPLATE:
   Clase abstracta que almacena las cosas comunes de las escenas en general
    -  Propiedades:
        - EntityManager 
        - DialogueManager
        - Botones 
    -  Métodos:
        - Init(parametros)
        - Update()
        - Unload()
 - SCENESALATEMPLATE:
  a 


