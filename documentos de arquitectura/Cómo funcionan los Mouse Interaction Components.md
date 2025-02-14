Los componentes del tipo `MouseIntaractionComponent` permiten suscribir una entidad a varios eventos de ratón.

  ##### Tipos de MouseIntaractionComponent:
	 ClickComponent
     TriggerComponent
     DragComponent

### ClickComponent
ClickComponent ofrece los eventos `JUST_CLICKED` y `JUST_RELEASED`, a los que es posible vincular el número de funciones que se deseen.

Ejemplo de uso:
```
ClickComponent* clk = entityManager->addComponent<ClickComponent>(entity);

// Al hacer click en la entidad se mostrará "CLICKED" en la consola
clk->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });
```