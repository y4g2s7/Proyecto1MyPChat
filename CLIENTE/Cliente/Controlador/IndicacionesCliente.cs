public class IndicacionesCliente{
    public static String? indicacionesC(String mensaje){
        if(mensaje=="salir"){
	    return "exit\n";
	}else if(mensaje =="conectarse"){
	    return ConstruccionJson.construirJson("IDENTIFY", SocketCliente.usuario, null, null);
	} else if(mensaje == "lista"){
	    return ConstruccionJson.construirJson("USERS", null, null, null);
	} else if(mensaje == "cambio estatus AWAY"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"AWAY");
	} else if(mensaje == "cambio estatus ACTIVE"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"ACTIVE");
	 } else if(mensaje == "cambio estatus BUSY"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"BUSY");
	} else if(mensaje == "mensaje privado"){
	    string? username = VistaConsola.leerUsuario();
	    if(username == null || username.Length ==0) return null;
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("TEXT",username, text, null);
	    
	} else if(mensaje == "mensaje publico"){
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("PUBLIC_TEXT",null, text, null);
	}
	return null;
    }

   
}

