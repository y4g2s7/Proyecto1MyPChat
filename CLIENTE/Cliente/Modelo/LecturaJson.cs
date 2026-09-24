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
}
