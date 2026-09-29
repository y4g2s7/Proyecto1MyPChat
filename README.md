# Proyecto1MyPChat

Aplicación de chat cliente-servidor sobre TCP con protocolo basado en JSON,
desarrollada para el curso de Manejo y Programación de Objetos (MyP).

- **Servidor**: C, con `cJSON` para el protocolo, `uthash` para las tablas
  de usuarios y salas, y `pthreads` para concurrencia.
- **Cliente**: C#/.NET, con arquitectura MVC.

El protocolo sigue la especificación entregada por el curso, por lo que este
cliente puede conectarse a servidores de otros compañeros, y este servidor
puede atender a sus clientes.

## Estructura del repositorio
├── SERVIDOR/ # Servidor en C (Meson + Ninja)
├── CLIENTE/ # Cliente en C# (.NET)
└── reporte/ # Reporte técnico en LaTeX (main.tex, Reporte_proyecto_1_MyP.pdf, secuencia_identify.png, Logo_UNAM.png, Logo_FC.png)

## Requerimientos

- **Servidor**: Linux, Meson, Ninja, `libcjson-dev`, `uthash-dev`
- **Cliente**: .NET SDK 8.0 o superior

## Cómo ejecutar

### Servidor
```bash
cd SERVIDOR
meson setup build      # solo la primera vez
meson compile -C build/
./build/Servidor
```
El servidor queda escuchando en el puerto 1234.

### Cliente
```bash
cd CLIENTE/Cliente
dotnet run
```
Al ejecutarse, pide la IP del servidor, el puerto y el nombre de usuario.

## Pruebas

```bash
# Servidor
meson test -C SERVIDOR/build/

# Cliente
cd CLIENTE/CLIENTE.Tests
dotnet test
```

## Reporte

El reporte técnico completo (análisis, diseño, diagramas e implementación)
está en [`reporte/Reporte_proyecto_1_MyP.pdf`](reporte/Reporte_proyecto_1_MyP.pdf).

## Autor

Yoltic Giovanni Santillan Ruiz— Facultad de Ciencias, UNAM
