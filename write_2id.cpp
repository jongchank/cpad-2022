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

#include <time.h>

int soc;
int read_can_port;
int open_port(const char *port)
{
    struct ifreq ifr;
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

    return 0;
}



void write_port()
{

    srand(time(NULL));

    struct can_frame frame1;	
    frame1.can_id = 0x123;
    frame1.can_dlc = 8;

    struct can_frame frame2;	
    frame2.can_id = 0x456;
    frame2.can_dlc = 8;

    short s1[4];
    int i1[2];

    s1[0] = rand() % 65336;
    frame1.data[0] = (s1[0]&0xFF00) >> 8;
    frame1.data[1] = (s1[0]&0x00FF);

    s1[1] = rand() % 65336;
    frame1.data[2] = (s1[1]&0xFF00) >> 8;
    frame1.data[3] = (s1[1]&0x00FF);

    i1[0] = rand() % 2147486647;
    frame1.data[4] = (i1[0]&0xFF000000) >> 24;
    frame1.data[5] = (i1[0]&0x00FF0000) >> 16;
    frame1.data[6] = (i1[0]&0xFF00FF00) >> 8;
    frame1.data[7] = (i1[0]&0x000000FF);    

    s1[2] = rand() % 65336;
    frame2.data[0] = (s1[2]&0xFF00) >> 8;
    frame2.data[1] = (s1[2]&0x00FF);

    s1[3] = rand() % 65336;
    frame2.data[2] = (s1[3]&0xFF00) >> 8;
    frame2.data[3] = (s1[3]&0x00FF);

    i1[1] = rand() % (2147486647);
    frame2.data[4] = (i1[1]&0xFF000000) >> 24;
    frame2.data[5] = (i1[1]&0x00FF0000) >> 16;
    frame2.data[6] = (i1[1]&0xFF00FF00) >> 8;
    frame2.data[7] = (i1[1]&0x000000FF);    
    
    ssize_t nbytes1 = write(soc, &frame1, sizeof(struct can_frame));
    ssize_t nbytes2 = write(soc, &frame2, sizeof(struct can_frame));

    if(nbytes1 < 1)
    {
	printf("send error!\n");
    }
	
    if(nbytes2 < 1)
    {
	printf("send error!\n");
    }

}
int close_port()
{
	close(soc);
	return 0;
}
int main(void)
{
 
	open_port("can0");
    while(60){
        sleep(2);
    	write_port();
    }	
    close_port();
    
    return 0;
}
