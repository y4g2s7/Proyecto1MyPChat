// Libreria para los sockets
using System.Net.Sockets;
//Libreria para los hilos
using System.Threading;

public class SocketCliente{
    // Variable para que el hilo de escucha o de escribe detecten si el servidor te desconecto
    static volatile bool terminado = false;
    
    //Metodo que recibe un String para la direccion ip y un int para el puerto
    public static void RunCliente(String ip, int puerto){
	//Creamos el cliente
	TcpClient cliente =new TcpClient();
	try{
	    //Conectamos pasandole la ip y el puerto
	    cliente.Connect(ip,puerto);
	    Console.WriteLine("Te haz conectado al servidor ¿Que deseas hacer?");
	} catch{
	    //Si falla la conexion avisamos al usuario
	    Console.WriteLine("ERROR no se pudo conectar al sevidor");
	    return;
	}
	//Creamos el hilo para escuchar lo que responde el servidor y llamamos a fEscucha 
	Thread hiloEscucha = new Thread(()=> fEscucha(cliente));
	hiloEscucha.Start();

	//Creamos el hilo para escribirle al servidor y llamamos a fEscribe 
	Thread hiloEscribe = new Thread(()=> fEscribe(cliente));
	// El hilo de escribe no impide que el programa termien
	hiloEscribe.IsBackground = true;
	hiloEscribe.Start();

	//Hacemos que el metodo espere a que termine el hilo de escuchar para terminar
	hiloEscucha.Join();
    }

    static void fEscucha(TcpClient cliente){
	NetworkStream stream = cliente.GetStream();
	//Arreglo para guardar lo que el servidor responde
	byte[] buffer = new byte[256];
	// variable para guardar lo que dice el ususario y poder manejarlo mejor
	string acumulado ="";
	//try catch para que cuando el usuario pida salir no truene de forma agresiva 
	try{
	    //Variable para guardar la cantidad de bytes recibidos
	    int bytesLeidos;
	    
	    //Cliclo de lectura, termina cuando el servidor manda 0
	    while((bytesLeidos = stream.Read(buffer, 0, buffer.Length))>0){
		//Convertimos los bytes a string 
		string mensajeDelServidor = System.Text.Encoding.UTF8.GetString(buffer, 0, bytesLeidos);
		// Le pasamos el mensajeDelServidor a la variable afuera de los ciclos
		acumulado += mensajeDelServidor;
		List <string> mensajes = procesarBuffer(ref acumulado);
		foreach(string texto in mensajes){
		    Console.WriteLine("El servidor respondio: "+texto);
		    if(texto == "{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"NOT_IDENTIFIED\"}"){
			Console.WriteLine("Cerrando Socket");
			terminado = true;
			cliente.Close();
			return;
		    }
		}
	    }
	} catch{
	    return;
	}
    }

    public static List<string> procesarBuffer(ref string acumulado) {
	List<string> mensajes = new List<string>();
	int indice;
	// Ciclo que termina cuando no encuentra en el string '\n'
	while((indice= acumulado.IndexOf('\n'))!=-1){
	    // Si tenemos la cadena vacia la ignoramos
	    if (indice== 0){
		acumulado = acumulado.Substring(indice+1);
		continue;
	    }
	    // Hacemos un substring que sabemos que es un mensaje completo para imprirlo
	    string mensajeCompleto = acumulado.Substring(0, indice);
	    mensajes.Add(mensajeCompleto);
	    //Actualizamos acumulado con el string que nos falta por tratar si es que existe,
	    //sino queda vacio
	    acumulado=acumulado.Substring(indice +1);
	}
	return mensajes;
    }
    
    static void fEscribe(TcpClient cliente){
	NetworkStream stream = cliente.GetStream();
	//Ciclo para escribir hasta que el usuario quiera salir o que el servidor te saque
	while(!terminado){
	     
	    //Leemos lo que quiere hacer el ususario
	    String? peticion = Console.ReadLine();

	    if(terminado) break;
	    
	    if(peticion == null){
		Console.WriteLine("Error tienes que escribir algo");
		continue;
	    }
	    //Lo traducimos para que el servidor lo entienda
	    String? solicitud = IndicacionesCliente.indicacionesC(peticion);
	    
	    //Arreglo de bytes para guardar la respuesta del cliente y comvertirla en bytes
	    byte[] bytesPeticion;
	    
	    //Si lo podemos entender continuamos
	    if(solicitud!=null){

		//Si el usuario quiere salir nos desconectamos
		if(solicitud == "exit\n"){
		    Console.WriteLine("Cerrando Socket");
		    terminado = true;
		    cliente.Close();
		    return;
		}
		//Convertimos la peticion a un arreglo de bytes para el write
		bytesPeticion = System.Text.Encoding.UTF8.GetBytes(solicitud);
	    } else{
		//Al no reconocer lo que dice pide el usuario mandamos un mensaje invalido al servidor
		bytesPeticion = System.Text.Encoding.UTF8.GetBytes("Mensaje invalido\n");		
	    }

	    try{
		stream.Write(bytesPeticion, 0, bytesPeticion.Length);
	    } catch {
		terminado = true;
		return;   
	    }
	}
    }
}
