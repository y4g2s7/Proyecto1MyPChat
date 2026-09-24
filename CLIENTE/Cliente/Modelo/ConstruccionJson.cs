using System.Text.Json.Nodes;
public class ConstruccionJson{
    public static string construirJson(string? type, string? username, string? text, string? status, string? roomname, List<string>? usernames){
	JsonObject obj = new JsonObject();    
        if (type != null)     obj["type"] = type;
        if (username != null) obj["username"] = username;
        if (text != null)     obj["text"] = text;
        if (status != null)   obj["status"] = status;
	if(roomname != null)  obj["roomname"] = roomname;
	if (usernames != null) {
	    JsonArray arr = new JsonArray();
	    foreach (string u in usernames) {
		arr.Add(u);
	    }
	    obj["usernames"] = arr;
	}
    return obj.ToJsonString() + "\n";
    }
}									
