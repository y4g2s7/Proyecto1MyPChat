public class IndicacionesCliente{
    public static String usuario="";
    
    public static String? indicacionesC(String mensaje){
        if(mensaje=="salir"){
	    return "exit\n";
	}else if(mensaje =="conectarse"){
	    return "{\"type\":\"IDENTIFY\",\"username\":\""+usuario+"\"}\n";
	}
	return null;
    }
}
