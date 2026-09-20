public class IndicacionesCliente{
    public static String usuario="";
    
    public static String? indicacionesC(String mensaje){
        if(mensaje=="salir"){
	    return "exit\n";
	}else if(mensaje =="conectarse"){
	    return ConstruccionJson.construirIdentificacion();
	} else if(mensaje == "lista"){
	    return ConstruccionJson.construirPeticionLista();
	} else if(mensaje == "cambio estatus AWAY"){
	    return ConstruccionJson.construirCambioEstado(2);
	} else if(mensaje == "cambio estatus ACTIVE"){
	    return ConstruccionJson.construirCambioEstado(1);
	 } else if(mensaje == "cambio estatus BUSY"){
	    return ConstruccionJson.construirCambioEstado(3);
	} else if(mensaje == "mensaje"){
	    return ConstruccionJson.construirMensaje();
	}
	return null;
    }

   
}

