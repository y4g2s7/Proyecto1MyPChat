public class IndicacionesCliente{
    public static String? indicacionesC(String mensaje){
        if(mensaje=="salir"){
	    return "exit\n";
	}else if(mensaje =="conectarse"){
	    return ConstruccionJson.construirJson("IDENTIFY", SocketCliente.usuario, null, null,null);
	} else if(mensaje == "lista"){
	    return ConstruccionJson.construirJson("USERS", null, null, null,null);
	} else if(mensaje == "cambio estatus AWAY"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"AWAY",null);
	} else if(mensaje == "cambio estatus ACTIVE"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"ACTIVE",null);
	 } else if(mensaje == "cambio estatus BUSY"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"BUSY",null);
	} else if(mensaje == "mensaje privado"){
	    string? username = VistaConsola.leerUsuario();
	    if(username == null || username.Length ==0) return null;
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("TEXT",username, text, null,null);
	    
	} else if(mensaje == "mensaje publico"){
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("PUBLIC_TEXT",null,text,null,null);
	} else if(mensaje== "nueva sala"){
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    return ConstruccionJson.construirJson("NEW_ROOM",null,null,null,nombreSala);
	}
	return null;
    }

   
}

