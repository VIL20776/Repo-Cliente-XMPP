# Proyecto 1 Redes: Cliente XMPP
Implementación de un cliente XMPP para el curso de Redes.

La implementación se realizó con C++ haciendo uso de las librerías Qt y QXmpp para implementar la lógica y la interfaz gráfica.

Se recomienda un entorno UNIX/Linux para compilar.

## Dependencias y herramientas
- QXmpp 1.8.0 (Librería XMPP)
- Qt 6.7 (Dependencia de QXmpp, también usada para la interfaz gráfica)
- Cmake (Build-system para C++, necesario para integrar los archivos .ui)

QXmpp puede ser agregado manualmente al proyecto. Crea una carpeta `external` y dentro de ella copia o clona el código fuente. Y a continuación, compila e install el proyecto con cmake según las instrucciones del README. Si se hace de esta forma pasa el argumento `-DCMAKE_INSTALL_PREFIX=build` para que pueda ser encontrado por la configuración del proyecto. Es recomendable pasar el argumento `QT_MAJOR_VERSION=6` para evitar instalar Qt5 como dependencia adicional.

## Funciones implementadas
- Mostrar usuarios/contactos y su estado
- Agregar usuario a los contactos
- Comunicación 1 a 1
- Definir presencia

## Como compilar
Este proyecto se puede abrir con el IDE QtCreator y ser compilado desde ahí.

También se puede compilar usando Cmake. Para ello, se debe crear la carpeta `build/` en la raíz del proyecto y ejecutar los siguientes comandos:
```
cmake -S . -B build/
cmake --build build/
```

El ejecutable se encontrará en la carpeta `build/` bajo en nombre de `QJab`.
