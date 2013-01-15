#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <exit_callback.h>

#define VERS    1 //Talk about this
#define REVS    0

PSP_MODULE_INFO("Hello World", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_MAX();

#define printf pspDebugScreenPrintf

int main()
{
    char running = 1;

        pspDebugScreenInit();
        setupExitCallback();

        SceCtrlData buttonInput;

        sceCtrlSetSamplingCycle(0);
        sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

        while(running)
        {
                running = isRunning();

                pspDebugScreenSetXY(0, 0);
                printf("Analog X = %d ", buttonInput.Lx);
                printf("Analog Y = %d \n", buttonInput.Ly);
                sceCtrlPeekBufferPositive(&buttonInput, 1);

                if(buttonInput.Buttons != 0)
                {
                        if(buttonInput.Buttons & PSP_CTRL_START){
                                                                        printf("Start");
                                                                        running = 0;
                        }
                        if(buttonInput.Buttons & PSP_CTRL_SELECT)       printf("Select");
                        if(buttonInput.Buttons & PSP_CTRL_UP)           printf("Up");
                        if(buttonInput.Buttons & PSP_CTRL_DOWN)         printf("Down");
                        if(buttonInput.Buttons & PSP_CTRL_RIGHT)        printf("Right");
                        if(buttonInput.Buttons & PSP_CTRL_LEFT)         printf("Left");

                        if(buttonInput.Buttons & PSP_CTRL_CROSS)        printf("Cross");
                        if(buttonInput.Buttons & PSP_CTRL_CIRCLE)       printf("Circle");
                        if(buttonInput.Buttons & PSP_CTRL_SQUARE)       printf("Square");
                        if(buttonInput.Buttons & PSP_CTRL_TRIANGLE)     printf("Triangle");

                        if(buttonInput.Buttons & PSP_CTRL_RTRIGGER)     printf("R-Trigger");
                        if(buttonInput.Buttons & PSP_CTRL_LTRIGGER)     printf("L-Trigger");
                }
        }

        sceKernelExitGame();
        return 0;
}
