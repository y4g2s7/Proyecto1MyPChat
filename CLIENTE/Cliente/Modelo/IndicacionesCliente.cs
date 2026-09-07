public class IndicacionesCliente{
    
    public static String? indicacionesC(String mensaje){
	if(mensaje == "saluda"){
	    return "hola";
	} else if(mensaje=="despidete"){
	    return "adios";
	}else if(mensaje=="salir"){
	    return "exit";
	}
	return null;
    }
}
