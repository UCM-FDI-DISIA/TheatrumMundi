Los componentes del tipo `MouseIntaractionComponent` permiten suscribir una entidad con un Arae2D a varios eventos de ratón.

  ##### Tipos de MouseIntaractionComponent:
	 ClickComponent
     TriggerComponent
     DragComponent

### ClickComponent
ClickComponent ofrece los eventos `JUST_CLICKED` y `JUST_RELEASED`, a los que es posible vincular el número de funciones que se deseen.

Ejemplo de uso:
```
entityManager->addComponent<RectArea2D>(entity); // Debe haber un Area2D
ClickComponent* clk = entityManager->addComponent<ClickComponent>(entity);

// Al hacer click en la entidad se mostrará "CLICKED" en la consola
clk->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });
```

### TriggerComponent
TriggerComponent ofrece los eventos `JUST_ENTERED` y `JUST_LEFT`, para cuando se entra y se sale del Area2D de la entidad.

Ejemplo de uso:
```
entityManager->addComponent<RectArea2D>(entity); // Debe haber un Area2D
TriggerComponent* trg = entityManager->addComponent<TriggerComponent>(entity);

// Al entrar en el area del objeto se muestra "ENTERED" y al salir "LEFT"
trg->connect(TriggerComponent::JUST_ENTERED, []() { std::cout << "ENTERED\n";  });
trg->connect(TriggerComponent::JUST_LEFT, []() { std::cout << "LEFT\n";  });
```

### DragComponent
DragComponent ofrece los eventos `DRAG_START`, `DRAG` y `DRAG_END`, para cuando se hace pincha en el Area2D, cuando se mueve el objeto, y para cuando se suelta el objeto.

Ejemplo de uso:
```
entityManager->addComponent<RectArea2D>(entity); // Debe haber un Area2D
DragComponent* drg = entityManager->addComponent<DragComponent>(entity);

// Al arrastrar el objeto se muestra "DRAGGING"
drg->connect(DragComponent::DRAG, []() { std::cout << "DRAGGING\n"; });
```


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