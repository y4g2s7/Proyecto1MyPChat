project('Chat', 'c')
cjson_dep = dependency('libcjson')
executable('Servidor', 'Servidor/SocketServidor.c','Servidor/IndicacionesServidor.c','Servidor/Servidor.c', 'Servidor/TiposDeJson.c', dependencies: [cjson_dep])

Test = executable('Test', 'Test/Test.c', 'Test/TestSocket.c','Servidor/SocketServidor.c','Servidor/IndicacionesServidor.c','Servidor/TiposDeJson.c', dependencies: [cjson_dep])
test('Pruebas', Test)
