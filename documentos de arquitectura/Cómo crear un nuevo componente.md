Lo primero es crear una clase que herede de ecs::Component.
```
//Header de las escenas ejemplo de la plantilla para las rooms
#include "SceneTemplate.h"
class NuevoComponent: public ecs::Component
{
	public:
	// IMPORTANTE (Crea el ID del comonente)
	__CMPID_DECL__(ecs::cmp::NUEVO_COMPONENTE);

	NuevoComponent();
	...
};
```
Es importante añadir `__CMPID_DECL__(ecs::cmp::NUEVO_COMPONENTE)` para darle un ID al nuevo tipo de componente, pero para esto es necesario definir el ID en el `enum cmpId` del `namespace cmp`, en "ecs.h".
```
namespace cmp {
// list of component identifiers - note that we rely on that the
// first number is 0 in C/C++ standard
enum cmpId : cmpId_t {
	_CMPS_LIST_, /* taken from ../game/ecs_defs */
	NUEVO_COMPONENTE,

	// do not remove this
	_LAST_CMP_ID
};
}
```
