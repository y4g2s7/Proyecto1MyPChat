//Libreria para los hilos
using System.Threading;
//Libreria para los Json
using System.Text.Json.Nodes;

// CONTROLADOR: conecta la Vista con el Modelo. Pide datos a la Vista, decide que hacer
// con ellos, se los pasa al Modelo y le pide a la Vista que muestre los resultados.
public class ControladorCliente{
    // Variable que coordina a los hilos para la identificacion
    static AutoResetEvent respuestaIdentificacion = new AutoResetEvent(false);

    // pedimos y validamos la ip y el puerto
    public static void Iniciar(){
	String? ip = VistaConsola.pedirIp();

	//Verificacion de que la ip que puso el usuario no es null
	if(ip == null || ip.Length == 0){
	    VistaConsola.mostrarMensaje("Error tienes que escribir una direccion IP");
	    return;
	}

	int numeroPuerto;
	String? puerto = VistaConsola.pedirPuerto();

	//Verificacion de que el puerto que puso el usuario no es null
	if(puerto == null || puerto.Length ==0){
	    VistaConsola.mostrarMensaje("Error tienes que escribir un puerto");
	    return;
	}
	//Intentamos pasar lo que escribio el usuario a int
	try{
	    numeroPuerto = int.Parse(puerto);
	}
	//Si falla le avisamos el usuario y terminamos
	catch{
	    VistaConsola.mostrarMensaje("Error no metiste un numero como el puerto");
	    return;
	}
	//Lamamos al metodo que crea la conexion y los hilos
	RunCliente(ip,numeroPuerto);
    }

    //Metodo que recibe un String para la direccion ip y un int para el puerto
    public static void RunCliente(String ip, int puerto){
	//Le pedimos al Modelo que se conecte
	if(SocketCliente.conectar(ip,puerto)){
	    VistaConsola.mostrarMensaje("Te haz conectado al servidor");
	} else{
	    //Si falla la conexion avisamos al usuario
	    VistaConsola.mostrarMensaje("ERROR no se pudo conectar al sevidor");
	    return;
	}

	//Nos suscribimos al evento del Modelo, cada mensaje completo llama a fRespuesta
	SocketCliente.MensajeRecibido += fRespuesta;

	//Creamos el hilo para escuchar lo que responde el servidor y llamamos a escuchar del Modelo
	Thread hiloEscucha = new Thread(()=> SocketCliente.escuchar());
	hiloEscucha.Start();

	//Creamos el hilo para escribirle al servidor y llamamos a fEscribe 
	Thread hiloEscribe = new Thread(()=> fEscribe());
	// El hilo de escribe no impide que el programa termien
	hiloEscribe.IsBackground = true;
	hiloEscribe.Start();

	//Hacemos que el metodo espere a que termine el hilo de escuchar para terminar
	hiloEscucha.Join();
    }

    // Se ejecuta (en el hilo de escucha) cada vez que el Modelo avisa de un mensaje completo
    static void fRespuesta(string texto){
	// Le pedimos a la Vista que escriba lo que el servidor nos dijo
	VistaConsola.mostrarMensaje("El servidor respondio: "+texto);

	// Parseamos lo que nos dice el servidor
	JsonNode? respuesta= JsonNode.Parse(texto);
		    
	// Si el servidor nos dice que no nos entendio, cerramos nuesto socket porque el ya nos desconecto
	if(LecturaJson.noEntendido(respuesta)){
	    VistaConsola.mostrarMensaje("Cerrando Socket");
	    SocketCliente.cerrar();
	    return;
	    // Verificamos si el servidor nos dice que la identificacion fue correcta
	} else if(LecturaJson.esIdentificacionExitosa(respuesta)){
	    // Actualizamos la variable para decir que ya se hizo el registro
	    SocketCliente.identificacionCompletada = true;   
	}
	//Despierta al hilo de escritura
	respuestaIdentificacion.Set();
    }

    static void fEscribe(){
	// Ciclo que se mantiene hasta que el usuario se logre registrar
	while(!SocketCliente.identificacionCompletada){
	      
	    //Le pedimos su nombre de usuario al usuario
	    String? usuario=VistaConsola.pedirUsuario();
	
	    //Verificacion de que el nombre que puso el usuario no es null
	    if(usuario == null || usuario.Length == 0){
		continue;
	    }

	    if (usuario.Length > 8) {
		usuario = usuario.Substring(0, 8);
	    }

	    // Actualizamos la variable del nombre de usuario
	    SocketCliente.usuario=usuario;
	    
	    String? conectarse = IndicacionesCliente.indicacionesC("conectarse");
	    if(conectarse==null)return;

	    // Le mandamos el mensaje al servidor de que nos queremos conectar con nuestro nombre de usuario
	    if(!SocketCliente.enviar(conectarse)) return;

	    //se bloquea aquí hasta que fRespuesta reciba la respuesta del servidor y responda
	    respuestaIdentificacion.WaitOne();
	}
	//Ciclo para escribir hasta que el usuario quiera salir o que el servidor te saque	
	while(!SocketCliente.terminado){
	     
	    //Leemos lo que quiere hacer el ususario
	    String? peticion = VistaConsola.leerPeticion();

	    if(SocketCliente.terminado) break;
	    
	    if(peticion == null || peticion.Length==0){
		VistaConsola.mostrarMensaje("Error tienes que escribir algo");
		continue;
	    }
	    //Lo traducimos para que el servidor lo entienda
	    String? solicitud = IndicacionesCliente.indicacionesC(peticion);
	    
	    //Si lo podemos entender continuamos
	    if(solicitud!=null){

		//Si el usuario quiere salir nos desconectamos
		if(solicitud == "exit\n"){
		    VistaConsola.mostrarMensaje("Cerrando Socket");
		    SocketCliente.cerrar();
		    return;
		}
	    } else{
		//Al no reconocer lo que dice pide el usuario mandamos un mensaje invalido al servidor
		solicitud = "Mensaje invalido\n";
	    }

	    if(!SocketCliente.enviar(solicitud)) return;
	}
    }
}
