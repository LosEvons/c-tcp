#include "handle_http.h"

char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";

void setHttpHeader(char httpHeader[]){
	FILE *htmlData = fopen("../resources/index.html", "r");

	char line[100];
	char responseData[8000];
	while (fgets(line, 100, htmlData) != 0){
		strcat(responseData, line);
	}

	strcat(httpHeader, responseData);
}