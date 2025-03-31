Esto es una breve guía de estilo para perder el menor tiempo posible descifrando el código de otros
-Class: Las clases siempre empezaran por mayúscula Esto incluye tanto la class como el .h y el .cpp
	-Componentes: los componentes no son mas que clases pero todo lo que sea un componente que al final de la clase lo ponga, Ej: EjemploComponent.
	-Manager: lo mismo que con los componentes. Ej: EjemploManager.
	-Handler: lo mismo que lo último
-Vaiables
	-Si una variable es privada que lleve la _ delante  _ velocity por ejemplo.
	-Utilizar geter y seter para consultar o modificar variables en vez de hacerlas publicas esto para tener mas seguridad y no loarla con valores que no deberían ser posibles etc.
-Métodos la primera palabra en minúscula y la primera letra del resto en mayúscula. ej render() o
getSpeed().
Y recordar que aunque no se ponga que algo es público o privado, por defecto esta en privado así que ponerlo para evitar errores y hacer el código mas legible.
Comentarios: los comentarios en el código se agradecen siempre que no sean paja(no comentéis un geter o un seter por ejemplo). Todo comentario en código debe estar en INGLÉS.
-Consolelogs: cuando acabéis de depurar o probar eliminarlos o borradlos para evitar que la consola se llene de basura innecesaria.
Por ultimo recordad liberar memoria dinámica <3