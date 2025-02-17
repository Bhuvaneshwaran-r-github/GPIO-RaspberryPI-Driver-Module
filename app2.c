#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define size 4
int write_buf[3]={0};
	
int fd;
void led_fun(int led);

int main(){
	fd= open("/dev/ext_device", O_RDWR);
	if(fd < 0){
		printf("cannot create file descriptor\n");
		return 0;
	}

	int num, time, led, blink;
	char option;

	system("clear");

	while(1){
	printf("***** Enter the operation *****\n      1.LED ON/OFF\n      2.LED BLINK\n      3.EXIT\n****************************\n");
	scanf(" %c", &option);

		switch(option){
			// 101 ==> led1 ON, led2 OFF, led3 ON 
			case '1': printf("Set 1 for LED ON\n0 for LED OFF\n101 ==> LEFT ON, CENTER OFF, RIGHT ON\n\nEnter :");
				  scanf("%d", &led);
				  led_fun(led);
				  break;
			case '2': printf("Set 1 for LED ON\n0 for LED OFF\n101 ==> LEFT ON, CENTER OFF, RIGHT ON\n\nEnter :");	
				  scanf("%d", &blink);
				  printf("no. of times to blink \n");
				  scanf("%d", &num);
				  printf("Interval of blinking (ms) \n");
				  scanf("%d", &time);
				  printf("LED is blinking......\n");
				  
				  for(int i=0; i < (2*num); i++){
					if(i%2 == 0)
					 	led_fun(blink);
					else
					 	led_fun(-blink);
					usleep(time*1000);
				  }
				  led_fun(led);
				  break;
			 
			case '3': led=0;
				  led_fun(led);
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

void led_fun(int led){
	if(led >= 0){
		for(int i=2; i>=0; i--){
			write_buf[i]=led%10;
			led=led/10;
		}
	}else{
		led= -led;
		for(int i=2; i>=0; i--){
			write_buf[i] = !(led%10);
			led = led/10;
		}	
	}
	write(fd, write_buf , sizeof(write_buf));

}
