
`BehaviorStateComponent` permite a una entidad comportarse de formas diferentes según el estado definido en el que se encuentre. Este componente fue creado pensando inicialmente en el comportamiento del loro de *Room3*, pero es genérico y por tanto aplicable a cualquier entidad que desee comportarse de maneras diferentes según una serie de estados definidos.

##### Funcionamiento:

El componente cuenta con un `std::map<BEHAVIOR_STATE, std::function<void()>>`, que relaciona un método `void`, que funcionará como método `update()`, con un identificador del estado (`using BEHAVIOR_STATE = int`). Dependiendo de cual sea el estado actual se ejecutará en el `update` una función u otra, si el valor del estado actual no está definido el `update` simplemente no ejecuta nada, aunque no es posible asignar desde fuera un estado que no haya sido añadido.

##### Métodos públicos:

`void defBehavior(BEHAVIOR_STATE newState, std::function<void()> stateUpdate):` 
Define un identificador del estado (`int`) o lo redefine asignándole un comportamiento para el `update`.

`void setState(BEHAVIOR_STATE)`:
Cambia el estado del componente a uno registrado en el mapa, si el estado no está registrado se da un error de aserción.

` BEHAVIOR_STATE getState()`:
Devuelve el valor del identificador del estado, útil para llamar en eventos en los que se involucre la entidad con estados de comportamiento (véase el ejemplo).

##### Ejemplo de uso:
```
enum State { IDLE, ALERT, SLEEPING };

BehaviorStateComponent* bh_st = 
	entityManager->addComponent<BehaviorStateComponent>(entity);

// Definir los estados de comportamiento

// Se ejecutará en el update cuando la entidad esté en estado IDLE
bh_st->defBehavior(State::IDLE, [](){ 
	std::cout << "I'm just standing\n";
});

// Se ejecutará en el update cuando la entidad esté en estado ALERT
bh_st->defBehavior(State::ALERT, [](){
	std::cout << "Alert! Alert! Alert!\n";
});

// Se ejecutará en el update cuando la entidad esté en estado SLEEPING
bh_st->defBehavior(State::SLEEPING, [](){
	std::cout << "zzzzzzzzzz\n";
});


// Interacciones (eventos) con la entidad según su estado

ClickComponent* clk = entityManager->addComponent<ClickComponent>(entity);

clk->connect(ClickComponent::JUST_CLICKED, [&]() { 
	switch(bh_st->getState()) {
		case State::IDLE:
			bh_st->setState(State::ALERT);
			break;
		case State::ALERT:
			std::cout << "ouch!!\n";
			bh_st->setState(State::SLEEPING);
			break;
		case State::SLEEPING:
			bh_st->setState(State::IDLE);
			break;
	}
});
```