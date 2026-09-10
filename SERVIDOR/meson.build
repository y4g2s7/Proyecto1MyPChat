project('Chat', 'c')
executable('Servidor', 'Servidor/SocketServidor.c','Servidor/IndicacionesServidor.c','Servidor/Servidor.c')		  
TestEjemplo = executable('TestEjemplo', 'Test/TestEjemplo.c', 'Servidor/SocketServidor.c','Servidor/IndicacionesServidor.c')
test('Prueba de ejemplo', TestEjemplo)
