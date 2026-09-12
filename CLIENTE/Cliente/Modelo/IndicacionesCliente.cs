public class IndicacionesCliente{
    
    public static String? indicacionesC(String mensaje){
	if(mensaje == "saluda"){
	    return "hola\n";
	} else if(mensaje=="despidete"){
	    return "adios\n";
	}else if(mensaje=="salir"){
	    return "exit\n";
	}
	return null;
    }
}
