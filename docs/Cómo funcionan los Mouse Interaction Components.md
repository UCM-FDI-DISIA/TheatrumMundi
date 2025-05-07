
Los componentes del tipo `MouseIntaractionComponent` permiten suscribir una entidad con un Arae2D a varios eventos de ratón.

  ##### Tipos de MouseIntaractionComponent:
	 ClickComponent
     TriggerComponent
     DragComponent

### ClickComponent
ClickComponent ofrece los eventos `JUST_CLICKED` y `JUST_RELEASED`, a los que es posible vincular el número de funciones que se deseen.

##### Ejemplo de uso:
```
entityManager->addComponent<RectArea2D>(entity, layerMan); // Debe haber un Area2D
ClickComponent* clk = entityManager->addComponent<ClickComponent>(entity);

// Al hacer click en la entidad se mostrará "CLICKED" en la consola
clk->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });
```

##### Métodos públicos:

`bool isBeingClicked()`: 
Devuelve true si el objeto no ha sido soltado después hacer click sobre él y false en el caso contrario.

### TriggerComponent
TriggerComponent ofrece los eventos `CURSOR_ENTERED`, `CURSOR_LEFT`, `AREA_ENTERED` y `AREA_LEFT`, para cuando se entra y se sale del Area2D de la entidad ya sea con el cursor o con otra entidad con Area2D.

##### Ejemplo de uso:
```
entityManager->addComponent<RectArea2D>(entity, layerMan); // Debe haber un Area2D
TriggerComponent* trg = entityManager->addComponent<TriggerComponent>(entity);

// Al entrar el cursor en el area del objeto se muestra "MOUSE" y al salir "NO MOUSE"
trg->connect(TriggerComponent::CURSOR_ENTERED, []() { std::cout << "MOUSE\n";  });
trg->connect(TriggerComponent::CURSOR_LEFT, []() { std::cout << "NO MOUSE\n";  });

// Al entrar una entidad en el area del objeto se musetra "HELLO" y "BYE"
tgr->connect(TriggerComponent::AREA_ENTERED, []() { std::cout << "HELLO\n"; });
tgr->connect(TriggerComponent::AREA_LEFT, []() { std::cout << "BYE\n"; });
```

##### Métodos públicos:

`bool mouseIsIn()`:
Devuelve true si el cursor está dentro del Area2D y false en caso contrario.

`const std::list<ecs::entity_t>& getOverlappingEntities()`:
Devuelve la lista de elementos que se estén solapando con la entidad.

`std::list<ecs::entity_t> const& triggerContextEntities()`:
Se llama desde la función de callback, devuelve la lista de entidades que han provocado el evento de trigger para dar contexto de qué entidades han entrado o salido.
```
tgr->connect(TriggerComponent::AREA_ENTERED, []() { 
	
	auto colliders = trg->triggerContextEntities(); // Entities that triggered
	
	for (ecs::entity_t e : colliders) {
		if (e->getMngr()->getComponent<ClickComponent>(e) != nullptr)
			std::cout << "It is a clickable object!!!\n";
	}
});
```

### DragComponent
DragComponent ofrece los eventos `DRAG_START`, `DRAG` y `DRAG_END`, para cuando se hace pincha en el Area2D, cuando se mueve el objeto, y para cuando se suelta el objeto.

##### Ejemplo de uso:
```
entityManager->addComponent<RectArea2D>(entity, layerMan); // Debe haber un Area2D
DragComponent* drg = entityManager->addComponent<DragComponent>(entity);

// Al arrastrar el objeto se muestra "DRAGGING"
drg->connect(DragComponent::DRAG, []() { std::cout << "DRAGGING\n"; });
```

___
### Detección de capas

Por defecto los componentes de este tipo ya detectan si la entidad está siendo solapada (si se produce *overlapping*), por lo que si hay dos objetos uno encima del otro con un componente de este tipo cada uno, solo el de más adelante detectará una acción de ratón. 

