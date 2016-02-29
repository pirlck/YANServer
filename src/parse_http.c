//
// Created by cl-k on 10/4/15
//

#incldue "parse_http.h"

#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: yashttpd/0.1.0\r\n"

// file types in dir
file_type serv_types[] =
        {
                {".html", "text/html"},
                {".xml", "text/xml"},
                {".css", "text/css"},
                {".txt", "text/plain"},
                {".pdf", "application/pdf"},
                {".png", "image/png"},
                {".gif", "image/gif"},
                {".jpg", "image/jpeg"},
                {".jpeg", "image/jpeg"},
                {".au", "audio/basic"},
                {".mpeg", "video/mpeg"},
                {".avi", "video/x-msvideo"},
                {".gz", "application/x-gzip"},
                {".tar", "application/x-tar"},
                {NULL ,"text/plain"}
        };


//deal with the resquest
void* do_resquest(void* cli_fd)
{
    // malloc the memory and read the data;
    // parse the  the first line of http resquest
    // store the header
    // check if the file exits and sendfile to the client
    char buff[2048];
    int flag;               //method implemented or not
    //int rev_len = 0;
    //char* query_string = NULL;
    ssize_t rev_len = read( *(int*)cli_fd,buff,sizeof(buff));
    if(rev_len < 0)
    {
        printf("read data error !\n");
        exit(-1);
    }

    //parse the data
    http_header _header;

    //parse the head from the buff data
    //if the method is not implemented,return;
    flag = parse_resquest(buff,&_header);
    if(flag == -1)
    {
        //debug_info
        printf("the method is unimplemented! \n");
        unimplemented( *(int*)cli_fd );
        return (void*)0;
    }

    //print the head
    print_header(&_header);

    //construct the real url
    char path[512];
    memset(path,0,sizeof(path));

    // make real url and send file
    make_real_url(&_header,path,*(int*)cli_fd);

    //send file to client
    //serv_static(cli_fd,path);

    return (void*)0;
}


//parse the first line into the array
//and store url method version into the header
int parse_resquest(char* buff,http_header* pheader)
{
    int i,j;
    i = j = 0;

    //read the method into the array
    while(!ISspace(buff[i]) &&  i<(sizeof(pheader->method) -1) )
    {
        pheader->method[j] = buff[i];
        i++,j++;
    }
    pheader->method[i] = '\0';

    //if the method not the "GET",return the unimplemented
    if( strcmp(pheader->method,"GET") != 0)
    {
        // debug_info
        printf("this method unimplemented !");
        //unimplemented method
        //unimplemented(cli_fd);
        //return -1 (method is unimplemented)
        return -1;
    }

    //skip all the space
    while(ISspace(buff[j]) && j<sizeof(buff))
        j++;

    //read the url into the array
    i = 0 ;
    while(!ISspace(buff[j]) && i<sizeof(pheader->url))
    {
        pheader->url[i] = buff[j];
        i++,j++;
    }
    pheader->url[i] = '\0';

    //read the version into the array
    while(ISspace(buff[j]) && j<sizeof(buff))
        j++;
    i = 0;
    while(!ISspace(buff[j]) && i<sizeof(pheader->version) )
    {
        pheader->version[i] = buff[j];
        i++,j++;
    }
    pheader->version[j] = '\0';

    return 0;
}


void print_header(http_header* pheader)
{
    printf("method:%s \n",pheader->method);
    printf("url:%s \n",pheader->url);
    printf("version:%s \n",pheader->version);
}

//construct the real url in the header->url
int make_real_url(http_header* pheader,char* path,int cli_fd)
{
    int cgi;
    //char path[512];
    char* query_string = NULL;

    serv_static(cli_fd,pheader->url);
    printf("runing this place ! \n");

    // to find char'?' to decided if need CGI
    if( strcmp(pheader->method,"GET") ==0 )
    {
        query_string = pheader->url;
        while( (*(query_string) != '?') && (*query_string != '\0'))
            query_string++;

        if(*query_string == '?')
        {
            //set the cgi flag,the program need cgi
            cgi = 1;

            //split the string
            *query_string = '\0';

            //the back part url used to execute cgi
            query_string++;
        }

    }
    //sprintf(),end at the char '\0'
    sprintf(path,"htdocs%s",pheader->url);
    size_t len = strlen(path);
    if( path[len-1] == '/')
        strcat(path,"index.html");

    //check if the file exits
    struct stat st_info;
    if(-1 == stat(path,&st_info) )
    {
        //debug_info()
        not_found(cli_fd);

    }
    else{
        //file is a dir,strcat() file with the dir
        //than path is the real url
        if((st_info.st_mode & S_IFMT) == S_IFDIR)
            strcat(path,"/index.html");

        printf("the file path:%s \n",path);
        serv_static(cli_fd, path);

        //if the file is  executed  ....extensions
        //set the cgi =1
        //than execute(cgi)

    }

    return 0;
}

//send the static file to client
int serv_static(int cli_fd,char* filename)
{
    const char* file_type;
    char* dot_pos = rindex(filename,'.');
    file_type = get_filetype(dot_pos);


    //open the file,construct the head,send
    FILE* ind = fopen(filename,"r");
    if(ind == NULL)
    {
        //debug_info
        printf("cannot open the file");
        //404 not found
        not_found(cli_fd);
        return -1;
    }
    else
    {
        //construct the http response header
        make_header(cli_fd,filename);
        //than send the file
        char buff[1024];
        memset(buff,0,sizeof(buff));
        while(!feof(ind))
        {
            fgets(buff,sizeof(buff),ind);
            send(cli_fd,buff,strlen(buff),0);
        }

    }
    return 0;
}

// send the file not found response to client
// 404 not found
void not_found(int cli_fd)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.1 404 NOT FOUND\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(cli_fd, buf, strlen(buf), 0);

    //body html
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(cli_fd, buf, strlen(buf), 0);
}

//construct the header
void make_header(int cli_fd,char* filename)
{
    char buf[1024];

    // could use filename to determine file type
    (void)filename;

    strcpy(buf, "HTTP/1.1 200 OK\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    send(cli_fd, buf, strlen(buf), 0);

    //add the file type recognize api
    sprintf(buf, "Content-Type: text/html\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(cli_fd, buf, strlen(buf), 0);
}

// info the client that the request method has not been
// implemented
void unimplemented(int cli_fd)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.1 501 Method Not Implemented\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(cli_fd, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(cli_fd, buf, strlen(buf), 0);
}


const char* get_filetype(const char* type)
{
    int i;
    if(type =NULL)
        return "text/plain";

    for(i=0;serv_types[i].type!= NULL;i++)
        if(strcmp(type,serv_types[i].type)==0)
            return serv_types[i].value;

    return serv_types[i].value;
}

#endif

