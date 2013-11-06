/*-----------------------------------------------------------------------------------------------------------------

			Author	:	Manikanda raj S

	Function Call	: AJAXRequest(url,paramstring,ctype,"callback");
	url		-	URL of the Servlet
	paramstring	-	Parameters to be Sent in AJAX Request , i.e POST Params
	ctype		-	Content Type , Can Be Only 'TEXT' , 'JSON', or  'XML'
	callback	-	Callback function where the response from server will be delivered

--------------------------------------------------------------------------------------------------------------------*/

//--------------------------------------------------AJAX FUNCTION-------------------------------------------*/
	function GetXmlHttpRequestObject()
		{
				var request = "";
				if (window.XMLHttpRequest) // For Safari, Firefox, and other non-MS
				{
					try {
					  request = new XMLHttpRequest();
					} 
					catch (e) {
					  request = false;
					}
				} 
				else if (window.ActiveXObject) // For Internet Explorer on Windows
				{
					try {
					  request = new ActiveXObject("Msxml2.XMLHTTP");
					} 
					catch (e) 
					{
						try {
							request = new ActiveXObject("Microsoft.XMLHTTP");
						} 
						catch (e) {
							request = false;
						}
					}
				}
				return request;
		}
	function AJAXRequest(url,paramstring,ctype,fname)
	{
		contentType=ctype;
		params=paramstring;
		funcname=fname;
		httpReq = GetXmlHttpRequestObject();					//new XMLHttpRequest();
		httpReq.onreadystatechange = AJAXRESULT;
		httpReq.open("POST", url, true);
		httpReq.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		httpReq.send(params);
	}
	function AJAXRESULT() //Call a function when the state changes.
	{
		if(httpReq.readyState == 4)
		{
			if(httpReq.status == 200)
			{
				if(contentType=="XML")
				{
					obj=httpReq.responseXML;
				}
				if (contentType=="JSON" || contentType=="TEXT")
				{
					obj=httpReq.responseText;
				}
				window[funcname](obj); // Calls the Called Function with Server Response 'obj'
			}
		}
	}
//----------------------------------------------------------------------------------------------------------------------*/
