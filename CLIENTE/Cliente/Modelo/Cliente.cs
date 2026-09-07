// See https://aka.ms/new-console-template for more information
class Cliente {
    static void Main(string[] args){
	//Le pedimos la IP al ususario
	Console.WriteLine("Escribe la IP del servidor al que te quieres conectar");
	String? ip =Console.ReadLine();

	//Verificacion de que la ip que puso el usuario no es null
	if(ip == null){
	    Console.WriteLine("Error tienes que escribir una direccion IP");
	    return;
	}
	
	//Le pedimos el puerto al usuario
	Console.WriteLine("Escribe el puerto del servidor al que te quieres conectar");
	int numeroPuerto;
	String? puerto=Console.ReadLine();
	
	//Verificacion de que la ip que puso el usuario no es null
	if(puerto == null){
	    Console.WriteLine("Error tienes que escribir un puerto");
	    return;
	}
	//Intentamos pasar lo que escribio el usuario a int
	try{
	    numeroPuerto = int.Parse(puerto);
	}
	//Si falla le avisamos el usuario y terminamos
	catch{
	    Console.WriteLine("Error no metiste un numero como el puerto");
	    return;
	}
	//Lamamos al metodo de Socket para crear los sockets y la conexion
	SocketCliente.RunCliente(ip,numeroPuerto);
    }
}
