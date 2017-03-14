/* 
 *  A forked server
 *  by Martin Broadhurst (www.martinbroadhurst.com)
 */

#ifndef _TCPFORKING
#define _TCPFORKING

#include <stdio.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <deque>
#include <cassert>

using namespace std;

#define BACKLOG     10  /* Passed to listen() */

class Annotator {
    public:

        virtual void annotate(char* ,size_t ,ostream& ){
            cerr << "Unimplemented" << endl;
            abort();
        }
        virtual ~Annotator(){}

};

class TCPFork {
    public:
        static void log(const char* msg){
#ifndef NDEBUG
            cout << msg << endl;
#endif
        }
        /* Signal handler to reap zombie processes */
        static void wait_for_child(int )
        {
            while (waitpid(-1, NULL, WNOHANG) > 0);
        }

        static void handle(int newsock,Annotator* annot) {
            struct timeval tv;
            log("Handling");


            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(newsock, &readfds);

            fd_set writefds;
            FD_ZERO(&writefds);
            FD_SET(newsock, &writefds);

            int res = 0;
            vector<char> msg;
            vector<char> buf(2048);
            deque<string> out;
            bool isclosing  =0;
            while(1){
                if (!isclosing){
                    log("Listening read");
                    tv.tv_sec = 30;
                    tv.tv_usec = 0;
                    res = select(newsock + 1, &readfds, 0, NULL, &tv);
                    if (res <= 0){
                        log("Select error or timeout ");
                        break;
                    }
                    if (FD_ISSET(newsock, &readfds)){
                        int shouldread = 0;
                        res = recv(newsock,&shouldread,4,0);
                        if (res < 0){
                            log("Read header error");
                            break;
                        }
                        // Closed read stream
                        if (res == 0){
                            log("Closed read stream");
                            isclosing = true;
                        }
                        // Empty message
                        else if (shouldread == 0){
                            log("Read zero in header");
                            out.push_front("");
                        }
                        else if (shouldread > 0){
                            buf.resize(shouldread);
                            res = recv(newsock,buf.data(),buf.size(),0);
                            if (res <= 0){
                                stringstream ss;
                                ss << "read error " << res;
                                log(ss.str().c_str());
                                break;
                            }

                            stringstream ss;
                            ss << "Read " << res;
                            log(ss.str().c_str());
                            stringstream os;
                            annot->annotate((char*)buf.data(),buf.size(),os);
                            out.push_front(os.str());
                            // TODO - Server error reporting
                            // Processing cannot give SPECIAL marker
                            // assert(os.str().size() > 0);
                            stringstream ss1;
                            ss1 << "Processed " << out.front().size();
                            log(ss1.str().c_str());
                        }
                        else {
                            log("???");
                            abort();
                        }
                    }
                    else{
                        log("Cannot read");
                        break;
                    }
                }
                if (out.size() >= 0){
                    tv.tv_sec = 30;
                    tv.tv_usec = 0;
#ifndef NDEBUG
                    log("Listening write");
#endif
                    res = select(newsock + 1, 0, &writefds, NULL, &tv);
                    if (res < 0){
                        stringstream ss;
                        ss << "Select error " << res << endl;
                        log(ss.str().c_str());
                        break;
                    }
                    if (FD_ISSET(newsock, &writefds)){
                        int wsz = (int)out.back().size();
                        res = send(newsock,&wsz ,4,0);
                        if (res <= 0){
                            stringstream ss;
                            ss << "Write header error " << res << endl;
                            log(ss.str().c_str());
                            break;
                        }
                        if (wsz > 0){
                            res = send(newsock,out.back().data(),wsz,0);
                            if (res <= 0){
                                stringstream ss;
                                ss << "Write error " << res << endl;
                                log(ss.str().c_str());
                                break;
                            }

                            stringstream ss;
                            ss << "Writen " << res << endl;
                            log(ss.str().c_str());
                        }
                        out.pop_back();
                    }
                    else{
                        log("Cannot write");
                        break;
                    }
                }
                if (isclosing && out.size() == 0){
                    log("All sent, quit" );
                    break;
                }
            }
            close(newsock);
            log("Closed");
        }


        static int start(Annotator* anot,int port)
        {
            int sock;
            struct sigaction sa;
            struct addrinfo hints, *res;
            int reuseaddr = 1; /* True */

            /* Get the address info */
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            stringstream ss;
            ss << port;
            if (getaddrinfo(NULL, ss.str().c_str(), &hints, &res) != 0) {
                perror("getaddrinfo");
                return 1;
            }

            /* Create the socket */
            sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (sock == -1) {
                perror("socket");
                return 1;
            }

            /* Enable the socket to reuse the address */
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
                perror("setsockopt");
                return 1;
            }

            /* Bind to the address */
            if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
                perror("bind");
                return 1;
            }

            /* Listen */
            if (listen(sock, BACKLOG) == -1) {
                perror("listen");
                return 1;
            }

            freeaddrinfo(res);

            /* Set up the signal handler */
            sa.sa_handler = TCPFork::wait_for_child;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;
            if (sigaction(SIGCHLD, &sa, NULL) == -1) {
                perror("sigaction");
                return 1;
            }

            /* Main loop */
            while (1) {
                struct sockaddr_in their_addr;
                size_t sz = sizeof(struct sockaddr_in);
                int newsock = accept(sock, (struct sockaddr*)&their_addr, (socklen_t*)&sz);
                int pid;

                if (newsock == -1) {
                    perror("accept");
                    return 0;
                }
#ifndef NDEBUG
                printf("Got a connection from %s on port %d\n", inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
#endif

                pid = fork();
                if (pid == 0) {
                    /* In child process */
                    close(sock);
                    TCPFork::handle(newsock,anot);
                    return 0;
                }
                else {

                    /* Parent process */
                    if (pid == -1) {
                        perror("fork");
                        return 1;
                    }
                    else {
                        close(newsock);
                    }
                }
            }

            close(sock);

            return 0;
        }

};

#endif
