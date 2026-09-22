using System.Text.Json.Nodes;
public class ConstruccionJson{
    public static string construirJson(string? type, string? username, string? text, string? status){
	JsonObject obj = new JsonObject();    
        if (type != null)     obj["type"] = type;
        if (username != null) obj["username"] = username;
        if (text != null)     obj["text"] = text;
        if (status != null)   obj["status"] = status;
    
        return obj.ToJsonString() + "\n";
    }
}									
