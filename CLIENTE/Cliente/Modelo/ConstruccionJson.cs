using System.Text.Json.Nodes;
public class ConstruccionJson{
    public static string construirIdentificacion(){
	JsonObject obj = new JsonObject{
	    ["type"] = "IDENTIFY",
	    ["username"] = IndicacionesCliente.usuario
	 };
	 return obj.ToJsonString()+"\n";
    }
}


