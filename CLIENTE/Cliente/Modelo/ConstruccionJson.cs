using System.Text.Json.Nodes;
public class ConstruccionJson{
    
   
   public static string construirMensaje(){
       	JsonObject obj = new JsonObject{
	    ["type"] = "TEXT",
	    ["username"] = "Luis",
	    ["text"] = "hola" 
	 };
	 return obj.ToJsonString()+"\n";
   
   }
   
  

public static string construirJson(string? type, string? username, string? text, string? status){
    JsonObject obj = new JsonObject();

    if (type != null)     obj["type"] = type;
    if (username != null) obj["username"] = username;
    if (text != null)     obj["text"] = text;
    if (status != null)   obj["status"] = status;
    
    return obj.ToJsonString() + "\n";
												}
												
}


