using System.Text.Json.Nodes;
public class LecturaJson{
    public static bool esIdentificacionExitosa(JsonNode? nodo){
	if (nodo == null) return false;
        return nodo["type"]?.GetValue<string>()      == "RESPONSE"
            && nodo["operation"]?.GetValue<string>() == "IDENTIFY"
            && nodo["result"]?.GetValue<string>()    == "SUCCESS"
            && nodo["extra"]?.GetValue<string>()     == SocketCliente.usuario;
    }

    public static bool esNombreUsado(JsonNode? nodo){
	if(nodo == null) return false;
	return nodo["type"]?.GetValue<string>()      == "RESPONSE"
            && nodo["operation"]?.GetValue<string>() == "IDENTIFY"
            && nodo["result"]?.GetValue<string>()    == "USER_ALREADY_EXISTS"
            && nodo["extra"]?.GetValue<string>()     == SocketCliente.usuario;
    }

    public static bool noEntendido(JsonNode? nodo){
	if(nodo == null) return false;
	return nodo["type"]?.GetValue<string>()      == "RESPONSE"
            && nodo["operation"]?.GetValue<string>() == "INVALID"
            && nodo["result"]?.GetValue<string>()    == "NOT_IDENTIFIED";
    }

    public static bool esRespuestaDeIdentify(JsonNode? nodo){
    if (nodo == null) return false;
    return nodo["type"]?.GetValue<string>()      == "RESPONSE"
        && nodo["operation"]?.GetValue<string>() == "IDENTIFY";
    }

    public static string traduccion(string texto){
	JsonNode? nodo = JsonNode.Parse(texto);
	if(nodo==null){
	    return "ERROR NO ES JSON";
	}
	string? tipo = nodo["type"]?.GetValue<string>();
	string? username;
	string? text;
	string? roomname;
	string? operation;
	switch (tipo){
	    
	    case "NEW_USER":
		username = nodo?["username"]?.GetValue<string>();
		return "Se conecto '" + (username ?? "desconocido")+"'";
	        
	    case "NEW_STATUS":
		username = nodo?["username"]?.GetValue<string>();
		string? status = nodo?["status"]?.GetValue<string>();
		return "'"+(username ?? "desconocido") + "' está " + (status ?? "desconocido");
		
	    case "USER_LIST":
		JsonNode? usersNodo = nodo?["users"];
		string lista = "";
		if(usersNodo is JsonObject users){
		    lista += "Usuarios conectados:";
		    foreach (var par in users){
			string nombre = par.Key;
			string? estado = par.Value?.GetValue<string>();
			lista += $"  '{nombre}' está {estado ?? "desconocido"},";
		    }
		}
		return lista;
		
	    case "TEXT_FROM":
		username = nodo?["username"]?.GetValue<string>();
		text = nodo?["text"]?.GetValue<string>();
		return "'"+(username ?? "desconocido") + "'\n" + (text ?? "");
		
	    case "PUBLIC_TEXT_FROM":
		username = nodo?["username"]?.GetValue<string>();
		text = nodo?["text"]?.GetValue<string>();
		return "GENERAL\n'"+ (username ?? "desconocido") + "'\n" + (text ?? "");
		
	    case "JOINED_ROOM":
		username = nodo?["username"]?.GetValue<string>();
		roomname = nodo?["roomname"]?.GetValue<string>();
		return "'"+(username ?? "desconocido") + "' se unio a '" + (roomname ?? "desconocido")+"'";
		
	    case "ROOM_USER_LIST":
		roomname = nodo?["roomname"]?.GetValue<string>();
		string lista2 = "";
		JsonNode? usersNodo2 = nodo?["users"];
		if(usersNodo2 is JsonObject users2){
		    lista2 += "Usuarios conectados en '"+ (roomname ?? "") + "':";
		    foreach (var par in users2){
			string nombre = par.Key;
			string? estado = par.Value?.GetValue<string>();
			lista2 += $"  '{nombre}' está {estado ?? "desconocido"},";
		    }
		}
		return lista2;
	    case "RESPONSE":
		operation = nodo?["operation"]?.GetValue<string>();
		string? result = nodo?["result"]?.GetValue<string>();
		string? extra = nodo?["extra"]?.GetValue<string>();
		    
		if(operation=="IDENTIFY"){
		    if(result == "SUCCESS"){
			return "Conectado con el usuario '"+ (extra ?? "desconocido")+"'";
		    } else if(result == "USER_ALREADY_EXISTS"){
			return "El usuario '"+(extra ?? "desconocido") + "' ya fue usado";
		    }
		}
		else if(operation == "TEXT"){
		    return "Error el usuario '" + (extra?? "desconocido") + "' no esta conectado";
		}
		else if(operation == "NEW_ROOM"){
		    string resultado = "La sala '"+ (extra ?? "desconocido");
		    if(result == "SUCCESS"){
			return resultado += "' fue creada con exito";
		    } else if(result == "ROOM_ALREADY_EXISTS"){
			return resultado += "' ya existe";
		    }
		}
		else if(operation == "INVITE"){
		    if(result == "NO_SUCH_ROOM"){
			return "Error la sala '"+(extra ?? "desconocido")+"' no existe";
		    }else if(result== "NO_SUCH_USER"){
			return "Error el usuario '"+(extra ?? "desconocido")+"' no esta conectado";
		    }
		}
		else if(operation == "JOIN_ROOM"){
		    if(result == "SUCCESS"){
			return "Te haz unido a la sala '"+ (extra ?? "desconocido") + "'";
		    } else if(result == "NO_SUCH_ROOM"){
			return "Error la sala '"+(extra ?? "desconocido")+"' no existe";
		    } else if(result == "NOT_INVITED"){
			return "Error no haz sido invitado a la sala '"+(extra ?? "desconocido")+"'";
		    }
		}
		else if((operation == "ROOM_USERS") || (operation == "ROOM_TEXT") || (operation == "LEAVE_ROOM")){
		    if(result == "NO_SUCH_ROOM"){
			return "Error la sala '"+(extra ?? "desconocido")+"' no existe";
		    } else if(result == "NOT_JOINED"){
			return "Error no eres miembro de la sala '"+(extra ?? "desconocido")+"'";
		    }
		}
		return "Mensaje desconocido: " + texto;
	    case "INVITATION":
		username = nodo?["username"]?.GetValue<string>();
		roomname = nodo?["roomname"]?.GetValue<string>();
		return "El usuario '" + (username ?? "desconocido") + "' te invito a la sala '" + (roomname ?? "desconocido") + "'";
	    case "ROOM_TEXT_FROM":
		username = nodo?["username"]?.GetValue<string>();
		roomname = nodo?["roomname"]?.GetValue<string>();
		text = nodo?["text"]?.GetValue<string>();
		return "'"+roomname + "'\n'"+username + "'\n"+text;

	    case "LEFT_ROOM":
		username = nodo?["username"]?.GetValue<string>();
		roomname = nodo?["roomname"]?.GetValue<string>();
		return "El usuario '"+username + "' salio de '"+roomname+"'";
	    case "DISCONNECTED":
		username = nodo?["username"]?.GetValue<string>();
		return "El usuario '"+username + "' se desconecto";
	    default:
		return "Mensaje desconocido: " + texto;
	}
    }
}
