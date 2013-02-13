#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#define JOY_DEV "/dev/input/js0"

int *axis = NULL;

int useaxis (int numaxis) {
	
	bool returning = false;
	int savezone = 1000;

	int maxvalue = 32767;
	int percent = NULL;
	
	if (axis[numaxis] < -savezone && axis[numaxis] < 0 || axis[numaxis] > savezone && axis[numaxis] > 0) {
		returning = true;
	}
	
	if (returning) {
		percent = axis[numaxis] / (maxvalue / 100);
		return percent;
	} else {
		return 0;
	}
		
}

int main()
{
        int joy_fd, num_of_axis=0, num_of_buttons=0, x;
        char *button=NULL, name_of_joystick[80];
        struct js_event js;

        if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
        {
                printf( "Couldn't open joystick\n" );
                return -1;
        }

        ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
        ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
        ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

        axis = (int *) calloc( num_of_axis, sizeof( int ) );
        button = (char *) calloc( num_of_buttons, sizeof( char ) );

		printf ("%s\n", name_of_joystick);
		
        fcntl( joy_fd, F_SETFL, O_NONBLOCK );

        while( 1 )
        {

                read(joy_fd, &js, sizeof(struct js_event));
                
                switch (js.type & ~JS_EVENT_INIT)
                {
                        case JS_EVENT_AXIS:
                                axis   [ js.number ] = js.value;
                                break;
                        case JS_EVENT_BUTTON:
                                button [ js.number ] = js.value;
                                break;
                }

                printf( "X1: %6d  Y1: %6d  ", useaxis(0), useaxis(1));	// Left Stick
				printf ("X2: %6d  Y2: %6d  ", useaxis(3), useaxis(4));	// Right Stick

				fflush(stdout);
			
                for( x=0 ; x<num_of_buttons ; ++x )
                        //printf("B%d: %d  ", x, button[x] );

                printf("  \r");
                fflush(stdout);
        }

        close( joy_fd );
        return 0;
}