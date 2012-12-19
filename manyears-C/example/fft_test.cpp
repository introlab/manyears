extern "C"
{
    #include "overallContext.h"
}

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{

    //Load params
    struct objOverall workspace;
    struct objFFT fft;
    float src_real[1024];
    float src_imag[1024];
    float dst_real[1024];
    float dst_imag[1024];

    workspace = createEmptyOverallContext();

    ParametersLoadDefault(workspace.myParameters);

    fftInit(&fft, workspace.myParameters, 1024);


    for (int i = 0; i < 100000; i++)
    {
        fftCompute(&fft, src_real,src_imag,dst_real, dst_imag);
        //cout << "Count : "<< i << endl;
    }



    fftTerminate(&fft);


    

	return 0;
}

