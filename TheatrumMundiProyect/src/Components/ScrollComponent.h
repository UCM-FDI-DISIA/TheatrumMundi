#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include <cassert>
#include <vector>
#include <unordered_map>
#include <functional>

class Transform;

class ScrollComponent : public ecs::Component {
private:
	Vector2D _dir;
	int _velocity;
	int _initialVelocity;
	float _timeScroll;
	float _initialTimeScroll; //Activates if this time > 0
	//bool _isRight;
	//bool _isScrolling = false;
	
public:

	__CMPID_DECL__(ecs::cmp::SCROLL_COMPONENT)

	using EVENT_TYPE = int; // Alias type that refers to the EventType enum, that will be extended
	using CALLBACK = std::function<void()>;

	enum EventTypeScroll { STARTSCROLLING, ENDEDSCROLLING }; // Type of scrolling event

	//ESTO SE ELIMINARIA POR PASARLE LA DIRECCION POR ADELANTADO
	enum Inverse { NORMAL, INVERSE };
	enum Direction { UP, DOWN, LEFT, RIGHT };
	enum Path { LEFTRIGHT, UPDOWN };
	Path _path;
	Inverse _inverse;
	
	ScrollComponent(int velocity, float time, Inverse isInverse,int numPhases);
	~ScrollComponent();
	//void initComponent() override;
	void Scroll(Direction _direction);
	void update() override;
	bool isScrolling();
	void addElementToScroll(Transform* _objectT);
	int numPhases();
	bool connect(EVENT_TYPE, CALLBACK);
	//void restartPosition();

	//AMPLIACION
	bool finalPhaseCheck() const { return (phase == finalPhase); }
	bool startPhaseCheck() const { return (phase == startPhase); }
	void addPhase();
	void addPhaseAndFollow() {
		finalPhase++;
		phase = finalPhase;
	}
	void resetPhase() {
		phase = startPhase;
	}

private:
	//Transform *_myTransform;
	std::vector<Transform*> _objectsTransform;

	//AMPLIACION
	int startPhase = 0;
	int finalPhase = 0; 
	int phase = 0; //Fase no puede ser > final y < inicio
	//Definimos el final como el tamaño
	//Inicializamos la fase dependiendo de en que sentido queremos moverlo,
	//UP:: FASE = FINAL 
	//DOWN:: FASE = INICIO
	//LEFT:: FASE = FINAL 
	//RIGHT:: FASE = INICIO

protected:
	std::unordered_map<EVENT_TYPE, std::vector<CALLBACK>> _eventConnectionsScroll;
};