Pero cabe destacar que para que el componente detecte capas de profundidad es necesario construir el componente de `Area2D` con una puntero a un objeto `Area2DLayerManager`, que no es más que un contenedor de tipo `std::list<Area2D*>` almacenados en lista con el que los componentes de tipo `MouseInteractionComponent` pueden determinar si están siendo clickeados directamente.

Si se desea no tener en cuenta la profundidad de los objetos, es posible desactivar esta funcionalidad de capas añadiendo la siguiente línea:
```
DragComponent* cmp = ...; // o ClickComponent, o TriggerComponent
cmp->setLayerOpposition(false);
```

___
### Posibles errores
##### Error al asignar el evento en la función `conect`
Es posible que a un `MouseIntaractionComponent` se le esté asignando un evento que el componente no es capaz de escuchar porque pertenece a otro tipo de componente.
```
TriggerComponent* trg = entityManager->addComponent<TriggerComponent>(_fighter);

// Un componente de Trigger no escucha el evento de "just clicked" 
trg->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "ENTERED\n";  });
```

##### La entidad no tiene componente de tipo `Area2D`
Los componentes `MouseIntaractionComponent` necesitan un `Area2D` para saber dónde está actuando el ratón.
```
entityManager->addComponent<RectArea2D>(entity);

// o
entityManager->addComponent<CircleArea2D>(entity);
```

##### El componente de tipo `Area2D` no usa el `Area2DLayerManager`
Si no se está respetando la profundidad de las entidades en los eventos de ratón a pesar de tener el layerOpposition activado , puede ser porque el componente `Area2D` de la entidad no está en la lista de capas del `Area2DLayerManager`. La solución es simplemente crear el área pasándole este objeto al constructor.
```
// Este Manager debe ser común a los otros componentes "Area2D"
Area2DLayerManager* areaLayerManager;

entityManager->addComponent<RectArea2D>(entity, areaLayerManager);
```

También es posible que no se estén ordenando bien los componentes de Area2D en el Area2DLayerManager de la escena. Por defecto los objetos de este tipo son apilados en la lista interna del manager, para modificar el orden de la capa en la que se encuentra el área es posible utilizar las funciones de `Area2DLayerManager`:

`void pushBack(Area2D* area)`:
Inserta un Area2D detrás de todas.

`void pushFront(Area2D* area)`:
Inserta un Area2D al frente de todas.

`iterator insert(Area2DLayerManager::iterator layerIt, Area2D* area)`:
Inserta un Area2D en la posición del iterador.

`void moveBack(Area2DLayerManager::iterator layerIt)`:
Mueve un Area2D una posición atrás.

`void moveFoward(Area2DLayerManager::iterator layerIt)`:
Mueve un Area2D una posición adelante.

`void sendBack(Area2DLayerManager::iterator layerIt)`:
Manda un Area2D atrás del todo mediante su iterador.

`void sendFront(Area2DLayerManager::iterator layerIt)`:
Manda un Area2D al frente mediante su iterador.

`void sendBehind(Area2DLayerManager::iterator otherAreaLayerIt, Area2DLayerManager::iterator layerIt)`:
Mueve un Area2D detrás de otra identificada por su iterador.

`void sendAfter(Area2DLayerManager::iterator otherAreaLayerIt, Area2DLayerManager::iterator layerIt)`:
Mueve un Area2D delante de otra identificada por su iterador.

`void erase(Area2DLayerManager::iterator layerIt)`:
Borra un Area2D de la lista del manager (pero no borra el componente).

`Area2DLayerManager::iterator begin()`:
Devuelve un iterador al principio de la lista.

`Area2DLayerManager::iterator end()`:
Devuelve un iterador al final de la lista.

`Area2DLayerManager::reverse_iterator rbegin()`:
Devuelve un iterador reverso al principio reverso de la lista.

`Area2DLayerManager::reverse_iterator rend()`:
Devuelve un iterador reverso al final reverso de la lista.