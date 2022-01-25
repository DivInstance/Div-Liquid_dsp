// pll_example.c : simulate a phase-locked loop

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

int main() {
    // parameters
    float phase_offset      = 0.00f;    // carrier phase offset
    float frequency_offset  = 0.30f;    // carrier frequency offset
    float wn                = 0.01f;    // pll bandwidth
    float zeta              = 0.707f;   // pll damping factor
    float K                 = 1000;     // pll loop gain
    unsigned int n          = 400;      // number of samples

    // generate loop filter parameters (active PI design)
    float t1 = K/(wn*wn);   // tau_1
    float t2 = 2*zeta/wn;   // tau_2

    // feed-forward coefficients (numerator)
    float b0 = (4*K/t1)*(1.+t2/2.0f);
    float b1 = (8*K/t1);
    float b2 = (4*K/t1)*(1.-t2/2.0f);

    // feed-back coefficients (denominator)
    //    a0 =  1.0  is implied
    float a1 = -2.0f;
    float a2 =  1.0f;

    // print filter coefficients (as comments)
    printf("#  b = [b0:%12.8f, b1:%12.8f, b2:%12.8f]\n", b0, b1, b2);
    printf("#  a = [a0:%12.8f, a1:%12.8f, a2:%12.8f]\n", 1., a1, a2);

    // filter buffer
    float v0=0.0f, v1=0.0f, v2=0.0f;
    
    // initialize states
    float phi     = phase_offset;   // input signal's initial phase
    float phi_hat = 0.0f;           // PLL's initial phase
    
    // print line legend to standard output
    printf("# %6s %12s %12s %12s %12s %12s\n",
            "index", "real(x)", "imag(x)", "real(y)", "imag(y)", "error");

    // run basic simulation
    unsigned int i;
    float complex x;
    float complex y;
    for (i=0; i<n; i++) {
        // compute input sinusoid and update phase
        x = cosf(phi) + _Complex_I*sinf(phi);
        phi += frequency_offset;

        // compute PLL output from phase estimate
        y = cosf(phi_hat) + _Complex_I*sinf(phi_hat);

        // compute error estimate
        float delta_phi = cargf( x * conjf(y) );

        // print results to standard output
        printf("  %6u %12.8f %12.8f %12.8f %12.8f %12.8f\n",
                  i,
                  crealf(x), cimagf(x),
                  crealf(y), cimagf(y),
                  delta_phi);

        // push result through loop filter, updating phase estimate

        // advance buffer
        v2 = v1;  // shift center register to upper register
        v1 = v0;  // shift lower register to center register

        // compute new lower register
        v0 = delta_phi - v1*a1 - v2*a2;

        // compute new output
        phi_hat = v0*b0 + v1*b1 + v2*b2;
    }
    
    return 0;
}
