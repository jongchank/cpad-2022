/* A simple SocketCAN example */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
int soc;
int read_can_port;
int open_port(const char *port)
{
    struct ifreq ifr;

    //printf("error!");
    struct sockaddr_can addr;
    /* open socket */
    soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(soc < 0)
    {
	printf("error!");
        return (-1);
    }
    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port);
    if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0)
    {
	printf("error!");
        return (-1);
    }
    addr.can_ifindex = ifr.ifr_ifindex;
    fcntl(soc, F_SETFL, O_NONBLOCK);
    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
	printf("binding error!");
        return (-1);
    }

    //printf("error!");
    return 0;
}



/* this is just an example, run in a thread */
void read_port()
{
    struct can_frame frame;
    int recvbytes = 0;
    read_can_port = 1;
    short short_;
    int int_;
    while(read_can_port)
    {
        struct timeval timeout = {1, 0};
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(soc, &readSet);
        if (select((soc + 1), &readSet, NULL, NULL, &timeout) >= 0)
        {
            if (!read_can_port)
            {
		        printf("error!");
                break;

            }
            if (FD_ISSET(soc, &readSet))
            {
                recvbytes = read(soc, &frame, sizeof(struct can_frame));
                if(recvbytes)
                { 
	            if(frame.can_id == 0x123)
                    {
                        printf("id = 123 \n");
                        for (int i = 0; i < frame.can_dlc; i++) 
                        {
	                        if (i == 0) short_ = (frame.data[i]<<8);
	                        if (i == 1) short_ |= frame.data[i]&0x00FF;
	                        if (i == 4) int_ = frame.data[i]<<24;
                                if (i == 5) int_ |= frame.data[i]<<16;   
 	                        if (i == 6) int_ |= frame.data[i]<<8;
                                if (i == 7) int_ |= frame.data[i]&0x00FF;                              
                        }
                    
                        
                        printf("short : %d, integer : %d \n", short_, int_);
                        printf("\n");
                    //printf("%02x", frame.can_id);
                
                    }
		        //printf("\n");
                        
                }
            }
        }
    }
}

int close_port()
{
	close(soc);
	return 0;
}
int main(void)
{
    //printf("error!");
    open_port("can0");
    read_port();
    //printf("error!");
    return 0;
}
