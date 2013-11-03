package mlabs.client;

import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.user.client.Window;

public class APIRequests
{
	public void GWTPOSTHTTP(String apiUrl,String requestData,RequestCallback callBack)
	{
		RequestBuilder builder = new RequestBuilder(RequestBuilder.POST, apiUrl);
		try {
			builder.sendRequest(requestData.toString(),callBack);
		} catch (RequestException e) {	//Couldn't connect to server
        	Window.alert(e.getMessage());
		}
	}
}