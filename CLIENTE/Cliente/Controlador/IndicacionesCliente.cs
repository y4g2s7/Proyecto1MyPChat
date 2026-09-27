public class IndicacionesCliente{
    public static string? indicacionesC(string mensaje){
        if(mensaje =="conectarse"){
	    return ConstruccionJson.construirJson("IDENTIFY", SocketCliente.usuario, null, null,null,null);
	} else if(mensaje == "-l"){
	    return ConstruccionJson.construirJson("USERS", null, null, null,null,null);
	} else if(mensaje == "-ce"){
	    string? estatus = VistaConsola.leerCambioEstatus();
	    if(estatus == null || estatus.Length ==0) return null;
	    return ConstruccionJson.construirJson("STATUS",null,null,estatus,null,null);
	} else if(mensaje == "-mp"){
	    SocketCliente.enviar(ConstruccionJson.construirJson("USERS", null, null, null,null,null));
	    string? username = VistaConsola.leerUsuario();
	    if(username == null || username.Length ==0) return null;
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("TEXT",username, text, null,null,null);
	    
	} else if(mensaje == "-m"){
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("PUBLIC_TEXT",null,text,null,null,null);
	} else if(mensaje== "-ns"){
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    return ConstruccionJson.construirJson("NEW_ROOM",null,null,null,nombreSala,null);
	} else if(mensaje == "-is"){

	    string? nombreSala = VistaConsola.leerInvitacion();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    
	    SocketCliente.enviar(ConstruccionJson.construirJson("USERS", null, null, null,null,null));
	    
	    string? username = VistaConsola.leerUsuarioInvitacion();
	    if(username == null || username.Length ==0) return null;
	    List<string> usernames = username.Split(',').ToList();
	    return ConstruccionJson.construirJson("INVITE",null,null,null,nombreSala,usernames);
	} else if(mensaje == "-us"){
	    string? nombreSala = VistaConsola.leerSalaUnirse();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    return ConstruccionJson.construirJson("JOIN_ROOM",null,null,null,nombreSala,null);
	}  else if(mensaje == "-ls"){
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0)return null;
	    return ConstruccionJson.construirJson("ROOM_USERS",null,null,null,nombreSala,null);
	}   else if(mensaje == "-ms"){
	    
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0) return null;
	    string? text = VistaConsola.leerMensaje();
	    if(text == null || text.Length ==0)return null;
	    return ConstruccionJson.construirJson("ROOM_TEXT", null, text, null, nombreSala,null);
	    
	} else if(mensaje == "-ss"){
	    string? nombreSala = VistaConsola.leerSala();
	    if(nombreSala == null || nombreSala.Length ==0) return null;
	    return ConstruccionJson.construirJson("LEAVE_ROOM", null, null, null, nombreSala,null);
	    
	} else if(mensaje == "-d"){
	    return ConstruccionJson.construirJson("DISCONNECT", null, null, null, null,null);
	}
	return null;
    }
    
    public static string help(){
	string help = "\nOPCIONES:\n* '-mp'------Mensaje privado\n";
	help += "* '-m'-------Mensaje publico\n";
	help += "* '-ce'------Cambio estatus\n";
	help += "* '-l'-------Lista de todos los usuarios\n";
	help += "* '-ns'------Crear nueva sala\n";
	help += "* '-is'------Invitar a una sala\n";
	help += "* '-us'------Unirte a una sala\n";
	help += "* '-ls'------Lista de usuarios en una sala\n";
	help += "* '-ms'------Mensaje en una sala\n";
	help += "* '-ss'------Salir de una sala\n";
	help += "* '-d'-------Desconectarse del servidor\n";
	help += "* '-h'-------help\n";
	return help;
    }
    
}

