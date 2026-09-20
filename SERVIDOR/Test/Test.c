#include <assert.h>
#include <string.h>
#include "TestSocket.h"

int main() {
  TestMensajeVacio();
  TestNombreDuplicado();
  TestMensajeSinSeparador();
  TestMensajeSinSeparadorCompletado();
  TestCambioEstatus();
  TestCambioEstatusNoAceptable();
  TestCambioEstatusRepetido();
  return 0;
}

