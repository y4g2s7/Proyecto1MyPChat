using System.Text.Json.Nodes;
public class ConstruccionJson{
    public static string construirIdentificacion(){
	JsonObject obj = new JsonObject{
	    ["type"] = "IDENTIFY",
	    ["username"] = SocketCliente.usuario
	 };
	 return obj.ToJsonString()+"\n";
   }
   public static string construirPeticionLista(){
	JsonObject obj = new JsonObject{
	    ["type"] = "USERS",
	 };
	 return obj.ToJsonString()+"\n";
   }
   
   public static string construirMensaje(){
       	JsonObject obj = new JsonObject{
	    ["type"] = "TEXT",
	    ["username"] = "Luis",
	    ["text"] = "hola" 
	 };
	 return obj.ToJsonString()+"\n";
   
   }
   
   public static string construirCambioEstado(int opcion){
       string estado="";
       if(opcion ==1){
	   estado = "ACTIVE";
       } else if(opcion ==2){
	   estado ="AWAY";
       } else if(opcion ==3){
	   estado ="BUSY";
       }
       JsonObject obj = new JsonObject{
	    ["type"] = "STATUS",
	    ["status"]= estado
	};
	 return obj.ToJsonString()+"\n";
   }
}


