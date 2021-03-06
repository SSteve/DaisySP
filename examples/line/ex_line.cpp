#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;

static daisy_handle seed;
static Line line_seg;
static Oscillator osc_sine;

uint8_t finished;

static void AudioCallback(float *in, float *out, size_t size)
{
	float sine, freq;
    for (size_t i = 0; i < size; i += 2)
    {
        if (finished)
        {
            // Start creating a Line segment from 100 to 500 in 1 seconds
            line_seg.Start(100, 500, 1);
        }

        freq = line_seg.Process(&finished);
        osc_sine.SetFreq(freq);
    	sine = osc_sine.Process();

    	// left out
        out[i] = sine;

        // right out
        out[i+1] = sine;
    }
}

int main(void)
{
	// initialize seed hardware and daisysp modules
    daisy_seed_init(&seed);

    // initialize Line module
    line_seg.Init(DSY_AUDIO_SAMPLE_RATE);
    finished = 1;

    // set parameters for sine oscillator object
    osc_sine.Init(DSY_AUDIO_SAMPLE_RATE);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);

    // define callback
    dsy_audio_set_callback(DSY_AUDIO_INTERNAL, AudioCallback);

    // Start callback
    dsy_audio_start(DSY_AUDIO_INTERNAL);

    while(1) {}
}
