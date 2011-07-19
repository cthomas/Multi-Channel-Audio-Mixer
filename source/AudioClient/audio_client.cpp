
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "AudioSample.h"
#include "PracticalSocket.h"

#define WAVE_FILE "test/data/test_22050_1.wav"

int main(int argc, char **argv)
{
//	if(argc == 4)
	{
		std::string msg = "QUIT";
		printf("Audio Client\n");

		TCPSocket sock("localhost", 22000);

		FILE *pFile = fopen(WAVE_FILE, "rb");

		if(pFile)
		{
			fseek(pFile, 40, SEEK_SET);

			int32_t data_size = 0;
			fread(&data_size, sizeof(int32_t), 1, pFile);

			AudioSample_t *samples = new AudioSample_t[data_size];

			if(samples)
			{
				fseek(pFile, 44, SEEK_SET);
				fread(samples, data_size, 1, pFile);
			}

			fclose(pFile);
			pFile = NULL;

			size_t samples_to_push = 22050;

			for(size_t i = 0; i < data_size/sizeof(AudioSample_t); i+=samples_to_push)
			{
				size_t actual_samples_pushed = (i + samples_to_push < data_size/sizeof(AudioSample_t)) ? samples_to_push : data_size/sizeof(AudioSample_t)-i;

				try
				{
					sock.send(&samples[i], actual_samples_pushed*sizeof(AudioSample_t));
				}
				catch(SocketException &e)
				{
					printf("Unable to send data...caught exception[%s]\n", e.what());
				}

				usleep(1000*1000*1);
			}

			delete[] samples;
			samples = NULL;
		}
	}
//	else
//	{
//		printf("Usage: %s wav_file host port\n", argv[0]);
//	}

	return 0;
}

