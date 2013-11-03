package mlabs.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.Response;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.RootPanel;


public class GWTClient implements EntryPoint
{
	public void onModuleLoad()
	{	
		String postUrl="http://mani:8000";
		String requestData="un=mani@vembu.com&token=server";
		APIRequests apiRequest = new APIRequests();
		apiRequest.GWTPOSTHTTP(postUrl,requestData,handleResponse);
	}

	private RequestCallback handleResponse = new RequestCallback()
	{
		public void onError(Request request, Throwable e) 
		{
			System.out.println(e.getMessage());
		}
		public void onResponseReceived(Request request, Response response)
        {
			if (200 == response.getStatusCode())
			{
				String _respText =response.getText();
				HTML check = new HTML(); 
				check.setText(_respText);
				RootPanel.get().add(check);
			} else {
				String _respText ="Received HTTP status code other than 200: "+response.getStatusText();
				HTML check = new HTML(); 
				check.setText(_respText);
				RootPanel.get().add(check);
			}
        }
	};
}
