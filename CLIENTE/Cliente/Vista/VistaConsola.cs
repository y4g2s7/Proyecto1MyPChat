// VISTA: es la unica clase que usa Console. No decide nada, solo muestra y lee.
public class VistaConsola{

    // Muestra cualquier texto al usuario
    public static void mostrarMensaje(String mensaje){
	Console.WriteLine(mensaje);
    }

    //Le pedimos la IP al ususario
    public static String? pedirIp(){
	Console.WriteLine("Escribe la IP del servidor al que te quieres conectar");
	return Console.ReadLine();
    }

    //Le pedimos el puerto al usuario
    public static String? pedirPuerto(){
	Console.WriteLine("Escribe el puerto del servidor al que te quieres conectar");
	return Console.ReadLine();
    }

    //Le pedimos su nombre de usuario al usuario
    public static String? pedirUsuario(){
	Console.WriteLine("Escribe el nombre con el que te quieres conectar");
	return Console.ReadLine();
    }

    //Leemos lo que quiere hacer el ususario
    public static String? leerPeticion(){
	return Console.ReadLine();
    }

    public static String? leerUsuario(){
	Console.WriteLine("Escribe a quien le quieres mandar el mensaje");
	return Console.ReadLine();
    }

    public static String? leerMensaje(){
	Console.WriteLine("Escribe el mensaje");
	return Console.ReadLine();
    }

    public static String? leerSala(){
	Console.WriteLine("Escribe el nombre de la sala");
	return Console.ReadLine();
    }
}
