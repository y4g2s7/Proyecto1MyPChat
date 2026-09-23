using System.Text.Json.Nodes;
public class ConstruccionJson{
    public static string construirJson(string? type, string? username, string? text, string? status, string? roomname){
	JsonObject obj = new JsonObject();    
        if (type != null)     obj["type"] = type;
        if (username != null) obj["username"] = username;
        if (text != null)     obj["text"] = text;
        if (status != null)   obj["status"] = status;
	if(roomname != null)  obj["roomname"] = roomname;
    return obj.ToJsonString() + "\n";
    }
}									
