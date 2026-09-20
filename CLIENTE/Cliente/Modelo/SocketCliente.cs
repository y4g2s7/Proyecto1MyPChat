// Libreria para los sockets
using System.Net.Sockets;
//Libreria para los hilos
using System.Threading;
//Libreria para los Json
using System.Text.Json.Nodes;

public class SocketCliente{
    // Variable para que el hilo de escucha o de escribe detecten si el servidor te desconecto
    static volatile bool terminado = false;
    // Variable que nos dice si el usuario ya se identifico
    static volatile bool identificacionCompletada = false;
    // Variable que coordina a los hilos para la identificacion
    static AutoResetEvent respuestaIdentificacion = new AutoResetEvent(false);

    //Metodo que recibe un String para la direccion ip y un int para el puerto
    public static void RunCliente(String ip, int puerto){
	//Creamos el cliente
	TcpClient cliente =new TcpClient();
	try{
	    //Conectamos pasandole la ip y el puerto
	    cliente.Connect(ip,puerto);
	    Console.WriteLine("Te haz conectado al servidor");
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
		    // Escribimos lo que el servidor nos dijo
		    Console.WriteLine("El servidor respondio: "+texto);

		    // Parseamos lo que nos dice el servidor
		    JsonNode? respuesta= JsonNode.Parse(texto);
		    
		    // Si el servidor nos dice que no nos entendio, cerramos nuesto socket porque el ya nos desconecto
		    if(LecturaJson.noEntendido(respuesta)){
			Console.WriteLine("Cerrando Socket");
			terminado = true;
			cliente.Close();
			return;
			// Verificamos si el servidor nos dice que la identificacion fue correcta
		    } else if(LecturaJson.esIdentificacionExitosa(respuesta)){
			// Actualizamos la variable para decir que ya se hizo el registro
			identificacionCompletada = true;   
		    }
		    //Despierta al hilo de escritura
		    respuestaIdentificacion.Set();
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

	// Ciclo que se mantiene hasta que el usuario se logre registrar
	while(!identificacionCompletada){
	      
	    //Le pedimos su nombre de usuario al usuario
	    Console.WriteLine("Escribe el nombre con el que te quieres conectar");
	    String? usuario=Console.ReadLine();
	
	    //Verificacion de que el nombre que puso el usuario no es null
	    if(usuario == null || usuario.Length == 0){
		continue;
	    }

	    if (usuario.Length > 8) {
		usuario = usuario.Substring(0, 8);
	    }

	    // Actualizamos la variable del nombre de usuario
	    IndicacionesCliente.usuario=usuario;
	    
	    String? conectarse = IndicacionesCliente.indicacionesC("conectarse");
	    //Arreglo de bytes para guardar la respuesta del cliente y convertirla en bytes
	    if(conectarse==null)return;
	    byte[] bytesPeticion;

	    // Le mandamos el mensaje al servidor de que nos queremos conectar con nuestro nombre de usuario
	    bytesPeticion = System.Text.Encoding.UTF8.GetBytes(conectarse);
	    try{
		stream.Write(bytesPeticion, 0, bytesPeticion.Length);
	    } catch {
		terminado = true;
		return;   
	    }
	    //se bloquea aquí hasta que fEscucha reciba la respuesta del servidor y responda
	    respuestaIdentificacion.WaitOne();
	}
	//Ciclo para escribir hasta que el usuario quiera salir o que el servidor te saque	
	while(!terminado){
	     
	    //Leemos lo que quiere hacer el ususario
	    String? peticion = Console.ReadLine();

	    if(terminado) break;
	    
	    if(peticion == null || peticion.Length==0){
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
