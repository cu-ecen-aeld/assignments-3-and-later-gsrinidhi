#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<syslog.h>
#include<errno.h>

//file descriptor for /var/tmp/aesdsocketdata
int fd,connection_complete = 0;
int accepting_connections = 1;

// Signal handler for SIGINT and SIGTERM
void signal_handler(int signum) {
    accepting_connections = 0; // Stop accepting new connections
    
}

int main(int argv, char *argc[]) {
    int should_daemonize = 0;
    if(argv == 2 && strcmp(argc[1], "-d") == 0) {
        // Log the daemonization process
        syslog(LOG_INFO, "Daemonizing the process");
        //set the flag to daemonize the process
        should_daemonize = 1;
    }
    int sockfd, newsockfd, portno = 9000;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[1024],rcv_char;
    int n,i;

    struct sigaction sa;

    sa.sa_handler = signal_handler;   // function to handle signal
    sigemptyset(&sa.sa_mask);        // no additional signals blocked
    sa.sa_flags = 0;  

    // Open syslog for logging
    openlog(NULL,0,LOG_USER);

    //Create and open file /var/tmp/aesdsocketdata
    fd = open("/var/tmp/aesdsocketdata", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("ERROR opening file");
        // Log the error to syslog
        syslog(LOG_ERR, "ERROR opening file: %s", strerror(errno));
        return -1;
    }

    // Log the successful opening of the file
    syslog(LOG_INFO, "File /var/tmp/aesdsocketdata opened successfully");

    printf("File /var/tmp/aesdsocketdata opened successfully\n");

    
    

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        // Log the error to syslog
        syslog(LOG_ERR, "ERROR opening socket: %s", strerror(errno));
        close(fd);
        return -1;
    }

    // Log the successful creation of the socket
    syslog(LOG_INFO, "Socket created successfully");

    // Set up the server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        // Log the error to syslog
        syslog(LOG_ERR, "ERROR on binding: %s", strerror(errno));
        close(sockfd);
        close(fd);
        return -1;
    }

    // Daemonize the process if the flag is set
    if (should_daemonize) {
        int pid = fork();
        if (pid < 0) {
            perror("ERROR on fork");
            // Log the error to syslog
            syslog(LOG_ERR, "ERROR on fork: %s", strerror(errno));
            close(sockfd);
            close(fd);
            return -1;
        }
        if (pid != 0) {
            // Parent process: exits immediately
            close(sockfd); // Close the server socket in the parent process
            close(fd);     // Close the file descriptor in the parent process
            return 0;
        }
    }

    // Log the successful binding of the socket
    syslog(LOG_INFO, "Socket bound successfully to port %d", portno);

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Server is listening on port %d\n", portno);

     // Set up signal handlers for SIGINT and SIGTERM
    // signal(SIGINT, signal_handler);
    // signal(SIGTERM, signal_handler);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    while (accepting_connections) {
        // Accept a new connection
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue; // Continue accepting new connections
        }

        

        connection_complete = 1;

        // Log the client's IP address
        syslog(LOG_INFO, "Accepted connection from %s", inet_ntoa(cli_addr.sin_addr));

        // Read data from the client

        i = 0; // Initialize buffer index

        while (1)
        {
            n = recv(newsockfd, &rcv_char, 1, 0);
            if (n < 0) {
                perror("ERROR reading from socket");
                close(newsockfd);
                break; // Break the loop to accept new connections  
            }
            buffer[i] = rcv_char;
            i++;
            //write to file if i > 1023
            if (i > 1023) {
                if (write(fd, buffer, 1024) < 0) {
                    perror("ERROR writing to file");
                    syslog(LOG_ERR, "ERROR writing to file: %s when i > 1023", strerror(errno));
                    close(newsockfd);
                    break; // Break the loop to accept new connections
                }
                i = 0; // Reset the buffer index
            }
            if (rcv_char == '\n')
            {
                printf("buffer: %s\n", buffer);
                // syslog(LOG_INFO, "Received line: %s", buffer);

                break; // Break the loop when a newline character is received
            }
        }

        //write the received message to the file if i > 0 
        if(i > 0) {
            if (write(fd, buffer, i) < 0) {
                perror("ERROR writing to file");
                syslog(LOG_ERR, "ERROR writing to file: %s when i > 0", strerror(errno));
                close(newsockfd);
                continue; // Continue accepting new connections
            }
        }
        

        printf("Received message: %s\n", buffer);

        // Read the contents of the file and send it back to the client
        lseek(fd, 0, SEEK_SET); // Move the file pointer to the beginning
        char file_buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(fd, file_buffer, sizeof(file_buffer))) > 0) {
            if (send(newsockfd, file_buffer, bytes_read, 0) <  0) {
                perror("ERROR sending to socket");
                close(newsockfd);
                break; // Break the loop to accept new connections
            }
        }
        if (bytes_read < 0) {
            perror("ERROR reading from file");
            close(newsockfd);
            continue; // Continue accepting new connections
        }

        // Echo the message back to the client
        // n = write(newsockfd, buffer, strlen(buffer));
        // if (n < 0) {
        //     perror("ERROR writing to socket");
        //     close(newsockfd);
        //     continue; // Continue accepting new connections
        // }

        // Close the connection with the client
        close(newsockfd);
        connection_complete = 0;

        // Log the disconnection
        syslog(LOG_INFO, "Closed connection from %s", inet_ntoa(cli_addr.sin_addr));
    }

    // Close the server socket and the file descriptor
    close(fd);
    close(sockfd);
    //log received signal
    syslog(LOG_INFO, "Caught signal, exiting");
    //close syslog
    closelog();
    return 0;
}   