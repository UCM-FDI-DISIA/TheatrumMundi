Lo primero es crear una clase que herede de ecs::Component.
```
//Header de las escenas ejemplo de la plantilla para las rooms
#include "SceneTemplate.h"
class NuevoComponent: public ecs::Component
{
	public:
	// IMPORTANTE (Crea el ID del comonente)
	__CMPID_DECL__(ecs::cmp::NUEVO_COMPONENTE)

	NuevoComponent();
	...
};
```
Es importante añadir `__CMPID_DECL__(ecs::cmp::NUEVO_COMPONENTE)` para darle un ID al nuevo tipo de componente, pero para esto es necesario definir el ID en el `enum cmpId` del `namespace cmp`, pero esto hay que hacerlo a través del macro "_CMPS_LIST_" en "ecs_defs.h".
```
// Components list - must have at least one element
//
#define _CMPS_LIST_ \
	TRANSFORM, \
	IMAGE, \
	NUEVO_COMPONENTE
```