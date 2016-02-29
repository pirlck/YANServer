//
// Created by cl-k on 11/30/15.
//


//这个http报文解析的代码是从别的地方copy过来的.
//用的是C++写的

#include "parse.h"

 void tyhp_print_http_header_header(const tyhp_header& head)
 {
 	if(!head.empty())
 	{
 		tyhp_header::const_iterator cit = head.begin();
 		while(cit != head.end())
 		{
 			cout<<cit->first<<":"<<cit->second<<endl;
 			++cit;
 		}
 	}
 }


 void tyhp_print_http_header(tyhp_http_header_t *phttphdr)
 {
 	if(NULL == phttphdr)
 	{
 		perror("phttphdr == NULL");
 		return ;
 	}

 	cout<<phttphdr->method<<" "<<phttphdr->url<<" "<<phttphdr->version<<endl;
	tyhp_print_http_header_header(phttphdr->header);
	cout<<endl<<phttphdr->body<<endl;
 }


tyhp_http_header_t *tyhp_alloc_http_header()
{
	tyhp_http_header_t *phttphdr = (tyhp_http_header_t *)new tyhp_http_header_t;
	if(phttphdr == NULL)
	{
		perror("tyhp_alloc_http_header");
		exit(-1);
	}
	return phttphdr;
}


void tyhp_free_http_header(tyhp_http_header_t *phttphdr)
{
	if(phttphdr == NULL)
		return ;
	delete phttphdr;
}


bool tyhp_parse_http_request(const string& http_request, tyhp_http_header_t *phttphdr)
{
	if(http_request.empty())
	{
		perror("tyhp_parse_http_request: http_request is empty");
		return false;
	}
	if(phttphdr == NULL)
	{
		perror("tyhp_parse_http_request: phttphdr is NULL");
		return false;
	}

	string crlf("\r\n"), crlfcrlf("\r\n\r\n");
	size_t prev = 0, next = 0;


	if(string::npos != (next = http_request.find(crlf, prev)))
	{
		string first_line(http_request.substr(prev, next - prev));
		prev = next;
		stringstream sstream(first_line);
		sstream >> (phttphdr->method);
		sstream >> (phttphdr->url);
		sstream >> (phttphdr->version);
	}
	else
	{
		perror("tyhp_parse_http_request: http_request has not a \\r\\n");
		return false;
	}


	size_t pos_crlfcrlf = http_request.find(crlfcrlf, prev);
	if(pos_crlfcrlf == string::npos)
	{
		perror("tyhp_parse_http_request: http_request has not a \"\r\n\r\n\"");
		return false;
	}


	string buff, key, value;
	while(1)
	{
		next = http_request.find(crlf, prev+2);
		

		if(next <= pos_crlfcrlf)
		{

			buff = http_request.substr(prev + 2, next - prev - 2);
			size_t end = 0;

			for(; isblank(buff[end]); ++end)
				;
			int beg = end;

			for(; buff[end] != ':' && !isblank(buff[end]); ++end)
				;
			key = buff.substr(beg, end - beg);

			for(; (!isalpha(buff[end]) && !isdigit(buff[end])); ++end)
				;
			beg = end;

			for(; next != end; ++end)
				;
			value = buff.substr(beg, end - beg);
			phttphdr->header.insert(make_tyhp_header(key, value));

			prev = next;
		}
		else
		{
			break;
		}
	}

	phttphdr->body = http_request.substr(pos_crlfcrlf + 4, http_request.size() - pos_crlfcrlf - 4);

	return true;
}


string tyhp_get_value_from_http_header(const string& key, const tyhp_header& header)
{
	if(header.empty())
		return "";
	tyhp_header::const_iterator cit = header.find(key);
	if(cit == header.end())
		return "";
	return (*cit).second;
}


/*
int main(int argc,char* argv[])
{
    tyhp_http_header_t* phttphdr = tyhp_alloc_http_header();
    string http_resquest("GET /home?name=index.html HTTP/1.1\r\n" \
                             "Host: api.yeelink.net\r\n" \
                             "U-ApiKey: 121234132432143\r\n" \
                             "\r\n"
    );

    cout<<"http resquest size:"<<http_resquest.size()<<endl;
    tyhp_parse_http_request(http_resquest,phttphdr);
    cout<<phttphdr->method<<" "<<phttphdr->url<<" "<<phttphdr->version<<endl;

    tyhp_free_http_header(phttphdr);
    return 0;
}
*/





