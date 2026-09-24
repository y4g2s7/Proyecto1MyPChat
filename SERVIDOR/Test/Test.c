#include <assert.h>
#include <string.h>
#include "TestSocket.h"

int main() {
  TestMensajeVacio();
  TestSocketDuplicado();
  TestNombreDuplicado();
  TestMensajeSinSeparador();
  TestMensajeSinSeparadorCompletado();
  TestCambioEstatus();
  TestCambioEstatusNoAceptable();
  TestCambioEstatusRepetido();
  TestMensajePrivado();
  TestMensajeUsuarioNoEncontrado();
  TestCreacionSala();
  TestSalaRepetida();
  TestInvitacionSala();
  TestInvitacionSalaInexistente();
  TestInvitacionSalaUsuarioInexistente();
  TestIngresaSala();
  TestIngresaInexistente();
  TestIngresaSinInvitacion();
  return 0;

  
}

