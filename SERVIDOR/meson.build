project('Chat', 'c')
executable('Servidor', 'Servidor/SocketServidor.c','Servidor/IndicacionesServidor.c','Servidor/Servidor.c','cJSON/cJSON.c')		  
Test = executable('Test', 'Test/Test.c', 'Test/TestSocket.c','Servidor/SocketServidor.c','Servidor/IndicacionesServidor.c')
test('Pruebas', Test)
