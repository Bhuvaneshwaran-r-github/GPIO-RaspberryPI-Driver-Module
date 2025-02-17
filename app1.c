#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define size 4

int read_buf;
int write_buf;
	
int fd;
void led_fun(int led);

int main(){
	fd= open("/dev/ext_device", O_RDWR);
	if(fd < 0){
		printf("cannot create file descriptor\n");
		return 0;
	}
	int num, time, led;
	char option;

	system("clear");

	while(1){
	printf("***** Enter the operation *****\n      1.LED ON\n      2.LED OFF\n      3.LED BLINK\n      4.EXIT\n****************************\n");
	scanf(" %c", &option);

		switch(option){
 
			case '1': printf("   1 = LEFT\n   2 = CENTER\n   3 = LEFT, CENTER\n   4 = RIGHT\n   5 = LEFT, RIGHT\n   6 = CENTER, RIGHT\n   7 = LEFT, CENTER, RIGHT\nEnter the LED to ON\n");
				  scanf("%d", &led);
				  led_fun(led);
				  break;
			
			case '2': printf("   1 = LEFT\n   2 = CENTER\n   3 = LEFT, CENTER\n   4 = RIGHT\n   5 = LEFT, RIGHT\n   6 = CENTER, RIGHT\n   7 = LEFT, CENTER, RIGHT\nEnter the LED to OFF\n");
				  scanf("%d", &led);
				  led_fun(-led);
				  break;

			case '3': printf("   1 = LEFT\n   2 = CENTER\n   3 = LEFT, CENTER\n   4 = RIGHT\n   5 = LEFT, RIGHT\n   6 = CENTER, RIGHT\n   7 = LEFT, CENTER, RIGHT\nEnter the LED to BLINK\n");
				  scanf("%d", &led);
				  printf("no. of times to blink \n");
				  scanf("%d", &num);
				  printf("Interval of blinking (ms) \n");
				  scanf("%d", &time);
				  printf("LED is blinking......\n");

				  int led_backup = led;//taking backup when to make the led glow as previous 
				  for(int i=0; i < (2*num); i++){
					if(i%2 == 0)
					 	led_fun(led);
					else
					 	led_fun(-led);
					usleep(time*1000);
				  }
				  led_fun(led_backup);
				  break;
			
			case '4': led=7;//Turn OFF all led while exiting the API
				  led_fun(-led);
				  close(fd);
				  return 0;

			default : printf("Enter the valid option !!!\n");
				  break;
		}

		sleep(2);
		system("clear");
	}
	close(fd);
}

/*  

 set to '1'
 1 = LEFT
 2 = CENTER
 3 = LEFT, CENTER
 4 = RIGHT
 5 = LEFT, RIGHT
 6 = CENTER, RIGHT
 7 = LEFT, CENTER, RIGHT

 set to '0'
 -1 = LEFT
 -2 = CENTER
 -3 = LEFT, CENTER
 -4 = RIGHT
 -5 = LEFT, RIGHT
 -6 = CENTER, RIGHT
 -7 = LEFT, CENTER, RIGHT

 */


void led_fun(int led)
{
	int pos_arr[3]={4,2,1};
	int neg_arr[3]={-4,-2,-1};

	if(led > 0){
		for(int i=0; i<3; i++){
			if(pos_arr[i] <= led){
				write_buf = pos_arr[i];
				write(fd, &write_buf, size);
				led = led - pos_arr[i];
			}
		}
	}else{
		for(int i=0; i<3; i++){
			if(neg_arr[i] >= led){
				write_buf = neg_arr[i];
				write(fd, &write_buf, size);
				led = led - neg_arr[i];
			}
		}
	}
}
