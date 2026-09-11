namespace CLIENTE.Tests;

public class TestSocketCliente
{
    // Prueba unitaria que reviza que ignora mensajes vacios \n
    [Fact]
    public void TestMensajeVacio(){
	string acumulado ="hola\n\nadios\n";

	List <string> mensajes = SocketCliente.procesarBuffer(ref acumulado);
	Assert.Equal("hola",mensajes[0]);
	Assert.Equal("adios",mensajes[1]);
	Assert.Equal(2, mensajes.Count);
    }
    
    // Prueba unitaria que reviza si se se guarda un mensaje que aun no tiene \n 
    // para despues unirlo cuando nos llegue otro mensaje 
    [Fact]
    public void TestMensajeSinSeparador(){
	string acumulado ="este mensaje no tiene separador";

	List <string> mensajes = SocketCliente.procesarBuffer(ref acumulado);
        Assert.Empty(mensajes);
	Assert.Equal("este mensaje no tiene separador", acumulado);	 
    }

    // Prueba unitaria que reviza si se guarda un mensaje que no tiene \n para despues
    // usarlo cuando le envian el complemento
    [Fact]
    public void TestMensajeSinSeparadorCompletado(){
	string acumulado ="este mensaje no tiene separador";
	List <string> mensajes = SocketCliente.procesarBuffer(ref acumulado);
        Assert.Empty(mensajes);
	Assert.Equal("este mensaje no tiene separador", acumulado);
	acumulado += "\n";
	List <string> mensajesCompletos = SocketCliente.procesarBuffer(ref acumulado);
	Assert.Equal("este mensaje no tiene separador",mensajesCompletos[0]);
	Assert.Equal("", acumulado);
    }
}
