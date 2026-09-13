public class IndicacionesCliente{
    
    public static String? indicacionesC(String mensaje){
        if(mensaje=="salir"){
	    return "exit\n";
	}else if(mensaje =="identificacion json"){
	    return "{\"type\":\"IDENTIFY\",\"username\":\"Kimberly\"}\n";
	}
	return null;
    }
}
