public class IndicacionesCliente{
    public static String? indicacionesC(String mensaje){
        if(mensaje=="salir"){
	    return "exit\n";
	}else if(mensaje =="conectarse"){
	    return ConstruccionJson.construirJson("IDENTIFY", SocketCliente.usuario, null, null,null,null);
	} else if(mensaje == "lista"){
	    return ConstruccionJson.construirJson("USERS", null, null, null,null,null);
	} else if(mensaje == "cambio estatus AWAY"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"AWAY",null,null);
	} else if(mensaje == "cambio estatus ACTIVE"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"ACTIVE",null,null);
	 } else if(mensaje == "cambio estatus BUSY"){
	    return ConstruccionJson.construirJson("STATUS",null,null,"BUSY",null,null);
	} else if(mensaje == "mensaje privado"){
	    SocketCliente.enviar(ConstruccionJson.construirJson("USERS", null, null, null,null,null));
	    string? username = VistaConsola.leerUsuario();
	    if(username == null || username.Length ==0) return null;
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("TEXT",username, text, null,null,null);
	    
	} else if(mensaje == "mensaje publico"){
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("PUBLIC_TEXT",null,text,null,null,null);
	} else if(mensaje== "crear sala"){
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    return ConstruccionJson.construirJson("NEW_ROOM",null,null,null,nombreSala,null);
	} else if(mensaje == "invitar sala"){

	    string? nombreSala = VistaConsola.leerInvitacion();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    
	    SocketCliente.enviar(ConstruccionJson.construirJson("USERS", null, null, null,null,null));
	    
	    string? username = VistaConsola.leerUsuarioInvitacion();
	    if(username == null || username.Length ==0) return null;
	    List<string> usernames = username.Split(',').ToList();
	    return ConstruccionJson.construirJson("INVITE",null,null,null,nombreSala,usernames);
	} else if(mensaje == "ingresar sala"){
	    string? nombreSala = VistaConsola.leerSalaUnirse();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    return ConstruccionJson.construirJson("JOIN_ROOM",null,null,null,nombreSala,null);
	}  else if(mensaje == "usuarios sala"){
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    return ConstruccionJson.construirJson("ROOM_USERS",null,null,null,nombreSala,null);
	}   else if(mensaje == "texto en sala"){
	    
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0) return null;
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("ROOM_TEXT", null, text, null, nombreSala,null);
	    
	} else if(mensaje == "salir sala"){
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0) return null;
	    return ConstruccionJson.construirJson("LEAVE_ROOM", null, null, null, nombreSala,null);
	    
	}
	return null;
    }

   
}

