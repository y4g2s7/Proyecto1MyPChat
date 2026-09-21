// Libreria para los sockets
using System.Net.Sockets;

//MODELO: guarda el estado del cliente, maneja el socket y arma los mensajes completos.
//No imprime nada ni lee nada de la consola.
public class SocketCliente{
    // Nombre con el que el usuario se identifico
    public static String usuario="";
    // Variable para que el hilo de escucha o de escribe detecten si el servidor te desconecto
    public static volatile bool terminado = false;
    // Variable que nos dice si el usuario ya se identifico
    public static volatile bool identificacionCompletada = false;

    //Cliente
    static TcpClient cliente = new TcpClient();

    //Evento con el que el Modelo avisa que llego un mensaje completo del servidor
    public static event Action<string>? MensajeRecibido;

    //Metodo que recibe un String para la direccion ip y un int para el puerto
    //Regresa true si se pudo conectar y false si no
    public static bool conectar(String ip, int puerto){
	try{
	    //Conectamos pasandole la ip y el puerto
	    cliente.Connect(ip,puerto);
	    return true;
	} catch{
	    return false;
	}
    }

    //Convierte el mensaje a bytes y lo manda al servidor, regresa false si fallo el envio
    public static bool enviar(String mensaje){
	//Arreglo de bytes para guardar el mensaje convertido en bytes
	byte[] bytesPeticion = System.Text.Encoding.UTF8.GetBytes(mensaje);
	try{
	    NetworkStream stream = cliente.GetStream();
	    stream.Write(bytesPeticion, 0, bytesPeticion.Length);
	    return true;
	} catch {
	    terminado = true;
	    return false;
	}
    }

    //Marca que terminamos y cierra el socket
    public static void cerrar(){
	terminado = true;
	cliente.Close();
    }

    //Avisa con el evento que se recibio un mensaje
    public static void escuchar(){
	NetworkStream stream = cliente.GetStream();
	//Arreglo para guardar lo que el servidor responde
	byte[] buffer = new byte[1024*1024];
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
		    // Le avisamos a quien este suscrito que llego un mensaje completo
		    MensajeRecibido?.Invoke(texto);
		    // Si al procesar el mensaje se cerro el socket, dejamos de escuchar
		    if(terminado) return;
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
}
